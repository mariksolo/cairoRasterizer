#include <xcb/xcb.h>
#include <cairo.h>
#include <cairo-xcb.h>
#include "point.h"
#include <unistd.h>
#include <stdio.h>
#include "draw.h"



void draw_screen(cairo_t *cr) {
    cairo_set_source_rgb(cr, 1.0, 0, 0);
    struct point points[3] = {{0.0, 50.0}, {100.0, 150.0}, {150.0, 0.0}};
    float color_shades[3] = {0.0, 1.0, 0.5};
    // draw_wireframe_triangle(cr, points);
    draw_filled_triangle(cr, points, color_shades);
    
}