#include <xcb/xcb.h>
#include <unistd.h>
#include <stdio.h>
// #include <cairo.h>

int main()
{
    xcb_connection_t *c;
    xcb_screen_t *screen;
    xcb_drawable_t drawable;
    int screen_nbr;
    xcb_screen_iterator_t iter;
    xcb_window_t windowID;
    xcb_gcontext_t gcontext;
    uint32_t mask;
    uint32_t value[3];
    xcb_rectangle_t rect[] = {0, 0, 150, 150};
    xcb_point_t point[] = {50, 50};
    xcb_generic_event_t *e;
    xcb_generic_error_t *err;
    xcb_colormap_t    colormap;
    xcb_alloc_color_cookie_t alloc_color_cookie;
    xcb_alloc_color_reply_t *color_info;

    c = xcb_connect(NULL, NULL);

    iter = xcb_setup_roots_iterator(xcb_get_setup(c));
    screen = iter.data;

    
    
    
    drawable = screen->root;

    colormap = screen->default_colormap;
    color_info = xcb_alloc_color_reply (c, xcb_alloc_color (c, colormap, 65535, 65535, 0), NULL);

    if (!color_info)
        return 0;
    free(color_info);


    gcontext = xcb_generate_id(c);
    mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    value[0] = color_info->pixel;
    value[1] = 0;
    xcb_create_gc(c, gcontext, drawable, mask, value);

    drawable = xcb_generate_id(c);

    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    value[0] = screen->black_pixel;
    value[1] = XCB_EVENT_MASK_EXPOSURE;

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

    
    xcb_flush(c);

    while ((e = xcb_wait_for_event(c)))
    {
        switch (e->response_type & ~0x80)
        {
        case XCB_EXPOSE:
        {
            
            xcb_poly_point(c, XCB_COORD_MODE_ORIGIN, drawable, gcontext, 1, point);
            xcb_poly_fill_rectangle(c, drawable, gcontext, 1, rect);
            xcb_flush(c);
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
    }

    // pause();

    return 0;
}