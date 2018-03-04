//
// Created by Alex O'Donnell on 2018-03-04.
//
// Description: a 2 dimentional triangle

#include "Triangle.h"

void Triangle::draw(TGAImage &image, TGAColor color) {
    Vec2i bboxmin(image.get_width()-1,  image.get_height()-1);
    Vec2i bboxmax(0, 0);
    Vec2i clamp(image.get_width()-1, image.get_height()-1);
    for (int i=0; i<3; i++) {
        for (int j=0; j<2; j++) {
            bboxmin[j] = std::max(0, std::min(bboxmin[j], this->Vertices[i][j]));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], this->Vertices[i][j]));
        }
    }
    Vec2i P;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
            Vec3f bc_screen  = barycentric(this->Vertices, P);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
            image.set(P.x, P.y, color);
        }
    }
}

