//
//  GameCore.cpp
//  LD29
//
//  Created by Kristof Niederholtmeyer on 26.04.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//

#include "GameCore.h"
#include "GameHelper.h"
#include <random>

GameCore::GameCore(int view_width, int view_height)
: _view_width(view_width), _view_height(view_height),
_camera_zoom(0.0f), _second_timer(0.0f), _rand_engine((unsigned int)time(0)),
_game_over(false) {
    _kingdom_colors[0] = Color4(100, 30, 30, 255);
    _kingdom_colors[1] = Color4(30, 100, 100, 255);
    _kingdom_colors[2] = Color4(30, 100, 30, 255);
    _kingdom_colors[3] = Color4(30, 30, 100, 255);
    
    Color4 diff(80, 80, 80, 0);
    _kingdom_map_colors[0] = _kingdom_colors[0] + diff;
    _kingdom_map_colors[1] = _kingdom_colors[1] + diff;
    _kingdom_map_colors[2] = _kingdom_colors[2] + diff;
    _kingdom_map_colors[3] = _kingdom_colors[3] + diff;
    
    Color4 diff2(45, 45, 45, 0);
    _kingdom_map_highlight_colors[0] = _kingdom_map_colors[0] + diff2;
    _kingdom_map_highlight_colors[1] = _kingdom_map_colors[1] + diff2;
    _kingdom_map_highlight_colors[2] = _kingdom_map_colors[2] + diff2;
    _kingdom_map_highlight_colors[3] = _kingdom_map_colors[3] + diff2;
    
    Color4 diff3(30, 30, 30, 0);
    _select_color = color_interpolation(_kingdom_colors[0], Color4(255, 255, 255, 255), 0.9f);
    
    _background_color = Color4(200, 220, 255, 255);
    
    _cell_color = Color4(150, 210, 160, 255);
    
    //_indicator_color = Color4(100, 0, 0, 255);
    _indicator_color = _kingdom_colors[0];
    
    _gold_color = Color4(200, 200, 64, 255);
    
    float const size = 7.5f;
    
    std::vector<Vector2> points;
    
    std::uniform_real_distribution<float> dist(-size*0.5f,size*0.5f);
    for (int i = 0; i < 200; ++i) {
        points.push_back(Vector2(dist(_rand_engine), dist(_rand_engine)));
    }
    for (int i = 0; i < 10; ++i) {
        points = smooth(size, size, points);
    }
    std::vector<Point2*> ppoints;
    for (Vector2 const& p : points) {
        ppoints.push_back(new Point2{p});
    }
    
    _world = new VoronoiDiagram(size, size, ppoints);
    
    // Generate a map:
    float cutoff = 0.5f * size - 0.5f;
    
    // 1.) find a valid seed cell
    std::vector<VoronoiCell2*> map;
    for (VoronoiCell2* c : _world->cells()) {
        if (valid_cell(c, cutoff)) {
            c->type = 0;
            map.push_back(c);
            break;
        }
    }
    
    // 2.) expand the map starting from the seed. All cells have to be connected.
    for (int i = 0; i < 100; ++i) {
        bool found = false;
        for (VoronoiCell2* c : _world->cells()) {
            if (c->type == 1 && valid_cell(c, cutoff)) {
                for (VoronoiCell2* m : map) {
                    if (std::find(m->n.begin(), m->n.end(), c) != m->n.end()) {
                        found = true;
                        map.push_back(c);
                        c->type = 0;
                        break;
                    }
                }
            }
            if (found) {
                break;
            }
        }
        if (!found) {
            std::cout << i << std::endl;
            throw std::runtime_error("malformed point set.");
        }
    }
    
    // 3.) place mines
    std::uniform_int_distribution<int> dist3(0, 8);
    for (int i = 0; i < 20; ++i) {
        for (VoronoiCell2* c : _world->cells()) {
            if (c->type == 0 && c->building != 1) {
                c->building = 1;
                c->spawn = dist3(_rand_engine);
                break;
            }
        }
    }
    
    _selected_cell = 0;
    _turn_state = 0;
    _turn_timer = 0.0f;
    _current_unit = 0;
    //std::uniform_int_distribution<uint8_t> dist2(0, 200);
    _units.resize(4);
    int i = 0;
    for (VoronoiCell2* c : _world->cells()) {
        bool valid = true;
        for (int j = 0; j < i; ++j) {
            if (std::find(_units[j].location->n.begin(),
                          _units[j].location->n.end(),
                          c) != _units[j].location->n.end()) {
                valid = false;
            }
        }
        if (valid && c->type == 0 && c->building == 1) {
            c->kingdom = i;
            _units[i].location = c;
            _units[i].destination = c;
            _units[i].coins = 4;
            _units[i].kingdom = i;
            //_units[i].dead = i != 0;
            i++;
            if (i >= 4) {
                break;
            }
        }
    }
    
    init_flag_mesh(_flag_mesh);
    init_small_flag_mesh(_small_flag_mesh);
    init_dot_mesh(_coin_mesh, 0.025f, 16);
    init_crown_mesh(_crown_mesh);
    init_indicator_mesh(_indicator_mesh);
    init_mine_mesh(_mine_base_mesh, _mine_wheel_mesh);
    init_decal_mesh(_decal_mesh);
    init_star_mesh(_star_mesh);
    
    gl_init(_camera, _background_color);
    
    _projection = _camera.projection();
}

GameCore::~GameCore() {
    delete _world;
}

void GameCore::mouse_moved(float x, float y) {
    _cursor = Vector2(x, y);
}

void GameCore::mouse_dragged(MouseButton button, float xrel, float yrel) {
    if (button == MBRight) {
        _camera_rotation -= Vector2(0.01f * yrel, 0.01f * xrel);
    }
}

void GameCore::mouse_down(MouseButton button, float x, float y) {
    
}

void GameCore::mouse_up(MouseButton button, float x, float y) {
    if (current_unit().kingdom == 0 && button == MBLeft && _selected_cell) {
        perform_move(_selected_cell);
    }
}

void GameCore::mouse_wheel(float w) {
    _camera_zoom += 0.5f * w;
}

void GameCore::update_camera(float dt) {
    if (_camera_rotation[0] > -PI * 0.1f) {
        _camera_rotation[0] = -PI * 0.1f;
    }
    if (_camera_rotation[0] < -PI * 0.4f) {
        _camera_rotation[0] = -PI * 0.4f;
    }
    if (_camera_rotation[1] < 0.0f) {
        _camera_rotation[1] += 2.0f * PI;
    }
    if (_camera_rotation[1] > 2.0f * PI) {
        _camera_rotation[1] -= 2.0f * PI;
    }
    if (_camera_zoom < 1.0f) {
        _camera_zoom = 1.0f;
    }
    if (_camera_zoom > 5.0f) {
        _camera_zoom = 5.0f;
    }
    
    if (_game_over) {
        _target_camera_position = Vector3(_winner_location->p->l[0], 0.0f, _winner_location->p->l[1]);
    } else {
        _target_camera_position = Vector3(current_unit().location->p->l[0], 0.0f, current_unit().location->p->l[1]);
    }
    
    _camera.set_rotaton(_camera_rotation[0], _camera_rotation[1]);
    _camera.set_zoom(_camera_zoom);
    Vector3 camera_position = _camera.position();
    Vector3 camera_target_delta = _target_camera_position - camera_position;
    _camera.set_position(camera_position + camera_target_delta * dt * 5.0f);
    _view = _camera.transformation().world_to_local();
    _camera_model = _camera.transformation().local_to_world();
    
    _view_projection = _projection * _view;
    Vector3 camera_normal = transformed_vector(_camera_model, Vector3(0.0f, 0.0f, 1.0f));
    Vector3 xz = vector_normal(camera_normal - Vector3(0.0f, 1.0f, 0.0f) * dot(Vector3(0.0f, 1.0f, 0.0f), camera_normal));
    float angle = atan2f(xz[0], xz[2]);
    _sprite_rotation = homogeneous_rotation(Quaternion<float>(Vector3(0.0f, 1.0f, 0.0f), angle));
}

Unit& GameCore::current_unit() {
    return _units[_current_unit];
}

std::vector<VoronoiCell2*> GameCore::valid_placements(Unit const& u) {
    std::vector<VoronoiCell2*> result;
    for (VoronoiCell2* c : u.location->n) {
        if (c->type != 0) {
            continue;
        }
        bool occupied = false;
        for (Unit& k : _units) {
            if (!k.dead && k.kingdom == u.kingdom && k.location == c) {
                occupied = true;
                break;
            }
        }
        if (!occupied) {
            result.push_back(c);
        }
    }
    return result;
}

std::vector<VoronoiCell2*> GameCore::valid_moves(Unit const& u) {
    std::vector<VoronoiCell2*> result{u.location};
    for (VoronoiCell2* c : u.location->n) {
        if (c->type != 0) {
            continue;
        }
        if (u.type == 0 && c->kingdom != u.kingdom) {
            continue;
        }
        bool occupied = false;
        for (Unit& k : _units) {
            if (!k.dead && k.kingdom == u.kingdom && k.location == c) {
                occupied = true;
                break;
            }
        }
        if (!occupied) {
            result.push_back(c);
        }
    }
    return result;
}

void GameCore::next_turn_state() {
    if (_game_over) {
        return;
    }
    
    if (_turn_state == 2) {
        current_unit().location = current_unit().destination;
    }
    
    std::set<int> remaining_kingdoms;
    for (Unit& u : _units) {
        if (!u.dead) {
            remaining_kingdoms.insert(u.kingdom);
        }
    }
    if (remaining_kingdoms.size() <= 1) {
        _game_over = true;
        for (Unit& u : _units) {
            if (u.kingdom == current_unit().kingdom && u.type == 0) {
                _winner_location = u.location;
            }
        }
    } else {
        if (remaining_kingdoms.find(0) == remaining_kingdoms.end()) {
            _game_over = true;
            for (Unit& u : _units) {
                if (u.kingdom == current_unit().kingdom && u.type == 0) {
                    _winner_location = u.location;
                }
            }
        }
    }
    
    _turn_timer = 0.0f;
    if (!(_game_over && _turn_state == 1)) {
        _turn_state++;
    }
    if (_turn_state > 3) {
        _turn_state = 0;
        int _previous_unit = _current_unit;
        _current_unit = _current_unit+1 < _units.size() ? _current_unit+1 : 0;
        while (current_unit().dead) {
            _current_unit = _current_unit+1 < _units.size() ? _current_unit+1 : 0;
        }
        
        // update mines
        if (_previous_unit >= _current_unit) {
            //std::cout << "==== NEXT ====" << std::endl;
            for (VoronoiCell2* c : _world->cells()) {
                if (c->building == 1 && c->coins == 0) {
                    c->spawn++;
                    if (c->spawn >= 8) {
                        c->spawn = 0;
                        c->coins = 1;
                    }
                }
            }
        }
    }
    
    //std::cout << "state: " << _turn_state << std::endl;
    //std::cout << "unit: " << _current_unit << std::endl;
    //std::cout << "kingdom: " << current_unit().kingdom << std::endl;
}

void GameCore::perform_move(VoronoiCell2* destination) {
    destination->kingdom = current_unit().kingdom;
    if (current_unit().coins >= 4) {
        kill_unit_at(destination);
        spawn_unit(destination, current_unit().kingdom);
        current_unit().coins = 0;
    } else {
        _sound_queue.push_back(_move_sound);
        if (destination != current_unit().location) {
            kill_unit_at(destination);
        }
        current_unit().destination = destination;
    }
    next_turn_state();
}

void GameCore::draw_unit(Unit const& unit) {
    Vector3 position = linear_interpolation(Vector3(unit.location->p->l[0], 0.0f, unit.location->p->l[1]),
                                            Vector3(unit.destination->p->l[0], 0.0f, unit.destination->p->l[1]),
                                            _turn_timer);
    
    Matrix4 model = homogeneous_translation(position);
    
    float t = 0.0f;
    if (current_unit().location == unit.location || _game_over) {
        t = 0.01f * (1.0f + cosf(4.0f * PI * _second_timer));
    }
    if (unit.type == 0) {
        Matrix4 offset = homogeneous_translation(Vector3(0.0f, t, 0.0f));
        gl_draw(_view * model * _sprite_rotation * offset, _flag_mesh, _kingdom_colors[unit.kingdom]);
        offset = homogeneous_translation(Vector3(0.0f, t + 0.31f, 0.0f));
        gl_draw(_view * model * _sprite_rotation * offset, _crown_mesh, _gold_color);
        if (unit.coins > 0) {
            offset = homogeneous_translation(Vector3(-0.05f, t + 0.25f, 0.0f));
            gl_draw(_view * model * _sprite_rotation * offset, _coin_mesh, _gold_color);
        }
        if (unit.coins > 1) {
            offset = homogeneous_translation(Vector3(0.05f, t + 0.25f, 0.0f));
            gl_draw(_view * model * _sprite_rotation * offset, _coin_mesh, _gold_color);
        }
        if (unit.coins > 2) {
            offset = homogeneous_translation(Vector3(-0.05f, t + 0.15f, 0.0f));
            gl_draw(_view * model * _sprite_rotation * offset, _coin_mesh, _gold_color);
        }
        if (unit.coins > 3) {
            offset = homogeneous_translation(Vector3(0.05f, t + 0.15f, 0.0f));
            gl_draw(_view * model * _sprite_rotation * offset, _coin_mesh, _gold_color);
        }
        
        if (_game_over) {
            offset = homogeneous_translation(Vector3(0.0f, 4.0f * t + 0.45f, 0.0f));
            gl_draw(_view * model * _sprite_rotation * offset, _star_mesh, _gold_color);
        }
    } else {
        Matrix4 offset = homogeneous_translation(Vector3(0.0f, t, 0.0f));
        gl_draw(_view * model * _sprite_rotation * offset, _small_flag_mesh, _kingdom_colors[unit.kingdom]);
        offset = homogeneous_translation(Vector3(0.0f, t + 0.16f, 0.0f));
        gl_draw(_view * model * _sprite_rotation * offset, _decal_mesh, _gold_color);
    }
}

bool GameCore::win_battle_at(VoronoiCell2* c) {
    for (Unit& u : _units) {
        if (!u.dead && u.kingdom != current_unit().kingdom && u.location == c) {
            return true;
        }
    }
    return false;
}

bool GameCore::danger_at(VoronoiCell2* cell) {
    for (VoronoiCell2* c : cell->n) {
        if (c->type == 0) {
            for (Unit& u : _units) {
                if (!u.dead && u.kingdom != current_unit().kingdom && u.location == c && (u.type == 1 || (u.type == 0 && u.coins >= 4))) {
                    return true;
                }
            }
        }
    }
    return false;
}

void GameCore::troops_ai(std::vector<VoronoiCell2*> const& valid) {
    std::vector<VoronoiCell2*> destinations;
    int max_value = -100000000;
    for (VoronoiCell2* c : valid) {
        int value = 0;
        if (c != current_unit().location) {
            value += 1;
        }
        if (c->kingdom != current_unit().kingdom) {
            value += 10;
            if (c->building != 0) {
                value += 5;
            }
        }
        if (danger_at(c)) {
            value -= 20;
        }
        if (win_battle_at(c)) {
            value += 40;
        }
        
        if (value > max_value) {
            max_value = value;
            destinations = {c};
        } else if (value == max_value) {
            destinations.push_back(c);
        }
    }
    
    std::uniform_int_distribution<int> dist(0, (int)destinations.size()-1);
    VoronoiCell2* destination = destinations[dist(_rand_engine)];
    perform_move(destination);
}

void GameCore::king_ai(std::vector<VoronoiCell2*> const& valid) {
    if (current_unit().coins >= 4) {
        troops_ai(valid);
    } else {
        std::vector<VoronoiCell2*> destinations;
        int max_value = -100000000;
        for (VoronoiCell2* c : valid) {
            int value = 0;
            if (c != current_unit().location && danger_at(current_unit().location)) {
                value += 20;
            }
            if (c != current_unit().location) {
                value += 1;
            }
            if (danger_at(c)) {
                value -= 20;
            }
            if (c->coins > 0) {
                value += 10;
            }
            
            if (value > max_value) {
                max_value = value;
                destinations = {c};
            } else if (value == max_value) {
                destinations.push_back(c);
            }
        }
        
        std::uniform_int_distribution<int> dist(0, (int)destinations.size()-1);
        VoronoiCell2* destination = destinations[dist(_rand_engine)];
        perform_move(destination);
    }
}

void GameCore::spawn_unit(VoronoiCell2* location, int kingdom) {
    auto it = _units.insert(_units.begin()+_current_unit, Unit());
    _current_unit++;
    
    it->location = location;
    it->destination = location;
    it->kingdom = kingdom;
    it->type = 1;
    it->coins = 0;
    
    _sound_queue.push_back(_spawn_sound);
}

void GameCore::kill_unit_at(VoronoiCell2* location) {
    for (Unit& u : _units) {
        if (u.location == location && !u.dead) {
            _sound_queue.push_back(_kill_sound);
            u.dead = true;
            if (u.type == 0) {
                remove_kingdom(u.kingdom);
            }
            break;
        }
    }
}

void GameCore::remove_kingdom(int kingdom) {
    for (VoronoiCell2* c : _world->cells()) {
        if (c->kingdom == kingdom) {
            c->kingdom = -1;
        }
    }
    for (Unit& u : _units) {
        if (u.kingdom == kingdom) {
            u.dead = true;
        }
    }
}

void GameCore::update(float dt) {
    _second_timer += dt;
    if (_second_timer > 1.0f) {
        _second_timer -= 1.0f;
    }
    
    gl_clear();
    
    update_camera(dt);
    
    std::vector<VoronoiCell2*> valid;
    if (current_unit().coins >= 4) {
        valid = valid_placements(current_unit());
    } else {
        valid = valid_moves(current_unit());
    }
    
    
    _selected_cell = 0;
    if (current_unit().kingdom == 0 && _turn_state == 1) {
        for (VoronoiCell2* c : valid) {
            if (cursor_on_shape(_view_projection, c->vertices, _cursor)) {
                _selected_cell = c;
            }
        }
    }
    
    for (VoronoiCell2* c : _world->cells()) {
        if (c->type == 0) {
            Color4 color(_cell_color);
            if (c->kingdom >= 0) {
                color = _kingdom_map_colors[c->kingdom];
            }
            
            if (!_game_over && std::find(valid.begin(), valid.end(), c) != valid.end()) {
                float t = 0.5f * (1.0f + sinf(4.0f * PI * _second_timer));
                Color4 fade_color = _kingdom_map_highlight_colors[current_unit().kingdom];
                if (c == _selected_cell) {
                    fade_color = _select_color;
                }
                color = color_interpolation(fade_color, color, t);
            }
            
            gl_draw(_view, c->vertices, color);
        }
    }
    
    for (VoronoiCell2* c : _world->cells()) {
        if (c->type == 0) {
            if (c->building == 1) {
                Matrix4 model = homogeneous_translation(Vector3(c->p->l[0], 0.0f, c->p->l[1]));
                Matrix4 offset = homogeneous_translation(Vector3(0.1f, 0.0f, 0.1f));
                gl_draw(_view * model * _sprite_rotation * offset, _mine_base_mesh, Color4(40, 40, 40, 255));
                offset = homogeneous_translation(Vector3(0.1f, 0.1f, 0.1f));
                Matrix4 rot = homogeneous_rotation(Quaternion<float>(Vector3(0.0f, 0.0f, 1.0f), _second_timer * 2.0f * PI));
                gl_draw(_view * model * _sprite_rotation * offset * rot, _mine_wheel_mesh, Color4(40, 40, 40, 255));
            }
            
            if (c->coins > 0) {
                Matrix4 model = homogeneous_translation(Vector3(c->p->l[0], 0.0f, c->p->l[1]));
                Matrix4 offset = homogeneous_translation(Vector3(0.1f, 0.25f + 0.025f * sinf(_second_timer * 2.0f * PI), 0.1f));
                gl_draw(_view * model * _sprite_rotation * offset, _coin_mesh, _gold_color);
            }
        }
    }
    
    if (_turn_state == 2) {
        _turn_timer += 6.0f * dt;
    }
    if (_turn_state == 0 || _turn_state == 3) {
        _turn_timer += 2.0f * dt;
    }
    if (_turn_timer > 1.0f) {
        next_turn_state();
    }
    
    if (_turn_state == 1 && !_game_over) {
        if (valid.size() == 0) {
            next_turn_state();
        } else if (current_unit().kingdom != 0) {
            if (current_unit().type == 0) {
                king_ai(valid);
            } else {
                troops_ai(valid);
            }
            //next_turn_state();
        }
    }
    
    for (Unit& u : _units) {
        if (!u.dead) {
            if (u.type == 0 && u.location->coins > 0) {
                _sound_queue.push_back(_coin_sound);
                u.location->coins = 0;
                u.coins++;
            }
            draw_unit(u);
        }
    }
    
    // draw indicator
    gl_disable_depth();
    if (current_unit().kingdom == 0 && _turn_state == 1 && _selected_cell && !_game_over) {
        float t = 0.05f + 0.05f * sinf(_second_timer * 2.0f * PI);
        Matrix4 model = homogeneous_translation(Vector3(_selected_cell->p->l[0], 0.0f, _selected_cell->p->l[1]));
        if (current_unit().coins >= 4) {
            gl_draw(_view * model * _sprite_rotation * homogeneous_translation(Vector3(0.0f, t, 0.0f)), _small_flag_mesh, _kingdom_colors[current_unit().kingdom]);
            Matrix4 offset = homogeneous_translation(Vector3(0.0f, t + 0.16f, 0.0f));
            gl_draw(_view * model * _sprite_rotation * offset, _decal_mesh, _gold_color);
        } else {
            
            gl_draw(_view * model * _sprite_rotation * homogeneous_translation(Vector3(0.0f, t, 0.0f)), _indicator_mesh, _indicator_color);
        }
    }
    gl_enable_depth();
}

bool GameCore::game_over() const {
    return _game_over;
}

void GameCore::set_sounds(int move_sound, int coin_sound, int kill_sound, int spawn_sound) {
    _move_sound = move_sound;
    _coin_sound = coin_sound;
    _kill_sound = kill_sound;
    _spawn_sound = spawn_sound;
}

int GameCore::next_sound() {
    if (_sound_queue.size() <= 0) {
        return -1;
    }
    int next = _sound_queue.front();
    _sound_queue.erase(_sound_queue.begin());
    return next;
}
