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
const TGAColor black = TGAColor(  0, 0,   0, 255);

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

//    Triangle triangle1(Vec2i(10, 10), Vec2i(width-10, height/2), Vec2i(width-100, height-10));
//
//    triangle1.draw(image, green);

//    // this block makes a flat shading render of the model
//    // for each face
//    for (int i=0; i<model->nfaces(); i++) {
//        std::vector<int> face = model->face(i);
//        Vec2i screen_coords[3];
//
//        // for all vertices
//        for (int j=0; j<3; j++) {
//            Vec3f world_coords = model->vert(face[j]);
//            screen_coords[j] = Vec2i((world_coords.x+1.)*width/2., (world_coords.y+1.)*height/2.);
//        }
//
//        // draw a triangle of a random color between all vertices
//        DWGTool::triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(rand()%255, rand()%255, rand()%255, 255));
//    }

//    // this block draws the mesh of a model object
//    // for each face in the model,
//    for (int i=0; i < model->nfaces(); i++) {
//        std::vector<int> face = model->face(i);
//
//        // for each cooresponding vertex,
//        for (int j = 0; j < 3; j++) {
//            Vec3f v0 = model->vert(face[j]);
//            Vec3f v1 = model->vert(face[(j + 1) % 3]);
//            int x0 = (v0.x + 1.) * width/2.;
//            int y0 = (v0.y + 1.) * height/2.;
//            int x1 = (v1.x + 1.) * width/2.;
//            int y1 = (v1.y + 1.) * height/2.;
//
//            // draw a line from vertex to vertex
//            DWGTool::line(x0, y0, x1, y1, image, black);
//        }
//    }


    // most basic shading possible. intensity is calculated as the dot product of the light direction and the normal
    // this intensity is then multiplied to
    Vec3f light_dir(0, -1, 0);
    for (int i=0; i<model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];
        for (int j=0; j<3; j++) {
            Vec3f v = model->vert(face[j]);
            screen_coords[j] = Vec2i((v.x+1.)*width/2., (v.y+1.)*height/2.);
            world_coords[j]  = v;
        }
        Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
        n.normalize();
        float intensity = n * light_dir;

        // if the dot product is negative, the light is coming from behing the shape,
        // let's not draw it! this is called black face culling
        if (intensity > 0) {
            DWGTool::triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(intensity*255, intensity*255, intensity*255, 255));
        }
    }

    puts("Done");
    image.flip_vertically(); // makes the origin at the left bottom corner of the image as opposed to the top left
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}
