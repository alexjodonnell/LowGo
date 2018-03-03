//
// Created by Alex O'Donnell on 2018-03-03.
//

#include "DWGTool.h"

// algorithm for drawing a line to a screen
void DWGTool::line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {

    for (int x=x0; x<=x1; x++) {
        // calculate stride based on the amount of pixels we need to draw
        float t = (x-x0)/(float)(x1-x0);
        int y = y0*(1.-t) + y1*t;
        image.set(x, y, color);
    }
}