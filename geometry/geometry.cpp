//
// Created by Alex O'Donnell on 2018-03-04.
//

#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include "geometry.h"


//VECTOR----------------------------------------------------------------------------------------------------------------

// note that gcc has to be used for this to work
template <>
template <>
Vec3<int>::Vec3<>(const Vec3<float> &v) : x(int(v.x+.5)), y(int(v.y+.5)), z(int(v.z+.5)) {
}

template <>
template <>
Vec3<float>::Vec3<>(const Vec3<int> &v) : x(v.x), y(v.y), z(v.z) {
}

//MATRIX----------------------------------------------------------------------------------------------------------------

Matrix::Matrix(int r, int c) : m(std::vector<std::vector<float> >(r, std::vector<float>(c, 0.f))), rows(r), cols(c) { }

int Matrix::nrows() {
    return rows;
}

int Matrix::ncols() {
    return cols;
}

Matrix Matrix::identity(int dimensions) {
    Matrix E(dimensions, dimensions);
    for (int i=0; i<dimensions; i++) {
        for (int j=0; j<dimensions; j++) {
            E[i][j] = (i==j ? 1.f : 0.f);
        }
    }
    return E;
}

std::vector<float>& Matrix::operator[](const int i) {
    assert(i>=0 && i<rows);
    return m[i];
}

Matrix Matrix::operator*(const Matrix& a) {
    // make sure that it is a valid multiplication
    assert(cols == a.rows);
    Matrix result(rows, a.cols);
    for (int i=0; i<rows; i++) {
        for (int j=0; j<a.cols; j++) {
            result.m[i][j] = 0.f;
            for (int k=0; k<cols; k++) {
                result.m[i][j] += m[i][k]*a.m[k][j];
            }
        }
    }
    return result;
}

Matrix Matrix::transpose() {
    Matrix result(cols, rows);
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            result[j][i] = m[i][j];
    return result;
}

Matrix Matrix::inverse() {
    assert(rows==cols);
    // augmenting the square matrix with the identity matrix of the same dimensions a => [ai]
    Matrix result(rows, cols*2);
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            result[i][j] = m[i][j];
    for(int i=0; i<rows; i++)
        result[i][i+cols] = 1;
    // first pass
    for (int i=0; i<rows-1; i++) {
        // normalize the first row
        for(int j=result.cols-1; j>=0; j--)
            result[i][j] /= result[i][i];
        for (int k=i+1; k<rows; k++) {
            float coeff = result[k][i];
            for (int j=0; j<result.cols; j++) {
                result[k][j] -= result[i][j]*coeff;
            }
        }
    }
    // normalize the last row
    for(int j=result.cols-1; j>=rows-1; j--)
        result[rows-1][j] /= result[rows-1][rows-1];
    // second pass
    for (int i=rows-1; i>0; i--) {
        for (int k=i-1; k>=0; k--) {
            float coeff = result[k][i];
            for (int j=0; j<result.cols; j++) {
                result[k][j] -= result[i][j]*coeff;
            }
        }
    }
    // cut the identity matrix back
    Matrix truncate(rows, cols);
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            truncate[i][j] = result[i][j+cols];
    return truncate;
}

std::ostream& operator<<(std::ostream& s, Matrix& m) {
    for (int i=0; i<m.nrows(); i++)  {
        for (int j=0; j<m.ncols(); j++) {
            s << m[i][j];
            if (j<m.ncols()-1) s << "\t";
        }
        s << "\n";
    }
    return s;
}

//HELPER-FUNCTIONS------------------------------------------------------------------------------------------------------

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
    for (int i = 2; i--; ) {
        s[i][0] = C[i] - A[i];
        s[i][1] = B[i] - A[i];
        s[i][2] = A[i] - P[i];
    }

    Vec3f u;
    cross(s[0], s[1], u);

    if (std::abs(u[2])>1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
        return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    return Vec3f(-1, 1, 1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}

// scales a vector from regular coord to fit nicely in any resolution
Vec3f world2screen(Vec3f v, int width, int height) {
    return Vec3f(int((v.x + 1.) * width/2. + .5), int((v.y + 1.) * height/2. + .5), v.z);
}

Vec3f m2v(Matrix m) {
    return Vec3f(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);
}

Matrix v2m(Vec3f v) {
    Matrix m(4, 1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.f;
    return m;
}


// maps the cube bi unit cube [-1,1]*[-1,1]*[-1,1] to the screen cube [x,x+w]*[y,y+h]*[0,d].
Matrix viewport(int x, int y, int w, int h, int d) {
    Matrix m = Matrix::identity(4);
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = d/2.f;

    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = d/2.f;
    return m;
}

void lookat(Vec3f eye, Vec3f center, Vec3f up, Matrix & modelView) {

    // define the new z vector as the vector travelling from our eye to the center and normalize it
    Vec3f z = (eye - center).normalize();

    // the new x is the cross product of up and z
    Vec3f x; cross(up, z, x); x.normalize();

    // the new y is the cross product of z and x
    Vec3f y; cross( z, x, y); y.normalize();

    // now we need to translate the origin to the center
    Matrix Minv = Matrix::identity(4);
    Matrix Tr   = Matrix::identity(4);
    for (int i = 0; i < 3; i++) {
        Minv[0][i] = x[i];
        Minv[1][i] = y[i];
        Minv[2][i] = z[i];
        Tr[i][3] = - center[i];
    }
    modelView = Minv * Tr;
}


