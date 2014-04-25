//
//  Matrix4.h
//  component_based_entity_system
//
//  Created by Administrator on 11.01.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//

#ifndef __game__Matrix4__
#define __game__Matrix4__

#include "Matrix.h"

template<typename Type>
struct Matrix<Type,4,4> {
    enum { R = 4, C = 4 };
    typedef Matrix<Type, R, C> MatrixType;
    
    Type data[R * C];
    
    /*
     * Create a identity matrix.
     */
    Matrix() : Matrix{1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1} {}
    
    /*
     * Create a diagonal matrix and initialize the diagonal with t.
     */
    explicit Matrix(const Type& t) : Matrix{t, 0, 0, 0,
                                            0, t, 0, 0,
                                            0, 0, t, 0,
                                            0, 0, 0, t} {}
    Matrix(const Type& m00, const Type& m01, const Type& m02, const Type& m03,
           const Type& m10, const Type& m11, const Type& m12, const Type& m13,
           const Type& m20, const Type& m21, const Type& m22, const Type& m23,
           const Type& m30, const Type& m31, const Type& m32, const Type& m33) {
        (*this)(0,0) = m00; (*this)(0,1) = m01; (*this)(0,2) = m02; (*this)(0,3) = m03;
        (*this)(1,0) = m10; (*this)(1,1) = m11; (*this)(1,2) = m12; (*this)(1,3) = m13;
        (*this)(2,0) = m20; (*this)(2,1) = m21; (*this)(2,2) = m22; (*this)(2,3) = m23;
        (*this)(3,0) = m30; (*this)(3,1) = m31; (*this)(3,2) = m32; (*this)(3,3) = m33;
    }
    
    const Type& operator () (int row, int col) const {
        return data[row + R * col];
    }
    Type& operator () (int row, int col) {
        return data[row + R * col];
    }
    
    const MatrixType& operator += (const MatrixType& a) {
        (*this)(0,0) += a(0,0); (*this)(0,1) += a(0,1); (*this)(0,2) += a(0,2); (*this)(0,3) += a(0,3);
        (*this)(1,0) += a(1,0); (*this)(1,1) += a(1,1); (*this)(1,2) += a(1,2); (*this)(1,3) += a(1,3);
        (*this)(2,0) += a(2,0); (*this)(2,1) += a(2,1); (*this)(2,2) += a(2,2); (*this)(2,3) += a(2,3);
        (*this)(3,0) += a(3,0); (*this)(3,1) += a(3,1); (*this)(3,2) += a(3,2); (*this)(3,3) += a(3,3);
        return *this;
    }
    const MatrixType& operator += (const Type& s) {
        (*this)(0,0) += s; (*this)(0,1) += s; (*this)(0,2) += s; (*this)(0,3) += s;
        (*this)(1,0) += s; (*this)(1,1) += s; (*this)(1,2) += s; (*this)(1,3) += s;
        (*this)(2,0) += s; (*this)(2,1) += s; (*this)(2,2) += s; (*this)(2,3) += s;
        (*this)(3,0) += s; (*this)(3,1) += s; (*this)(3,2) += s; (*this)(3,3) += s;
        return *this;
    }
    const MatrixType& operator -= (const MatrixType& a) {
        (*this)(0,0) -= a(0,0); (*this)(0,1) -= a(0,1); (*this)(0,2) -= a(0,2); (*this)(0,3) -= a(0,3);
        (*this)(1,0) -= a(1,0); (*this)(1,1) -= a(1,1); (*this)(1,2) -= a(1,2); (*this)(1,3) -= a(1,3);
        (*this)(2,0) -= a(2,0); (*this)(2,1) -= a(2,1); (*this)(2,2) -= a(2,2); (*this)(2,3) -= a(2,3);
        (*this)(3,0) -= a(3,0); (*this)(3,1) -= a(3,1); (*this)(3,2) -= a(3,2); (*this)(3,3) -= a(3,3);
        return *this;
    }
    const MatrixType& operator -= (const Type& s) {
        (*this)(0,0) -= s; (*this)(0,1) -= s; (*this)(0,2) -= s; (*this)(0,3) -= s;
        (*this)(1,0) -= s; (*this)(1,1) -= s; (*this)(1,2) -= s; (*this)(1,3) -= s;
        (*this)(2,0) -= s; (*this)(2,1) -= s; (*this)(2,2) -= s; (*this)(2,3) -= s;
        (*this)(3,0) -= s; (*this)(3,1) -= s; (*this)(3,2) -= s; (*this)(3,3) -= s;
        return *this;
    }
    const MatrixType& operator *= (const Type& s) {
        (*this)(0,0) *= s; (*this)(0,1) *= s; (*this)(0,2) *= s; (*this)(0,3) *= s;
        (*this)(1,0) *= s; (*this)(1,1) *= s; (*this)(1,2) *= s; (*this)(1,3) *= s;
        (*this)(2,0) *= s; (*this)(2,1) *= s; (*this)(2,2) *= s; (*this)(2,3) *= s;
        (*this)(3,0) *= s; (*this)(3,1) *= s; (*this)(3,2) *= s; (*this)(3,3) *= s;
        return *this;
    }
    const MatrixType& operator /= (const Type& s) {
        (*this)(0,0) /= s; (*this)(0,1) /= s; (*this)(0,2) /= s; (*this)(0,3) /= s;
        (*this)(1,0) /= s; (*this)(1,1) /= s; (*this)(1,2) /= s; (*this)(1,3) /= s;
        (*this)(2,0) /= s; (*this)(2,1) /= s; (*this)(2,2) /= s; (*this)(2,3) /= s;
        (*this)(3,0) /= s; (*this)(3,1) /= s; (*this)(3,2) /= s; (*this)(3,3) /= s;
        return *this;
    }
};

template<typename Type>
inline Type det(const Matrix<Type,4,4>& a) {
    Type t0 = a(2,2) * a(3,3) - a(3,2) * a(2,3);
    Type t1 = a(2,1) * a(3,3) - a(3,1) * a(2,3);
    Type t2 = a(2,0) * a(3,3) - a(3,0) * a(2,3);
    Type t3 = a(2,1) * a(3,2) - a(3,1) * a(2,2);
    Type t4 = a(2,0) * a(3,2) - a(3,0) * a(2,2);
    Type t5 = a(2,0) * a(3,1) - a(3,0) * a(2,1);
    return a(0,0) * (a(1,1) * t0 - a(1,2) * t1 + a(1,3) * t3)
         - a(0,1) * (a(1,0) * t0 - a(1,2) * t2 + a(1,3) * t4)
         + a(0,2) * (a(1,0) * t1 - a(1,1) * t2 + a(1,3) * t5)
         - a(0,3) * (a(1,0) * t3 - a(1,1) * t4 + a(1,2) * t5);
}

template<typename Type>
inline Matrix<Type,4,4> inverse(const Matrix<Type,4,4>& a) {
    Matrix<Type, 4, 4> result;
    result.data[0] = a.data[5]  * a.data[10] * a.data[15] -
                     a.data[5]  * a.data[11] * a.data[14] -
                     a.data[9]  * a.data[6]  * a.data[15] +
                     a.data[9]  * a.data[7]  * a.data[14] +
                     a.data[13] * a.data[6]  * a.data[11] -
                     a.data[13] * a.data[7]  * a.data[10];
    
    result.data[4] = -a.data[4]  * a.data[10] * a.data[15] +
                      a.data[4]  * a.data[11] * a.data[14] +
                      a.data[8]  * a.data[6]  * a.data[15] -
                      a.data[8]  * a.data[7]  * a.data[14] -
                      a.data[12] * a.data[6]  * a.data[11] +
                      a.data[12] * a.data[7]  * a.data[10];
    
    result.data[8] = a.data[4]  * a.data[9]  * a.data[15] -
                     a.data[4]  * a.data[11] * a.data[13] -
                     a.data[8]  * a.data[5]  * a.data[15] +
                     a.data[8]  * a.data[7]  * a.data[13] +
                     a.data[12] * a.data[5]  * a.data[11] -
                     a.data[12] * a.data[7]  * a.data[9];
    
    result.data[12] = -a.data[4]  * a.data[9]  * a.data[14] +
                       a.data[4]  * a.data[10] * a.data[13] +
                       a.data[8]  * a.data[5]  * a.data[14] -
                       a.data[8]  * a.data[6]  * a.data[13] -
                       a.data[12] * a.data[5]  * a.data[10] +
                       a.data[12] * a.data[6]  * a.data[9];
    
    result.data[1] = -a.data[1]  * a.data[10] * a.data[15] +
                      a.data[1]  * a.data[11] * a.data[14] +
                      a.data[9]  * a.data[2] * a.data[15] -
                      a.data[9]  * a.data[3] * a.data[14] -
                      a.data[13] * a.data[2] * a.data[11] +
                      a.data[13] * a.data[3] * a.data[10];
    
    result.data[5] = a.data[0]  * a.data[10] * a.data[15] -
                     a.data[0]  * a.data[11] * a.data[14] -
                     a.data[8]  * a.data[2] * a.data[15] +
                     a.data[8]  * a.data[3] * a.data[14] +
                     a.data[12] * a.data[2] * a.data[11] -
                     a.data[12] * a.data[3] * a.data[10];
    
    result.data[9] = -a.data[0]  * a.data[9] * a.data[15] +
                      a.data[0]  * a.data[11] * a.data[13] +
                      a.data[8]  * a.data[1] * a.data[15] -
                      a.data[8]  * a.data[3] * a.data[13] -
                      a.data[12] * a.data[1] * a.data[11] +
                      a.data[12] * a.data[3] * a.data[9];
    
    result.data[13] = a.data[0]  * a.data[9] * a.data[14] -
                      a.data[0]  * a.data[10] * a.data[13] -
                      a.data[8]  * a.data[1] * a.data[14] +
                      a.data[8]  * a.data[2] * a.data[13] +
                      a.data[12] * a.data[1] * a.data[10] -
                      a.data[12] * a.data[2] * a.data[9];
    
    result.data[2] = a.data[1]  * a.data[6] * a.data[15] -
                     a.data[1]  * a.data[7] * a.data[14] -
                     a.data[5]  * a.data[2] * a.data[15] +
                     a.data[5]  * a.data[3] * a.data[14] +
                     a.data[13] * a.data[2] * a.data[7] -
                     a.data[13] * a.data[3] * a.data[6];
    
    result.data[6] = -a.data[0]  * a.data[6] * a.data[15] +
                      a.data[0]  * a.data[7] * a.data[14] +
                      a.data[4]  * a.data[2] * a.data[15] -
                      a.data[4]  * a.data[3] * a.data[14] -
                      a.data[12] * a.data[2] * a.data[7] +
                      a.data[12] * a.data[3] * a.data[6];
    
    result.data[10] = a.data[0]  * a.data[5] * a.data[15] -
                      a.data[0]  * a.data[7] * a.data[13] -
                      a.data[4]  * a.data[1] * a.data[15] +
                      a.data[4]  * a.data[3] * a.data[13] +
                      a.data[12] * a.data[1] * a.data[7] -
                      a.data[12] * a.data[3] * a.data[5];
    
    result.data[14] = -a.data[0]  * a.data[5] * a.data[14] +
                       a.data[0]  * a.data[6] * a.data[13] +
                       a.data[4]  * a.data[1] * a.data[14] -
                       a.data[4]  * a.data[2] * a.data[13] -
                       a.data[12] * a.data[1] * a.data[6] +
                       a.data[12] * a.data[2] * a.data[5];
    
    result.data[3] = -a.data[1] * a.data[6] * a.data[11] +
                      a.data[1] * a.data[7] * a.data[10] +
                      a.data[5] * a.data[2] * a.data[11] -
                      a.data[5] * a.data[3] * a.data[10] -
                      a.data[9] * a.data[2] * a.data[7] +
                      a.data[9] * a.data[3] * a.data[6];
    
    result.data[7] = a.data[0] * a.data[6] * a.data[11] -
                     a.data[0] * a.data[7] * a.data[10] -
                     a.data[4] * a.data[2] * a.data[11] +
                     a.data[4] * a.data[3] * a.data[10] +
                     a.data[8] * a.data[2] * a.data[7] -
                     a.data[8] * a.data[3] * a.data[6];
    
    result.data[11] = -a.data[0] * a.data[5] * a.data[11] +
                       a.data[0] * a.data[7] * a.data[9] +
                       a.data[4] * a.data[1] * a.data[11] -
                       a.data[4] * a.data[3] * a.data[9] -
                       a.data[8] * a.data[1] * a.data[7] +
                       a.data[8] * a.data[3] * a.data[5];
    
    result.data[15] = a.data[0] * a.data[5] * a.data[10] -
                      a.data[0] * a.data[6] * a.data[9] -
                      a.data[4] * a.data[1] * a.data[10] +
                      a.data[4] * a.data[2] * a.data[9] +
                      a.data[8] * a.data[1] * a.data[6] -
                      a.data[8] * a.data[2] * a.data[5];
    Type det = a.data[0] * result.data[0] + a.data[1] * result.data[4]
             + a.data[2] * result.data[8] + a.data[3] * result.data[12];
    det = 1.0 / det;
    result *= det;
    return result;
}

#endif /* defined(__game__Matrix4__) */
