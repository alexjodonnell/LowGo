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

using namespace std;

class Model {
private:
    std::vector<Vec3f> verts_;            // list of vertices
    std::vector<std::vector<Vec3i>> faces_; // list of faces (vertex/texture/normal)
    std::vector<Vec2f> texts_; // list of textures

    TGAImage diffusemap_;
    void load_texture(std::string filename, TGAImage &img);

public:
    Model(const char *filename, const char *texturefile);
    ~Model();
    int nverts();
    int nfaces();
    int ntexts();
    Vec3f vert(int i);
    std::vector<int> face(int idx);
    Vec2i text(int iface, int nvert);
    std::vector<Vec3f> norms_;


    TGAColor diffuse(Vec2i uv);

    // mesh rendering of a given color
    void dwg0(TGAImage &image, const TGAColor &color);

    // flat shape rendering with random colors with z buffer
    void dwg1(TGAImage &image);

    // shaded mesh given a light vector
    void dwg2(TGAImage &image, Vec3f light);

    // shaded mesh given a light vector with z buffer
    void dwg3(TGAImage &image, Vec3f light);

    // coloured, shaded mesh given a light vector with z buffer
    void dwg4(TGAImage &image, const TGAColor &color, Vec3f light);

    // flat shaded mesh with a texture
    void dwg5(TGAImage &image, Vec3f light);

    // mesh in perspective with a texture
    void dwg6(TGAImage &image, Vec3f light, Vec3f camera, int depth, float *zbuffer);

    // draws the model in perspective with a given depth and eyeball
    void dwg7(TGAImage &image, Vec3f light, Vec3f eye, int depth, float *zbuffer);
};

#endif //LOWGO_MODEL_H
