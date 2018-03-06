//
// Created by Alex O'Donnell on 2018-03-03.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

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
