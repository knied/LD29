//
//  Matrix3.h
//  component_based_entity_system
//
//  Created by Administrator on 11.01.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//

#ifndef __game__Matrix3__
#define __game__Matrix3__

#include "Matrix.h"

template<typename Type>
struct Matrix<Type,3,3> {
    enum { R = 3, C = 3 };
    typedef Matrix<Type, R, C> MatrixType;
    
    Type data[R * C];
    
    Matrix() : Matrix{1, 0, 0,
                      0, 1, 0,
                      0, 0, 1} {}
    explicit Matrix(const Type& t) : Matrix{t, 0, 0,
                                            0, t, 0,
                                            0, 0, t} {}
    Matrix(const Type& m00, const Type& m01, const Type& m02,
           const Type& m10, const Type& m11, const Type& m12,
           const Type& m20, const Type& m21, const Type& m22) {
        (*this)(0,0) = m00; (*this)(0,1) = m01; (*this)(0,2) = m02;
        (*this)(1,0) = m10; (*this)(1,1) = m11; (*this)(1,2) = m12;
        (*this)(2,0) = m20; (*this)(2,1) = m21; (*this)(2,2) = m22;
    }
    
    const Type& operator () (int row, int col) const {
        return data[row + R * col];
    }
    Type& operator () (int row, int col) {
        return data[row + R * col];
    }
    
    const MatrixType& operator += (const MatrixType& a) {
        (*this)(0,0) += a(0,0); (*this)(0,1) += a(0,1); (*this)(0,2) += a(0,2);
        (*this)(1,0) += a(1,0); (*this)(1,1) += a(1,1); (*this)(1,2) += a(1,2);
        (*this)(2,0) += a(2,0); (*this)(2,1) += a(2,1); (*this)(2,2) += a(2,2);
        return *this;
    }
    const MatrixType& operator += (const Type& s) {
        (*this)(0,0) += s; (*this)(0,1) += s; (*this)(0,2) += s;
        (*this)(1,0) += s; (*this)(1,1) += s; (*this)(1,2) += s;
        (*this)(2,0) += s; (*this)(2,1) += s; (*this)(2,2) += s;
        return *this;
    }
    const MatrixType& operator -= (const MatrixType& a) {
        (*this)(0,0) -= a(0,0); (*this)(0,1) -= a(0,1); (*this)(0,2) -= a(0,2);
        (*this)(1,0) -= a(1,0); (*this)(1,1) -= a(1,1); (*this)(1,2) -= a(1,2);
        (*this)(2,0) -= a(2,0); (*this)(2,1) -= a(2,1); (*this)(2,2) -= a(2,2);
        return *this;
    }
    const MatrixType& operator -= (const Type& s) {
        (*this)(0,0) -= s; (*this)(0,1) -= s; (*this)(0,2) -= s;
        (*this)(1,0) -= s; (*this)(1,1) -= s; (*this)(1,2) -= s;
        (*this)(2,0) -= s; (*this)(2,1) -= s; (*this)(2,2) -= s;
        return *this;
    }
    const MatrixType& operator *= (const Type& s) {
        (*this)(0,0) *= s; (*this)(0,1) *= s; (*this)(0,2) *= s;
        (*this)(1,0) *= s; (*this)(1,1) *= s; (*this)(1,2) *= s;
        (*this)(2,0) *= s; (*this)(2,1) *= s; (*this)(2,2) *= s;
        return *this;
    }
    const MatrixType& operator /= (const Type& s) {
        (*this)(0,0) /= s; (*this)(0,1) /= s; (*this)(0,2) /= s;
        (*this)(1,0) /= s; (*this)(1,1) /= s; (*this)(1,2) /= s;
        (*this)(2,0) /= s; (*this)(2,1) /= s; (*this)(2,2) /= s;
        return *this;
    }
};
    
    template<typename Type>
    inline Type det(const Matrix<Type,3,3>& a) {
        return  a(0,0) * (a(1,1) * a(2,2) - a(1,2) * a(2,1)) +
                a(0,1) * (a(1,2) * a(2,0) - a(1,0) * a(2,2)) +
                a(0,2) * (a(1,0) * a(2,1) - a(2,2) * a(2,0));
    }
    
    template<typename Type>
    inline Matrix<Type,3,3> inverse(const Matrix<Type,3,3>& a) {
        Matrix<Type,3,3> result(a(1,1) * a(2,2) - a(1,2) * a(2,1), a(0,2) * a(2,1) - a(0,1) * a(2,2), a(0,1) * a(1,2) - a(0,2) * a(1,1),
                                a(1,2) * a(2,0) - a(1,0) * a(2,2), a(0,0) * a(2,2) - a(0,2) * a(2,0), a(0,2) * a(1,0) - a(0,0) * a(1,2),
                                a(1,0) * a(2,1) - a(1,1) * a(2,0), a(0,1) * a(2,0) - a(0,0) * a(2,1), a(0,0) * a(1,1) - a(0,1) * a(1,0));
        return result / det(a);
    }

#endif /* defined(__game__Matrix3__) */
