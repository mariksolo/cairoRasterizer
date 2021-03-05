#ifndef DRAW__H
#define DRAW__H
 
void draw_line(cairo_t *cr, int x1, int y1, int x2, int y2);
void draw_wireframe_triangle(cairo_t *cr, struct point points[3]);
void draw_filled_triangle(cairo_t *cr, struct point_3d points[3], float color_shades[3], float color[3], float depth_buffer[1920][1076]);
 
#endif

