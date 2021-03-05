#include "point.h"

struct point_3d project_vertex(struct point_3d p, float view_port_distance, float view_port_width, float view_port_height) {
    struct point_3d projected_p;

    float view_port_x = p.x * view_port_distance / p.z;
    float view_port_y = p.y * view_port_distance / p.z;
    
    float canvas_x = view_port_x * 1920 / view_port_width;
    float canvas_y = view_port_y * 1080 / view_port_height;

    projected_p.x = canvas_x;
    projected_p.y = canvas_y;
    projected_p.z = p.z;

    return projected_p;
}