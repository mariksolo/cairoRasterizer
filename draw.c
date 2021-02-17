#include <xcb/xcb.h>
#include <unistd.h>
#include <stdio.h>
#include "interpolate.h"
#include "set_drawing_color.h"

void draw_wireframe_triangle(xcb_connection_t *c, xcb_drawable_t drawable, xcb_gcontext_t gcontext, xcb_point_t points[3])
{
    draw_line(c, drawable, gcontext, points[0].x, points[0].y, points[1].x, points[1].y);
    draw_line(c, drawable, gcontext, points[1].x, points[1].y, points[2].x, points[2].y);
    draw_line(c, drawable, gcontext, points[2].x, points[2].y, points[0].x, points[0].y);
}

void draw_filled_triangle(xcb_connection_t *c, xcb_drawable_t drawable, xcb_gcontext_t gcontext, xcb_colormap_t colormap, xcb_point_t points[3], float color_shades[3])
{
    printf("hello world!");
    xcb_point_t tempPoint;
    xcb_point_t point[] = {0, 0};

    float longSideXvals[1920];
    float shortSideXvals1[1920];
    float shortSideXvals2[1920];
    float shortSidesXvals[1920];

    float leftXvals[1920];
    float rightXvals[1920];

    float longSideShades[1920];
    float shortSideShades1[1920];
    float shortSideShades2[1920];
    float shortSidesShades[1920];

    float leftShades[1920];
    float rightShades[1920];

    float calculatedShades[1920];

    float shadedR;
    float shadedG;
    float shadedB;

    if (points[1].y < points[0].y)
    {
        tempPoint = points[1];
        points[1] = points[0];
        points[0] = tempPoint;
    }

    if (points[2].y < points[0].y)
    {
        tempPoint = points[2];
        points[2] = points[0];
        points[0] = tempPoint;
    }

    if (points[2].y < points[1].y)
    {
        tempPoint = points[2];
        points[2] = points[1];
        points[1] = tempPoint;
    }
    printf("color shade 0: %f", 5.0);
    interpolate(points[0].y, points[0].x, points[2].y, points[2].x, longSideXvals);
    interpolate(points[0].y, points[0].x, points[1].y, points[1].x, shortSideXvals1);
    interpolate(points[1].y, points[1].x, points[2].y, points[2].x, shortSideXvals2);

    interpolate(points[0].y, color_shades[0], points[2].y, color_shades[2], longSideShades);
    interpolate(points[0].y, color_shades[0], points[1].y, color_shades[1], shortSideShades1);
    interpolate(points[1].y, color_shades[1], points[2].y, color_shades[2], shortSideShades2);

    // longSideXvals[sizeof(longSideXvals) / sizeof(longSideXvals[0]) - 1] = '\0';

    int i;
    for (i = 0; i < points[1].y - points[0].y + 1; i++)
    {
        shortSidesXvals[i] = shortSideXvals1[i];
        shortSidesShades[i] = shortSideShades1[i];
        printf("shade: %d", shortSidesXvals[i]);
    }

    for (int j = 0; j < points[2].y - points[1].y + 1; j++)
    {
        shortSidesXvals[i + j] = shortSideXvals2[j];
        shortSidesShades[i + j] = shortSideShades2[j];
    }

    int middleIndex = (int)((points[2].y - points[0].y + 1) / 2);
    if (longSideXvals[middleIndex] < shortSidesXvals[middleIndex])
    {
        memcpy(leftXvals, longSideXvals, sizeof(longSideXvals));
        memcpy(rightXvals, shortSidesXvals, sizeof(shortSidesXvals));
        memcpy(leftShades, longSideShades, sizeof(longSideShades));
        memcpy(rightShades, shortSidesShades, sizeof(shortSidesShades));
    }
    else
    {
        memcpy(leftXvals, shortSidesXvals, sizeof(shortSidesXvals));
        memcpy(rightXvals, longSideXvals, sizeof(longSideXvals));
        memcpy(leftShades, shortSidesShades, sizeof(shortSidesShades));
        memcpy(rightShades, longSideShades, sizeof(longSideShades));
    }

    int index = 0;
    for (int y = points[0].y; y <= points[2].y; y++)
    {
        for (int x = leftXvals[index]; x <= rightXvals[index]; x++)
        {
            interpolate(leftXvals[index], leftShades[index], rightXvals[index], rightShades[index], calculatedShades);
            point[0].x = x;
            point[0].y = y;
            shadedR = (float) 65535 * calculatedShades[index];
            shadedG = (float) 65535 * calculatedShades[index];
            shadedB = (float) 0 * calculatedShades[index];

            if (calculatedShades[index] != 0.0)
                printf("shaded R: %f\n", calculatedShades[index]);
            
            set_drawing_color(c, colormap, gcontext, 65535, 65535, shadedB);
            xcb_poly_point(c, XCB_COORD_MODE_ORIGIN, drawable, gcontext, 1, point);
        }

        index++;
    }

    xcb_flush(c);
}

void draw_line(xcb_connection_t *c, xcb_drawable_t drawable, xcb_gcontext_t gcontext, int x1, int y1, int x2, int y2)
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

        xcb_point_t point[] = {0, 0};

        // TODO: make it work like this, so it is more efficient

        // int index = 0;
        // for (int x = x1; x <= x2; x++) {
        //     point[index].x = x;
        //     point[index].y = yVals[index];
        //     index++;
        // }

        // xcb_poly_point(c, XCB_COORD_MODE_ORIGIN, drawable, gcontext, x2 - x1 + 1, point);

        int index = 0;
        for (int x = x1; x <= x2; x++)
        {
            point[0].x = x;
            point[0].y = yVals[index];
            xcb_poly_point(c, XCB_COORD_MODE_ORIGIN, drawable, gcontext, 1, point);
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

        xcb_point_t point[] = {0, 0};

        int index = 0;
        for (int y = y1; y <= y2; y++)
        {
            point[0].x = xVals[index];
            point[0].y = y;
            xcb_poly_point(c, XCB_COORD_MODE_ORIGIN, drawable, gcontext, 1, point);
            index++;
        }

        memset(&xVals[0], 0, sizeof(xVals));
    }

    xcb_flush(c);
}