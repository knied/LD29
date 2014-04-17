//
//  MathUtility.cpp
//  component_based_entity_system
//
//  Created by Administrator on 11.01.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//

#include "MathUtility.h"
#include <math.h>

template<>
float sqrt(const float& v) {
    return ::sqrtf(v);
}
template<>
double sqrt(const double& v) {
    return ::sqrt(v);
}

template<>
float sin(const float& v) {
    return ::sinf(v);
}
template<>
double sin(const double& v) {
    return ::sin(v);
}
template<>
float cos(const float& v) {
    return ::cosf(v);
}
template<>
double cos(const double& v) {
    return ::cos(v);
}
template<>
float tan(const float& v) {
    return ::tanf(v);
}
template<>
double tan(const double& v) {
    return ::tan(v);
}

template<>
float asin(const float& v) {
    return ::asinf(v);
}
template<>
double asin(const double& v) {
    return ::asin(v);
}
template<>
float acos(const float& v) {
    return ::acosf(v);
}
template<>
double acos(const double& v) {
    return ::acos(v);
}
template<>
float atan(const float& v) {
    return ::atanf(v);
}
template<>
double atan(const double& v) {
    return ::atan(v);
}

template<>
float atan2(const float& y, const float& x) {
    return ::atan2f(y, x);
}
template<>
double atan2(const double& y, const double& x) {
    return ::atan2(y, x);
}