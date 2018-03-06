//
// Created by Alex O'Donnell on 2018-03-03.
//

#ifndef LOWGO_MODEL_H
#define LOWGO_MODEL_H

#include <vector>
#include "../geometry/geometry.h"
#include "../tga/TGAImage.h"

// This model represents a .obj. It gets parsed and loaded during the constructor
// it also has a bunch of dwg* methods that render them in different ways

class Model {
private:
    std::vector<Vec3f> verts_;            // list of vertices
    std::vector<std::vector<int>> faces_; // list of faces

public:
    Model(const char *filename);
    ~Model();
    int nverts();
    int nfaces();
    Vec3f vert(int i);
    std::vector<int> face(int idx);

    // mesh rendering of a given color
    void dwg0(TGAImage &image, const TGAColor &color, int width, int height);

    // flat shape rendering with random colors
    void dwg1(TGAImage &image, int width, int height);

    // shaded mesh given a light vector
    void dwg2(TGAImage &image, Vec3f light, int width, int height);
};

#endif //LOWGO_MODEL_H
