#include <xcb/xcb.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xcb.h>
#include "point.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "draw.h"
#include "project_vertex.h"
#include "triangle_list.h"
#include "process_verteces.h"
#include "move_camera.h"

void draw_screen(cairo_t *cr, struct triangle_list triangles, float depth_buffer[1920][1076])
{
    struct point_3d temp_points[3];
    struct point_3d projected_points[3];
    float shades[3] = {1.0, 1.0, 1.0};
    // float depth_buffer[1920][1076];

    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_paint(cr);

    cairo_rectangle(cr, 0.0, 0.0, 30.0, 30.0);

    float d = 20;
    float v_w = 160;
    float v_h = 90;

    projected_points[0] = project_vertex(triangles.verteces[0], d, v_w, v_h);
    projected_points[1] = project_vertex(triangles.verteces[1], d, v_w, v_h);
    projected_points[2] = project_vertex(triangles.verteces[2], d, v_w, v_h);

    int current_triangle = 0;
    float color[3];

    for (int i = 0; i < triangles.num_triangles * 3; i += 3)
    {
        for (int j = 0; j < 3; j++)
        {
            projected_points[j] = project_vertex(triangles.verteces[i + j], d, v_w, v_h);
            shades[j] = triangles.shades[i + j];
        }

        color[0] = triangles.colors[current_triangle].r;
        color[1] = triangles.colors[current_triangle].g;
        color[2] = triangles.colors[current_triangle].b;

        draw_filled_triangle(cr, projected_points, shades, color, depth_buffer);

        current_triangle++;
    }

    
    // Fixes issue
    for (int i = 0; i < 1920; i++) {
        for (int j = 0; j < 1076; j++) {
            depth_buffer[i][j] = 0.0;
        }
    }
}