#include <iostream>

#include <vector>
#include <cmath>
#include <limits>
#include "tga/TGAImage.h"
#include "model/Model.h"
#include "tga/DWGTool.h"
#include "render/RenderUtils.h"

const TGAColor white   = TGAColor(255, 255, 255, 255);
const TGAColor black   = TGAColor(  0,   0,   0, 255);
const TGAColor red     = TGAColor(255,   0,   0, 255);
const TGAColor green   = TGAColor(  0, 255,   0, 255);
const TGAColor blue    = TGAColor(  0,   0, 255, 255);
const TGAColor cyan    = TGAColor(  0, 255, 255, 255);
const TGAColor yellow  = TGAColor(255, 255,   0, 255);
const TGAColor magenta = TGAColor(255,   0, 255, 255);
const TGAColor forest  = TGAColor( 34, 139,  34, 255);

Model *model = NULL;

const int width  = 800;
const int height = 800;

using namespace std;

int main(int argc, char** argv) {

    // create the z buffer and encode it within one dimension
    float *zbuffer = new float[width*height];
    for (int i = width * height; i--; zbuffer[i] = -std::numeric_limits<float>::max());

    if (2 == argc) {
        //model = new Model(argv[1]);
    } else {
        model = new Model("../resources/models/face.obj", "../resources/textures/face_diffuse.tga");
    }

    Vec3f light(0, 0, -1);

    TGAImage image(width, height, TGAImage::RGB);

    Vec3f pts[] = {Vec3f(200, 200, -100), Vec3f(800, 200, -100), Vec3f(400, 700, -200)};
//    triangle(pts, zbuffer, image, magenta );
    model->dwg5(image, light);



    puts("Done");
    image.flip_vertically(); // makes the origin at the left bottom corner of the image as opposed to the top left
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}
