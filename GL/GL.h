//
// Created by Alex O'Donnell on 2018-03-26.
//

#ifndef LOWGO_GL_H
#define LOWGO_GL_H

#include "GL.h"

#include <limits>
#include "../shared/geometry.h"
#include "../shared/TGAImage.h"

extern Matrix ModelView;
extern Matrix Viewport;
extern Matrix Projection;

void viewport(int x, int y, int w, int h);
void projection(float coeff=0.f); // coeff = -1/c
void lookat(Vec3f eye, Vec3f center, Vec3f up);

struct IShader {
    virtual ~IShader();
    virtual Vec4f vertex(int iface, int nthvert) = 0;
    virtual bool fragment(Vec3f bar, TGAColor &color) = 0;
};

void triangle(Vec4f *pts, IShader &shader, TGAImage &image, TGAImage &zbuffer);


#endif //LOWGO_GL_H
