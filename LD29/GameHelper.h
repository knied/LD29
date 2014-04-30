//
//  GameHelper.h
//  LD29
//
//  Created by Kristof Niederholtmeyer on 30.04.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//

#ifndef __LD29__GameHelper__
#define __LD29__GameHelper__

#include <iostream>
#include <vector>
#include "Geometry.h"
#include "Types.h"

struct Rect2 {
    Vector2 origin;
    Vector2 size;
};

Color4 color_interpolation(Color4 const& color0, Color4 const& color1, float t);
Vector2 point_on_screen(Matrix4 const& view_projection, Vector3 const& p);
Rect2 rect_on_screen(Matrix4 const& view_projection, std::vector<Vector3> const& vertices);
bool point_in_rect(Rect2 const& r, Vector2 const& p);
bool cursor_on_shape(Matrix4 const& model_view_projection, std::vector<Vector3> const& vertices, Vector2 const& cursor);

#endif /* defined(__LD29__GameHelper__) */
