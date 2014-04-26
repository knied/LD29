//
//  GameCore.h
//  LD29
//
//  Created by Kristof Niederholtmeyer on 26.04.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//

#ifndef __LD29__GameCore__
#define __LD29__GameCore__

#include <iostream>
#include "Camera.h"
#include "Types.h"
#include "DelaunayTriangulation.h"

typedef enum {
    MBLeft,
    MBRight
} MouseButton;

struct King {
    VoronoiCell2* location;
    int coins;
};

class GameCore {
    int _view_width;
    int _view_height;
    
    Camera _camera;
    Vector2 _camera_rotation;
    float _camera_zoom;
    Vector3 _target_camera_position;
    
    VoronoiDiagram* _world;
    std::vector<Vector3> _flag_mesh;
    std::vector<Vector3> _coin_mesh;
    std::vector<Vector3> _crown_mesh;
    std::vector<Vector3> _indicator_mesh;
    
    Vector2 _cursor;
    
    Matrix4 _view;
    Matrix4 _projection;
    Matrix4 _view_projection;
    Matrix4 _sprite_rotation;
    Matrix4 _camera_model;
    
    void update_camera(float dt);
    void draw_king(King const& king);
    
    King _player;
    
    VoronoiCell2* _selected_cell;
    
    float _second_timer;
    
public:
    GameCore(int view_width, int view_height);
    ~GameCore();
    
    void mouse_moved(float x, float y);
    void mouse_dragged(MouseButton button, float xrel, float yrel);
    void mouse_down(MouseButton button, float x, float y);
    void mouse_up(MouseButton button, float x, float y);
    void mouse_wheel(float w);
    
    void update(float dt);
};

#endif /* defined(__LD29__GameCore__) */
