//
//  main.m
//  LD29
//
//  Created by Kristof Niederholtmeyer on 17.04.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//

#include <Cocoa/Cocoa.h>
#include "SDLClasses.h"
//#include "GLClasses.h"
#include <OpenGL/gl.h>
#include "Camera.h"
#include "DelaunayTriangulation.h"
#include <random>

void plot(std::vector<Vector2> const& points) {
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

/*struct Mesh {
    GLBuffer buffer;
    GLVertexArray vertex_array;
};*/

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

void init_world_mesh(std::vector<Vector3>& mesh) {
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
    VoronoiDiagram dt(ppoints);
    dt.vertex_data(2.0f * size, 2.0f * size, mesh);
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

void draw(Camera& camera, Vector3 const& position, std::vector<Vector3> const& shape, Color4 const& color) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixf(&camera.transformation().world_to_local()(0,0));
    glTranslatef(position[0], position[1], position[2]);
    
    glBegin(GL_TRIANGLES);
    
    glColor3ub(color[0], color[1], color[2]);
    
    for (Vector3 const& v : shape) {
        glVertex3fv(&v[0]);
    }
    
    glEnd();
}

void draw_sprite(Camera& camera, Vector3 const& position, Vector3 const& offset, std::vector<Vector3> const& shape, Color4 const& color) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixf(&camera.transformation().world_to_local()(0,0));
    glTranslatef(position[0], position[1], position[2]);
    
    Vector3 xz = vector_normal(camera.transformation().world_z() - Vector3(0.0f, 1.0f, 0.0f) * dot(Vector3(0.0f, 1.0f, 0.0f), camera.transformation().world_z()));
    float angle = atan2f(xz[0], xz[2]);
    glRotatef(angle * 180.0f / PI, 0.0f, 1.0f, 0.0f);
    glTranslatef(offset[0], offset[1], offset[2]);
    
    glBegin(GL_TRIANGLES);
    
    glColor3ub(color[0], color[1], color[2]);
    
    for (Vector3 const& v : shape) {
        glVertex3fv(&v[0]);
    }
    
    glEnd();
}

int main(int argc, const char * argv[])
{
    SDLSystem system([[[NSBundle mainBundle] resourcePath] UTF8String]);
    SDLGLWindow window(system, {1024, 768}, "LD29");
    window.set_vsync(true);
    //window.set_fullscreen(true);
    //system.set_show_cursor(false);
    
    // setup a camera
    Camera camera;
    
    // setup world map
    std::vector<Vector3> world_mesh;
    init_world_mesh(world_mesh);
    
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
    
    init_gl(camera, Color4(100, 150, 100, 255));
    
    bool done = false;
    bool first = true;
    SDL_Event event;
    while (!done) {
        while (system.poll_event(event)) {
            if (event.type == SDL_QUIT) {
                done = true;
            } else if (event.type == SDL_MOUSEMOTION && event.motion.state) {
                y -= event.motion.xrel * 0.01f;
                x -= event.motion.yrel * 0.01f;
            } else if (event.type == SDL_MOUSEWHEEL) {
                d += event.wheel.y * 0.5f;
            }
        }
        
        if (x > -PI * 0.02f) {
            x = -PI * 0.02f;
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
        
        draw(camera, Vector3(), world_mesh, Color4(80, 127, 80, 255));
        draw_sprite(camera, Vector3(0.5f, 0.0f, 0.0f), Vector3(), flag_mesh, Color4(200, 200, 200, 255));
        draw_sprite(camera, Vector3(0.5f, 0.31f, 0.0f), Vector3(), crown_mesh, Color4(200, 200, 64, 255));
        draw_sprite(camera, Vector3(0.5f, 0.0f, 0.0f), Vector3(-0.05f, 0.25f, 0.0f), dot_mesh, Color4(200, 200, 64, 255));
        draw_sprite(camera, Vector3(0.5f, 0.0f, 0.0f), Vector3(0.05f, 0.25f, 0.0f), dot_mesh, Color4(200, 200, 64, 255));
        draw_sprite(camera, Vector3(0.5f, 0.0f, 0.0f), Vector3(-0.05f, 0.15f, 0.0f), dot_mesh, Color4(200, 200, 64, 255));
        
        draw_sprite(camera, Vector3(-0.5f, 0.0f, 0.0f), Vector3(), flag_mesh, Color4(32, 32, 32, 255));
        draw_sprite(camera, Vector3(-0.5f, 0.31f, 0.0f), Vector3(), crown_mesh, Color4(200, 200, 64, 255));
        draw_sprite(camera, Vector3(-0.5f, 0.0f, 0.0f), Vector3(-0.05f, 0.25f, 0.0f), dot_mesh, Color4(200, 200, 64, 255));
        draw_sprite(camera, Vector3(-0.5f, 0.0f, 0.0f), Vector3(0.05f, 0.25f, 0.0f), dot_mesh, Color4(200, 200, 64, 255));
        draw_sprite(camera, Vector3(-0.5f, 0.0f, 0.0f), Vector3(-0.05f, 0.15f, 0.0f), dot_mesh, Color4(200, 200, 64, 255));
        
        draw_sprite(camera, Vector3(0.0f, 0.0f, 0.5f), Vector3(), flag_mesh, Color4(255, 180, 180, 255));
        draw_sprite(camera, Vector3(0.0f, 0.31f, 0.5f), Vector3(), crown_mesh, Color4(200, 200, 64, 255));
        draw_sprite(camera, Vector3(0.0f, 0.0f, 0.5f), Vector3(-0.05f, 0.25f, 0.0f), dot_mesh, Color4(200, 200, 64, 255));
        draw_sprite(camera, Vector3(0.0f, 0.0f, 0.5f), Vector3(0.05f, 0.25f, 0.0f), dot_mesh, Color4(200, 200, 64, 255));
        draw_sprite(camera, Vector3(0.0f, 0.0f, 0.5f), Vector3(-0.05f, 0.15f, 0.0f), dot_mesh, Color4(200, 200, 64, 255));
        
        draw_sprite(camera, Vector3(0.0f, 0.0f, -0.5f), Vector3(), flag_mesh, Color4(180, 180, 255, 255));
        draw_sprite(camera, Vector3(0.0f, 0.31f, -0.5f), Vector3(), crown_mesh, Color4(200, 200, 64, 255));
        draw_sprite(camera, Vector3(0.0f, 0.0f, -0.5f), Vector3(-0.05f, 0.25f, 0.0f), dot_mesh, Color4(200, 200, 64, 255));
        draw_sprite(camera, Vector3(0.0f, 0.0f, -0.5f), Vector3(0.05f, 0.25f, 0.0f), dot_mesh, Color4(200, 200, 64, 255));
        draw_sprite(camera, Vector3(0.0f, 0.0f, -0.5f), Vector3(-0.05f, 0.15f, 0.0f), dot_mesh, Color4(200, 200, 64, 255));
        
        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        
        /*{
            GLBindProgram bound_program(program);
            
            bound_program.set_uniform(MVP_location, camera.projection() * camera.transformation().world_to_local());
            {
                GLBindVertexArray bound_vertex_array(world_mesh.vertex_array);
                bound_vertex_array.set(world_mesh.buffer, {{ 0, 0, true }, { 1, 1, true }});
                bound_vertex_array.draw(GL_TRIANGLES);
            }
            
            bound_program.set_uniform(MVP_location, camera.projection() * camera.transformation().world_to_local());
            {
                GLBindVertexArray bound_vertex_array(flag_mesh.vertex_array);
                bound_vertex_array.set(flag_mesh.buffer, {{ 0, 0, true }, { 1, 1, true }});
                bound_vertex_array.draw(GL_TRIANGLES);
            }
            
            bound_program.set_uniform(MVP_location, camera.projection() * camera.transformation().world_to_local() * homogeneous_translation(Vector3(0.0f, 0.31f, 0.0f)));
            {
                GLBindVertexArray bound_vertex_array(crown_mesh.vertex_array);
                bound_vertex_array.set(crown_mesh.buffer, {{ 0, 0, true }, { 1, 1, true }});
                bound_vertex_array.draw(GL_TRIANGLES);
            }
            
            bound_program.set_uniform(MVP_location, camera.projection() * camera.transformation().world_to_local() * homogeneous_translation(Vector3(-0.05f, 0.25f, 0.0f)));
            {
                GLBindVertexArray bound_vertex_array(dot_mesh.vertex_array);
                bound_vertex_array.set(dot_mesh.buffer, {{ 0, 0, true }, { 1, 1, true }});
                bound_vertex_array.draw(GL_TRIANGLES);
            }
            bound_program.set_uniform(MVP_location, camera.projection() * camera.transformation().world_to_local() * homogeneous_translation(Vector3(0.05f, 0.25f, 0.0f)));
            {
                GLBindVertexArray bound_vertex_array(dot_mesh.vertex_array);
                bound_vertex_array.set(dot_mesh.buffer, {{ 0, 0, true }, { 1, 1, true }});
                bound_vertex_array.draw(GL_TRIANGLES);
            }
            bound_program.set_uniform(MVP_location, camera.projection() * camera.transformation().world_to_local() * homogeneous_translation(Vector3(-0.05f, 0.15f, 0.0f)));
            {
                GLBindVertexArray bound_vertex_array(dot_mesh.vertex_array);
                bound_vertex_array.set(dot_mesh.buffer, {{ 0, 0, true }, { 1, 1, true }});
                bound_vertex_array.draw(GL_TRIANGLES);
            }
            bound_program.set_uniform(MVP_location, camera.projection() * camera.transformation().world_to_local() * homogeneous_translation(Vector3(0.05f, 0.15f, 0.0f)));
            {
                GLBindVertexArray bound_vertex_array(dot_mesh.vertex_array);
                bound_vertex_array.set(dot_mesh.buffer, {{ 0, 0, true }, { 1, 1, true }});
                bound_vertex_array.draw(GL_TRIANGLES);
            }
        }*/
        
        window.swap();
        
        first = false;
    }
    
    return 0;
}
