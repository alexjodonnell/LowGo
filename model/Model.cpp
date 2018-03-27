//
// Created by Alex O'Donnell on 2018-03-03.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include "../tga/DWGTool.h"


// constructor that takes in a filename
Model::Model(const char *filename, const char *texturefile, const char *normalfile) : verts_(), faces_() {

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
            texts_.push_back(uv);

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
    std::cerr << "# v# " << verts_.size() << " vt# " << texts_.size() <<" f# "  << faces_.size() << std::endl;
    // todo add an "isTextured" field
    load_texture(texturefile, diffusemap_);
    load_texture(normalfile, normalmap_);
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

int Model::ntexts() {
    return (int)texts_.size();
}

//Vec3f Model::normal(int iface, int nthvert) {
//    Vec2i uv(iface*normalmap_.get_width(), nthvert*normalmap_.get_height());
//    TGAColor c = normalmap_.get(iface, nthvert);
//    Vec3f res;
//    for (int i=0; i<3; i++)
//        res[2-i] = (float)c[i]/255.f*2.f - 1.f;
//    return res;
//}

Vec3f Model::normal(int iface, int nthvert) {
    int idx = faces_[iface][nthvert][2];
    return norms_[idx].normalize();
}

std::vector<int> Model::face(int idx) {
    std::vector<int> face;
    for ( int i=0; i < (int)faces_[idx].size(); i++) {

        // the first value of all the elements will give you the vertices of the face
        face.push_back(faces_[idx][i][0]);
    }
    return face;
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

Vec2i Model::text(int iface, int nvert) {
    int idx = faces_[iface][nvert][1];
    return Vec2i(texts_[idx].x * diffusemap_.get_width(), texts_[idx].y * diffusemap_.get_height());
}

void Model::load_texture(std::string filename, TGAImage &img) {
    std::string texfile(filename);
    size_t dot = texfile.find_last_of(".");
    if (dot!=std::string::npos) {
        std::cerr << "texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
        img.flip_vertically();
    }
}

TGAColor Model::diffuse(Vec2i uv) {
    return diffusemap_.get(uv.x, uv.y);
}

// fun ways to draw the model

void Model::dwg0(TGAImage &image, const TGAColor &color ){

    int width = image.get_width();
    int height = image.get_height();

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
void Model::dwg1(TGAImage &image){

    int width = image.get_width();
    int height = image.get_height();

    Vec2i screen_coords[3];

    float *zbuffer = new float[width*height];
    for (int i = width * height; i--; zbuffer[i] = -std::numeric_limits<float>::max());

    for (int i=0; i < this->nfaces(); i++) {
        std::vector<int> face = this->face(i);
        Vec3f pts[3];

        for (int i = 0; i < 3; i++) pts[i] = world2screen(this->vert(face[i]), width, height);

        std::cout << pts[0] << pts[1] << pts[2] << std::endl;
        triangle(pts, zbuffer, image, TGAColor(rand()%255, rand()%255, rand()%255, 255));
    }
}

// shaded mesh given a light vector ( still no z buffer so depricated )
void Model::dwg2(TGAImage &image, Vec3f light){

    int width = image.get_width();
    int height = image.get_height();

    std::cerr << "dwg2 has no x buffer therefore it's not the best. Use dwg 3 instead" << std::endl;

    for (int i = 0; i < this->nfaces(); i++) {
        std::vector<int> face = this->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];

        for (int j=0; j<3; j++) {
            Vec3f v = this->vert(face[j]);
            screen_coords[j] = Vec2i((v.x + 1.) * width / 2., (v.y + 1.) * height / 2.);
            world_coords[j] = v;
        }

        Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
        n.normalize();
        float intensity = n * light;

         // if the dot product is negative, the light is coming from behing the shape,
         // let's not draw it! this is called black face culling
        if (intensity > 0) {
             triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(intensity*255, intensity*255, intensity*255, 255));
        }
    }

}

// shaded mesh given a light vector. z buffer is implemented
void Model::dwg3(TGAImage &image, Vec3f light){

    int width = image.get_width();
    int height = image.get_height();

    // Initialize the z buffer
    float *zbuffer = new float[width * height];
    for (int i = width*height; i--; zbuffer[i] = -std::numeric_limits<float>::max());

    // used to calculate the normal
    Vec3f world_coords[3];

    // temporary vector used to transform the coords into world coords
    Vec3f pts[3];

    for (int i = 0; i < this->nfaces(); i++) {

        std::vector<int> face = this->face(i);

        for (int i = 0; i < 3; i++) pts[i] = world2screen(this->vert(face[i]), width, height);

        for (int j = 0; j < 3; j++) {
            Vec3f v = this->vert(face[j]);
            world_coords[j] = v;
        }

        Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
        n.normalize();
        float intensity = n * light;

        // if the triangle is behind the light, don't draw it
        if (intensity > 0)
            triangle(pts, zbuffer, image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
    }
}

// coloured, shaded mesh given a light vector. z buffer is implemented
void Model::dwg4(TGAImage &image, const TGAColor &color, Vec3f light){

    int width = image.get_width();
    int height = image.get_height();

    // Initialize the z buffer
    float *zbuffer = new float[width * height];
    for (int i = width*height; i--; zbuffer[i] = -std::numeric_limits<float>::max());

    // used to calculate the normal
    Vec3f world_coords[3];

    // temporary vector used to transform the coords into world coords
    Vec3f pts[3];

    for (int i = 0; i < this->nfaces(); i++) {

        std::vector<int> face = this->face(i);

        for (int i = 0; i < 3; i++) pts[i] = world2screen(this->vert(face[i]), width, height);

            for (int j = 0; j < 3; j++) {
                Vec3f v = this->vert(face[j]);
                world_coords[j] = v;
            }

        Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
        n.normalize();
        float intensity = n * light;

        // if the triangle is behind the light, don't draw it
        if (intensity > 0)

            triangle(pts, zbuffer, image, TGAColor(intensity * color.r, intensity * color.g, intensity * color.b, 255));
    }
}

void Model::dwg5(TGAImage &image, Vec3f light){

    // todo: you should pass in the z buffer so we only use one for the entire program

    int width = image.get_width();
    int height = image.get_height();

    // Initialize the z buffer
    float *zbuffer = new float[width * height];
    for (int i = width*height; i--; zbuffer[i] = -std::numeric_limits<float>::max());

    // used to calculate the normal
    Vec3f world_coords[3];

    // temporary vectors
    Vec3f pts[3];
    Vec2i uv[3];

    // for every face
    for (int i = 0; i < this->nfaces(); i++) {

        std::vector<int> face = this->face(i);

        // scale to screen sized
        for (int i = 0; i < 3; i++) pts[i] = world2screen(this->vert(face[i]), width, height);

        for (int j = 0; j < 3; j++) {
            Vec3f v = this->vert(face[j]);
            world_coords[j] = v;
        }

        Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
        n.normalize();
        float intensity = n * light;

        // if the triangle is behind the light, don't draw it
        if (intensity > 0){

            for (int k=0; k<3; k++) {
                uv[k] = this->text(i, k);
            }
            triangle(pts, uv, zbuffer, intensity, image, this);
        }
    }
}

// draws the model in perspective with a given depth and camera
void Model::dwg6(TGAImage &image, Vec3f light, Vec3f camera, int depth, float *zbuffer){

    int width = image.get_width();
    int height = image.get_height();

    Matrix Projection = Matrix::identity(4);
    Matrix ViewPort   = viewport(width/8, height/8, width*3/4, height*3/4, depth);

    Projection[3][2] = -1.f/camera.z;

    // for every face
    for (int i=0; i<this->nfaces(); i++) {
        std::vector<int> face = this->face(i);
        Vec3i screen_coords[3];
        Vec3f world_coords[3];
        for (int j=0; j<3; j++) {
            Vec3f v = this->vert(face[j]);

            // Transform the world coords into the screen coords (multiply them by the transformation matrices)
//            screen_coords[j] =  m2v(ViewPort * Projection * v2m(v));
            screen_coords[j] =  m2v(ViewPort * Projection * v2m(v));
            world_coords[j]  = v;
        }

        Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
        n.normalize();
        float intensity = n*light;

        // if the triangle is behind the light, don't draw it
        if (intensity>0) {
            Vec2i uv[3];
            for (int k=0; k<3; k++) {
                uv[k] = this->text(i, k);
            }

            triangle(screen_coords, uv, zbuffer, intensity, image, this);
            // void triangle(Vec3f *verts, Vec2i *texts, float *zbuffer, float intensity, TGAImage &image, Model * model)
        }
    }
}


// draws the model in perspective with a given depth and eyeball
void Model::dwg7(TGAImage &image, Vec3f light, Vec3f eye, int depth, float *zbuffer){

    int width = image.get_width();
    int height = image.get_height();

    Vec3f center(0,0,0);

    Matrix ModelView; lookat(eye, center, Vec3f(0,1,0), ModelView);
    Matrix Projection = Matrix::identity(4);
    Matrix ViewPort   = viewport(width/8, height/8, width*3/4, height*3/4, depth);

    Projection[3][2] = -1.f/(eye - center).norm();

    // for every face
    for (int i = 0; i < this->nfaces(); i++) {
        std::vector<int> face = this->face(i);
        Vec3i screen_coords[3];
        Vec3f world_coords[3];
        Vec3f intensity;

        for (int j = 0; j < 3; j++) {
            Vec3f v = this->vert(face[j]);

            // Transform the world coords into the screen coords (multiply them by the transformation matrices)
            screen_coords[j] =  m2v(ViewPort * Projection * ModelView * v2m(v));
            world_coords[j]  = v;
            intensity[j] = this->normal(i, j) * light;
        }

            triangle(screen_coords, intensity, zbuffer, image);
        //void triangle(Vec3f *verts, Vec3f intensity, float *zbuffer, TGAImage &image)

    }
}