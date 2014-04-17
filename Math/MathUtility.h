//
//  MathUtility.h
//  component_based_entity_system
//
//  Created by Administrator on 11.01.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//

#ifndef __game__MathUtility__
#define __game__MathUtility__

#include <iostream>

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

template<typename Type>
Type sqrt(const Type& v);

template<typename Type>
Type sin(const Type& v);
template<typename Type>
Type cos(const Type& v);
template<typename Type>
Type tan(const Type& v);

template<typename Type>
Type asin(const Type& v);
template<typename Type>
Type acos(const Type& v);
template<typename Type>
Type atan(const Type& v);

template<typename Type>
Type atan2(const Type& y, const Type& x);

#endif /* defined(__game__MathUtility__) */
