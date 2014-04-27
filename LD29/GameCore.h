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

struct Unit {
    VoronoiCell2* location;
    VoronoiCell2* destination;
    int coins;
    int kingdom;
    int type; // 0: king; 1: troops
    bool dead = false;
};

class GameCore {
    int _view_width;
    int _view_height;
    
    Camera _camera;
    Vector2 _camera_rotation;
    float _camera_zoom;
    Vector3 _target_camera_position;
    
    std::mt19937 _rand_engine;
    
    VoronoiDiagram* _world;
    std::vector<Vector3> _flag_mesh;
    std::vector<Vector3> _small_flag_mesh;
    std::vector<Vector3> _coin_mesh;
    std::vector<Vector3> _crown_mesh;
    std::vector<Vector3> _decal_mesh;
    std::vector<Vector3> _indicator_mesh;
    std::vector<Vector3> _mine_base_mesh;
    std::vector<Vector3> _mine_wheel_mesh;

    Color4 _kingdom_colors[4];
    Color4 _kingdom_map_colors[4];
    Color4 _kingdom_map_highlight_colors[4];
    Color4 _background_color;
    Color4 _cell_color;
    Color4 _indicator_color;
    Color4 _gold_color;
    
    Vector2 _cursor;
    
    Matrix4 _view;
    Matrix4 _projection;
    Matrix4 _view_projection;
    Matrix4 _sprite_rotation;
    Matrix4 _camera_model;
    
    void update_camera(float dt);
    
    Unit& current_unit();
    std::vector<VoronoiCell2*> valid_placements(Unit const& u);
    std::vector<VoronoiCell2*> valid_moves(Unit const& u);
    void draw_unit(Unit const& unit);
    void king_ai(std::vector<VoronoiCell2*> const& valid);
    void spawn_unit(VoronoiCell2* location, int kingdom);
    void kill_unit_at(VoronoiCell2* location);
    void remove_kingdom(int kingdom);
    
    std::vector<Unit> _units;
    int _current_unit;
    
    int _turn_state;
    float _turn_timer;
    void next_turn_state();
    
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