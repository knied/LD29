//
//  GameShapes.cpp
//  LD29
//
//  Created by Kristof Niederholtmeyer on 30.04.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//

#include "GameShapes.h"
#include "Geometry.h"

GLShape to_xy(std::vector<Vector2> const& shape) {
    std::vector<Vector3> result(shape.size());
    for (int i = 0; i < result.size(); ++i) {
        result[i] = Vector3(shape[i][0], shape[i][1], 0.0f);
    }
    return result;
}

GLShape to_xz(std::vector<Vector2> const& shape) {
    std::vector<Vector3> result(shape.size());
    for (int i = 0; i < result.size(); ++i) {
        result[i] = Vector3(shape[i][0], 0.0f, shape[i][1]);
    }
    return result;
}

void init_flag_mesh(GLShape& mesh) {
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

void init_small_flag_mesh(GLShape& mesh) {
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

void init_crown_mesh(GLShape& mesh) {
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

void init_decal_mesh(GLShape& mesh) {
    float x0 = 0.05f;
    float y0 = 0.023f;
    
    Vector2 p0(-x0, 0.0f);
    Vector2 p1(x0, 0.0f);
    Vector2 p2(x0, y0);
    Vector2 p3(-x0, y0);
    
    mesh = to_xy(triangulate({p0, p1, p2, p3}));
}

void init_dot_mesh(std::vector<Vector3>& mesh, float r, int d) {
    mesh = to_xy(triangulate(circle(Vector2(), r, d)));
}

void init_indicator_mesh(GLShape& mesh) {
    float x0 = 0.05f;
    float y0 = 0.1f;
    float y1 = 0.15f;
    
    Vector2 p0(0.0f, 0.0f);
    Vector2 p1(x0, y1);
    Vector2 p2(0.0f, y0);
    Vector2 p3(-x0, y1);
    
    mesh = to_xy(triangulate({p0, p1, p2, p3}));
}

void init_mine_mesh(GLShape& base_mesh, GLShape& wheel_mesh) {
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

void init_star_mesh(GLShape& mesh) {
    Vector2 center(0.0f, 0.2f);
    std::vector<Vector2> shape = circle(center, 0.2f, 16);
    for (int i = 1; i < 16; i+=2) {
        shape[i] += 0.5f * (center - shape[i]);
    }
    mesh = to_xy(triangulate(shape));
}