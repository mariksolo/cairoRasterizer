#ifndef SET_DRAWING_COLOR_H
#define SET_DRAWING_COLOR_H

void set_drawing_color(xcb_connection_t *c, xcb_colormap_t colormap, xcb_gcontext_t gcontext, int r, int g, int b);
 
#endif