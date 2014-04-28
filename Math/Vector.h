//
//  Vector.h
//  component_based_entity_system
//
//  Created by Administrator on 11.01.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//

#ifndef __game__Vector__
#define __game__Vector__

#include <iostream>
#include "MathUtility.h"

/*
 * A n-dimensional vector.
 */
template<typename Type, int N>
struct Vector {
    typedef Vector<Type,N> VectorType;
    
    Type data[N];
    
    /*
     * Create a vector an initialize each element with 0.
     */
    //Vector() : Vector{0} {}
    
    /*
     * Create a vector an initialize each element with t.
     */
    explicit Vector(const Type& t = Type(0)) {
        for (int i = 0; i < N; ++i) {
            data[i] = t;
        }
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
        for (int i = 0; i < N; ++i) {
            (*this)[i] += v[i];
        }
        return *this;
    }
    
    /*
     * Componentwise addition of a scalar.
     */
    const VectorType& operator += (const Type& s) {
        for (int i = 0; i < N; ++i) {
            (*this)[i] += s;
        }
        return *this;
    }
    
    /*
     * Componentwise subtraction.
     */
    const VectorType& operator -= (const VectorType& v) {
        for (int i = 0; i < N; ++i) {
            (*this)[i] -= v[i];
        }
        return *this;
    }
    
    /*
     * Componentwise subtraction of a scalar.
     */
    const VectorType& operator -= (const Type& s) {
        for (int i = 0; i < N; ++i) {
            (*this)[i] -= s;
        }
        return *this;
    }
    
    /*
     * Componentwise multiplication.
     */
    const VectorType& operator *= (const VectorType& v) {
        for (int i = 0; i < N; ++i) {
            (*this)[i] *= v[i];
        }
        return *this;
    }
    
    /*
     * Componentwise multiplication of a scalar.
     */
    const VectorType& operator *= (const Type& s) {
        for (int i = 0; i < N; ++i) {
            (*this)[i] *= s;
        }
        return *this;
    }
    
    /*
     * Componentwise division.
     */
    const VectorType& operator /= (const VectorType& v) {
        for (int i = 0; i < N; ++i) {
            (*this)[i] /= v[i];
        }
        return *this;
    }
    
    /*
     * Componentwise division by a scalar.
     */
    const VectorType& operator /= (const Type& s) {
        for (int i = 0; i < N; ++i) {
            (*this)[i] /= s;
        }
        return *this;
    }
}; // Vector
    
template<typename Type, int N>
inline Vector<Type, N> operator + (const Vector<Type, N>& a, const Vector<Type, N>& b) {
    Vector<Type, N> result;
    for(int i = 0; i < N; ++i) {
        result[i] = a[i] + b[i];
    }
    return result;
}

template<typename Type, int N>
inline Vector<Type, N> operator + (const Vector<Type, N>& a, const Type& s) {
    Vector<Type, N> result;
    for(int i = 0; i < N; ++i) {
        result[i] = a[i] + s;
    }
    return result;
}

template<typename Type, int N>
inline Vector<Type, N> operator + (const Type& s, const Vector<Type, N>& a) {
    Vector<Type, N> result;
    for(int i = 0; i < N; ++i) {
        result[i] = a[i] + s;
    }
    return result;
}

template<typename Type, int N>
inline Vector<Type, N> operator - (const Vector<Type, N>& a, const Vector<Type, N>& b) {
    Vector<Type, N> result;
    for(int i = 0; i < N; ++i) {
        result[i] = a[i] - b[i];
    }
    return result;
}

template<typename Type, int N>
inline Vector<Type, N> operator - (const Vector<Type, N>& a, const Type& s) {
    Vector<Type, N> result;
    for(int i = 0; i < N; ++i) {
        result[i] = a[i] - s;
    }
    return result;
}

template<typename Type, int N>
inline Vector<Type, N> operator - (const Vector<Type, N>& a) {
    Vector<Type, N> result;
    for(int i = 0; i < N; ++i) {
        result[i] = -a[i];
    }
    return result;
}

template<typename Type, int N>
inline Vector<Type, N> operator * (const Vector<Type, N>& a, const Vector<Type, N>& b) {
    Vector<Type, N> result;
    for(int i = 0; i < N; ++i) {
        result[i] = a[i] * b[i];
    }
    return result;
}

template<typename Type, int N>
inline Vector<Type, N> operator * (const Vector<Type, N>& a, const Type& s) {
    Vector<Type, N> result;
    for(int i = 0; i < N; ++i) {
        result[i] = a[i] * s;
    }
    return result;
}

template<typename Type, int N>
inline Vector<Type, N> operator * (const Type& s, const Vector<Type, N>& a) {
    Vector<Type, N> result;
    for(int i = 0; i < N; ++i) {
        result[i] = a[i] * s;
    }
    return result;
}

template<typename Type, int N>
inline Vector<Type, N> operator / (const Vector<Type, N>& a, const Vector<Type, N>& b) {
    Vector<Type, N> result;
    for(int i = 0; i < N; ++i) {
        result[i] = a[i] / b[i];
    }
    return result;
}

template<typename Type, int N>
inline Vector<Type, N> operator / (const Vector<Type, N>& a, const Type& s) {
    Vector<Type, N> result;
    for(int i = 0; i < N; ++i) {
        result[i] = a[i] / s;
    }
    return result;
}

template<typename Type, int N>
Type dot(const Vector<Type, N>& a, const Vector<Type, N>& b) {
    Type result = static_cast<Type>(0);
    for (int i = 0; i < N; ++i) {
        result += a[i] * b[i];
    }
    return result;
}

template<typename Type, int N>
Type squared_length(const Vector<Type, N>& a) {
    return dot(a, a);
}

template<typename Type, int N>
Type length(const Vector<Type, N>& a) {
    return sqrt(dot(a, a));
}

template<typename Type, int N>
Vector<Type, N> vector_normal(const Vector<Type, N>& a) {
    return a / (length(a) + (Type)0.000000000001);
}

template<typename Type, int N>
std::ostream& operator << (std::ostream& stream, const Vector<Type, N>& v) {
    for(int i = 0; i < N; ++i) {
        stream << v[i];
        if(i != (N - 1)) {
            stream << " ";
        }
    }
    return stream;
}

template<typename Type, int N>
    std::istream& operator >> (std::istream& stream, Vector<Type, N>& v) {
    for(int i = 0; i < N; ++i) {
        stream >> v[i];
    }
    return stream;
}

#endif /* defined(__game__Vector__) */
