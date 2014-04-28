//
//  Matrix.h
//  component_based_entity_system
//
//  Created by Administrator on 11.01.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//

#ifndef __game__Matrix__
#define __game__Matrix__

#include "Vector.h"
#include "MathUtility.h"

template<typename Type, int R, int C>
struct Matrix {
    typedef Matrix<Type, R, C> MatrixType;
    
    Type data[R * C];
    
    //Matrix() : Matrix{1} {}
    explicit Matrix(const Type& t = Type(1)) {
        for (int col = 0; col < C; ++col) {
            for (int row = 0; row < R; ++row) {
                (*this)(row,col) = row == col ? t : static_cast<Type>(0);
            }
        }
    }
    
    const Type& operator () (int row, int col) const {
        return data[row + R * col];
    }
    Type& operator () (int row, int col) {
        return data[row + R * col];
    }
    
    const MatrixType& operator += (const MatrixType& a) {
        for(int i = 0; i < C * R; ++i) {
            data[i] += a.data[i];
        }
        return *this;
    }
    const MatrixType& operator += (const Type& s) {
        for(int i = 0; i < C * R; ++i) {
            data[i] += s;
        }
        return *this;
    }
    const MatrixType& operator -= (const MatrixType& a) {
        for(int i = 0; i < C * R; ++i) {
            data[i] -= a.data[i];
        }
        return *this;
    }
    const MatrixType& operator -= (const Type& s) {
        for(int i = 0; i < C * R; ++i) {
            data[i] -= s;
        }
        return *this;
    }
    const MatrixType& operator *= (const Type& s) {
        for(int i = 0; i < C * R; ++i) {
            data[i] *= s;
        }
        return *this;
    }
    const MatrixType& operator /= (const Type& s) {
        for(int i = 0; i < C * R; ++i) {
            data[i] /= s;
        }
        return *this;
    }
};
    
template<typename Type, int R, int C>
inline Matrix<Type, R, C> operator + (const Matrix<Type, R, C>& a, const Matrix<Type, R, C>& b) {
    Matrix<Type, R, C> result(a);
    result += b;
    return result;
}

template<typename Type, int R, int C>
inline Matrix<Type, R, C> operator + (const Matrix<Type, R, C>& a, const Type& s) {
    Matrix<Type, R, C> result(a);
    result += s;
    return result;
}

template<typename Type, int R, int C>
inline Matrix<Type, R, C> operator + (const Type& s, const Matrix<Type, R, C>& a) {
    Matrix<Type, R, C> result(a);
    result += s;
    return result;
}

template<typename Type, int R, int C>
inline Matrix<Type, R, C> operator - (const Matrix<Type, R, C>& a, const Matrix<Type, R, C>& b) {
    Matrix<Type, R, C> result(a);
    result -= b;
    return result;
}

template<typename Type, int R, int C>
inline Matrix<Type, R, C> operator - (const Matrix<Type, R, C>& a, const Type& s) {
    Matrix<Type, R, C> result(a);
    result -= s;
    return result;
}

template<typename Type, int R0, int C0, int C1>
inline Matrix<Type, R0, C1> operator * (const Matrix<Type, R0, C0>& a, const Matrix<Type, C0, C1>& b) {
    Matrix<Type, R0, C1> result(static_cast<Type>(0));
    for (int i = 0; i < C1; ++i) {
        for (int j = 0; j < R0; ++j) {
            for (int k = 0; k < C0; ++k) {
                result(j,i) += a(j,k) * b(k,i);
            }
        }
    }
    return result;
}

template<typename Type, int R, int C>
inline Vector<Type, R> operator * (const Matrix<Type, R, C>& a, const Vector<Type, R>& b) {
    Vector<Type, R> result;
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            result[i] += a(i,j) * b[j];
        }
    }
    return result;
}

template<typename Type, int R, int C>
inline Matrix<Type, R, C> operator * (const Matrix<Type, R, C>& a, const Type& s) {
    Matrix<Type, R, C> result(a);
    result *= s;
    return result;
}

template<typename Type, int R, int C>
inline Matrix<Type, R, C> operator * (const Type& s, const Matrix<Type, R, C>& a) {
    Matrix<Type, R, C> result(a);
    result *= s;
    return result;
}

template<typename Type, int R, int C>
inline Matrix<Type, R, C> operator / (const Matrix<Type, R, C>& a, const Type& s) {
    Matrix<Type, R, C> result(a);
    result /= s;
    return result;
}
    
template<typename Type, int R, int C>
inline Matrix<Type, R, C> transpose(const Matrix<Type, R, C>& a) {
    Matrix<Type, R, C> result(static_cast<Type>(0));
    for (int i = 0; i < C; ++i) {
        for (int j = 0; j < R; ++j) {
            result(i,j) = a(j,i);
        }
    }
    return result;
}

template<typename Type, int R, int C>
std::ostream& operator << (std::ostream& stream, const Matrix<Type, R, C>& a) {
    for(int i = 0; i < R; ++i) {
        for(int j = 0; j < C; ++j) {
            stream << a(i, j) << " ";
        }
        stream << std::endl;
    }
    return stream;
}

#endif /* defined(__game__Matrix__) */
