//
//  GameShapes.h
//  LD29
//
//  Created by Kristof Niederholtmeyer on 30.04.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//

#ifndef __LD29__GameShapes__
#define __LD29__GameShapes__

#include "GameDraw.h"

void init_flag_mesh(GLShape& mesh);
void init_small_flag_mesh(GLShape& mesh);
void init_crown_mesh(GLShape& mesh);
void init_decal_mesh(GLShape& mesh);
void init_dot_mesh(GLShape& mesh, float r, int d);
void init_indicator_mesh(GLShape& mesh);
void init_mine_mesh(GLShape& base_mesh, GLShape& wheel_mesh);
void init_star_mesh(GLShape& mesh);

#endif /* defined(__LD29__GameShapes__) */
