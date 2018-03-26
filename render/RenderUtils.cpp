//
// Created by Alex O'Donnell on 2018-03-05.
//

#include "RenderUtils.h"

// 2d rasterize for demonstration purposes. Projects a 2d shape into a 1d image
// similar concept to how your screen is a 3d image projected to 2d. y buffer is a
// 1D array
void rasterize(Vec2i p0, Vec2i p1, TGAImage &image, TGAColor color, int ybuffer[]) {

    // draw from left to right
    if (p0.x > p1.x) {
        std::swap(p0, p1);
    }

    // (the following isn't the best line drawing algorithm byt it's good for demonstration)

    // for the path traveling along the x axis,
    for (int x = p0.x; x <= p1.x; x++) {

        // If the current y-value is closer to the camera than the value in the ybuffer,
        // then i draw it on the screen and update the ybuffer.
        float t = (x - p0.x) / (float)(p1.x - p0.x);

        // barycentric hack:
        // (1-t, t) are barycentric coordinates of the point (x,y) with respect to the segment
        // p0, p1: (x,y) = p0*(1-t) + p1*t.
        int y = p0.y*(1. - t) + p1.y * t;

        if (ybuffer[x] < y) {
            ybuffer[x] = y;
            image.set(x, 0, color);
        }
    }
}


