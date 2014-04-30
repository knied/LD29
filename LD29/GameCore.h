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
#include <random>
#include "Camera.h"
#include "Types.h"
#include "GameShapes.h"
#include "GameMap.h"

typedef enum {
    MBLeft,
    MBRight
} MouseButton;

struct Unit {
    Tile* location;
    Tile* destination;
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
    
    GameMap _map;
    
    GLShape _flag_mesh;
    GLShape _small_flag_mesh;
    GLShape _coin_mesh;
    GLShape _crown_mesh;
    GLShape _decal_mesh;
    GLShape _indicator_mesh;
    GLShape _mine_base_mesh;
    GLShape _mine_wheel_mesh;
    GLShape _star_mesh;

    Color4 _kingdom_colors[4];
    Color4 _kingdom_map_colors[4];
    Color4 _kingdom_map_highlight_colors[4];
    Color4 _background_color;
    Color4 _cell_color;
    Color4 _indicator_color;
    Color4 _gold_color;
    Color4 _select_color;
    
    Vector2 _cursor;
    
    Matrix4 _view;
    Matrix4 _projection;
    Matrix4 _view_projection;
    Matrix4 _sprite_rotation;
    Matrix4 _camera_model;
    
    void update_camera(float dt);
    
    Unit& current_unit();
    std::vector<Tile*> valid_placements(Unit const& u);
    std::vector<Tile*> valid_moves(Unit const& u);
    void perform_move(Tile* destination);
    void draw_unit(Unit const& unit);
    bool win_battle_at(Tile* c);
    bool danger_at(Tile* c);
    void troops_ai(std::vector<Tile*> const& valid);
    void king_ai(std::vector<Tile*> const& valid);
    void spawn_unit(Tile* location, int kingdom);
    void kill_unit_at(Tile* location);
    void remove_kingdom(int kingdom);
    
    std::vector<Unit> _units;
    int _current_unit;
    
    int _turn_state;
    float _turn_timer;
    void next_turn_state();
    
    Tile* _selected_cell;
    
    float _second_timer;
    
    bool _game_over;
    Tile* _winner_location;
    
    std::vector<int> _sound_queue;
    
    int _move_sound;
    int _coin_sound;
    int _kill_sound;
    int _spawn_sound;
    
public:
    GameCore(int view_width, int view_height);
    
    void mouse_moved(float x, float y);
    void mouse_dragged(MouseButton button, float xrel, float yrel);
    void mouse_down(MouseButton button, float x, float y);
    void mouse_up(MouseButton button, float x, float y);
    void mouse_wheel(float w);
    
    void update(float dt);
    
    bool game_over() const;
    
    void set_sounds(int move_sound, int coin_sound, int kill_sound, int spawn_sound);
    int next_sound();
};

#endif /* defined(__LD29__GameCore__) */
