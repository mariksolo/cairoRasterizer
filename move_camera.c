#include "triangle_list.h"
#include "point.h"

struct triangle_list translate_camera(struct triangle_list triangles, float x_vector, float y_vector, float z_vector) {
    for (int i = 0; i < triangles.num_triangles * 3; i++) {
        triangles.verteces[i].x -= x_vector;
        triangles.verteces[i].y -= y_vector;
        triangles.verteces[i].z -= z_vector;
    }

    return triangles;
}