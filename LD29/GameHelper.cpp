//
//  GameHelper.cpp
//  LD29
//
//  Created by Kristof Niederholtmeyer on 30.04.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//

#include "GameHelper.h"

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

Vector2 point_on_screen(Matrix4 const& view_projection, Vector3 const& p) {
    Vector4 proj = view_projection * Vector4(p[0], p[1], p[2], 1.0f);
    return Vector2(proj[0]/fabs(proj[3]), proj[1]/fabs(proj[3]));
}

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

bool valid_cell(VoronoiCell2* c, float cutoff) {
    if (c->p->l[0] < -cutoff || c->p->l[0] > cutoff || c->p->l[1] < -cutoff || c->p->l[1] > cutoff) {
        return false;
    }
    return true;
}