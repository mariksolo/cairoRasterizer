#include <xcb/xcb.h>
#include <cairo.h>
#include <cairo-xcb.h>
#include "point.h"
#include <unistd.h>
#include <stdio.h>
#include "draw.h"
#include "project_vertex.h"



void draw_screen(cairo_t *cr) {
    
    // struct point points[3] = {{0.0, 50.0}, {100.0, 150.0}, {150.0, 0.0}};
    // float color_shades[3] = {1.0, 0.0, 0.25};
    // draw_filled_triangle(cr, points, color_shades);
    struct point_3d front_verteces[4] = {{100,  100, 150}, {100,  200, 150}, {200, 200, 150}, {200, 100, 150}};
    struct point_3d back_verteces[4] = {{100,  100, 200}, {100,  200, 200}, {200, 200, 200}, {200, 100, 200}};

    struct point front_projected[4];
    struct point back_projected[4];

    struct point temp_p;

    float d = 20;
    float v_w = 160;
    float v_h = 90;

    for (int i = 0; i < sizeof(front_verteces)/sizeof(front_verteces[0]); i++) {
        front_projected[i] = project_vertex(front_verteces[i], d, v_w, v_h);
    }

     for (int i = 0; i < sizeof(back_verteces)/sizeof(back_verteces[0]); i++) {
        back_projected[i] = project_vertex(back_verteces[i], d, v_w, v_h);
    }

    cairo_set_source_rgb(cr, 1.0, 0, 0);
    draw_line(cr, front_projected[0].x, front_projected[0].y, front_projected[1].x, front_projected[1].y);
    draw_line(cr, front_projected[1].x, front_projected[1].y, front_projected[2].x, front_projected[2].y);
    draw_line(cr, front_projected[2].x, front_projected[2].y, front_projected[3].x, front_projected[3].y);
    draw_line(cr, front_projected[3].x, front_projected[3].y, front_projected[0].x, front_projected[0].y);


    cairo_set_source_rgb(cr, 0.0, 1.0, 0);
    draw_line(cr, back_projected[0].x, back_projected[0].y, back_projected[1].x, back_projected[1].y);
    draw_line(cr, back_projected[1].x, back_projected[1].y, back_projected[2].x, back_projected[2].y);
    draw_line(cr, back_projected[2].x, back_projected[2].y, back_projected[3].x, back_projected[3].y);
    draw_line(cr, back_projected[3].x, back_projected[3].y, back_projected[0].x, back_projected[0].y);

    cairo_set_source_rgb(cr, 0.0, 0.0, 1.0);
    draw_line(cr, back_projected[0].x, back_projected[0].y, front_projected[0].x, front_projected[0].y);
    draw_line(cr, back_projected[1].x, back_projected[1].y, front_projected[1].x, front_projected[1].y);
    draw_line(cr, back_projected[2].x, back_projected[2].y, front_projected[2].x, front_projected[2].y);
    draw_line(cr, back_projected[3].x, back_projected[3].y, front_projected[3].x, front_projected[3].y);
    
}