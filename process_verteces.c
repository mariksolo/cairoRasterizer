#include <stdio.h>
#include <stdlib.h>
#include "triangle_list.h"

struct triangle_list process_verteces() {
    struct triangle_list triangles;
    struct point_3d verteces[100];
    float shades[100];
    float colors[3][34];
    FILE *fp;

    fp = fopen("./triangles.txt", "r");
    char current_value[20];
    float current_float;
    struct point_3d current_vertex;
    int num_triangles;
    int vertex_count = 0;

    int c; // note: int, not char, required to handle EOF
    int i = 0;
    int prev_val = "a"; // Anything except /n
    // Get number of triangles in file
    while ((c = fgetc(fp)) != EOF) { // standard C I/O file reading loop
        if (c == prev_val && c == '\n') {
            break;
        }   

        current_value[i] = c;
        i++;
        prev_val = c;
    }

    num_triangles = atoi(current_value);
    

    i = 0;
    prev_val = "a";
    int temp_vertex_count = 0;
    int count = 0;
    // Get verteces of triangle (9 coordinates per triangle)
    while ((c = fgetc(fp)) != EOF) { // standard C I/O file reading loop
        if (c == prev_val && c == '\n') {
            current_float = atof(current_value);
            current_vertex.z = current_float;
            temp_vertex_count = 0;
            verteces[vertex_count] = current_vertex;
            vertex_count++;
            count++;
            break;
        }
        putchar(c);

        if (c == ',') {
            
            
            current_float = atof(current_value);
            i = 0;
            prev_val = "a";
            
            switch (temp_vertex_count) {
                case 0:
                    current_vertex.x = current_float;
                    temp_vertex_count++;
                    count++;
                    break;
                case 1:
                    current_vertex.y = current_float;
                    temp_vertex_count++;
                    count++;
                    break;
                case 2:
                    current_vertex.z = current_float;
                    temp_vertex_count = 0;
                    verteces[vertex_count] = current_vertex;
                    vertex_count++;
                    count++;
                    break;
            }

            // TODO: Currently only works if all floats have same number of characters
            
        } else {
            current_value[i] = c;
            i++;
            prev_val = c;
        }

        
        
    }
    fprintf(stdout, "count: %d\n", count);

    for(i = 0; i < num_triangles * 3; i++) {
        fprintf(stdout, "x: %f, ", verteces[i].x);
        fprintf(stdout, "y: %f, ", verteces[i].y);
        fprintf(stdout, "z: %f\n", verteces[i].z);
    }

    fclose(fp);
    return triangles;
}