#include <xcb/xcb.h>
// #include <xcb/shm.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xcb.h>
#include <unistd.h>
#include <stdio.h>
#include "draw_screen.h"
#include <stdlib.h>
#include "triangle_list.h"
#include "process_verteces.h"
#include "move_camera.h"

// Copy pasted find_visual from tutorial, have no idea how it works
static xcb_visualtype_t *find_visual(xcb_connection_t *c, xcb_visualid_t visual)
{
    xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(xcb_get_setup(c));

    for (; screen_iter.rem; xcb_screen_next(&screen_iter))
    {
        xcb_depth_iterator_t depth_iter = xcb_screen_allowed_depths_iterator(screen_iter.data);
        for (; depth_iter.rem; xcb_depth_next(&depth_iter))
        {
            xcb_visualtype_iterator_t visual_iter = xcb_depth_visuals_iterator(depth_iter.data);
            for (; visual_iter.rem; xcb_visualtype_next(&visual_iter))
                if (visual == visual_iter.data->visual_id)
                    return visual_iter.data;
        }
    }

    return NULL;
}

int main()
{
    xcb_connection_t *c;
    xcb_screen_t *screen;
    xcb_drawable_t drawable;
    xcb_screen_iterator_t iter;
    xcb_gcontext_t gcontext;
    // uint32_t mask;
    uint32_t mask;
    uint32_t value[3];

    xcb_generic_event_t *e;
    xcb_generic_error_t *err;
    xcb_colormap_t colormap;
    xcb_alloc_color_cookie_t alloc_color_cookie;
    xcb_alloc_color_reply_t *color_info;
    c = xcb_connect(NULL, NULL);

    xcb_visualtype_t *visual;
    cairo_t *cr;
    cairo_surface_t *surface;
    float depth_buffer[1920][1076];

    struct triangle_list triangles = process_verteces();

    iter = xcb_setup_roots_iterator(xcb_get_setup(c));
    screen = iter.data;

    drawable = xcb_generate_id(c);

    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    value[0] = screen->black_pixel;
    value[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS;

    xcb_create_window(c,                             /* Connection          */
                      XCB_COPY_FROM_PARENT,          /* depth (same as root)*/
                      drawable,                      /* window Id           */
                      screen->root,                  /* parent window       */
                      0, 0,                          /* x, y                */
                      1920, 1080,                    /* width, height       */
                      10,                            /* border_width        */
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
                      screen->root_visual,           /* visual              */
                      mask, value);

    xcb_map_window(c, drawable);

    visual = find_visual(c, screen->root_visual);
    if (visual == NULL)
    {
        fprintf(stderr, "Some weird internal error...?!");
        xcb_disconnect(c);
        return 1;
    }
    surface = cairo_xcb_surface_create(c, drawable, visual, 1920, 1080);

    cr = cairo_create(surface);

    xcb_flush(c);
    // fprintf(stdout, "Hello?");
    // fprintf(stdout, "Hellooo?");
    while ((e = xcb_wait_for_event(c)))
    {
        sleep(1.0 / 24.0);
        draw_screen(cr, triangles, depth_buffer);

        switch (e->response_type & ~0x80)
        {
        case XCB_EXPOSE:
        {
            // triangles = translate_camera(triangles, 0.0, 0.0, 100.0);
            draw_screen(cr, triangles, depth_buffer);
            break;
        }

        // W = 25 '\031'
        // S = 39 '\''
        // A = 38 '&'
        // D = 40 '('
        case XCB_KEY_PRESS:
        {
            xcb_key_press_event_t *ev = (xcb_key_press_event_t *)e;
            // fprintf(stdout, "Hello?");
            printf("Key pressed in window %ld\n",
                   ev->event);

            switch (ev->event)
            {
            case 97:
            {
                triangles = translate_camera(triangles, 0.0, 0.0, 10.0);
                break;
            }
            default:
            {
                triangles = translate_camera(triangles, 0.0, 0.0, 10.0);
                break;
            }
            }

            // triangles = translate_camera(triangles, 0.0, 0.0, -1.0);

            // draw_screen(cr, triangles, depth_buffer);
            break;
        }

        default:
        {
            /* Unknown event type, ignore it */
            break;
        }
        }
        /* Free the Generic Event */
        free(e);
        xcb_flush(c);
    }

    // pause();

    return 0;
}