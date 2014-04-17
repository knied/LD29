//
//  Vector3.h
//  component_based_entity_system
//
//  Created by Administrator on 11.01.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//

#ifndef __game__Vector3__
#define __game__Vector3__

#include "Vector.h"

/*
 * A 3-dimensional vector.
 */
template<typename Type>
struct Vector<Type,3> {
    enum { N = 3 };
    typedef Vector<Type,N> VectorType;
    
    Type data[N];
    
    /*
     * Create a vector an initialize each element with 0.
     */
    Vector() : Vector{0, 0, 0} {}
    
    /*
     * Create a vector an initialize each element with t.
     */
    explicit Vector(const Type& t) : Vector{t, t, t} {}
    
    /*
     * Create a vector from the provided values
     */
    Vector(const Type& x, const Type& y, const Type& z) {
        data[0] = x;
        data[1] = y;
        data[2] = z;
    }
    
    /*
     * Access the i´th element in the vector.
     */
    const Type& operator[] (int i) const {
        return data[i];
    }
    
    /*
     * Access the i´th element in the vector.
     */
    Type& operator[] (int i) {
        return data[i];
    }
    
    /*
     * Componentwise addition.
     */
    const VectorType& operator += (const VectorType& v) {
        (*this)[0] += v[0];
        (*this)[1] += v[1];
        (*this)[2] += v[2];
        return *this;
    }
    
    /*
     * Componentwise addition of a scalar.
     */
    const VectorType& operator += (const Type& s) {
        (*this)[0] += s;
        (*this)[1] += s;
        (*this)[2] += s;
        return *this;
    }
    
    /*
     * Componentwise subtraction.
     */
    const VectorType& operator -= (const VectorType& v) {
        (*this)[0] -= v[0];
        (*this)[1] -= v[1];
        (*this)[2] -= v[2];
        return *this;
    }
    
    /*
     * Componentwise subtraction of a scalar.
     */
    const VectorType& operator -= (const Type& s) {
        (*this)[0] -= s;
        (*this)[1] -= s;
        (*this)[2] -= s;
        return *this;
    }
    
    /*
     * Componentwise multiplication.
     */
    const VectorType& operator *= (const VectorType& v) {
        (*this)[0] *= v[0];
        (*this)[1] *= v[1];
        (*this)[2] *= v[2];
        return *this;
    }
    
    /*
     * Componentwise multiplication of a scalar.
     */
    const VectorType& operator *= (const Type& s) {
        (*this)[0] *= s;
        (*this)[1] *= s;
        (*this)[2] *= s;
        return *this;
    }
    
    /*
     * Componentwise division.
     */
    const VectorType& operator /= (const VectorType& v) {
        (*this)[0] /= v[0];
        (*this)[1] /= v[1];
        (*this)[2] /= v[2];
        return *this;
    }
    
    /*
     * Componentwise division by a scalar.
     */
    const VectorType& operator /= (const Type& s) {
        (*this)[0] /= s;
        (*this)[1] /= s;
        (*this)[2] /= s;
        return *this;
    }
}; // Vector

template<typename Type>
inline Vector<Type, 3> operator + (const Vector<Type, 3>& a, const Vector<Type, 3>& b) {
    return Vector<Type, 3>{a[0] + b[0],
                           a[1] + b[1],
                           a[2] + b[2]};
}

template<typename Type>
inline Vector<Type, 3> operator + (const Vector<Type, 3>& a, const Type& s) {
    return Vector<Type, 3>{a[0] + s,
                           a[1] + s,
                           a[2] + s};
}

template<typename Type>
inline Vector<Type, 3> operator + (const Type& s, const Vector<Type, 3>& a) {
    return Vector<Type, 3>{a[0] + s,
                           a[1] + s,
                           a[2] + s};
}

template<typename Type>
inline Vector<Type, 3> operator - (const Vector<Type, 3>& a, const Vector<Type, 3>& b) {
    return Vector<Type, 3>{a[0] - b[0],
                           a[1] - b[1],
                           a[2] - b[2]};
}

template<typename Type>
inline Vector<Type, 3> operator - (const Vector<Type, 3>& a, const Type& s) {
    return Vector<Type, 3>{a[0] - s,
                           a[1] - s,
                           a[2] - s};
}

template<typename Type>
inline Vector<Type, 3> operator - (const Vector<Type, 3>& a) {
    return Vector<Type, 3>{-a[0],
                           -a[1],
                           -a[2]};
}

template<typename Type>
inline Vector<Type, 3> operator * (const Vector<Type, 3>& a, const Vector<Type, 3>& b) {
    return Vector<Type, 3>{a[0] * b[0],
                           a[1] * b[1],
                           a[2] * b[2]};
}

template<typename Type>
inline Vector<Type, 3> operator * (const Vector<Type, 3>& a, const Type& s) {
    return Vector<Type, 3>{a[0] * s,
                           a[1] * s,
                           a[2] * s};
}

template<typename Type>
inline Vector<Type, 3> operator * (const Type& s, const Vector<Type, 3>& a) {
    return Vector<Type, 3>{a[0] * s,
                           a[1] * s,
                           a[2] * s};
}

template<typename Type>
inline Vector<Type, 3> operator / (const Vector<Type, 3>& a, const Vector<Type, 3>& b) {
    return Vector<Type, 3>{a[0] / b[0],
                           a[1] / b[1],
                           a[2] / b[2]};
}

template<typename Type>
inline Vector<Type, 3> operator / (const Vector<Type, 3>& a, const Type& s) {
    return Vector<Type, 3>{a[0] / s,
                           a[1] / s,
                           a[2] / s};
}

template<typename Type>
Type dot(const Vector<Type, 3>& a, const Vector<Type, 3>& b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

template<typename Type>
Vector<Type, 3> cross(const Vector<Type, 3>& a, const Vector<Type, 3>& b) {
    return Vector<Type, 3>{a[1] * b[2] - a[2] * b[1],
                           a[2] * b[0] - a[0] * b[2],
                           a[0] * b[1] - a[1] * b[0]};
}

#endif /* defined(__game__Vector3__) */
