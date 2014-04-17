//
//  Quaternion.h
//  component_based_entity_system
//
//  Created by Administrator on 11.01.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//

#ifndef __game__Quaternion__
#define __game__Quaternion__

#include <iostream>
#include "Vector.h"
#include "Matrix.h"

template<typename Type>
struct Quaternion {
    Type data[4];
    
    Quaternion() : Quaternion{1, 0, 0, 0} {}
    Quaternion(const Type& real, const Type& x, const Type& y, const Type& z) {
        data[0] = real;
        data[1] = x;
        data[2] = y;
        data[3] = z;
    }
    Quaternion(const Vector<Type,3>& axis_normal, const Type& angle) {
        Type cos_angle_2 = cos(angle * 0.5);
        Type sin_angle_2 = sin(angle * 0.5);
        data[0] = cos_angle_2;
        data[1] = sin_angle_2 * axis_normal[0];
        data[2] = sin_angle_2 * axis_normal[1];
        data[3] = sin_angle_2 * axis_normal[2];
    }
    explicit Quaternion(const Vector<Type, 3>& euler_angles) {
        Type cos_z_2 = cos(0.5 * euler_angles[2]);
        Type cos_y_2 = cos(0.5 * euler_angles[1]);
        Type cos_x_2 = cos(0.5 * euler_angles[0]);
        
        Type sin_z_2 = sin(0.5 * euler_angles[2]);
        Type sin_y_2 = sin(0.5 * euler_angles[1]);
        Type sin_x_2 = sin(0.5 * euler_angles[0]);
        
        // and now compute quaternion
        data[0] = cos_z_2 * cos_y_2 * cos_x_2 + sin_z_2 * sin_y_2 * sin_x_2;
        data[1] = cos_z_2 * cos_y_2 * sin_x_2 - sin_z_2 * sin_y_2 * cos_x_2;
        data[2] = cos_z_2 * sin_y_2 * cos_x_2 + sin_z_2 * cos_y_2 * sin_x_2;
        data[3] = sin_z_2 * cos_y_2 * cos_x_2 - cos_z_2 * sin_y_2 * sin_x_2;
    }
    Quaternion(const Vector<Type, 3>& eye, const Vector<Type, 3>& look_at, const Vector<Type, 3>& up) {
        Vector<Type, 3> z = vector_normal(eye - look_at); // look in the negative z direction (like OpenGL does)
        Vector<Type, 3> x = vector_normal(cross(up, z));
        Vector<Type, 3> y = vector_normal(cross(z, x));
        
        data[0] = sqrt(1.0f + x[0] + y[1] + z[2]) * 0.5f;
        float w4_recip = 1.0f / (4.0f * data[0]);
        data[1] = (y[2] - z[1]) * w4_recip;
        data[2] = (z[0] - x[2]) * w4_recip;
        data[3] = (x[1] - y[0]) * w4_recip;
    }
    
    const Type& operator[] (int i) const {
        return data[i];
    }
    Type& operator[] (int i) {
        return data[i];
    }
    
    const Quaternion<Type>& operator += (const Quaternion<Type>& q) {
        data[0] += q.data[0];
        data[1] += q.data[1];
        data[2] += q.data[2];
        data[3] += q.data[3];
        return *this;
    }
    const Quaternion<Type>& operator -= (const Quaternion<Type>& q) {
        data[0] -= q.data[0];
        data[1] -= q.data[1];
        data[2] -= q.data[2];
        data[3] -= q.data[3];
        return *this;
    }
    const Quaternion<Type>& operator *= (const Quaternion<Type>& q) {
        Type x = data[1], y = data[2], z = data[3], sn = data[0] * q.data[0] - (data[1] * q.data[1] + data[2] * q.data[2] + data[3] * q.data[3]);
        data[1] = y * q.data[3] - z * q.data[2] + data[0] * q.data[1] + x * q.data[0];
        data[2] = z * q.data[1] - x * q.data[3] + data[0] * q.data[2] + y * q.data[0];
        data[3] = x * q.data[2] - y * q.data[1] + data[0] * q.data[3] + z * q.data[0];
        data[0] = sn;
        return *this;
    }
    
    const Quaternion<Type>& operator *= (const Type& s) {
        data[0] *= s;
        data[1] *= s;
        data[2] *= s;
        data[3] *= s;
        return *this;
    }
    const Quaternion<Type>& operator /= (const Type& s) {
        data[0] /= s;
        data[1] /= s;
        data[2] /= s;
        data[3] /= s;
        return *this;
    }
};

template<typename Type>
Quaternion<Type> operator + (const Quaternion<Type>& q0, const Quaternion<Type>& q1) {
    return Quaternion<Type>{q0.data[0] + q1.data[0],
                            q0.data[1] + q1.data[1],
                            q0.data[2] + q1.data[2],
                            q0.data[3] + q1.data[3]};
}

template<typename Type>
Quaternion<Type> operator - (const Quaternion<Type>& q0, const Quaternion<Type>& q1) {
    return Quaternion<Type>{q0.data[0] - q1.data[0],
                            q0.data[1] - q1.data[1],
                            q0.data[2] - q1.data[2],
                            q0.data[3] - q1.data[3]};
}

template<typename Type>
Quaternion<Type> operator * (const Quaternion<Type>& q0, const Quaternion<Type>& q1) {
    return Quaternion<float>{q0.data[0] * q1.data[0] - q0.data[1] * q1.data[1] - q0.data[2] * q1.data[2] - q0.data[3] * q1.data[3],
                             q0.data[2] * q1.data[3] - q0.data[3] * q1.data[2] + q0.data[0] * q1.data[1] + q0.data[1] * q1.data[0],
                             q0.data[3] * q1.data[1] - q0.data[1] * q1.data[3] + q0.data[0] * q1.data[2] + q0.data[2] * q1.data[0],
                             q0.data[1] * q1.data[2] - q0.data[2] * q1.data[1] + q0.data[0] * q1.data[3] + q0.data[3] * q1.data[0]};
}

template<typename Type>
Quaternion<Type> operator * (const Quaternion<Type>& q, const Type& s) {
    return Quaternion<Type>{q.data[0] * s,
                            q.data[1] * s,
                            q.data[2] * s,
                            q.data[3] * s};
}

template<typename Type>
Quaternion<Type> operator * (const Type& s, const Quaternion<Type>& q) {
    return Quaternion<Type>{q.data[0] * s,
                            q.data[1] * s,
                            q.data[2] * s,
                            q.data[3] * s};
}

template<typename Type>
Quaternion<Type> operator / (const Quaternion<Type>& q, const Type& s) {
    return Quaternion<Type>{q.data[0] / s,
                            q.data[1] / s,
                            q.data[2] / s,
                            q.data[3] / s};
}

template<typename Type>
Type length(const Quaternion<Type>& q) {
    return sqrt(q.data[0] * q.data[0] + q.data[1] * q.data[1] + q.data[2] * q.data[2] + q.data[3] * q.data[3]);
}

template<typename Type>
Type length_squared(const Quaternion<Type>& q) {
    return q.data[0] * q.data[0] + q.data[1] * q.data[1] + q.data[2] * q.data[2] + q.data[3] * q.data[3];
}

template<typename Type>
Quaternion<Type> quaternion_normal(const Quaternion<Type>& q) {
    return q * (Type(1) / length(q));
}

template<typename Type>
Quaternion<Type> conjugate(const Quaternion<Type>& q) {
    return Quaternion<Type>{q.data[0],
                            -q.data[1],
                            -q.data[2],
                            -q.data[3]};
}

template<typename Type>
Quaternion<Type> inverse(const Quaternion<Type>& q) {
    return conjugate(q) / length_squared(q);
}

template<typename Type>
Type dot(const Quaternion<Type>& q0, const Quaternion<Type>& q1) {
    return q0.data[0] * q1.data[0] + q0.data[1] * q1.data[1] + q0.data[2] * q1.data[2] + q0.data[3] * q1.data[3];
}

template<typename Type>
Vector<Type, 3> rotate(const Quaternion<Type>& q, const Vector<Type, 3>& v) {
    Quaternion<Type> V{0, v[0], v[1], v[2]};
    Quaternion<Type> result = q * V * conjugate(q);
    return Vector<Type, 3>{result.data[1], result.data[2], result.data[3]};
}

template<typename Type>
Vector<Type, 3> euler_angles(const Quaternion<Type>& q) {
    Type square_w = q.data[0] * q.data[0];
    Type square_x = q.data[1] * q.data[1];
    Type square_y = q.data[2] * q.data[2];
    Type square_z = q.data[3] * q.data[3];
    return Vector<Type, 3>{atan2((Type)2.0 * (q.data[2] * q.data[3] + q.data[0] * q.data[1]),
                                 square_z - square_y - square_x + square_w),
                           asin((Type)2.0 * (q.data[0] * q.data[2] - q.data[1] * q.data[3])),
                           atan2((Type)2.0 * (q.data[1] * q.data[2] + q.data[0] * q.data[3]),
                                 square_x + square_w - square_z - square_y)};
}

template<typename Type>
Vector<Type, 3> axis(Quaternion<Type> const& q) {
    Type angle = acos(q.data[0]);
    Type k = 1.0f / sin(angle);
    return Vector<Type, 3>(q.data[1] * k,
                           q.data[2] * k,
                           q.data[3] * k);
}

template<typename Type>
Type angle(Quaternion<Type> const& q) {
    return acos(q.data[0]) * 2.0f;
}

template<typename Type>
std::ostream& operator << (std::ostream& stream, const Quaternion<Type>& q) {
    stream << q.data[0] << " " << q.data[1] << " " << q.data[2] << " " << q.data[3];
    return stream;
}

template<typename Type>
std::istream& operator >> (std::istream& stream, Quaternion<Type>& q) {
    stream >> q.data[0] >> q.data[1] >> q.data[2] >> q.data[3];
    return stream;
}

#endif /* defined(__game__Quaternion__) */
