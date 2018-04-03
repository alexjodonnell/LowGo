//
// Created by Alex O'Donnell on 2018-03-26.
//


#include "GL.h"


Matrix ModelView;
Matrix Viewport;
Matrix Projection;

IShader::~IShader() {}

void viewport(int x, int y, int w, int h) {
    Viewport = Matrix::identity();
    Viewport[0][3] = x + w/2.f;
    Viewport[1][3] = y + h/2.f;
    Viewport[2][3] = 255.f/2.f;
    Viewport[0][0] = w/2.f;
    Viewport[1][1] = h/2.f;
    Viewport[2][2] = 255.f/2.f;
}

void projection(float coeff) {
    Projection = Matrix::identity();
    Projection[3][2] = coeff;
}

void lookat(Vec3f eye, Vec3f center, Vec3f up) {

    // define the new z vector as the vector travelling from our eye to the center and normalize it
    Vec3f z = (eye - center).normalize();

    // the new x is the cross product of up and z
    Vec3f x = cross(up, z).normalize();

    // the new y is the cross product of z and x
    Vec3f y = cross(z, x).normalize();

    // Initialize the model view
    ModelView = Matrix::identity();

    // now we need to translate the origin to the new center
    for (int i = 0; i < 3; i++) {
        ModelView[0][i] = x[i];
        ModelView[1][i] = y[i];
        ModelView[2][i] = z[i];
        ModelView[i][3] = -center[i];
    }
}

// computes the barycenter in 3D
Vec3f barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P) {
    Vec3f s[2];
    for (int i=2; i--; ) {
        s[i][0] = C[i]-A[i];
        s[i][1] = B[i]-A[i];
        s[i][2] = A[i]-P[i];
    }
    Vec3f u = cross(s[0], s[1]);
    if (std::abs(u[2])>1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
        return Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
    return Vec3f(-1,1,1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}

// triangle that implements z buffer so that shapes that are blocked by other objects aren't drawn (best triangle yet)
void triangle(Vec4f *pts, IShader &shader, TGAImage &image, TGAImage &zbuffer) {

    // initialize the bounding box (eventually will be the bottom left and top right of our triangle)
    Vec2f bboxmin( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());

    // iterate through the vertices of the triangle and choose the real min/max coordinates for the bounding box.
    for (int i=0; i<3; i++) {
        for (int j=0; j<2; j++) {
            bboxmin[j] = std::min(bboxmin[j], pts[i][j]/pts[i][3]);
            bboxmax[j] = std::max(bboxmax[j], pts[i][j]/pts[i][3]);
        }
    }

    // initialize barycenter and the color that gets pulled from the shader
    Vec2i P;
    TGAColor color;

    // for the span of the barycentric coordinates (the entire triangle)
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {

            // compute the barycenter of the triangle
            Vec3f bar = barycentric(proj<2>(pts[0]/pts[0][3]), proj<2>(pts[1]/pts[1][3]), proj<2>(pts[2]/pts[2][3]), proj<2>(P));

            float z = pts[0][2]*bar.x + pts[1][2]*bar.y + pts[2][2]*bar.z;
            float w = pts[0][3]*bar.x + pts[1][3]*bar.y + pts[2][3]*bar.z;
            int frag_depth = std::max(0, std::min(255, int(z/w + .5)));

            // if we're still in bounds, keep going
            if (bar.x<0 || bar.y<0 || bar.z<0 || zbuffer.get(P.x, P.y)[0] > frag_depth) continue;

            // run our shader program to get the color
            bool discard = shader.fragment(bar, color);
            if (!discard) {
                zbuffer.set(P.x, P.y, TGAColor(frag_depth));
                image.set(P.x, P.y, color);
            }
        }
    }
}