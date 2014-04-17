//
//  Transformation3.h
//  ComponentBasedGame
//
//  Created by Administrator on 18.06.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __ComponentBasedGame__Transformation3__
#define __ComponentBasedGame__Transformation3__

////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Math.h"

////////////////////////////////////////////////////////////////////////////////

class Transformation3 {
    // cached matrices
    bool _dirty_local_to_world;
    Matrix<float, 4, 4> _local_to_world;
    bool _dirty_world_to_local;
    Matrix<float, 4, 4> _world_to_local;
    
    // cached axis vectors
    bool _dirty_world_x;
    Vector<float, 3> _world_x;
    bool _dirty_world_y;
    Vector<float, 3> _world_y;
    bool _dirty_world_z;
    Vector<float, 3> _world_z;
    
    // manage the cache
    void set_dirty();
    void set_rotation_dirty();
    void set_translation_dirty();
    void update_local_to_world();
    void update_world_to_local();
    void update_world_x();
    void update_world_y();
    void update_world_z();
    
    // rotation
    Quaternion<float> _rotation;
    
    // position in world coordinate system
    Vector<float, 3> _translation;
    
public:
    Transformation3();
    Transformation3(const Quaternion<float>& rotation);
    Transformation3(const Vector<float, 3>& translation);
    Transformation3(const Quaternion<float>& rotation, const Vector<float, 3>& translation);
    Transformation3(const Vector<float, 3>& eye, const Vector<float, 3>& look_at, const Vector<float, 3>& up);
    
    // returns the rotation in world coordinates
    const Quaternion<float>& rotation() const;
    // returns the translation in world coordinates
    const Vector<float, 3>& translation() const;
    
    // translate without taking the rotaion into account
    void translate_world(const Vector<float, 3>& diff);
    // translate in the local coordinate system
    void translate_local(const Vector<float, 3>& diff);
    
    // rotate relatice to the world coordinate system
    void rotate_world(const Quaternion<float>& diff);
    // rotate relative to local coordinate system
    void rotate_local(const Quaternion<float>& diff);
    
    // set the position in the world coordinate system
    void set_translation(const Vector<float, 3>& position);
    
    // set the rotation in the world coordinate system
    void set_rotation(const Quaternion<float>& orientation);
    
    // returns the cached transformation matrix and updates it if needed
    const Matrix<float, 4, 4>& local_to_world();
    const Matrix<float, 4, 4>& world_to_local();
    
    // returns the cached axis vectors and updates if needed
    const Vector<float, 3>& world_x();
    const Vector<float, 3>& world_y();
    const Vector<float, 3>& world_z();
    
    // normalize the rotation quaternion
    void normalize();
};

////////////////////////////////////////////////////////////////////////////////

#endif /* defined(__ComponentBasedGame__Transformation3__) */

////////////////////////////////////////////////////////////////////////////////
