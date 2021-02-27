#include <stdio.h>
#include "triangle_list.h"

struct triangle_list process_verteces() {
    struct triangle_list triangles;
    FILE *fp;

    fp = fopen("./triangles.txt", "r");
    fprintf(fp, "This is testing for fprintf...\n");
    fputs("This is testing for fputs...\n", fp);
    fclose(fp);

    return triangles;
}