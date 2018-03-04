#include <iostream>

#include <vector>
#include <cmath>
#include "tga/TGAImage.h"
#include "model/model.h"
#include "tga/DWGTool.h"
#include "geometry/Triangle.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255,   0,   0, 255);
const TGAColor cyan  = TGAColor(  0, 255, 255, 255);
const TGAColor green = TGAColor(  0, 128,   0, 255);

Model *model = NULL;

const int width  = 1000;
const int height = 1000;

int main(int argc, char** argv) {

    if (2 == argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("../resources/Totodile2.obj");
    }

    // create an image
    TGAImage image(width, height, TGAImage::RGB);

    Triangle triangle1(Vec2i(10, 10), Vec2i(width-10, height/2), Vec2i(width-100, height-10));

    triangle1.draw(image, green);

    // this block draws the mesh of an obj file
    for (int i=0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        for (int j = 0; j < 3; j++) {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j + 1) % 3]);
            int x0 = (v0.x + 1.) * width/2.;
            int y0 = (v0.y + 1.) * height/2.;
            int x1 = (v1.x + 1.) * width/2.;
            int y1 = (v1.y + 1.) * height/2.;
            DWGTool::line(x0, y0, x1, y1, image, cyan);
        }
    }

    puts("Done");
    image.flip_vertically(); // makes the origin at the left bottom corner of the image as opposed to the top left
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}
