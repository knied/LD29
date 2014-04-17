//
//  Plane3.cpp
//  ComponentBasedGame
//
//  Created by Kristof Niederholtmeyer on 04.10.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Plane3.h"

////////////////////////////////////////////////////////////////////////////////

Vector<float, 3> intersection(const Plane3& p0, const Plane3& p1, const Plane3& p2) {
    Vector<float, 3> c12 = cross(p1.normal(), p2.normal());
    Vector<float, 3> c20 = cross(p2.normal(), p0.normal());
    Vector<float, 3> c01 = cross(p0.normal(), p1.normal());
    return (-c12 * p0.d() - c20 * p1.d() - c01 * p2.d()) / dot(p0.normal(), c12);
}

std::ostream& operator << (std::ostream& stream, const Plane3& plane) {
    stream << plane.a() << " " << plane.b() << " " << plane.c() << " " << plane.d();
    return stream;
}

////////////////////////////////////////////////////////////////////////////////
