#ifndef DRAW__H
#define DRAW__H
 
void draw_line(xcb_connection_t *c, xcb_drawable_t drawable, xcb_gcontext_t gcontext, int x1, int y1, int x2, int y2);
void draw_wireframe_triangle(xcb_connection_t *c, xcb_drawable_t drawable, xcb_gcontext_t gcontext, xcb_point_t points[3]);
void draw_filled_triangle(xcb_connection_t *c, xcb_drawable_t drawable, xcb_gcontext_t gcontext, xcb_colormap_t colormap, xcb_point_t points[3], float color_shades[3]);
 
#endif

