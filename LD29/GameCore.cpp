//
//  GameCore.cpp
//  LD29
//
//  Created by Kristof Niederholtmeyer on 26.04.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//

#include "GameCore.h"
//#include <random>
#include <OpenGL/gl.h>

Color4 color_interpolation(Color4 const& color0, Color4 const& color1, float t) {
    Vector3 v0(color0[0]/255.0f, color0[1]/255.0f, color0[2]/255.0f);
    Vector3 v1(color1[0]/255.0f, color1[1]/255.0f, color1[2]/255.0f);
    Vector3 r = linear_interpolation(v0, v1, t);
    return Color4(r[0]*255.0f, r[1]*255.0f, r[2]*255.0f, 255);
}

std::vector<Vector2> smooth(float width, float height, std::vector<Vector2> const& points) {
    float const k = 0.01f;
    std::vector<Vector2> result(points.size());
    for (int i = 0; i < result.size(); ++i) {
        Vector2 old = points[i];
        result[i] = old;
        for (int j = 0; j < points.size(); ++j) {
            if (i == j) continue;
            Vector2 diff = points[j] - old;
            float len = length(diff);
            if (len < 1.0f) {
                result[i] -= k * 1.0f/len * vector_normal(diff);
            }
        }
    }
    return result;
}

std::vector<Vector3> to_xy(std::vector<Vector2> const& shape) {
    std::vector<Vector3> result(shape.size());
    for (int i = 0; i < result.size(); ++i) {
        result[i] = Vector3(shape[i][0], shape[i][1], 0.0f);
    }
    return result;
}

std::vector<Vector3> to_xz(std::vector<Vector2> const& shape) {
    std::vector<Vector3> result(shape.size());
    for (int i = 0; i < result.size(); ++i) {
        result[i] = Vector3(shape[i][0], 0.0f, shape[i][1]);
    }
    return result;
}

std::vector<Vector2> circle(Vector2 center, float r, int d) {
    std::vector<Vector2> points;
    float dt = 2.0f * PI / (float)std::max(3, d);
    for (int i = 0; i < std::max(3, d); ++i) {
        points.push_back(center + Vector2(r * cosf(i * dt), r * sinf(i * dt)));
    }
    return points;
}

void init_flag_mesh(std::vector<Vector3>& mesh) {
    float x = 0.1f;
    float y0 = 0.1f;
    float y1 = 0.3f;
    Vector2 p0(-x, y0);
    Vector2 p1(0.0f, 0.0f);
    Vector2 p2(x, y0);
    Vector2 p3(x, y1);
    Vector2 p4(-x, y1);
    
    std::vector<Vector2> shape(5);
    shape[0] = p0;
    shape[1] = p1;
    shape[2] = p2;
    shape[3] = p3;
    shape[4] = p4;
    shape = cut(shape, circle(Vector2(-0.05f, 0.15f), 0.035f, 16));
    shape = cut(shape, circle(Vector2(-0.05f, 0.25f), 0.035f, 16));
    shape = cut(shape, circle(Vector2(0.05f, 0.15f), 0.035f, 16));
    shape = cut(shape, circle(Vector2(0.05f, 0.25f), 0.035f, 16));
    
    mesh = to_xy(triangulate(shape));
}

void init_small_flag_mesh(std::vector<Vector3>& mesh) {
    float x = 0.05f;
    float y0 = 0.05f;
    float y1 = 0.15f;
    Vector2 p0(-x, y0);
    Vector2 p1(0.0f, 0.0f);
    Vector2 p2(x, y0);
    Vector2 p3(x, y1);
    Vector2 p4(-x, y1);
    
    std::vector<Vector2> shape(5);
    shape[0] = p0;
    shape[1] = p1;
    shape[2] = p2;
    shape[3] = p3;
    shape[4] = p4;
    
    mesh = to_xy(triangulate(shape));
}

void init_crown_mesh(std::vector<Vector3>& mesh) {
    float x0 = 0.1f;
    float y0 = 0.025f;
    float y1 = 0.05f;
    float x1 = 0.5f * x0;
    
    Vector2 p0(-x0, 0.0f);
    Vector2 p1(x0, 0.0f);
    Vector2 p2(x0, y1);
    Vector2 p3(x1, y0);
    Vector2 p4(0.0f, y1);
    Vector2 p5(-x1, y0);
    Vector2 p6(-x0, y1);
    
    std::vector<Vector2> shape(7);
    shape[0] = p0;
    shape[1] = p1;
    shape[2] = p2;
    shape[3] = p3;
    shape[4] = p4;
    shape[5] = p5;
    shape[6] = p6;
    
    mesh = to_xy(triangulate(shape));
}

void init_decal_mesh(std::vector<Vector3>& mesh) {
    float x0 = 0.05f;
    float y0 = 0.023f;
    
    Vector2 p0(-x0, 0.0f);
    Vector2 p1(x0, 0.0f);
    Vector2 p2(x0, y0);
    Vector2 p3(-x0, y0);
    
    std::vector<Vector2> shape(4);
    shape[0] = p0;
    shape[1] = p1;
    shape[2] = p2;
    shape[3] = p3;
    
    mesh = to_xy(triangulate(shape));
}

void init_dot_mesh(std::vector<Vector3>& mesh, float r, int d) {
    mesh = to_xy(triangulate(circle(Vector2(), r, d)));
}

void init_indicator_mesh(std::vector<Vector3>& mesh) {
    float x0 = 0.05f;
    float y0 = 0.1f;
    float y1 = 0.15f;
    
    Vector2 p0(0.0f, 0.0f);
    Vector2 p1(x0, y1);
    Vector2 p2(0.0f, y0);
    Vector2 p3(-x0, y1);
    
    std::vector<Vector2> shape(4);
    shape[0] = p0;
    shape[1] = p1;
    shape[2] = p2;
    shape[3] = p3;
    
    mesh = to_xy(triangulate(shape));
}

void init_mine_mesh(std::vector<Vector3>& base_mesh, std::vector<Vector3>& wheel_mesh) {
    float d = 0.025f;
    float x0 = 1.5f * d;
    float x1 = 2.0f * d;
    float y0 = 3.0f * d;
    float y1 = 4.0f * d;
    float z0 = d * 0.25f;
    float z1 = d;
    
    Vector3 p0(-x1, 0.0f, z1);
    Vector3 p1(-x0, 0.0f, z1);
    Vector3 p2(-x0, y0, z0);
    Vector3 p3(x0, y0, z0);
    Vector3 p4(x0, 0.0f, z1);
    Vector3 p5(x1, 0.0f, z1);
    Vector3 p6(x1, y1, 0.0f);
    Vector3 p7(-x1, y1, 0.0f);
    
    Vector3 q0(-x1, 0.0f, -z1);
    Vector3 q1(-x0, 0.0f, -z1);
    Vector3 q2(-x0, y0, -z0);
    Vector3 q3(x0, y0, -z0);
    Vector3 q4(x0, 0.0f, -z1);
    Vector3 q5(x1, 0.0f, -z1);
    Vector3 q6(x1, y1, 0.0f);
    Vector3 q7(-x1, y1, 0.0f);
    
    base_mesh.resize(2 * 18);
    base_mesh[0] = p0;
    base_mesh[1] = p1;
    base_mesh[2] = p2;
    base_mesh[3] = p0;
    base_mesh[4] = p2;
    base_mesh[5] = p7;
    base_mesh[6] = p2;
    base_mesh[7] = p3;
    base_mesh[8] = p6;
    base_mesh[9] = p2;
    base_mesh[10] = p6;
    base_mesh[11] = p7;
    base_mesh[12] = p3;
    base_mesh[13] = p4;
    base_mesh[14] = p5;
    base_mesh[15] = p3;
    base_mesh[16] = p5;
    base_mesh[17] = p6;
    
    base_mesh[18] = q0;
    base_mesh[19] = q1;
    base_mesh[20] = q2;
    base_mesh[21] = q0;
    base_mesh[22] = q2;
    base_mesh[23] = q7;
    base_mesh[24] = q2;
    base_mesh[25] = q3;
    base_mesh[26] = q6;
    base_mesh[27] = q2;
    base_mesh[28] = q6;
    base_mesh[29] = q7;
    base_mesh[30] = q3;
    base_mesh[31] = q4;
    base_mesh[32] = q5;
    base_mesh[33] = q3;
    base_mesh[34] = q5;
    base_mesh[35] = q6;
    
    std::vector<Vector2> wheel = circle(Vector2(), 3.0f * d, 16);
    wheel = cut(wheel, circle(Vector2(), 2.5f * d, 16));
    
    wheel_mesh = to_xy(triangulate(wheel));
    
    Vector3 w0(-d * 0.25f, -2.75f * d, 0.0f);
    Vector3 w1(d * 0.25f, -2.75f * d, 0.0f);
    Vector3 w2(d * 0.25f, 2.75f * d, 0.0f);
    Vector3 w3(-d * 0.25f, 2.75f * d, 0.0f);
    
    Vector3 w4(-d * 2.75f, -0.25f * d, 0.0f);
    Vector3 w5(d * 2.75f, -0.25f * d, 0.0f);
    Vector3 w6(d * 2.75f, 0.25f * d, 0.0f);
    Vector3 w7(-d * 2.75f, 0.25f * d, 0.0f);
    
    wheel_mesh.push_back(w0);
    wheel_mesh.push_back(w1);
    wheel_mesh.push_back(w2);
    wheel_mesh.push_back(w0);
    wheel_mesh.push_back(w2);
    wheel_mesh.push_back(w3);
    
    wheel_mesh.push_back(w4);
    wheel_mesh.push_back(w5);
    wheel_mesh.push_back(w6);
    wheel_mesh.push_back(w4);
    wheel_mesh.push_back(w6);
    wheel_mesh.push_back(w7);
}

void init_star_mesh(std::vector<Vector3>& mesh) {
    Vector2 center(0.0f, 0.2f);
    std::vector<Vector2> shape = circle(center, 0.2f, 16);
    for (int i = 1; i < 16; i+=2) {
        shape[i] += 0.5f * (center - shape[i]);
    }
    mesh = to_xy(triangulate(shape));
}

void init_gl(Camera const& camera, Color4 const& clear_color) {
    glClearColor(clear_color[0]/255.0f, clear_color[1]/255.0f, clear_color[2]/255.0f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixf(&camera.projection()(0,0));
}

void draw(Matrix4 const& model_view, std::vector<Vector3> const& shape, Color4 const& color) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixf(&model_view(0,0));
    
    glBegin(GL_TRIANGLES);
    
    glColor3ub(color[0], color[1], color[2]);
    
    for (int i = 0; i < shape.size(); ++i) {
        glVertex3fv(&shape[i][0]);
    }
    
    glEnd();
}

Vector2 point_on_screen(Matrix4 const& view_projection, Vector3 const& p) {
    Vector4 proj = view_projection * Vector4(p[0], p[1], p[2], 1.0f);
    return Vector2(proj[0]/fabs(proj[3]), proj[1]/fabs(proj[3]));
}

struct Rect2 {
    Vector2 origin;
    Vector2 size;
};

Rect2 rect_on_screen(Matrix4 const& view_projection, std::vector<Vector3> const& vertices) {
    Vector2 p = point_on_screen(view_projection, vertices[0]);
    Vector2 min = p;
    Vector2 max = p;
    for (int i = 0; i < vertices.size(); ++i) {
        p = point_on_screen(view_projection, vertices[i]);
        min = minimum(min, p);
        max = maximum(max, p);
    }
    Rect2 result;
    result.origin = min;
    result.size = max - min;
    return result;
}

bool point_in_rect(Rect2 const& r, Vector2 const& p) {
    return (p[0] > r.origin[0]) && (p[0] < r.origin[0] + r.size[0]) && (p[1] > r.origin[1]) && (p[1] < r.origin[1] + r.size[1]);
}

bool cursor_on_shape(Matrix4 const& model_view_projection, std::vector<Vector3> const& vertices, Vector2 const& cursor) {
    for (int i = 0; i < vertices.size(); i+=3) {
        Vector2 p0 = point_on_screen(model_view_projection, vertices[i+0]);
        Vector2 p1 = point_on_screen(model_view_projection, vertices[i+1]);
        Vector2 p2 = point_on_screen(model_view_projection, vertices[i+2]);
        if (area(p0, p1, cursor) <= 0.0f && area(p1, p2, cursor) <= 0.0f && area(p2, p0, cursor) <= 0.0f) {
            return true;
        }
    }
    return false;
}

bool valid_cell(VoronoiCell2* c, float cutoff) {
    if (c->p->l[0] < -cutoff || c->p->l[0] > cutoff || c->p->l[1] < -cutoff || c->p->l[1] > cutoff) {
        return false;
    }
    return true;
}

float frand(float min, float max) {
    float r = (float)rand() / (float)RAND_MAX;
    return r * (max - min) + min;
}

int rand(int min, int max) {
    return min + rand() % (max + 1 - min);
}

GameCore::GameCore(int view_width, int view_height)
: _view_width(view_width), _view_height(view_height),
_camera_zoom(0.0f), _second_timer(0.0f)/*, _rand_engine((unsigned int)time(0))*/,
_game_over(false) {
    srand((unsigned int)time(0));
    
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
    
    //std::uniform_real_distribution<float> dist(-size*0.5f,size*0.5f);
    for (int i = 0; i < 200; ++i) {
        points.push_back(Vector2(frand(-size*0.5f,size*0.5f), frand(-size*0.5f,size*0.5f)));
    }
    for (int i = 0; i < 10; ++i) {
        points = smooth(size, size, points);
    }
    std::vector<Point2*> ppoints;
    for (int i = 0; i < points.size(); ++i) {
        ppoints.push_back(new Point2(points[i]));
    }
    
    _world = new VoronoiDiagram(size, size, ppoints);
    
    // Generate a map:
    float cutoff = 0.5f * size - 0.5f;
    
    // 1.) find a valid seed cell
    std::vector<VoronoiCell2*> map;
    for (int i = 0; i < _world->cells().size(); ++i) {
        VoronoiCell2* c = _world->cells()[i];
        if (valid_cell(c, cutoff)) {
            c->type = 0;
            map.push_back(c);
            break;
        }
    }
    
    // 2.) expand the map starting from the seed. All cells have to be connected.
    for (int i = 0; i < 100; ++i) {
        bool found = false;
        for (int j = 0; j < _world->cells().size(); ++j) {
            VoronoiCell2* c = _world->cells()[j];
            if (c->type == 1 && valid_cell(c, cutoff)) {
                for (int k = 0; k < map.size(); ++k) {
                    VoronoiCell2* m = map[k];
                    if (std::find(m->n.begin(), m->n.end(), c->p) != m->n.end()) {
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
            exit(1);
            //throw std::runtime_error("malformed point set.");
        }
    }
    
    // 3.) place mines
    //std::uniform_int_distribution<int> dist3(0, 8);
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < _world->cells().size(); ++j) {
            VoronoiCell2* c = _world->cells()[j];
            if (c->type == 0 && c->building != 1) {
                c->building = 1;
                c->spawn = rand(0, 8);
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
    for (int ijk = 0; ijk < _world->cells().size(); ++ijk) {
        VoronoiCell2* c = _world->cells()[ijk];
        bool valid = true;
        for (int j = 0; j < i; ++j) {
            if (std::find(_units[j].location->n.begin(),
                          _units[j].location->n.end(),
                          c->p) != _units[j].location->n.end()) {
                valid = false;
            }
        }
        if (valid && c->type == 0 && c->building == 1) {
            c->kingdom = i;
            _units[i].location = c;
            _units[i].destination = c;
            _units[i].coins = 4;
            _units[i].kingdom = i;
            _units[i].dead = false;
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
    
    init_gl(_camera, _background_color);
    
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
        /*_selected_cell->kingdom = current_unit().kingdom;
        if (current_unit().coins >= 4) {
            kill_unit_at(_selected_cell);
            spawn_unit(_selected_cell, current_unit().kingdom);
            current_unit().coins = 0;
        } else {
            _sound_queue.push_back(_move_sound);
            if (_selected_cell != current_unit().location) {
                kill_unit_at(_selected_cell);
            }
            current_unit().destination = _selected_cell;
        }
        
        next_turn_state();*/
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
    for (int ijk = 0; ijk < _world->cells().size(); ++ijk) {
        VoronoiCell2* c = _world->cells()[ijk];
        if (c->type != 0) {
            continue;
        }
        if (std::find(u.location->n.begin(), u.location->n.end(), c->p) == u.location->n.end()) {
            continue;
        }
        bool full = false;
        for (int ijk2 = 0; ijk2 < _units.size(); ++ijk2) {
            Unit& k = _units[ijk2];
            if (!k.dead && k.kingdom == u.kingdom && k.location == c) {
                full = true;
                break;
            }
        }
        if (!full) {
            result.push_back(c);
        }
    }
    return result;
}

std::vector<VoronoiCell2*> GameCore::valid_moves(Unit const& u) {
    std::vector<VoronoiCell2*> result;
    for (int ijk = 0; ijk < _world->cells().size(); ++ijk) {
        VoronoiCell2* c = _world->cells()[ijk];
        if (c->type != 0) {
            continue;
        }
        if (u.type == 0 && c->kingdom != u.kingdom) {
            continue;
        }
        if (u.location != c && std::find(u.location->n.begin(), u.location->n.end(), c->p) == u.location->n.end()) {
            continue;
        }
        bool full = false;
        for (int ijk2 = 0; ijk2 < _units.size(); ++ijk2) {
            Unit& k = _units[ijk2];
            if (!k.dead && k.kingdom == u.kingdom && k.location != u.location && k.location == c) {
                full = true;
                break;
            }
        }
        if (!full) {
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
    for (int ijk2 = 0; ijk2 < _units.size(); ++ijk2) {
        Unit& u = _units[ijk2];
        if (!u.dead) {
            remaining_kingdoms.insert(u.kingdom);
        }
    }
    if (remaining_kingdoms.size() <= 1) {
        _game_over = true;
        for (int ijk2 = 0; ijk2 < _units.size(); ++ijk2) {
            Unit& u = _units[ijk2];
            if (u.kingdom == current_unit().kingdom && u.type == 0) {
                _winner_location = u.location;
            }
        }
    } else {
        if (remaining_kingdoms.find(0) == remaining_kingdoms.end()) {
            _game_over = true;
            for (int ijk2 = 0; ijk2 < _units.size(); ++ijk2) {
                Unit& u = _units[ijk2];
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
            for (int ijk = 0; ijk < _world->cells().size(); ++ijk) {
                VoronoiCell2* c = _world->cells()[ijk];
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
        draw(_view * model * _sprite_rotation * offset, _flag_mesh, _kingdom_colors[unit.kingdom]);
        offset = homogeneous_translation(Vector3(0.0f, t + 0.31f, 0.0f));
        draw(_view * model * _sprite_rotation * offset, _crown_mesh, _gold_color);
        if (unit.coins > 0) {
            offset = homogeneous_translation(Vector3(-0.05f, t + 0.25f, 0.0f));
            draw(_view * model * _sprite_rotation * offset, _coin_mesh, _gold_color);
        }
        if (unit.coins > 1) {
            offset = homogeneous_translation(Vector3(0.05f, t + 0.25f, 0.0f));
            draw(_view * model * _sprite_rotation * offset, _coin_mesh, _gold_color);
        }
        if (unit.coins > 2) {
            offset = homogeneous_translation(Vector3(-0.05f, t + 0.15f, 0.0f));
            draw(_view * model * _sprite_rotation * offset, _coin_mesh, _gold_color);
        }
        if (unit.coins > 3) {
            offset = homogeneous_translation(Vector3(0.05f, t + 0.15f, 0.0f));
            draw(_view * model * _sprite_rotation * offset, _coin_mesh, _gold_color);
        }
        
        if (_game_over) {
            offset = homogeneous_translation(Vector3(0.0f, 4.0f * t + 0.45f, 0.0f));
            draw(_view * model * _sprite_rotation * offset, _star_mesh, _gold_color);
        }
    } else {
        Matrix4 offset = homogeneous_translation(Vector3(0.0f, t, 0.0f));
        draw(_view * model * _sprite_rotation * offset, _small_flag_mesh, _kingdom_colors[unit.kingdom]);
        offset = homogeneous_translation(Vector3(0.0f, t + 0.16f, 0.0f));
        draw(_view * model * _sprite_rotation * offset, _decal_mesh, _gold_color);
    }
}

bool GameCore::win_battle_at(VoronoiCell2* c) {
    for (int ijk2 = 0; ijk2 < _units.size(); ++ijk2) {
        Unit& u = _units[ijk2];
        if (!u.dead && u.kingdom != current_unit().kingdom && u.location == c) {
            return true;
        }
    }
    return false;
}

bool GameCore::danger_at(VoronoiCell2* cell) {
    for (int ijk = 0; ijk < _world->cells().size(); ++ijk) {
        VoronoiCell2* c = _world->cells()[ijk];
        if (c->type == 0 && std::find(cell->n.begin(), cell->n.end(), c->p) != cell->n.end()) {
            for (int ijk2 = 0; ijk2 < _units.size(); ++ijk2) {
                Unit& u = _units[ijk2];
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
    for (int ijk = 0; ijk < valid.size(); ++ijk) {
        VoronoiCell2* c = valid[ijk];
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
            destinations.clear();
            destinations.push_back(c);
        } else if (value == max_value) {
            destinations.push_back(c);
        }
    }
    
    //std::uniform_int_distribution<int> dist(0, (int)destinations.size()-1);
    VoronoiCell2* destination = destinations[rand(0, (int)destinations.size()-1)];
    perform_move(destination);
}

void GameCore::king_ai(std::vector<VoronoiCell2*> const& valid) {
    if (current_unit().coins >= 4) {
        troops_ai(valid);
    } else {
        std::vector<VoronoiCell2*> destinations;
        int max_value = -100000000;
        for (int ijk = 0; ijk < valid.size(); ++ijk) {
            VoronoiCell2* c = valid[ijk];
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
                destinations.clear();
                destinations.push_back(c);
            } else if (value == max_value) {
                destinations.push_back(c);
            }
        }
        
        //std::uniform_int_distribution<int> dist(0, (int)destinations.size()-1);
        VoronoiCell2* destination = destinations[rand(0, (int)destinations.size()-1)];
        perform_move(destination);
    }
}

void GameCore::spawn_unit(VoronoiCell2* location, int kingdom) {
    std::vector<Unit>::iterator it = _units.insert(_units.begin()+_current_unit, Unit());
    _current_unit++;
    
    it->location = location;
    it->destination = location;
    it->kingdom = kingdom;
    it->type = 1;
    it->coins = 0;
    it->dead = false;
    
    _sound_queue.push_back(_spawn_sound);
}

void GameCore::kill_unit_at(VoronoiCell2* location) {
    for (int ijk2 = 0; ijk2 < _units.size(); ++ijk2) {
        Unit& u = _units[ijk2];
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
    for (int ijk = 0; ijk < _world->cells().size(); ++ijk) {
        VoronoiCell2* c = _world->cells()[ijk];
        if (c->kingdom == kingdom) {
            c->kingdom = -1;
        }
    }
    for (int ijk2 = 0; ijk2 < _units.size(); ++ijk2) {
        Unit& u = _units[ijk2];
        if (u.kingdom == kingdom) {
            u.dead = true;
        }
    }
}

void GameCore::update(float dt) {
    dt *= 16.0f;
    //std::cout << dt << std::endl;
    
    _second_timer += dt;
    if (_second_timer > 1.0f) {
        _second_timer -= 1.0f;
    }
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    update_camera(dt);
    
    std::vector<VoronoiCell2*> valid;
    if (current_unit().coins >= 4) {
        valid = valid_placements(current_unit());
    } else {
        valid = valid_moves(current_unit());
    }
    
    
    _selected_cell = 0;
    if (current_unit().kingdom == 0 && _turn_state == 1) {
        for (int ijk = 0; ijk < valid.size(); ++ijk) {
            VoronoiCell2* c = valid[ijk];
            if (cursor_on_shape(_view_projection, c->vertices, _cursor)) {
                _selected_cell = c;
            }
        }
    }
    
    for (int ijk = 0; ijk < _world->cells().size(); ++ijk) {
        VoronoiCell2* c = _world->cells()[ijk];
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
            
            draw(_view, c->vertices, color);
        }
    }
    
    for (int ijk = 0; ijk < _world->cells().size(); ++ijk) {
        VoronoiCell2* c = _world->cells()[ijk];
        if (c->type == 0) {
            if (c->building == 1) {
                Matrix4 model = homogeneous_translation(Vector3(c->p->l[0], 0.0f, c->p->l[1]));
                Matrix4 offset = homogeneous_translation(Vector3(0.1f, 0.0f, 0.1f));
                draw(_view * model * _sprite_rotation * offset, _mine_base_mesh, Color4(40, 40, 40, 255));
                offset = homogeneous_translation(Vector3(0.1f, 0.1f, 0.1f));
                Matrix4 rot = homogeneous_rotation(Quaternion<float>(Vector3(0.0f, 0.0f, 1.0f), _second_timer * 2.0f * PI));
                draw(_view * model * _sprite_rotation * offset * rot, _mine_wheel_mesh, Color4(40, 40, 40, 255));
            }
            
            if (c->coins > 0) {
                Matrix4 model = homogeneous_translation(Vector3(c->p->l[0], 0.0f, c->p->l[1]));
                Matrix4 offset = homogeneous_translation(Vector3(0.1f, 0.25f + 0.025f * sinf(_second_timer * 2.0f * PI), 0.1f));
                draw(_view * model * _sprite_rotation * offset, _coin_mesh, _gold_color);
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
    
    for (int ijk2 = 0; ijk2 < _units.size(); ++ijk2) {
        Unit& u = _units[ijk2];
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
    glDisable(GL_DEPTH_TEST);
    if (current_unit().kingdom == 0 && _turn_state == 1 && _selected_cell && !_game_over) {
        float t = 0.05f + 0.05f * sinf(_second_timer * 2.0f * PI);
        Matrix4 model = homogeneous_translation(Vector3(_selected_cell->p->l[0], 0.0f, _selected_cell->p->l[1]));
        if (current_unit().coins >= 4) {
            draw(_view * model * _sprite_rotation * homogeneous_translation(Vector3(0.0f, t, 0.0f)), _small_flag_mesh, _kingdom_colors[current_unit().kingdom]);
            Matrix4 offset = homogeneous_translation(Vector3(0.0f, t + 0.16f, 0.0f));
            draw(_view * model * _sprite_rotation * offset, _decal_mesh, _gold_color);
        } else {
            
            draw(_view * model * _sprite_rotation * homogeneous_translation(Vector3(0.0f, t, 0.0f)), _indicator_mesh, _indicator_color);
        }
    }
    glEnable(GL_DEPTH_TEST);
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
