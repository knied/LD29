//
//  Camera.h
//  Debug3D
//
//  Created by Kristof Niederholtmeyer on 09.02.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __Debug3D__Camera__
#define __Debug3D__Camera__

////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Types.h"

////////////////////////////////////////////////////////////////////////////////

class Camera {
    Transformation3 _transformation;
    Vector3 _position;
    float _distance;
    
    int _width;
    int _height;
    float _near;
    float _far;
    float _fov;
    
public:
    Camera(Vector3 const& position = Vector3(), float const distance = 2.0f) {
        _position = position;
        _distance = distance;
        _transformation.set_translation(_position);
        _transformation.translate_local(Vector3(0.0f, 0.0f, _distance));
        _width = 800;
        _height = 600;
        _near = 0.1f;
        _far = 100.0f;
        _fov = 80.0f;
    }
    
    void rotate(float const x, float const y) {
        _transformation.set_translation(Vector3());
        _transformation.rotate_local(Quaternion<float>(Vector3(1.0f, 0.0f, 0.0f), x));
        _transformation.rotate_world(Quaternion<float>(Vector3(0.0f, 1.0f, 0.0f), y));
        _transformation.set_translation(_position);
        _transformation.translate_local(Vector3(0.0f, 0.0f, _distance));
    }
    
    void set_rotaton(float const x, float const y) {
        _transformation.set_translation(Vector3());
        _transformation.set_rotation(Quaternion<float>(Vector3(0.0f, 1.0f, 0.0f), y) * Quaternion<float>(Vector3(1.0f, 0.0f, 0.0f), x));
        _transformation.set_translation(_position);
        _transformation.translate_local(Vector3(0.0f, 0.0f, _distance));
    }
    
    void set_position(Vector3 const& position) {
        _position = position;
        _transformation.set_translation(_position);
        _transformation.translate_local(Vector3(0.0f, 0.0f, _distance));
    }
    
    void move(float const dx, float const dy, float const dz) {
        Matrix<float, 4, 4> _matrix = _transformation.local_to_world();
        Vector3 up = transformed_vector(_matrix, Vector3(0.0f, 1.0f, 0.0f));
        Vector3 right = transformed_vector(_matrix, Vector3(1.0f, 0.0f, 0.0f));
        Vector3 back = transformed_vector(_matrix, Vector3(0.0f, 0.0f, 1.0f));
        
        _position += (up * dy + right * dx + back * dz);
        _transformation.set_translation(_position);
        _transformation.translate_local(Vector3(0.0f, 0.0f, _distance));
    }
    
    void zoom(float const dzoom) {
        _distance += dzoom;
        _transformation.set_translation(_position);
        _transformation.translate_local(Vector3(0.0f, 0.0f, _distance));
    }
    
    void set_zoom(float const zoom) {
        _distance = zoom;
        _transformation.set_translation(_position);
        _transformation.translate_local(Vector3(0.0f, 0.0f, _distance));
    }
    
    Transformation3& transformation() {
        return _transformation;
    }
    
    Matrix4 projection() const {
        return perspective_projection(_width, _height, _fov, _near, _far);
    }
    
    int width() const {
        return _width;
    }
    
    int height() const {
        return _height;
    }
    
    float near() const {
        return _near;
    }
    
    float far() const {
        return _far;
    }
    
    float fov() const {
        return _fov;
    }
    
    void set(Vector3 const& position = Vector3(), Quaternion<float> const& orientation = Quaternion<float>(), float const distance = 2.0f) {
        _position = position;
        _distance = distance;
        _transformation = Transformation3(orientation, position);
        _transformation.translate_local(Vector3(0.0f, 0.0f, _distance));
    }
    
    void set_view(int width, int height) {
        _width = width;
        _height = height;
    }
    
    void set_projection(float near, float far, float fov) {
        _near = near;
        _far = far;
        _fov = fov;
    }
    
    Vector3 const& position() const { return _position; }
    
};

////////////////////////////////////////////////////////////////////////////////

#endif /* defined(__Debug3D__Camera__) */

////////////////////////////////////////////////////////////////////////////////
