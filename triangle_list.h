#ifndef TRIANGLE_LIST_H
#define TRIANGLE_LIST_H

#include "point.h"

struct triangle_list {
    struct point verteces[100];
    struct point shades[100];
    struct point colors[3][34];
};

#endif