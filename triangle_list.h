#ifndef TRIANGLE_LIST_H
#define TRIANGLE_LIST_H

#include "point.h"

struct triangle_list {
    int num_triangles;
    struct point_3d verteces[100];
    float shades[100];
    struct color colors[34];
};

#endif