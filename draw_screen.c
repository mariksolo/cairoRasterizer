#include <xcb/xcb.h>
#include <unistd.h>
#include <stdio.h>
#include "draw.h"

void draw_screen(xcb_connection_t *c, xcb_drawable_t drawable, xcb_gcontext_t gcontext, xcb_colormap_t colormap) {
    xcb_point_t points[3] = {{0.0, 50.0}, {100.0, 150.0}, {150.0, 0.0}};
    float color_shades[3] = {0.0, 1.0, 0.5};
    // draw_wireframe_triangle(c, drawable, gcontext, points);
    set_drawing_color(c, colormap, gcontext, 65535, 0, 65535);
    draw_filled_triangle(c, drawable, gcontext, colormap, points, color_shades);
}