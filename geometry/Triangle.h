//
// Created by Alex O'Donnell on 2018-03-04.
//

#ifndef LOWGO_TRIANGLE_H
#define LOWGO_TRIANGLE_H


#include "geometry.h"
#include "../tga/TGAImage.h"

class Triangle {

private:
    Vec2i Vertices[3];

public:
    Triangle( Vec2i v0, Vec2i v1, Vec2i v2){
        Vertices[0] = v0;
        Vertices[1] = v1;
        Vertices[2] = v2;
    }

    void draw(TGAImage &image, TGAColor color);
};


#endif //LOWGO_TRIANGLE_H
