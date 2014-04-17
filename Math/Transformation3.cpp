//
//  Transformation3.cpp
//  ComponentBasedGame
//
//  Created by Administrator on 18.06.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Transformation3.h"

////////////////////////////////////////////////////////////////////////////////

void Transformation3::set_dirty() {
    _dirty_local_to_world = true;
    _dirty_world_to_local = true;
    _dirty_world_x = true;
    _dirty_world_y = true;
    _dirty_world_z = true;
}

void Transformation3::set_rotation_dirty() {
    _dirty_local_to_world = true;
    _dirty_world_to_local = true;
    _dirty_world_x = true;
    _dirty_world_y = true;
    _dirty_world_z = true;
}

void Transformation3::set_translation_dirty() {
    _dirty_local_to_world = true;
    _dirty_world_to_local = true;
}

void Transformation3::update_local_to_world() {
    if (_dirty_local_to_world) {
        _local_to_world = ::homogeneous_rotation(_rotation);
        _local_to_world(0,3) = _translation[0];
        _local_to_world(1,3) = _translation[1];
        _local_to_world(2,3) = _translation[2];
        //_local_to_world = ::homogeneous_translation(_translation) * ::homogeneous_rotation(_rotation);
    }
    _dirty_local_to_world = false;
}

void Transformation3::update_world_to_local() {
    if (_dirty_world_to_local) {
        _world_to_local = ::homogeneous_rotation(inverse(_rotation));
        _world_to_local(0,3) = -(_world_to_local(0,0) * _translation[0] + _world_to_local(0,1) * _translation[1] + _world_to_local(0,2) * _translation[2]);
        _world_to_local(1,3) = -(_world_to_local(1,0) * _translation[0] + _world_to_local(1,1) * _translation[1] + _world_to_local(1,2) * _translation[2]);
        _world_to_local(2,3) = -(_world_to_local(2,0) * _translation[0] + _world_to_local(2,1) * _translation[1] + _world_to_local(2,2) * _translation[2]);
        //_world_to_local = ::homogeneous_rotation(inverse(_rotation)) * ::homogeneous_translation(-_translation);
    }
    _dirty_world_to_local = false;
}

void Transformation3::update_world_x() {
    if (_dirty_world_x) {
        _world_x = rotate(_rotation, Vector<float, 3>(1.0f, 0.0f, 0.0f));
    }
    _dirty_world_x = false;
}

void Transformation3::update_world_y() {
    if (_dirty_world_y) {
        _world_y = rotate(_rotation, Vector<float, 3>(0.0f, 1.0f, 0.0f));
    }
    _dirty_world_y = false;
}

void Transformation3::update_world_z() {
    if (_dirty_world_z) {
        _world_z = rotate(_rotation, Vector<float, 3>(0.0f, 0.0f, 1.0f));
    }
    _dirty_world_z = false;
}

Transformation3::Transformation3()
: _dirty_local_to_world(false), _dirty_world_to_local(false),
_dirty_world_x(true), _dirty_world_y(true), _dirty_world_z(true) {
    
}

Transformation3::Transformation3(const Quaternion<float>& rotation)
: _rotation(rotation),
_dirty_local_to_world(true), _dirty_world_to_local(true),
_dirty_world_x(true), _dirty_world_y(true), _dirty_world_z(true) {
    
}

Transformation3::Transformation3(const Vector<float, 3>& translation)
: _translation(translation),
_dirty_local_to_world(true), _dirty_world_to_local(true),
_dirty_world_x(true), _dirty_world_y(true), _dirty_world_z(true) {
    
}

Transformation3::Transformation3(const Quaternion<float>& rotation, const Vector<float, 3>& translation)
: _rotation(rotation), _translation(translation),
_dirty_local_to_world(true), _dirty_world_to_local(true),
_dirty_world_x(true), _dirty_world_y(true), _dirty_world_z(true) {
    
}

Transformation3::Transformation3(const Vector<float, 3>& eye, const Vector<float, 3>& look_at, const Vector<float, 3>& up)
: _rotation(eye, look_at, up), _translation(eye),
_dirty_local_to_world(true), _dirty_world_to_local(true),
_dirty_world_x(true), _dirty_world_y(true), _dirty_world_z(true){
    
}

// returns the rotation in world coordinates
const Quaternion<float>& Transformation3::rotation() const {
    return _rotation;
}

// returns the translation in world coordinates
const Vector<float, 3>& Transformation3::translation() const {
    return _translation;
}

// translate without taking the rotaion into account
void Transformation3::translate_world(const Vector<float, 3>& diff) {
    _translation += diff;
    set_translation_dirty();
}

// translate in the local coordinate system
void Transformation3::translate_local(const Vector<float, 3>& diff) {
    _translation += ::rotate(_rotation, diff);
    set_translation_dirty();
}

// rotate relative to world coordinate system
void Transformation3::rotate_world(const Quaternion<float>& diff) {
    _rotation = diff * _rotation;
    set_rotation_dirty();
}

// rotate relative to local coordinate system
void Transformation3::rotate_local(const Quaternion<float>& diff) {
    _rotation = _rotation * diff;
    set_rotation_dirty();
}

// set the position in the world coordinate system
void Transformation3::set_translation(const Vector<float, 3>& position) {
    _translation = position;
    set_translation_dirty();
}

// set the rotation in the world coordinate system
void Transformation3::set_rotation(const Quaternion<float>& orientation) {
    _rotation = orientation;
    set_rotation_dirty();
}

// returns the cached transformation matrix and updates it if needed
const Matrix<float, 4, 4>& Transformation3::local_to_world() {
    update_local_to_world();
    return _local_to_world;
}

const Matrix<float, 4, 4>& Transformation3::world_to_local() {
    update_world_to_local();
    return _world_to_local;
}

// returns the cached axis vectors and updates if needed
const Vector<float, 3>& Transformation3::world_x() {
    update_world_x();
    return _world_x;
}

const Vector<float, 3>& Transformation3::world_y() {
    update_world_y();
    return _world_y;
}

const Vector<float, 3>& Transformation3::world_z() {
    update_world_z();
    return _world_z;
}

// normalize the rotation quaternion
void Transformation3::normalize() {
    _rotation = quaternion_normal(_rotation);
    set_rotation_dirty();
}

////////////////////////////////////////////////////////////////////////////////
