//
// Created by Alex O'Donnell on 2018-03-03.
//

#ifndef LOWGO_MODEL_H
#define LOWGO_MODEL_H

#include <vector>
#include "geometry.h"
#include "TGAImage.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>

// This model represents a .obj. It gets parsed and loaded during the constructor
// it also has a bunch of dwg* methods that render them in different ways

using namespace std;

class Model {
private:
    std::vector<Vec3f> verts_;              // list of vertices
    std::vector<std::vector<Vec3i>> faces_; // list of faces (vertex/texture/normal)
    std::vector<Vec2f> uv_;                 // list of texture coords
    std::vector<Vec3f> norms_;              // list of normals

    TGAImage diffusemap_;
    TGAImage normalmap_;
    TGAImage specularmap_;
    void load_texture(std::string filename, TGAImage &img);

public:
    Model(const char * filename, const char * texturefile, const char * normalfile, const char * speculfile);

    ~Model();

    int nverts();

    int nfaces();

    Vec3f normal(int iface, int nthvert);

    Vec3f normal(Vec2f uv);

    Vec3f vert(int i);

    Vec3f vert(int iface, int nthvert);

    Vec2f uv(int iface, int nthvert);

    TGAColor diffuse(Vec2f uv);

    float specular(Vec2f uv);

    std::vector<int> face(int idx);

};

#endif //LOWGO_MODEL_H
