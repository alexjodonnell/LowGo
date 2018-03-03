//
// Created by Alex O'Donnell on 2018-03-03.
//

#include "DWGTool.h"

// algorithm for drawing a line to a screen
void DWGTool::line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {

    bool steep = false;
    // steep just means that dy > dx
    if (std::abs(x0-x1)<std::abs(y0-y1)) { // if the line is steep, we transpose the image
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) { // make it left−to−right
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    for (int x=x0; x<=x1; x++) {
        float t = (x-x0)/(float)(x1-x0);
        int y = y0*(1.-t) + y1*t;
        if (steep) {
            image.set(y, x, color); // if transposed, de−transpose
        } else {
            image.set(x, y, color);
        }
    }
}