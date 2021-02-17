#include <xcb/xcb.h>

void set_drawing_color(xcb_connection_t *c, xcb_colormap_t colormap, xcb_gcontext_t gcontext, int r, int g, int b) {
    xcb_alloc_color_reply_t *color_info;
    uint32_t mask;
    uint32_t value[3];
    color_info = xcb_alloc_color_reply(c, xcb_alloc_color(c, colormap, r, g, b), NULL);

    if (!color_info)
        return 0;
    free(color_info);

    // gcontext = xcb_generate_id(c);
    mask = XCB_GC_FOREGROUND;
    value[0] = color_info->pixel;
    xcb_change_gc(c, gcontext, mask, value);

    // return color_info;

}