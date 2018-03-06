//
// Created by Alex O'Donnell on 2018-03-03.
//

#ifndef LOWGO_MODEL_H
#define LOWGO_MODEL_H

#include <vector>
#include "../geometry/geometry.h"

// This model represents a .obj. It gets parsed and loaded during the constructor

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
};

#endif //LOWGO_MODEL_H
