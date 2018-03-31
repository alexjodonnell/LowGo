#include <iostream>

#include <vector>
#include <cmath>
#include <limits>
#include "shared/TGAImage.h"
#include "shared/Model.h"
#include "GL/GL.h"

Model *model = NULL;

// scene dimensions
const int width  = 800;
const int height = 800;

// scene params
Vec3f     light( 1, -1, 1);
Vec3f       eye(1, 1, 3);
Vec3f    center( 0, 0, 0);
Vec3f        up( 0, 1, 0);

using namespace std;

// Our Shader Programs
struct GouraudShader : public IShader {

    Vec3f varying_intensity; // written by vertex shader, read by fragment shader

    // Vertex Shader
    virtual Vec4f vertex(int iface, int nthvert) {

        Vec4f gl_Vertex = embed<4>(model->vert(iface, nthvert)); // read the vertex from .obj file
        gl_Vertex = Viewport*Projection*ModelView*gl_Vertex;     // transform it to screen coordinates
        varying_intensity[nthvert] = std::max(0.f, model->normal(iface, nthvert)*light); // get diffuse lighting intensity
        return gl_Vertex;
    }

    // Fragment Shader
    virtual bool fragment(Vec3f bar, TGAColor &color) {
        float intensity = varying_intensity * bar;      // interpolate intensity for the current pixel
        color = TGAColor( 100, 100, 100) * intensity;     // apply out intensity
        return false;                                   // no, we do not discard this pixel
    }
};


int main(int argc, char** argv) {
    if (2 == argc) {
//        model = new Model(argv[1]); broke lol
    } else {
//        model = new Model("../resources/diablo3_pose/diablo3_pose.obj",
//                          "../resources/diablo3_pose/diablo3_pose_diffuse.tga",
//                          "../resources/diablo3_pose/diablo3_pose_nm_tangent.tga",
//                          "../resources/diablo3_pose/diablo3_pose_spec.tga");

        model = new Model("../resources/models/face.obj",
                          "../resources/textures/face_diffuse.tga",
                          "../resources/normals/face_nm.tga",
                          "../resources/spec/face_spec.tga");
    }

    // creating the image
    TGAImage image  (width, height, TGAImage::RGB);
    TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);

    // setting the scene
    lookat(eye, center, up);
    viewport(width/8, height/8, width*3/4, height*3/4);
    projection(-1.f/(eye-center).norm());
    light.normalize();

    GouraudShader shader;
    for (int i=0; i<model->nfaces(); i++) {
        Vec4f screen_coords[3];
        for (int j=0; j<3; j++) {
            screen_coords[j] = shader.vertex(i, j);
        }
        triangle(screen_coords, shader, image, zbuffer);
    }

    // to place the origin in the bottom left corner of the image
    image.  flip_vertically();
    zbuffer.flip_vertically();
    image.  write_tga_file("output.tga");
    zbuffer.write_tga_file("zbuffer.tga");

    // cleanup
    puts("Ω");

    return 0;
}
