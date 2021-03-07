#include <xcb/xcb.h>
#include "point.h"
#include <unistd.h>
#include <stdio.h>
#include "interpolate.h"
#include <cairo/cairo.h>
#include <cairo/cairo-xcb.h>

void draw_wireframe_triangle(cairo_t *cr, struct point points[3])
{

    //TODO figure out why this is broken

    draw_line(cr, points[0].x, points[0].y, points[1].x, points[1].y);
    draw_line(cr, points[1].x, points[1].y, points[2].x, points[2].y);
    draw_line(cr, points[2].x, points[2].y, points[0].x, points[0].y);
}

void draw_filled_triangle(cairo_t *cr, struct point_3d points[3], float color_shades[3], float color[3], float depth_buffer[1920][1076])
{
    // printf("hello world!");
    struct point_3d tempPoint;
    float tempShade;

    float longSideXvals[1000];
    float shortSideXvals1[1000];
    float shortSideXvals2[1000];
    float shortSidesXvals[1000];

    float leftXvals[1000];
    float rightXvals[1000];

    float longSideShades[1000];
    float shortSideShades1[1000];
    float shortSideShades2[1000];
    float shortSidesShades[1000];

    float leftShades[1000];
    float rightShades[1000];

    float leftZvals[1000];
    float rightZvals[1000];

    float longSideZvals[1000];
    float shortSideZvals1[1000];
    float shortSideZvals2[1000];
    float shortSidesZvals[1000];

    float calculatedShades[1000];
    float calculatedZvals[1000];

    float shadedR;
    float shadedG;
    float shadedB;

    if (points[1].y < points[0].y)
    {
        tempPoint = points[1];
        points[1] = points[0];
        points[0] = tempPoint;

        tempShade = color_shades[1];
        color_shades[1] = color_shades[0];
        color_shades[0] = tempShade;
    }

    if (points[2].y < points[0].y)
    {
        tempPoint = points[2];
        points[2] = points[0];
        points[0] = tempPoint;

        tempShade = color_shades[2];
        color_shades[2] = color_shades[0];
        color_shades[0] = tempShade;
    }

    if (points[2].y < points[1].y)
    {
        tempPoint = points[2];
        points[2] = points[1];
        points[1] = tempPoint;

        tempShade = color_shades[2];
        color_shades[2] = color_shades[1];
        color_shades[1] = tempShade;
    }
    // printf("color shade 0: %f", 5.0);
    interpolate(points[0].y, points[0].x, points[2].y, points[2].x, longSideXvals);
    interpolate(points[0].y, points[0].x, points[1].y, points[1].x, shortSideXvals1);
    interpolate(points[1].y, points[1].x, points[2].y, points[2].x, shortSideXvals2);

    interpolate(points[0].y, color_shades[0], points[2].y, color_shades[2], longSideShades);
    interpolate(points[0].y, color_shades[0], points[1].y, color_shades[1], shortSideShades1);
    interpolate(points[1].y, color_shades[1], points[2].y, color_shades[2], shortSideShades2);

    // Z vals are fundamentally different from x vals. They do not represent the z of the pixel - pixels are
    // already 2d here. They are the z value of the point they represent, necessary to fill the depth_buffer
    // for hidden surface removal.
    interpolate(points[0].y, points[0].z, points[2].y, points[2].z, longSideZvals);
    interpolate(points[0].y, points[0].z, points[1].y, points[1].z, shortSideZvals1);
    interpolate(points[1].y, points[1].z, points[2].y, points[2].z, shortSideZvals2);

    // longSideXvals[sizeof(longSideXvals) / sizeof(longSideXvals[0]) - 1] = '\0';

    int i;
    for (i = 0; i < points[1].y - points[0].y + 1; i++)
    {
        shortSidesXvals[i] = shortSideXvals1[i];
        shortSidesShades[i] = shortSideShades1[i];
        shortSidesZvals[i] = shortSideZvals1[i];
        // printf("shade: %f\n", shortSidesShades[i]);
    }

    for (int j = 0; j < points[2].y - points[1].y + 1; j++)
    {
        shortSidesXvals[i + j] = shortSideXvals2[j];
        shortSidesShades[i + j] = shortSideShades2[j];
        shortSidesZvals[i + j] = shortSideZvals2[j];
        // printf("shade: %f\n", shortSidesShades[i + j]);
    }

    // TODO get rid of memcpy's and make more efficient - this is probably what is killing performance
    int middleIndex = (int)((points[2].y - points[0].y + 1) / 2);
    if (longSideXvals[middleIndex] < shortSidesXvals[middleIndex])
    {
        memcpy(leftXvals, longSideXvals, sizeof(longSideXvals));
        memcpy(rightXvals, shortSidesXvals, sizeof(shortSidesXvals));
        memcpy(leftShades, longSideShades, sizeof(longSideShades));
        memcpy(rightShades, shortSidesShades, sizeof(shortSidesShades));
        memcpy(leftZvals, longSideZvals, sizeof(longSideZvals));
        memcpy(rightZvals, shortSidesZvals, sizeof(shortSidesZvals));
    }
    else
    {
        memcpy(leftXvals, shortSidesXvals, sizeof(shortSidesXvals));
        memcpy(rightXvals, longSideXvals, sizeof(longSideXvals));
        memcpy(leftShades, shortSidesShades, sizeof(shortSidesShades));
        memcpy(rightShades, longSideShades, sizeof(longSideShades));
        memcpy(leftZvals, shortSidesZvals, sizeof(shortSidesZvals));
        memcpy(rightZvals, longSideZvals, sizeof(longSideZvals));
    }

    int index = 0;
    // TODO change name of shadeIndex to include use for Zvals too
    int shadeIndex = 0;
    float previous_depth;
    for (float y = points[0].y; y <= points[2].y; y++)
    {
        interpolate(leftXvals[index], leftShades[index], rightXvals[index], rightShades[index], calculatedShades);
        interpolate(leftXvals[index], leftZvals[index], rightXvals[index], rightZvals[index], calculatedZvals);
        shadeIndex = 0;
        for (float x = leftXvals[index]; x <= rightXvals[index]; x++)
        {
                // fprintf(stdout, "Hello?");

            previous_depth = depth_buffer[(int)x][(int)y];
            if (!previous_depth || (calculatedZvals[shadeIndex] > previous_depth))
            {
                shadedR = (float)color[0] * calculatedShades[shadeIndex];
                shadedG = (float)color[1] * calculatedShades[shadeIndex];
                shadedB = (float)color[2] * calculatedShades[shadeIndex];
                // shadedR = (float)color[0];
                // shadedG = (float)color[1];
                // shadedB = (float)color[2];

                // if (calculatedShades[index] != 1.0)
                // printf("calculatedShades[index]: %f\n", calculatedShades[shadeIndex]);
                cairo_set_source_rgb(cr, shadedR, shadedG, shadedB);
                cairo_rectangle(cr, x, y, 1.0, 1.0);
                

                depth_buffer[(int)x][(int)y] = calculatedZvals[shadeIndex];
            }

            shadeIndex++;
        }

        index++;
    }
    cairo_fill(cr);
}

void draw_line(cairo_t *cr, int x1, int y1, int x2, int y2)
{

    if (abs(x2 - x1) > abs(y2 - y1))
    {

        if (x1 > x2)
        {
            int temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }

        float yVals[1920];
        interpolate(x1, y1, x2, y2, yVals);

        int index = 0;
        for (int x = x1; x <= x2; x++)
        {

            cairo_rectangle(cr, x, yVals[index], 1.0, 1.0);
            cairo_fill(cr);

            index++;
        }

        memset(&yVals[0], 0, sizeof(yVals));
    }
    else
    {

        if (y1 > y2)
        {
            int temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
        float xVals[1920];
        interpolate(y1, x1, y2, x2, xVals);

        int index = 0;
        for (int y = y1; y <= y2; y++)
        {

            cairo_rectangle(cr, xVals[index], y, 1.0, 1.0);
            cairo_fill(cr);

            index++;
        }

        memset(&xVals[0], 0, sizeof(xVals));
    }
}