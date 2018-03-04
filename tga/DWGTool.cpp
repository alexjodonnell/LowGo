//
// Created by Alex O'Donnell on 2018-03-03.
//

#include "DWGTool.h"

// algorithm for drawing a line to a screen
void DWGTool::line(int x0, int y0, int x1, int y1, TGAImage &image, const TGAColor &color) {

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

// algorithm for drawing a line to a screen
void DWGTool::line(Vec2i p1, Vec2i p2, TGAImage &image, const TGAColor &color){
   line(p1.x, p1.y, p2.x, p2.y, image, color);
}

// algorithm for drawing a triangle to a screen
void DWGTool::triangle(Vec2i p0, Vec2i p1, Vec2i p2, TGAImage &image, const TGAColor &color){

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

    // draw the bottom half of the triangle
    for (int y = p0.y; y <= p1.y; y++) {
        
        int s_height = p1.y - p0.y + 1; // height of the segment
        float alpha = (float)(y - p0.y) / h_total;
        float beta  = (float)(y - p0.y) / s_height; // be careful with divisions by zero
        Vec2i A = p0 + (p2 - p0) * alpha;
        Vec2i B = p0 + (p1 - p0) * beta;
        if (A.x>B.x) std::swap(A, B);

        // follow the path from
        for (int j=A.x; j<=B.x; j++) {
            image.set(j, y, color); // attention, due to int casts p0.y+i != A.y 
        }
    }

    // draw the top half of the triangle
    for (int y = p1.y; y <= p2.y; y++) {
        int s_height =  p2.y - p1.y+1;
        float alpha = (float)(y - p0.y) / h_total;
        float beta  = (float)(y - p1.y) / s_height; // be careful with divisions by zero
        Vec2i A = p0 + (p2 - p0) * alpha;
        Vec2i B = p1 + (p2 - p1) * beta;
        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j <= B.x; j++) {
            image.set(j, y, color); // attention, due to int casts p0.y+i != A.y 
        }
    }
}
