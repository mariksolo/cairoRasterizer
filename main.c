#include <xcb/xcb.h>
#include <xcb/shm.h>
#include <unistd.h>
#include <stdio.h>
#include "draw_screen.h"
#include <cairo.h>
#include <cairo-xcb.h>
#include <stdlib.h>
#include "set_drawing_color.h"

static xcb_visualtype_t *find_visual(xcb_connection_t *c, xcb_visualid_t visual)
{
    xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(xcb_get_setup(c));

    for (; screen_iter.rem; xcb_screen_next(&screen_iter)) {
        xcb_depth_iterator_t depth_iter = xcb_screen_allowed_depths_iterator(screen_iter.data);
        for (; depth_iter.rem; xcb_depth_next(&depth_iter)) {
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

    iter = xcb_setup_roots_iterator(xcb_get_setup(c));
    screen = iter.data;

    // drawable = screen->root;

    // colormap = screen->default_colormap;

    // // color_info = set_drawing_color(c, colormap, 65535, 65535, 0);
    // color_info = xcb_alloc_color_reply(c, xcb_alloc_color(c, colormap, 65535, 65535, 0), NULL);

    // if (!color_info)
    //     return 0;
    // free(color_info);

    // gcontext = xcb_generate_id(c);
    // mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    // value[0] = color_info->pixel;
    // value[1] = 0;
    // xcb_create_gc(c, gcontext, drawable, mask, value);

    drawable = xcb_generate_id(c);

    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    value[0] = screen->black_pixel;
    value[1] = XCB_EVENT_MASK_EXPOSURE;

    // mask = XCB_CW_OVERRIDE_REDIRECT | XCB_CW_EVENT_MASK;
    // value[0] = 1;
    // value[1] = XCB_EVENT_MASK_EXPOSURE;

    xcb_create_window(c,                             /* Connection          */
                      XCB_COPY_FROM_PARENT,          /* depth (same as root)*/
                      drawable,                      /* window Id           */
                      screen->root,                  /* parent window       */
                      0, 0,                          /* x, y                */
                      150, 150,                      /* width, height       */
                      10,                            /* border_width        */
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
                      screen->root_visual,           /* visual              */
                      mask, value);

    xcb_map_window(c, drawable);

    // xcb_create_window(c, XCB_COPY_FROM_PARENT, drawable, screen->root,
    //         20, 20, 150, 150, 0,
    //         XCB_WINDOW_CLASS_INPUT_OUTPUT,
    //         screen->root_visual,
    //         mask,
    //         value);

    // xcb_map_window(c, drawable);

    

    /* XPM */
    // static char * XFACE[] = {
    //     /* <Values> */
    //     /* <width/columns> <height/rows> <colors> <chars per pixel>*/
    //     "1 1 1 1",
    //     /* <Colors> */
    //     "a c #ffff00",
    //     /* <Pixels> */
    //     "a"
    // };

    // xcb_pixmap_t pixmap = xcb_generate_id(c);
    // xcb_shm_create_pixmap(c, XCB_COPY_FROM_PARENT, pixmap, drawable, 1, 1);

    xcb_visualtype_t *visual;
    cairo_t *cr;
    visual = find_visual(c, screen->root_visual);
    if (visual == NULL) {
        fprintf(stderr, "Some weird internal error...?!");
        xcb_disconnect(c);
        return 1;
    }
    cairo_surface_t * surface = cairo_xcb_surface_create(c, drawable, visual, 100, 100);

    cr = cairo_create(surface);

    xcb_flush(c);

    while ((e = xcb_wait_for_event(c)))
    {
        switch (e->response_type & ~0x80)
        {
        case XCB_EXPOSE:
        {
            // draw_screen(c, drawable, gcontext, colormap);
            // xcb_put_image(c, XCB_IMAGE_FORMAT_XY_PIXMAP, drawable, gcontext, 48, 4, 0, 0, 0, XCB_COPY_FROM_PARENT, sizeof(XFACE), XFACE);
            //   if (((xcb_expose_event_t *) e)->count != 0)
            //     break;

            cairo_set_source_rgb(cr, 0, 1, 0);
            cairo_paint(cr);

            cairo_set_source_rgb(cr, 1, 0, 0);
            cairo_move_to(cr, 0, 0);
            cairo_line_to(cr, 150, 0);
            cairo_line_to(cr, 150, 150);
            cairo_close_path(cr);
            cairo_fill(cr);

            cairo_set_source_rgb(cr, 0, 0, 1);
            cairo_set_line_width(cr, 20);
            cairo_move_to(cr, 0, 150);
            cairo_line_to(cr, 150, 0);
            cairo_stroke(cr);

            cairo_surface_flush(surface);
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