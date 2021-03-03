#ifndef TRIANGLE_LIST_H
#define TRIANGLE_LIST_H

#include "point.h"

struct triangle_list {
    struct point_3d verteces[100];
    float shades[100];
    float colors[3][34];
};

#endif