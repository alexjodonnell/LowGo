//
// Created by Alex O'Donnell on 2018-03-03.
//

#include "DWGTool.h"

// algorithm for drawing a line to a screen
void DWGTool::line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor &color) {

    // steep just means that dy > dx.
    // if the line is steep, we transpose the line during rendering
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    // make it left−to−right
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    // the entire stride
    int dx = x1 - x0;
    int dy = y1 - y0;

    // rate of change of the error
    int derror = std::abs(dy) * 2;

    // prime the loop
    int error = 0;
    int y = y0;

    // travel in the x direction,
    // accumilate error in the y direction,
    // step y up or down once the error is larger than a pixel
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            image.set(y, x, color); // need to reverse transpose if it's steep
        } else {
            image.set(x, y, color);
        }
        error += derror;
        if (error > dx) {
            y += (y1 > y0? 1: -1); // move either up or down
            error -= dx * 2;
        }
    }
}