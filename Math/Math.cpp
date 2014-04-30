//
//  Math.cpp
//  component_based_entity_system
//
//  Created by Administrator on 11.01.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//

#include "Math.h"

/*void random_seed(unsigned int seed) {
    srand(seed);
}*/

/*int random(int from, int to) {
    return rand() % (to - from + 1) + from;
}*/

Matrix<float, 4, 4> perspective_projection(int width, int height, float field_of_view, float znear, float zfar) {
    float aspect = (float)width / (float)height;
    const float h = 1.0f / tan(field_of_view * PI / 360.0f);
    Matrix<float, 4, 4> result(h / aspect,  0.0f,   0.0f,                       0.0f,
                               0.0f,        h,      0.0f,                       0.0f,
                               0.0f,        0.0f,   -(zfar+znear)/(zfar-znear), -2.0f*zfar*znear/(zfar-znear),
                               0.0f,        0.0f,   -1.0f,                      0.0f);
    return result;
}

Matrix<float, 4, 4> perspective_projection(float left, float right, float bottom, float top, float znear, float zfar) {
    Matrix<float, 4, 4> result(2.0f * znear / (right - left),   0.0f,                           (right + left) / (right - left),    0.0f,
                               0.0f,                            2.0f * znear / (top - bottom),  (top + bottom) / (top - bottom),    0.0f,
                               0.0f,                            0.0f,                           -(zfar+znear)/(zfar-znear),         -2.0f*zfar*znear/(zfar-znear),
                               0.0f,                            0.0f,                           -1.0f,                              0.0f);
    return result;
}

Matrix<float, 4, 4> orthogonal_projection(int width, int height, float znear, float zfar) {
    float left = -(float)width / 2.0f;
    float right = -left;
    float top = (float)height / 2.0f;
    float bottom = -top;
    Matrix<float, 4, 4> result(2.0f / (right -left),    0.0f,                   0.0f,                   -(right+left)/(right-left),
                               0.0f,                    2.0f / (top-bottom),    0.0f,                   -(top+bottom)/(top-bottom),
                               0.0f,                    0.0f,                   -2.0f / (zfar-znear),   -(zfar+znear)/(zfar-znear),
                               0.0f,                    0.0f,                   0.0f,                   1.0f);
    return result;
}

Matrix<float, 4, 4> orthogonal_projection(float left, float right, float bottom, float top, float znear, float zfar) {
    Matrix<float, 4, 4> result(2.0f / (right -left),    0.0f,                   0.0f,                   -(right+left)/(right-left),
                               0.0f,                    2.0f / (top-bottom),    0.0f,                   -(top+bottom)/(top-bottom),
                               0.0f,                    0.0f,                   -2.0f / (zfar-znear),   -(zfar+znear)/(zfar-znear),
                               0.0f,                    0.0f,                   0.0f,                   1.0f);
    return result;
}

Matrix<float, 3, 3> normal_matrix(const Matrix<float, 4, 4>& model_view_matrix) {
    Matrix<float, 4, 4> result = transpose(inverse(model_view_matrix));
    return Matrix<float, 3, 3>(result(0,0), result(0,1), result(0,2),
                               result(1,0), result(1,1), result(1,2),
                               result(2,0), result(2,1), result(2,2));
}

Matrix<float, 4, 4> look_at(const Vector<float, 3> at, const Vector<float, 3> eye, const Vector<float, 3> up) {
    Vector<float, 3> zaxis = vector_normal(at - eye);
    Vector<float, 3> xaxis = vector_normal(cross(up, zaxis));
    Vector<float, 3> yaxis = cross(zaxis, xaxis);

    return Matrix<float, 4, 4>(xaxis[0], xaxis[1], xaxis[2], -dot(xaxis, eye),
                               yaxis[0], yaxis[1], yaxis[2], -dot(yaxis, eye),
                               zaxis[0], zaxis[1], zaxis[2], -dot(zaxis, eye),
                               0, 0, 0, 1);
}
