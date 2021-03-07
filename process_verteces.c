#include <stdio.h>
#include <stdlib.h>
#include "triangle_list.h"

struct triangle_list process_verteces()
{
    struct triangle_list triangles;
    struct point_3d verteces[100];
    float shades[100];
    struct color colors[34];
    FILE *fp;

    fp = fopen("./triangles.txt", "r");

    char current_value[20];

    float current_float;
    struct point_3d current_vertex;
    int num_triangles;
    int vertex_count = 0;
    int shade_count = 0;
    int color_count = 0;

    int c; // note: int, not char, required to handle EOF
    int i = 0;
    int prev_val = 'a'; // Anything except /n
    // Get number of triangles in file
    while ((c = fgetc(fp)) != EOF)
    { // standard C I/O file reading loop
        if (c == prev_val && c == '\n')
        {
            break;
        }

        current_value[i] = c;
        i++;
        prev_val = c;
    }

    num_triangles = atoi(current_value);

    i = 0;
    prev_val = 'a';
    int temp_vertex_count = 0;

    // TODO figure out why the hell this works with linebreaks even though I never added that feature
    // Get verteces of triangle (9 coordinates per triangle)
    while ((c = fgetc(fp)) != EOF)
    { // standard C I/O file reading loop

        if (c == prev_val && c == '\n')
        {
            current_float = atof(current_value);
            current_vertex.z = current_float;
            temp_vertex_count = 0;
            triangles.verteces[vertex_count] = current_vertex;
            vertex_count++;

            break;
        }

        if (c == ',')
        {

            current_float = atof(current_value);
            i = 0;
            // prev_val = 'a';

            switch (temp_vertex_count)
            {
            case 0:
                current_vertex.x = current_float;
                temp_vertex_count++;

                break;
            case 1:
                current_vertex.y = current_float;
                temp_vertex_count++;

                break;
            case 2:
                current_vertex.z = current_float;

                temp_vertex_count = 0;
                triangles.verteces[vertex_count] = current_vertex;
                vertex_count++;

                break;
            }

            memset(&current_value[0], 0, sizeof(current_value));
        }
        else
        {
            current_value[i] = c;
            i++;
            prev_val = c;
        }
    }
    memset(&current_value[0], 0, sizeof(current_value));

    for (i = 0; i < num_triangles; i++) {
        fprintf(stdout, "%f, %f, %f\n", verteces[i].x, verteces[i].y, verteces[i].z);
    }
    prev_val = 'a';
    i = 0;
    while ((c = fgetc(fp)) != EOF)
    { // standard C I/O file reading loop
        if (c == prev_val && c == '\n')
        {
            current_float = atof(current_value);
            triangles.shades[shade_count] = current_float;
            shade_count++;
            break;
        }

        if (c == ',')
        {
            current_float = atof(current_value);
            i = 0;
            triangles.shades[shade_count] = current_float;
            shade_count++;
        }
        else
        {
            current_value[i] = c;
            i++;
            prev_val = c;
        }
    };

    // for (i = 0; i < num_triangles * 3; i++) {
    //     fprintf(stdout, "%f\n", shades[i]);
    // }
    memset(&current_value[0], 0, sizeof(current_value));

    prev_val = 'a';
    i = 0;
    int temp_color_count = 0;
    struct color current_color;
    while ((c = fgetc(fp)) != EOF)
    { // standard C I/O file reading loop


        if (c == '\n')
        {
            current_float = atof(current_value);
            current_color.b = current_float;
            temp_color_count = 0;
            triangles.colors[color_count] = current_color;
            color_count++;

            break;
        }

        if (c == ',')
        {

            current_float = atof(current_value);
            i = 0;
            // prev_val = 'a';

            switch (temp_color_count)
            {
            case 0:
                current_color.r = current_float;
                temp_color_count++;

                break;
            case 1:
                current_color.g = current_float;
                temp_color_count++;

                break;
            case 2:
                current_color.b = current_float;
                temp_color_count = 0;
                triangles.colors[color_count] = current_color;
                color_count++;

                break;
            }

            memset(&current_value[0], 0, sizeof(current_value));
        }
        else
        {
            current_value[i] = c;
            i++;
            prev_val = c;
        }

    }

    triangles.num_triangles = num_triangles;


    fclose(fp);
    return triangles;
}