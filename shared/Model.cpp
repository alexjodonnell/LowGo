//
// Created by Alex O'Donnell on 2018-03-03.
//


#include "Model.h"


// constructor that takes in a filename
Model::Model(const char *filename, const char *texturefile, const char *normalfile, const char *speculfile) : verts_(), faces_() {

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
                iss >> v[i];
            }

            // push it on to the list of the models vertices
            verts_.push_back(v);

        }else if (!line.compare(0, 2, "vt")) { // look for a texture coordinate
            iss >> trash >> trash;
            Vec2f uv;
            for (int i=0;i<2;i++) iss >> uv[i];
            uv_.push_back(uv);

        }else if (!line.compare(0, 2, "vn")) { // look for a vertex normal
            iss >> trash >> trash;
            Vec3f n;
            for (int i=0;i<3;i++) iss >> n[i];
            norms_.push_back(n);

        }else if (!line.compare(0, 2, "f ")) { // look for a face

            std::vector<Vec3i> f;
            Vec3i tmp;
            iss >> trash;

            // parse the entire row to get the entire face with textures
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
                for (int i=0; i < 3; i++){
                    tmp[i]--; // in wavefront obj all indices start at 1, not zero
                }
                f.push_back(tmp);
            }

            // push it onto the list of faces
            faces_.push_back(f);

        }else if  (!line.compare(0, 1, "#")) { // look for a comment
            std::cerr << "# comment: " << line << std::endl;
        }
    }
    // log the amount of vertices and faces todo remove me if I become annoying
    std::cerr << "# v# " << verts_.size() << " vt# " << uv_.size() <<" f# "  << faces_.size() << " vn# " << norms_.size() << std::endl;
    // todo add an "isTextured" field
    load_texture(texturefile, diffusemap_);
    load_texture(normalfile, normalmap_);
    load_texture(speculfile, specularmap_);
}

Model::~Model() {}
void Model::load_texture(std::string filename, TGAImage &img) {
        std::cerr << "texture file " << filename << " loading " << (img.read_tga_file(filename.c_str()) ? "ok" : "failed") << std::endl;
        img.flip_vertically();
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    std::vector<int> face;
    for (int i=0; i<(int)faces_[idx].size(); i++) face.push_back(faces_[idx][i][0]);
    return face;
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

Vec3f Model::vert(int iface, int nthvert) {
    return verts_[faces_[iface][nthvert][0]];
}

TGAColor Model::diffuse(Vec2f uvf) {
    Vec2i uv(uvf[0]*diffusemap_.get_width(), uvf[1]*diffusemap_.get_height());
    return diffusemap_.get(uv[0], uv[1]);
}

Vec3f Model::normal(Vec2f uvf) {
    Vec2i uv(uvf[0]*normalmap_.get_width(), uvf[1]*normalmap_.get_height());
    TGAColor c = normalmap_.get(uv[0], uv[1]);
    Vec3f res;
    for (int i=0; i<3; i++)
        res[2-i] = (float)c[i]/255.f*2.f - 1.f;
    return res;
}

Vec2f Model::uv(int iface, int nthvert) {
    return uv_[faces_[iface][nthvert][1]];
}

float Model::specular(Vec2f uvf) {
    Vec2i uv(uvf[0]*specularmap_.get_width(), uvf[1]*specularmap_.get_height());
    return specularmap_.get(uv[0], uv[1])[0]/1.f;
}

Vec3f Model::normal(int iface, int nthvert) {
    int idx = faces_[iface][nthvert][2];
    return norms_[idx].normalize();
}
