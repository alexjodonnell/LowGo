//
// Created by Alex O'Donnell on 2018-03-03.
//

#ifndef LOWGO_DWGTOOLS_H
#define LOWGO_DWGTOOLS_H

// this library has tools to draw on a TGA image

#include "TGAImage.h"
#include "../geometry/geometry.h"

class DWGTool {
public:

    // algorithm for drawing a line to a screen
    static void line(int x0, int y0, int x1, int y1, TGAImage &image, const TGAColor &color);

    // algorithm for drawing a line to a screen
    static void line(Vec2i p1, Vec2i p2, TGAImage &image, const TGAColor &color);

    // algorithm for drawing a line to a screen
    static void triangle(Vec2i p1, Vec2i p2, Vec2i p3, TGAImage &image, const TGAColor &color);

};


#endif //LOWGO_DWGTOOLS_H
