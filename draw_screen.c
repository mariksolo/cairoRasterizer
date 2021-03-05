#include <xcb/xcb.h>
#include <cairo.h>
#include <cairo-xcb.h>
#include "point.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "draw.h"
#include "project_vertex.h"
#include "triangle_list.h"
#include "process_verteces.h"

void draw_screen(cairo_t *cr)
{
    struct triangle_list triangles;
    triangles = process_verteces();
    struct point_3d temp_points[3];
    struct point_3d projected_points[3];
    float shades[3] = {1.0, 1.0, 1.0};
    float depth_buffer[1920][1076];
    

    float d = 20;
    float v_w = 160;
    float v_h = 90;
    projected_points[0] = project_vertex(triangles.verteces[0], d, v_w, v_h);
    projected_points[1] = project_vertex(triangles.verteces[1], d, v_w, v_h);
    projected_points[2] = project_vertex(triangles.verteces[2], d, v_w, v_h);

    

    // draw_wireframe_triangle(cr, projected_points);


    int current_triangle = 0;
    float color[3];
    for (int i = 0; i < triangles.num_triangles * 3; i += 3)
    {
        for (int j = 0; j < 3; j++)
        {
            projected_points[j] = project_vertex(triangles.verteces[i + j], d, v_w, v_h);
            shades[j] = triangles.shades[i + j];
        }
        // cairo_set_source_rgb(cr, triangles.colors[current_triangle].r, triangles.colors[current_triangle].g, triangles.colors[current_triangle].b);
        // draw_line(cr, projected_points[0].x, projected_points[0].y, projected_points[1].x, projected_points[1].y);
        // draw_line(cr, projected_points[1].x, projected_points[1].y, projected_points[2].x, projected_points[2].y);
        // draw_line(cr, projected_points[2].x, projected_points[2].y, projected_points[0].x, projected_points[0].y);


        color[0] = triangles.colors[current_triangle].r;
        color[1] = triangles.colors[current_triangle].g;
        color[2] = triangles.colors[current_triangle].b;

        draw_filled_triangle(cr, projected_points, shades, color, depth_buffer);

        current_triangle++;
    }
}