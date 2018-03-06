//
// Created by Alex O'Donnell on 2018-03-05.
//

#ifndef LOWGO_RENDERER_H
#define LOWGO_RENDERER_H

#include "../geometry/geometry.h"
#include "../tga/TGAImage.h"


// 2d rasturize for proof of concept
void rasterize(Vec2i p0, Vec2i p1, TGAImage &image, TGAColor color, int ybuffer[]);

#endif //LOWGO_RENDERER_H
