//
//  GameDraw.h
//  LD29
//
//  Created by Kristof Niederholtmeyer on 30.04.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//

#ifndef __LD29__GameDraw__
#define __LD29__GameDraw__

#include <iostream>
#include <vector>
#include "Camera.h"

typedef std::vector<Vector3> GLShape;

void gl_init(Camera const& camera, Color4 const& clear_color);
void gl_draw(Matrix4 const& model_view, GLShape const& shape, Color4 const& color);
void gl_clear();
void gl_enable_depth();
void gl_disable_depth();

#endif /* defined(__LD29__GameDraw__) */
