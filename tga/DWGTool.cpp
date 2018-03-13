//
// Created by Alex O'Donnell on 2018-03-03.
//

#include "DWGTool.h"

// algorithm for drawing a line to a screen
void line(int x0, int y0, int x1, int y1, TGAImage &image, const TGAColor &color) {

    // steep just means that dy > dx.
    // if the line is steep, we transpose the line during rendering
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    // make it left−to−right
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    // the entire stride
    int dx = x1 - x0;
    int dy = y1 - y0;

    // rate of change of the error in the y direction.
    // this value is estimated as the rate of change in the y direction times 2
    int derror = std::abs(dy) * 2;

    // prime the loop
    int error = 0;
    int y = y0;

    // travel in the x direction,
    // accumilate error in the y direction,
    // step y up or down once the error is larger than a pixel
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            image.set(y, x, color); // need to reverse transpose if it's steep
        } else {
            image.set(x, y, color);
        }
        error += derror;
        if (error > dx) {
            y += (y1 > y0? 1: -1); // move either up or down
            error -= dx * 2;
        }
    }
}

// algorithm for drawing a line to a screen using vectors as inputs
void line(Vec2i p1, Vec2i p2, TGAImage &image, const TGAColor &color){
    line(p1.x, p1.y, p2.x, p2.y, image, color);
}

// algorithm for drawing a triangle shaded to a screen (no z buffer implemented so pretty rough)
void triangle(Vec2i p0, Vec2i p1, Vec2i p2, TGAImage &image, const TGAColor &color){

    // really hacky bubble sort to sort the vertices in the y direction
    // (this is so that vertices can be passed in any order)
    if (p0.y>p1.y) std::swap(p0, p1);
    if (p0.y>p2.y) std::swap(p0, p2);
    if (p1.y>p2.y) std::swap(p1, p2);

    // at this point, p2 will have the largest y component and p0 will have the smallest
    // this means that the line travelling from p0 to p2 can be used as a boundary in order to horizontally
    // sweep the triangle and fill it in.

    // total height of the triangle (y span)
    int h_total = p2.y - p0.y;

    // bit messy lol. For the entire span of the triangle
    for (int i = 0; i < h_total; i++) {

        // are we in the top or bottom half of the triangle?
        bool second_half = i > p1.y - p0.y || p1.y == p0.y;

        // based on which half we are in, calculate segment height as the difference in the y direction from p1 to our half
        int segment_height = second_half ? p2.y - p1.y : p1.y - p0.y;

        // alpha = ratio of completed portion to the entire triangle
        float alpha = (float)i / h_total;

        // beta = ratio of the completed portion to the current segment
        float beta  = (float)(i - (second_half ? p1.y - p0.y : 0)) / segment_height;

        Vec2i A = p0 + (p2 - p0) * alpha; // this isn't really an error, the operation is overridden in the vector class
        Vec2i B = second_half ? p1 + (p2 - p1) * beta : p0 + (p1 - p0) * beta;

        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j <= B.x; j++) {
            image.set(j, p0.y + i, color); // attention, due to int casts p0.y+i != A.y
        }
    }
}

// triangle that implements z buffer so that shapes that are blocked by other objects aren't drawn
void triangle(Vec3f *pts, float *zbuffer, TGAImage &image, TGAColor color, int width) {

    // initialize the bounding box (eventually will be the bottom left and top right of our triangle)
    Vec2f bboxmin( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());

    // limit the rendering of the screen to the screen itself
    Vec2f clamp(image.get_width()-1, image.get_height()-1);

    // iterate through the vertices of the triangle and choose min/max coordinates for the bounding box.
    for (int i=0; i<3; i++) {
        for (int j=0; j<2; j++) {
            bboxmin[j] = std::max(0.f,      std::min(bboxmin[j], pts[i][j]));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
        }
    }

    // initialize barycenter
    Vec3f P;

    // for the span of the barycentric coordinates (the entire triangle)
    for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) {
        for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) {

            // compute the barycenter of the screen
            Vec3f bc_screen  = barycentric(pts[0], pts[1], pts[2], P);

            // if we're still in bounds, keep going
            if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue;


            P.z = 0;
            for (int i=0; i<3; i++) P.z += pts[i][2]*bc_screen[i];
            if (zbuffer[int(P.x+P.y * width)]<P.z) {
                zbuffer[int(P.x+P.y * width)] = P.z;
                image.set(P.x, P.y, color);
            }
        }
    }
}