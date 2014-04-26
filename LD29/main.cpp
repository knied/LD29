//
//  main.m
//  LD29
//
//  Created by Kristof Niederholtmeyer on 17.04.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//

#include "SDLClasses.h"
#include <OpenGL/gl.h>
#include "Camera.h"
#include "DelaunayTriangulation.h"
#include <random>
#include "GameCore.h"

/*void plot(std::vector<Vector2> const& points) {
    for (Vector2 const& p : points) {
        std::cout << p[0] << "\t" << p[1] << std::endl;
    }
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
    
    std::vector<Vector2> shape{p0, p1, p2, p3, p4};
    shape = cut(shape, circle(Vector2(-0.05f, 0.15f), 0.035f, 16));
    shape = cut(shape, circle(Vector2(-0.05f, 0.25f), 0.035f, 16));
    shape = cut(shape, circle(Vector2(0.05f, 0.15f), 0.035f, 16));
    shape = cut(shape, circle(Vector2(0.05f, 0.25f), 0.035f, 16));
    
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
}*/

int main(int argc, const char * argv[])
{
    typedef std::chrono::system_clock Clock;
    typedef std::chrono::time_point<Clock> TimeStamp;
    typedef std::chrono::duration<double> Seconds;
    
    int const view_width = 1024;
    int const view_height = 768;
    
    // [[[NSBundle mainBundle] resourcePath] UTF8String]
    SDLSystem system("");
    SDLGLWindow window(system, {view_width, view_height}, "LD29");
    window.set_vsync(true);
    //window.set_fullscreen(true);
    //system.set_show_cursor(false);
    
    GameCore game(view_width, view_height);
    
    TimeStamp old_time(Clock::now());
    
    bool done = false;
    SDL_Event event;
    while (!done) {
        while (system.poll_event(event)) {
            if (event.type == SDL_QUIT) {
                done = true;
            } else if (event.type == SDL_MOUSEMOTION) {
                Vector2 cursor(event.motion.x, event.motion.y);
                cursor /= Vector2(view_width, view_height);
                cursor *= 2.0f;
                cursor -= 1.0f;
                cursor[1] *= -1.0f;
                game.mouse_moved(cursor[0], cursor[1]);
                if (event.motion.state) {
                    MouseButton mb = MBLeft;
                    if (event.button.button != SDL_BUTTON_LEFT) {
                        mb = MBRight;
                    }
                    game.mouse_dragged(mb, event.motion.xrel, event.motion.yrel);
                }
            } else if (event.type == SDL_MOUSEWHEEL) {
                game.mouse_wheel(event.wheel.y);
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                MouseButton mb = MBLeft;
                if (event.button.button != SDL_BUTTON_LEFT) {
                    mb = MBRight;
                }
                game.mouse_down(mb, event.button.x, event.button.y);
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                MouseButton mb = MBLeft;
                if (event.button.button != SDL_BUTTON_LEFT) {
                    mb = MBRight;
                }
                game.mouse_up(mb, event.button.x, event.button.y);
            }
        }
        
        TimeStamp new_time(Clock::now());
        Seconds elapsed = new_time - old_time;
        game.update(elapsed.count());
        old_time = new_time;
        
        window.swap();
    }
    
    // setup a camera
    /*Camera camera;
    
    // setup world map
    //std::vector<Vector3> world_mesh;
    //init_world_mesh(world_mesh);
    
    float const size = 10.0f;
    
    std::vector<Vector2> points;
    std::mt19937 eng;
    std::uniform_real_distribution<float> dist(-size*0.5f,size*0.5f);
    for (int i = 0; i < 500; ++i) {
        points.push_back(Vector2(dist(eng), dist(eng)));
    }
    for (int i = 0; i < 10; ++i) {
        points = smooth(size, size, points);
    }
    std::vector<Point2*> ppoints;
    for (Vector2 const& p : points) {
        ppoints.push_back(new Point2{p});
    }
    
    VoronoiDiagram dt(2.0f * size, 2.0f * size, ppoints);
    float cutoff = 0.5f * size - 0.5f;
    for (VoronoiCell2* c : dt.cells()) {
        if (c->p->l[0] < -cutoff || c->p->l[0] > cutoff || c->p->l[1] < -cutoff || c->p->l[1] > cutoff) {
            c->draw = false;
        }
    }
    
    // setup flag
    std::vector<Vector3> flag_mesh;
    init_flag_mesh(flag_mesh);
    
    // setup crown
    std::vector<Vector3> crown_mesh;
    init_crown_mesh(crown_mesh);
    
    // setup dot
    std::vector<Vector3> dot_mesh;
    init_dot_mesh(dot_mesh, 0.025f, 16);
    
    float x = 0.0f;
    float y = 0.0f;
    float d = 2.0f;
    
    Vector2 cursor;
    
    init_gl(camera, Color4(100, 150, 100, 255));
    
    bool done = false;
    bool first = true;
    SDL_Event event;
    while (!done) {
        while (system.poll_event(event)) {
            if (event.type == SDL_QUIT) {
                done = true;
            } else if (event.type == SDL_MOUSEMOTION) {
                cursor = Vector2(event.motion.x, event.motion.y);
                cursor /= Vector2(1024, 768);
                cursor *= 2.0f;
                cursor -= 1.0f;
                cursor[1] *= -1.0f;
                if (event.motion.state) {
                    y -= event.motion.xrel * 0.01f;
                    x -= event.motion.yrel * 0.01f;
                }
            } else if (event.type == SDL_MOUSEWHEEL) {
                d += event.wheel.y * 0.5f;
            }
        }
        
        if (x > -PI * 0.1f) {
            x = -PI * 0.1f;
        }
        if (x < -PI * 0.4f) {
            x = -PI * 0.4f;
        }
        if (y < 0.0f) {
            y += 2.0f * PI;
        }
        if (y > 2.0f * PI) {
            y -= 2.0f * PI;
        }
        if (d < 1.0f) {
            d = 1.0f;
        }
        if (d > 10.0f) {
            d = 10.0f;
        }
        
        camera.set_rotaton(x, y);
        camera.set_zoom(d);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        Matrix4 view = camera.transformation().world_to_local();
        Matrix4 projection = camera.projection();
        Matrix4 view_projection = projection * view;
        
        Vector3 camera_normal = transformed_vector(camera.transformation().local_to_world(), Vector3(0.0f, 0.0f, 1.0f));
        Vector3 xz = vector_normal(camera_normal - Vector3(0.0f, 1.0f, 0.0f) * dot(Vector3(0.0f, 1.0f, 0.0f), camera_normal));
        float angle = atan2f(xz[0], xz[2]);
        Matrix4 sprite_rotation = homogeneous_rotation(Quaternion<float>(Vector3(0.0f, 1.0f, 0.0f), angle));
        
        Vector2 character;
        for (VoronoiCell2* c : dt.cells()) {
            if (c->draw) {
                Color4 color(80, 127, 80, 255);
                if (cursor_on_shape(view_projection, c->vertices, cursor)) {
                    //std::cout << "on" << std::endl;
                    color = Color4(120, 167, 120, 255);
                    character = c->p->l;
                }
                draw(view, c->vertices, color);
            }
        }
        
        //std::cout << character << std::endl;
        
        Matrix4 model = homogeneous_translation(Vector3(character[0], 0.0f, character[1]));
        draw(view * model * sprite_rotation, flag_mesh, Color4(200, 200, 200, 255));
        //draw_sprite(camera, Vector3(0.5f, 0.31f, 0.0f), Vector3(), crown_mesh, Color4(200, 200, 64, 255));
        //draw_sprite(camera, Vector3(0.5f, 0.0f, 0.0f), Vector3(-0.05f, 0.25f, 0.0f), dot_mesh, Color4(200, 200, 64, 255));
        //draw_sprite(camera, Vector3(0.5f, 0.0f, 0.0f), Vector3(0.05f, 0.25f, 0.0f), dot_mesh, Color4(200, 200, 64, 255));
        //draw_sprite(camera, Vector3(0.5f, 0.0f, 0.0f), Vector3(-0.05f, 0.15f, 0.0f), dot_mesh, Color4(200, 200, 64, 255));
        
        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        
        window.swap();
        
        first = false;
    }*/
    
    return 0;
}
