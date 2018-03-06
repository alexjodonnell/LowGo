//
// Created by Alex O'Donnell on 2018-03-03.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Model.h"
#include "../tga/DWGTool.h"

// constructor that takes in a filename
Model::Model(const char *filename) : verts_(), faces_() {

    // todo: this code assumes that there are texture coordinates. You should fix this so that when a face is parsed that doesn't have it, it won't break the code
    // todo: (cont) the best workaround to open a model without texture coordinates is to replace all "//" with "/1/"

    std::ifstream in;
    in.open (filename, std::ifstream::in);

    if (in.fail()){
        std::cerr << "File: \"" << filename << "\", not found." << std::endl;
        return;
    }

    // parse the file line by line
    std::string line;
    while (! in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) { // look for a vertex
            iss >> trash;

            // parse it into a 3d vector object
            Vec3f v;
            for (int i = 0; i < 3; i++) {
                iss >> v.raw[i];
            }

            // push it on to the list of the models vertices
            verts_.push_back(v);

        } else if (!line.compare(0, 2, "f ")) { // look for a face

            std::vector<int> f;
            int itrash, idx;
            iss >> trash;

            // parse faces
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
            }

            // push it onto the list of faces
            faces_.push_back(f);
        }
    }
    // log the amount of vertices and faces todo remove me
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    return faces_[idx];
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

// fun ways to draw the model

void Model::dwg0(TGAImage &image, const TGAColor &color, int width, int height){

        // this block draws the mesh of a model object
    // for each face in the model,
    for (int i=0; i < this->nfaces(); i++) {
        std::vector<int> face = this->face(i);

        // for each cooresponding vertex,
        for (int j = 0; j < 3; j++) {
            Vec3f v0 = this->vert(face[j]);
            Vec3f v1 = this->vert(face[(j + 1) % 3]);
            int x0 = (v0.x + 1.) * width/2.;
            int y0 = (v0.y + 1.) * height/2.;
            int x1 = (v1.x + 1.) * width/2.;
            int y1 = (v1.y + 1.) * height/2.;

            // draw a line from vertex to vertex
            line(x0, y0, x1, y1, image, color);
        }
    }
}

// flat shape rendering with random colors
void Model::dwg1(TGAImage &image, int width, int height){
    float *zbuffer = new float[width*height];
    for (int i=width*height; i--; zbuffer[i] = -std::numeric_limits<float>::max());
    Vec3f world_coords[3];

    for (int i=0; i<this->nfaces(); i++) {
        std::vector<int> face = this->face(i);
        Vec3f pts[3];
        for (int i=0; i<3; i++) pts[i] = world2screen(this->vert(face[i]), width, height);
        triangle(pts, zbuffer, image, TGAColor(rand()%255, rand()%255, rand()%255, 255), width);
    }
}

// shaded mesh given a light vector (still no z buffer)
void Model::dwg2(TGAImage &image, Vec3f light, int width, int height){

    for (int i = 0; i < this->nfaces(); i++) {
        std::vector<int> face = this->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];

        for (int j=0; j<3; j++) {
            Vec3f v = this->vert(face[j]);
            screen_coords[j] = Vec2i((v.x+1.)*width/2., (v.y+1.)*height/2.);
            world_coords[j]  = v;
        }

        Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
        n.normalize();
        float intensity = n * light;

         // if the dot product is negative, the light is coming from behing the shape,
         // let's not draw it! this is called black face culling
        if (intensity > 0) {
             triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(intensity*255, intensity*255, intensity*255, 255));
        }
    }
}