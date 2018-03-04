#include <iostream>

#include <vector>
#include <cmath>
#include "tga/TGAImage.h"
#include "model/model.h"
#include "tga/DWGTool.h"


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255,   0,   0, 255);
const TGAColor cyan  = TGAColor(  0, 255, 255, 255);

Model *model = NULL;

const int width  = 1000;
const int height = 1000;

int main(int argc, char** argv) {
    if (2 == argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("../resources/african_head.obj");
    }

    TGAImage image(width, height, TGAImage::RGB);
//    for (int i=0; i < model->nfaces(); i++) {
//        std::vector<int> face = model->face(i);
//        for (int j=0; j<3; j++) {
//            Vec3f v0 = model->vert(face[j]);
//            Vec3f v1 = model->vert(face[(j+1)%3]);
//            int x0 = (v0.x+1.)*width/2.;
//            int y0 = (v0.y+1.)*height/2.;
//            int x1 = (v1.x+1.)*width/2.;
//            int y1 = (v1.y+1.)*height/2.;
//            DWGTool::line(x0, y0, x1, y1, image, cyan);
//        }
//    }

    Vec2i triangle0[] = {Vec2i(330,200), Vec2i(660,200), Vec2i(500,500)};
    Vec2i triangle1[] = {Vec2i(330,800), Vec2i(660,800), Vec2i(500,500)};

    DWGTool::triangle(triangle0[0], triangle0[1], triangle0[2], image, cyan);
    DWGTool::triangle(triangle1[0], triangle1[1], triangle1[2], image, red);

    image.flip_vertically(); // makes the origin at the left bottom corner of the image as opposed to the top
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}
