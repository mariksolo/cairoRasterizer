#include <xcb/xcb.h>
#include <cairo.h>
#include <cairo-xcb.h>
#include "point.h"
#include <unistd.h>
#include <stdio.h>
#include "draw.h"
#include "project_vertex.h"
#include "triangle_list.h"
#include "process_verteces.h"

void draw_screen(cairo_t *cr) {
    struct triangle_list triangles;
    triangles = process_verteces();
}