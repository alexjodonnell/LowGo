//
// Created by Alex O'Donnell on 2018-03-03.
//

#include "DWGTool.h"

// very poor algorithm for drawing a line to a screen
void DWGTool::badline(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    // for the path of our line, travelling in increments of .01, draw a single pixel
    for (float t=0.; t<1.; t+=.01) {
        int x = x0*(1.-t) + x1*t;
        int y = y0*(1.-t) + y1*t;
        image.set(x, y, color);
    }
}

// another very poor algorithm for drawing a line to a screen with variable resolution
void DWGTool::badline(int x0, int y0, int x1, int y1, int res, TGAImage &image, TGAColor color) {
    // for the path of our line, travelling in increments of res, draw a single pixel
    for (float t=0.; t<1.; t+=.01) {
        int x = x0*(1.-t) + x1*t;
        int y = y0*(1.-t) + y1*t;
        image.set(x, y, color);
    }
}