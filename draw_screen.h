#ifndef DRAW_SCREEN_H
#define DRAW_SCREEN_H

#include "triangle_list.h"
void draw_screen(cairo_t *cr, struct triangle_list triangles, float depth_buffer[1920][1076]);
 
#endif

