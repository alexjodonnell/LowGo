#include <iostream>

#include <vector>
#include <cmath>
#include <limits>
#include "shared/TGAImage.h"
#include "shared/Model.h"
#include "GL/GL.h"
#include "shared/colors.h"

Model *model = NULL;

// scene dimensions
const int width  = 800;
const int height = 800;

// scene params
Vec3f     light( 1, 1, 0);
Vec3f       eye( 1, 1, 4);
Vec3f    center( 0, 0, 0);
Vec3f        up( 0, 1, 0);

using namespace std;

// Our Shader Programs
// Diffuse texture shader with diffused and specular lighting
struct Shader : public IShader {

    // varying is a reserved keyword in GLSL language.
    // in varying variables we store data to be interpolated inside the triangle,
    // and the fragment shaders get the interpolated value (for the current pixel).

    // diffuse texture coordinates (2 per vertex, 3 vertices)
    mat<2, 3, float> varying_uv;

    // Uniform is a reserved keyword in GLSL, it allows to pass constants to the shaders

    // It's faster to combine all these matrices
    //  Projection*ModelView
    mat<4,4,float> uniform_M;

    // (Projection*ModelView).invert_transpose()
    mat<4,4,float> uniform_MIT;

    // Vertex Shader
    virtual Vec4f vertex(int iface, int nthvert) {

        // read the vertex from .obj file
        Vec4f gl_Vertex = embed<4>(model->vert(iface, nthvert));

        // read the uv from the .obj file and populate the matrix
        varying_uv.set_col(nthvert, model->uv(iface, nthvert));

        // transform it to screen coordinates
        return Viewport*Projection*ModelView*gl_Vertex;
    }

    // Fragment Shader
    virtual bool fragment(Vec3f bar, TGAColor &color) {

        // interpolate uv to retrieve the texture from this pixel
        Vec2f uv = varying_uv*bar;

        // for diffuse lighting we comput the (cosine of) angle between vectors n and l
        Vec3f n = proj<3>(uniform_MIT*embed<4>(model->normal(uv))).normalize();
        Vec3f l = proj<3>(uniform_M  *embed<4>(light        )).normalize();
        float diff = std::max(0.f, n*l);

        // now we are interested in the (cosine of) angle between vectors r (reflected light direction) and v (view direction).
        // find r, our reflected light vector
        Vec3f r = (n*(n*l*2.f) - l).normalize();
        float spec = pow(std::max(r.z, 0.0f), model->specular(uv));

        // apply our intensity to the color read from the diffuse texture
        TGAColor c = model->diffuse(uv);
        color = c;

        // compute our lighting as a weighted sum of ambient, diffuse and specular lighting
        // (here our coefficients are 5, 1 and .6)
        for (int i=0; i<3; i++){
            color[i] = std::min<float>(5 + c[i]*(diff + .6*spec), 255);
        }

        // no, we do not discard this pixel
        return false;
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
                          "../resources/normals/face_nm2.tga",
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

    Shader shader;
    shader.uniform_M   =  Projection*ModelView;
    shader.uniform_MIT = (Projection*ModelView).invert_transpose();

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
