//
//  Plane3.h
//  ComponentBasedGame
//
//  Created by Kristof Niederholtmeyer on 04.10.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __ComponentBasedGame__Plane3__
#define __ComponentBasedGame__Plane3__

////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Math.h"

////////////////////////////////////////////////////////////////////////////////

class Plane3 {
    Vector<float, 3> _normal;
    float _d;
    
public:
    Plane3() : _normal(0.0f, 1.0f, 0.0f), _d(0) {
        
    }
    Plane3(const Vector<float, 3>& dir, float d) {
        float l = 1.0f/length(dir);
        _normal = dir * l;
        _d = d * l;
    }
    Plane3(float a, float b, float c, float d) {
        Vector<float, 3> dir(a, b, c);
        float l = 1.0f/length(dir);
        _normal = dir * l;
        _d = d * l;
    }
    
    const Vector<float, 3>& normal() const {
        return _normal;
    }
    float a() const {
        return _normal[0];
    }
    float b() const {
        return _normal[1];
    }
    float c() const {
        return _normal[2];
    }
    float d() const {
        return _d;
    }
    
};

Vector<float, 3> intersection(const Plane3& p0, const Plane3& p1, const Plane3& p2);

std::ostream& operator << (std::ostream& stream, const Plane3& plane);

////////////////////////////////////////////////////////////////////////////////

#endif /* defined(__ComponentBasedGame__Plane3__) */

////////////////////////////////////////////////////////////////////////////////
