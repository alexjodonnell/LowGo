//
// Created by Alex O'Donnell on 2018-03-04.
//

#include "geometry.h"

// transforms a set of 2 R2 points from cartesian to barycentric given some point P which is the barycenter
Vec3f barycentric(Vec2i *pts, Vec2i P) {

    Vec3f u;
    Vec3f a = Vec3f(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - P.x);
    Vec3f b = Vec3f(pts[2].y-pts[0].y, pts[1].y-pts[0].y, pts[0].y - P.y );

    cross(a ,b , u);

    if (std::abs(u.z) < 1) return Vec3f( -1, 1, 1); // triangle is degenerate, in this case return smth with negative coordinates

    return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

// transforms a set of 3 R3 points from cartesian to barycentric given some point P which is the barycenter
Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
    Vec3f s[2];
    for (int i=2; i--; ) {
        s[i][0] = C[i]-A[i];
        s[i][1] = B[i]-A[i];
        s[i][2] = A[i]-P[i];
    }

    Vec3f u;
    cross(s[0], s[1], u);

    if (std::abs(u[2])>1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
        return Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
    return Vec3f(-1,1,1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}

// scales a vector from regular coord to fit nicely in any resolution
Vec3f world2screen(Vec3f v, int width, int height) {
    return Vec3f(int((v.x + 1.) * width/2. + .5), int((v.y + 1.) * height/2. + .5), v.z);
}

