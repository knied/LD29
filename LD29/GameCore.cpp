//
//  GameCore.cpp
//  LD29
//
//  Created by Kristof Niederholtmeyer on 26.04.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//

#include "GameCore.h"
#include <random>
#include <OpenGL/gl.h>

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
    
    std::vector<Vector2> shape{p0, p1, p2, p3, p4};
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
    
    std::vector<Vector2> shape{p0, p1, p2, p3, p4};
    
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
    
    mesh = to_xy(triangulate({p0, p1, p2, p3, p4, p5, p6}));
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
    
    mesh = to_xy(triangulate({p0, p1, p2, p3}));
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
    
    base_mesh = {
        p0, p1, p2, p0, p2, p7, p2, p3, p6, p2, p6, p7, p3, p4, p5, p3, p5, p6,
        q0, q1, q2, q0, q2, q7, q2, q3, q6, q2, q6, q7, q3, q4, q5, q3, q5, q6
    };
    
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
    
    for (Vector3 const& v : shape) {
        glVertex3fv(&v[0]);
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
    for (Vector3 const& v : vertices) {
        p = point_on_screen(view_projection, v);
        min = minimum(min, p);
        max = maximum(max, p);
    }
    return Rect2{min, max - min};
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

GameCore::GameCore(int view_width, int view_height)
: _view_width(view_width), _view_height(view_height), _camera_zoom(0.0f), _second_timer(0.0f) {
    _kingdom_colors[0] = Color4(100, 30, 30, 255);
    _kingdom_colors[1] = Color4(30, 100, 100, 255);
    _kingdom_colors[2] = Color4(30, 100, 30, 255);
    _kingdom_colors[3] = Color4(30, 30, 100, 255);
    
    Color4 diff(80, 80, 80, 0);
    _kingdom_map_colors[0] = _kingdom_colors[0] + diff;
    _kingdom_map_colors[1] = _kingdom_colors[1] + diff;
    _kingdom_map_colors[2] = _kingdom_colors[2] + diff;
    _kingdom_map_colors[3] = _kingdom_colors[3] + diff;
    
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
    
    _selected_cell = 0;
    std::uniform_int_distribution<int> dist3(0, 8);
    float cutoff = 0.5f * size - 0.5f;
    for (VoronoiCell2* c : _world->cells()) {
        if (c->p->l[0] < -cutoff || c->p->l[0] > cutoff || c->p->l[1] < -cutoff || c->p->l[1] > cutoff) {
            c->draw = false;
        } else {
            if (dist3(_rand_engine) == 0) {
                c->building = 1;
            } else if (dist3(_rand_engine) < 2) {
                c->type = 1;
            }
            if (c->building == 1) {
                c->spawn = dist3(_rand_engine);
            }
        }
    }
    _current_king = 0;
    _turn_state = 0;
    _turn_timer = 0.0f;
    
    //std::uniform_int_distribution<uint8_t> dist2(0, 200);
    _kings.resize(4);
    int i = 0;
    for (VoronoiCell2* c : _world->cells()) {
        if (c->draw && c->building == 1) {
            _kings[i].location = c;
            _kings[i].destination = c;
            _kings[i].coins = 0;
            _kings[i].color = _kingdom_colors[i];
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
    if (_current_king == 0 && button == MBLeft && _selected_cell) {
        _kings[0].destination = _selected_cell;
        next_turn_state();
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
    
    _target_camera_position = Vector3(_kings[_current_king].location->p->l[0], 0.0f, _kings[_current_king].location->p->l[1]);
    
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

void GameCore::next_turn_state() {
    if (_turn_state == 2) {
        _kings[_current_king].location = _kings[_current_king].destination;
    }
    
    _turn_timer = 0.0f;
    _turn_state++;
    if (_turn_state > 3) {
        _turn_state = 0;
        _current_king = _current_king+1 < _kings.size() ? _current_king+1 : 0;
    }
    
    // update mines
    if (_current_king == 0 && _turn_state == 0) {
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
    
    std::cout << "state: " << _turn_state << std::endl;
    std::cout << "king: " << _current_king << std::endl;
}

void GameCore::draw_king(King const& king) {
    Vector3 position = linear_interpolation(Vector3(king.location->p->l[0], 0.0f, king.location->p->l[1]),
                                            Vector3(king.destination->p->l[0], 0.0f, king.destination->p->l[1]),
                                            _turn_timer);
    
    Matrix4 model = homogeneous_translation(position);
    draw(_view * model * _sprite_rotation, _flag_mesh, king.color);
    Matrix4 offset = homogeneous_translation(Vector3(0.0f, 0.31f, 0.0f));
    draw(_view * model * _sprite_rotation * offset, _crown_mesh, _gold_color);
    if (king.coins > 0) {
        offset = homogeneous_translation(Vector3(-0.05f, 0.25f, 0.0f));
        draw(_view * model * _sprite_rotation * offset, _coin_mesh, _gold_color);
    }
    if (king.coins > 1) {
        offset = homogeneous_translation(Vector3(0.05f, 0.25f, 0.0f));
        draw(_view * model * _sprite_rotation * offset, _coin_mesh, _gold_color);
    }
    if (king.coins > 2) {
        offset = homogeneous_translation(Vector3(-0.05f, 0.15f, 0.0f));
        draw(_view * model * _sprite_rotation * offset, _coin_mesh, _gold_color);
    }
    if (king.coins > 3) {
        offset = homogeneous_translation(Vector3(0.05f, 0.15f, 0.0f));
        draw(_view * model * _sprite_rotation * offset, _coin_mesh, _gold_color);
    }
}

void GameCore::update_king(float dt, King& king) {
    if (king.location != king.destination) {
        _turn_timer += 4.0f * dt;
        if (_turn_timer > 1.0f) {
            _turn_timer = 0.0f;
            king.location = king.destination;
        }
    }
}

void GameCore::king_ai(King& king) {
    std::vector<VoronoiCell2*> valid_destinations;
    for (VoronoiCell2* c : _world->cells()) {
        if (c == king.location || (c->draw && c->type == 0 && std::find(king.location->n.begin(), king.location->n.end(), c->p) != king.location->n.end())) {
            valid_destinations.push_back(c);
        }
    }
    
    std::uniform_int_distribution<int> dist(0, (int)valid_destinations.size()-1);
    king.destination = valid_destinations[dist(_rand_engine)];
}

void GameCore::update(float dt) {
    _second_timer += dt;
    if (_second_timer > 1.0f) {
        _second_timer -= 1.0f;
    }
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    update_camera(dt);
    
    _selected_cell = 0;
    for (VoronoiCell2* c : _world->cells()) {
        if (c->draw && c->type == 0) {
            Color4 color(_cell_color);
            if (c == _kings[_current_king].location || std::find(_kings[_current_king].location->n.begin(),
                          _kings[_current_king].location->n.end(),
                          c->p) != _kings[_current_king].location->n.end()) {
                //color = Color4(160, 210, 160, 255);
                color = _kingdom_map_colors[_current_king];
                if (_current_king == 0 && _turn_state == 1 && cursor_on_shape(_view_projection, c->vertices, _cursor)) {
                    _selected_cell = c;
                }
            }
            draw(_view, c->vertices, color);
        }
    }
    
    for (VoronoiCell2* c : _world->cells()) {
        if (c->draw) {
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
    
    if (_turn_state == 1) {
        if (_current_king != 0) {
            king_ai(_kings[_current_king]);
            next_turn_state();
        }
    }
    
    for (King& k : _kings) {
        if (k.location->coins > 0) {
            k.location->coins = 0;
            k.coins++;
        }
        draw_king(k);
    }
    
    // draw indicator
    glDisable(GL_DEPTH_TEST);
    if (_current_king == 0 && _turn_state == 1 && _selected_cell) {
        Matrix4 model = homogeneous_translation(Vector3(_selected_cell->p->l[0], 0.0f, _selected_cell->p->l[1]));
        if (_kings[_current_king].coins >= 4) {
            draw(_view * model * _sprite_rotation * homogeneous_translation(Vector3(0.0f, 0.05f + 0.05f * sinf(_second_timer * 2.0f * PI), 0.0f)), _small_flag_mesh, _kings[_current_king].color);
        } else {
            draw(_view * model * _sprite_rotation * homogeneous_translation(Vector3(0.0f, 0.05f + 0.05f * sinf(_second_timer * 2.0f * PI), 0.0f)), _indicator_mesh, _indicator_color);
        }
    }
    glEnable(GL_DEPTH_TEST);
}
