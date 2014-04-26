//
//  Math.h
//  component_based_entity_system
//
//  Created by Administrator on 11.01.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//

#ifndef __game__Math__
#define __game__Math__

#include "MathUtility.h"

#include "Vector.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "Matrix.h"
#include "Matrix2.h"
#include "Matrix3.h"
#include "Matrix4.h"

#include "Quaternion.h"

#include <math.h>

/*template <typename Type>
Type minimum(const Type& a, const Type& b) {
    return a < b ? a : b;
}*/

template<typename Type>
bool vector_nearly_zero(const Vector<Type,3>& v, Type eps) {
    //return fabsf(vector[0]) < eps && fabsf(vector[1]) < eps && fabsf(vector[2]) < eps;
    return dot(v, v) < eps * eps;
}

// component wise
template <typename Type, int N>
Vector<Type, N> minimum(const Vector<Type, N>& a, const Vector<Type, N>& b) {
    Vector<float, N> result;
    for (int i = 0; i < N; ++i) {
        result[i] = a[i] < b[i] ? a[i] : b[i];
    }
    return result;
}

/*template <typename Type>
Type maximum(const Type& a, const Type& b) {
    return a > b ? a : b;
}*/
// component wise
template <typename Type, int N>
Vector<Type, N> maximum(const Vector<Type, N>& a, const Vector<Type, N>& b) {
    Vector<float, N> result;
    for (int i = 0; i < N; ++i) {
        result[i] = a[i] > b[i] ? a[i] : b[i];
    }
    return result;
}

template<typename Type>
Type clamp(Type const& value, Type const& lower, Type const& upper) {
    if (value <= lower) return lower;
    if (value >= upper) return upper;
    return value;
}

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

// random
void random_seed(unsigned int seed);
int random(int from, int to);

// Projection
Matrix<float, 4, 4> perspective_projection(int width, int height, float field_of_view, float znear, float zfar);
Matrix<float, 4, 4> perspective_projection(float left, float right, float bottom, float top, float znear, float zfar);
Matrix<float, 4, 4> orthogonal_projection(int width, int height, float znear, float zfar);
Matrix<float, 4, 4> orthogonal_projection(float left, float right, float bottom, float top, float znear, float zfar);
Matrix<float, 3, 3> normal_matrix(const Matrix<float, 4, 4>& model_view_matrix);
Matrix<float, 4, 4> look_at(const Vector<float, 3> at, const Vector<float, 3> eye, const Vector<float, 3> up);

// Transformation
template<typename Type>
Matrix<Type, 3, 3> rotation(const Quaternion<Type>& q) {
    Type data_0_1 = q.data[0] * q.data[1];
    Type data_0_2 = q.data[0] * q.data[2];
    Type data_0_3 = q.data[0] * q.data[3];
    
    Type data_1_1 = q.data[1] * q.data[1];
    Type data_1_2 = q.data[1] * q.data[2];
    Type data_1_3 = q.data[1] * q.data[3];
    
    Type data_2_2 = q.data[2] * q.data[2];
    Type data_2_3 = q.data[2] * q.data[3];
    
    Type data_3_3 = q.data[3] * q.data[3];
    return Matrix<Type, 3, 3>{1.0 - 2.0 * (data_2_2 + data_3_3), 2.0 * (data_1_2 - data_0_3),       2.0 * (data_1_3 + data_0_2),
                              2.0 * (data_1_2 + data_0_3),       1.0 - 2.0 * (data_1_1 + data_3_3), 2.0 * (data_2_3 - data_0_1),
                              2.0 * (data_1_3 - data_0_2),       2.0 * (data_2_3 + data_0_1),       1.0 - 2.0 * (data_1_1 + data_2_2)};
}
template<typename Type>
Matrix<Type, 4, 4> homogeneous_rotation(const Quaternion<Type>& q) {
    Type data_0_1 = q.data[0] * q.data[1];
    Type data_0_2 = q.data[0] * q.data[2];
    Type data_0_3 = q.data[0] * q.data[3];
    
    Type data_1_1 = q.data[1] * q.data[1];
    Type data_1_2 = q.data[1] * q.data[2];
    Type data_1_3 = q.data[1] * q.data[3];
    
    Type data_2_2 = q.data[2] * q.data[2];
    Type data_2_3 = q.data[2] * q.data[3];
    
    Type data_3_3 = q.data[3] * q.data[3];
    return Matrix<Type, 4, 4>{1.0 - 2.0 * (data_2_2 + data_3_3), 2.0 * (data_1_2 - data_0_3),       2.0 * (data_1_3 + data_0_2),       0.0,
                              2.0 * (data_1_2 + data_0_3),       1.0 - 2.0 * (data_1_1 + data_3_3), 2.0 * (data_2_3 - data_0_1),       0.0,
                              2.0 * (data_1_3 - data_0_2),       2.0 * (data_2_3 + data_0_1),       1.0 - 2.0 * (data_1_1 + data_2_2), 0.0,
                              0.0,                               0.0,                               0.0,                               1.0};
}


template<typename Type>
Matrix<Type, 4, 4> homogeneous_translation(const Vector<Type, 3>& t) {
    return Matrix<Type, 4, 4>{1.0, 0.0, 0.0, t[0],
                              0.0, 1.0, 0.0, t[1],
                              0.0, 0.0, 1.0, t[2],
                              0.0, 0.0, 0.0, 1.0};
}

template<typename Type>
Matrix<Type, 4, 4> homogeneous_scale(const Vector<Type, 3>& s) {
    return Matrix<Type, 4, 4>{s[0], 0.0,  0.0,  0.0,
                              0.0,  s[1], 0.0,  0.0,
                              0.0,  0.0,  s[2], 0.0,
                              0.0,  0.0,  0.0,  1.0};
}

template<typename Type>
Vector<Type, 3> transformed_point(const Matrix<Type, 4, 4>& transformation, const Vector<Type, 3>& point) {
    return Vector<Type, 3> {
        transformation(0,0) * point[0] + transformation(0,1) * point[1] + transformation(0,2) * point[2] + transformation(0,3),
        transformation(1,0) * point[0] + transformation(1,1) * point[1] + transformation(1,2) * point[2] + transformation(1,3),
        transformation(2,0) * point[0] + transformation(2,1) * point[1] + transformation(2,2) * point[2] + transformation(2,3)
    };
}

template<typename Type>
Vector<Type, 3> transformed_vector(const Matrix<Type, 4, 4>& transformation, const Vector<Type, 3>& vector) {
    return Vector<Type, 3> {
        transformation(0,0) * vector[0] + transformation(0,1) * vector[1] + transformation(0,2) * vector[2],
        transformation(1,0) * vector[0] + transformation(1,1) * vector[1] + transformation(1,2) * vector[2],
        transformation(2,0) * vector[0] + transformation(2,1) * vector[1] + transformation(2,2) * vector[2]
    };
}

// Interpolation
/*template<typename Type, int N>
Vector<Type, N> linear_interpolation(const Vector<Type, N>& p0,
                                     const Vector<Type, N>& p1,
                                     float t) {
    return p0 + (p1 - p0) * t;
}*/
template <typename Type>
Type linear_interpolation(const Type& p0,
                          const Type& p1,
                          float t) {
    return p0 + (p1 - p0) * t;
}

/*template<typename Type, int N>
Vector<Type, N> linear_interpolation_dir(const Vector<Type, N>& p0,
                                         const Vector<Type, N>& p1,
                                         float t) {
    return vector_normal(p1 - p0);
}*/
template <typename Type>
Type linear_interpolation_dir(const Type& p0,
                          const Type& p1,
                          float t) {
    return p1 - p0;
}

/*template<typename Type, int N>
Vector<Type, N> quadratic_interpolation(const Vector<Type, N>& p0,
                                        const Vector<Type, N>& p1,
                                        const Vector<Type, N>& p2,
                                        float t) {
    return (1.0f - t) * ((1.0f - t) * p0 + 2.0f * t * p1) + t * t * p2;
}*/
template <typename Type>
Type quadratic_interpolation(const Type& p0,
                             const Type& p1,
                             const Type& p2,
                             float t) {
    return (1.0f - t) * ((1.0f - t) * p0 + 2.0f * t * p1) + t * t * p2;
}

/*template<typename Type, int N>
Vector<Type, N> quadratic_interpolation_dir(const Vector<Type, N>& p0,
                                            const Vector<Type, N>& p1,
                                            const Vector<Type, N>& p2,
                                            float t) {
    return vector_normal(2.0f * (1.0f - t) * (p1 - p0) + 2.0f * t * (p2 - p1));
}*/
template <typename Type>
Type quadratic_interpolation_dir(const Type& p0,
                             const Type& p1,
                             const Type& p2,
                             float t) {
    return 2.0f * (1.0f - t) * (p1 - p0) + 2.0f * t * (p2 - p1);
}

/*template<typename Type, int N>
Vector<Type, N> cubic_interpolation(const Vector<Type, N>& p0,
                                    const Vector<Type, N>& p1,
                                    const Vector<Type, N>& p2,
                                    const Vector<Type, N>& p3,
                                    float t) {
    return (1.0f - t) * ((1.0f - t) * ((1.0f - t) * p0 + 3.0f * t * p1) + 3.0f * t * t * p2) + t * t * t * p3;
}*/
template<typename Type>
Type cubic_interpolation(const Type& p0,
                         const Type& p1,
                         const Type& p2,
                         const Type& p3,
                         float t) {
    return (1.0f - t) * ((1.0f - t) * ((1.0f - t) * p0 + 3.0f * t * p1) + 3.0f * t * t * p2) + t * t * t * p3;
}

/*template<typename Type, int N>
Vector<Type, N> cubic_interpolation_dir(const Vector<Type, N>& p0,
                                        const Vector<Type, N>& p1,
                                        const Vector<Type, N>& p2,
                                        const Vector<Type, N>& p3,
                                        float t) {
    return normal(3.0f * ((1.0f - t) * ((1.0f - t) * (p1 - p0) + 2.0f * t * (p2 - p1)) + t * t * (p3 - p2)));
}*/
template<typename Type>
Type cubic_interpolation_dir(const Type& p0,
                         const Type& p1,
                         const Type& p2,
                         const Type& p3,
                         float t) {
    return 3.0f * ((1.0f - t) * ((1.0f - t) * (p1 - p0) + 2.0f * t * (p2 - p1)) + t * t * (p3 - p2));
}

/*template<typename Type>
Quaternion<Type> linear_interpolation(const Quaternion<Type>& q0,
                                      const Quaternion<Type>& q1,
                                      float t) {
    return q0 + (q1 - q0) * t;
}*/

template<typename Type>
Quaternion<Type> spherical_linear_interpolation(const Quaternion<Type>& q0,
                                                const Quaternion<Type>& q1,
                                                float t) {
    Type sign = 1;
    Type d = dot(q0, q1);
    if (d < 0) {
        d = -d;
        sign = -1;
    }
    if (d < 0.95) {
        Type angle = acos(d);
        return (q0 * sin(angle * (1 - t)) + sign * q1 * sin(angle * t)) / sin(angle);
    }
    return linear_interpolation(q0, sign * q1, t);
}

template <typename Type>
bool plane_line_intersection(Vector<Type, 3> const& plane_normal, Vector<Type, 3> const& plane_point,
                             Vector<Type, 3> const& line_normal, Vector<Type, 3> const& line_point,
                             Vector<Type, 3>& result) {
    Type dn = dot(plane_normal, line_normal);
    if (dn == 0.0f) {
        return false;
    }
    result = line_point + line_normal * dot(plane_normal, plane_point - line_point) / dn;
    return true;
}

// triangle
template <typename Type, int N>
void parametric(Vector<Type, N> const& a, Vector<Type, N> const& b, Vector<Type, N> const& c,
                Vector<Type, N> const& p, Type& s, Type& t) {
    Vector<Type, 3> u = b - a;
    Vector<Type, 3> v = c - a;
    Vector<Type, 3> w = p - a;
    Type uv = dot(u, v);
    Type uu = dot(u, u);
    Type vv = dot(v, v);
    Type wv = dot(w, v);
    Type wu = dot(w, u);
    Type dn = uv * uv - uu * vv;
    s = (uv * wv - vv * wu) / dn;
    t = (uv * wu - uu * wv) / dn;
}

template <typename Type, int N>
void parametric(Vector<Type, N> const& a, Vector<Type, N> const& b, Vector<Type, N> const& c,
                Vector<Type, N> const& p, Type& a0, Type& a1, Type& a2) {
    Vector<Type, N> u = b - a;
    Vector<Type, N> v = c - a;
    Vector<Type, N> w = p - a;
    Type uv = dot(u, v);
    Type uu = dot(u, u);
    Type vv = dot(v, v);
    Type wv = dot(w, v);
    Type wu = dot(w, u);
    Type dn = uv * uv - uu * vv;
    a1 = (uv * wv - vv * wu) / dn;
    a2 = (uv * wu - uu * wv) / dn;
    a0 = (Type)1 - a1 - a2;
}

// line
template <typename Type, int N>
void parametric(Vector<Type, N> const& a, Vector<Type, N> const& b,
                Vector<Type, N> const& p, Type& s) {
    Vector<Type, 3> u = b - a;
    s = dot(u, p - a) / dot(u, u);
}

template <typename Type, int N>
void parametric(Vector<Type, N> const& a, Vector<Type, N> const& b,
                Vector<Type, N> const& p, Type& a0, Type& a1) {
    Vector<Type, 3> u = b - a;
    a0 = dot(u, p - a) / dot(u, u);
    a1 = 1.0f - a0;
}

#endif /* defined(__game__Math__) */
