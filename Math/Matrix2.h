//
//  Matrix2.h
//  component_based_entity_system
//
//  Created by Administrator on 11.01.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//

#ifndef __game__Matrix2__
#define __game__Matrix2__

#include "Matrix.h"

template<typename Type>
struct Matrix<Type,2,2> {
    enum { R = 2, C = 2 };
    typedef Matrix<Type, R, C> MatrixType;
    
    Type data[R * C];
    
    explicit Matrix(const Type& t = Type(1)) {
        (*this)(0,0) = t; (*this)(0,1) = Type(0);
        (*this)(1,0) = Type(0); (*this)(1,1) = t;
    }
    Matrix(const Type& m00, const Type& m01,
           const Type& m10, const Type& m11) {
        (*this)(0,0) = m00; (*this)(0,1) = m01;
        (*this)(1,0) = m10; (*this)(1,1) = m11;
    }
    
    const Type& operator () (int row, int col) const {
        return data[row + R * col];
    }
    Type& operator () (int row, int col) {
        return data[row + R * col];
    }
    
    const MatrixType& operator += (const MatrixType& a) {
        (*this)(0,0) += a(0,0); (*this)(0,1) += a(0,1);
        (*this)(1,0) += a(1,0); (*this)(1,1) += a(1,1);
        return *this;
    }
    const MatrixType& operator += (const Type& s) {
        (*this)(0,0) += s; (*this)(0,1) += s;
        (*this)(1,0) += s; (*this)(1,1) += s;
        return *this;
    }
    const MatrixType& operator -= (const MatrixType& a) {
        (*this)(0,0) -= a(0,0); (*this)(0,1) -= a(0,1);
        (*this)(1,0) -= a(1,0); (*this)(1,1) -= a(1,1);
        return *this;
    }
    const MatrixType& operator -= (const Type& s) {
        (*this)(0,0) -= s; (*this)(0,1) -= s;
        (*this)(1,0) -= s; (*this)(1,1) -= s;
        return *this;
    }
    const MatrixType& operator *= (const Type& s) {
        (*this)(0,0) *= s; (*this)(0,1) *= s;
        (*this)(1,0) *= s; (*this)(1,1) *= s;
        return *this;
    }
    const MatrixType& operator /= (const Type& s) {
        (*this)(0,0) /= s; (*this)(0,1) /= s;
        (*this)(1,0) /= s; (*this)(1,1) /= s;
        return *this;
    }
};

template<typename Type>
inline Type det(const Matrix<Type,2,2>& a) {
    return a(0,0) * a(1,1) - a(1,0) * a(0,1);
}

template<typename Type>
inline Matrix<Type,2,2> inverse(const Matrix<Type,2,2>& a) {
    Matrix<Type,2,2> result(a(1,1), -a(0,1),
                            -a(1,0), a(0,0));
    return result / det(a);
}

#endif /* defined(__game__Matrix2__) */
