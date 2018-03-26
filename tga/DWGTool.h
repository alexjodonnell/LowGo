//
// Created by Alex O'Donnell on 2018-03-03.
//
// Description: This library has tools to draw on a TGA image. Most of them are just for playing around with different
// algorithms and eventually everything of interest in here will implemented with a stronger OO paradigm. A lot of the
// OO parallels are also further optimized and/or abstracted than these functions

#ifndef LOWGO_DWGTOOLS_H
#define LOWGO_DWGTOOLS_H

#include <vector>
#include "TGAImage.h"
#include "../geometry/geometry.h"
#include "../model/Model.h"


// algorithm for drawing a line to a screen
    void line(int x0, int y0, int x1, int y1, TGAImage &image, const TGAColor &color);

    // algorithm for drawing a line to a screen
    void line(Vec2i p1, Vec2i p2, TGAImage &image, const TGAColor &color);

    // algorithm for drawing a uniform triangle to a screen
    void triangle(Vec2i p1, Vec2i p2, Vec2i p3, TGAImage &image, const TGAColor &color);

    // triangle that implements z buffer so that shapes that are blocked by other objects aren't drawn
    void triangle(Vec3f *pts, float *zbuffer, TGAImage &image, TGAColor color);

    // triangle pulling from diffuse texture
    void triangle(Vec3f *verts, Vec2i *texts, float *zbuffer, float intensity, TGAImage &image, Model * model);

    // triangle pulling from diffuse texture
    void triangle(Vec3i *verts, Vec2i *texts, float *zbuffer, float intensity, TGAImage &image, Model * model);


#endif //LOWGO_DWGTOOLS_H
