//
// Created by Alex O'Donnell on 2018-03-04.
//

#include "geometry.h"

// transforms a set of points from cartesian to barycentric given some point P which is the barycenter
Vec3f barycentric(Vec2i *pts, Vec2i P) {

    Vec3f u;
    Vec3f a = Vec3f(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - P.x);
    Vec3f b = Vec3f(pts[2].y-pts[0].y, pts[1].y-pts[0].y, pts[0].y - P.y );

    cross(a ,b , u);

    if (std::abs(u.z) < 1) return Vec3f( -1, 1, 1); // triangle is degenerate, in this case return smth with negative coordinates

    return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

