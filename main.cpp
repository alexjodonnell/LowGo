#include <iostream>

#include <vector>
#include <cmath>
#include <limits>
#include "tga/TGAImage.h"
#include "model/Model.h"
#include "tga/DWGTool.h"
#include "render/RenderUtils.h"

const TGAColor white    = TGAColor(255, 255, 255, 255);
const TGAColor black    = TGAColor(  0,   0,   0, 255);
const TGAColor red      = TGAColor(255,   0,   0, 255);
const TGAColor green    = TGAColor(  0, 255,   0, 255);
const TGAColor blue     = TGAColor(  0,   0, 255, 255);
const TGAColor cyan     = TGAColor(  0, 255, 255, 255);
const TGAColor yellow   = TGAColor(255, 255,   0, 255);
const TGAColor magenta  = TGAColor(255,   0, 255, 255);
const TGAColor forest   = TGAColor( 34, 139,  34, 255);
const TGAColor charcoal = TGAColor( 34,  34,  34, 255);

Model *model = NULL;

// scene dimensions
const int width  = 800;
const int height = 800;
const int depth  = 255;

using namespace std;

int main(int argc, char** argv) {

    // create the z buffer and encode it within one dimension
    float *zbuffer = new float[width * height];
    for (int i = width * height; i--; zbuffer[i] = -std::numeric_limits<float>::max());

    if (2 == argc) {
        //model = new Model(argv[1]);
    } else {
        //model = new Model("../resources/diablo3_pose/diablo3_pose.obj", "../resources/diablo3_pose/diablo3_pose_diffuse.tga");
        model = new Model("../resources/models/face.obj", "../resources/textures/face_diffuse.tga");
    }

    Vec3f camera( 0,  0,  3);
    Vec3f light ( 0,  0,  -1);
    TGAImage image(width, height, TGAImage::RGB);

    model->dwg6(image, light, camera, depth, zbuffer);
    //dwg6(TGAImage &image, Vec3f light, Vec3f camera, int depth, float *zbuffer)

    { // dump z-buffer (debugging purposes only)
        TGAImage zbimage(width, height, TGAImage::GRAYSCALE);
        for (int i=0; i<width; i++) {
            for (int j=0; j<height; j++) {
                zbimage.set(i, j, TGAColor(zbuffer[i+j*width], 1));
            }
        }
        zbimage.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        zbimage.write_tga_file("zbuffer.tga");
    }

    image.flip_vertically();
    image.write_tga_file("output.tga");

    // cleanup
    delete model;
    delete [] zbuffer;

    puts("Done");
    return 0;
}
