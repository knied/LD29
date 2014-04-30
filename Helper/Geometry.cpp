//
//  Geometry.cpp
//  LD29
//
//  Created by Kristof Niederholtmeyer on 20.04.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//

#include "Geometry.h"

Face2::Face2(Point2* p0, Point2* p1, Point2* p2)
: p{p0, p1, p2}, e{{this, 0, 0}, {this, 1, 0}, {this, 2, 0}} {}
Face2::Face2(Face2 const& f)
: p{f.p[0], f.p[1], f.p[2]}, e{{this, 0, f.e[0].e}, {this, 1, f.e[1].e}, {this, 2, f.e[2].e}} {}
Face2 const& Face2::operator = (Face2 const& f) {
    p[0] = f.p[0];
    p[1] = f.p[1];
    p[2] = f.p[2];
    e[0] = {this, 0, f.e[0].e};
    e[1] = {this, 1, f.e[1].e};
    e[2] = {this, 2, f.e[2].e};
    return *this;
}

// next index in counter clockwise direction
int ccw_next(int i) {
    static int lookup[] = {1, 2, 0};
    return lookup[i];
}

// next index in clockwise direction
int cw_next(int i) {
    static int lookup[] = {2, 0, 1};
    return lookup[i];
}

bool ccw(Point2* p0, Point2* p1, Point2* p2) {
    Vector2 d01 = p1->l - p0->l;
    Vector2 d02 = p2->l - p0->l;
    if (d01[0] * d02[1] - d01[1] * d02[0] >= 0.0f) {
        return true;
    }
    return false;
}

float area(Point2* p0, Point2* p1, Point2* p2) {
    Vector2 d01 = p1->l - p0->l;
    Vector2 d02 = p2->l - p0->l;
    return 0.5f * (d01[0] * d02[1] - d01[1] * d02[0]);
}

float area(Vector2 const& p0, Vector2 const& p1, Vector2 const& p2) {
    Vector2 d01 = p1 - p0;
    Vector2 d02 = p2 - p0;
    return 0.5f * (d01[0] * d02[1] - d01[1] * d02[0]);
}

bool in_circle(Point2* p0, Point2* p1, Point2* p2, Point2* p3) {
    Vector2 u = p0->l;
    Vector2 v = p1->l;
    Vector2 p = p2->l;
    Vector2 q = p3->l;
    Matrix4 delta(1.0f, u[0], u[1], u[0] * u[0] + u[1] * u[1],
                  1.0f, v[0], v[1], v[0] * v[0] + v[1] * v[1],
                  1.0f, p[0], p[1], p[0] * p[0] + p[1] * p[1],
                  1.0f, q[0], q[1], q[0] * q[0] + q[1] * q[1]);
    Matrix3 gamma(1.0f, u[0], u[1],
                  1.0f, v[0], v[1],
                  1.0f, p[0], p[1]);
    return det(gamma) * det(delta) < 0.0f;
}

void connect(Face2* f0, int i0, Face2* f1, int i1) {
    Edge2* e0 = &f0->e[i0];
    Edge2* e1 = &f1->e[i1];
    e0->e = e1;
    e1->e = e0;
}

void disconnect(Face2* f) {
    for (int i = 0; i < 3; ++i) {
        if (f->e[i].e) {
            f->e[i].e->e = 0;
        }
        f->e[i].e = 0;
    }
}

void disconnect(Face2* f, int i) {
    if (f->e[i].e) {
        f->e[i].e->e = 0;
    }
    f->e[i].e = 0;
}

Face2* neighbour(Face2* f, int i) {
    if (f->e[i].e) {
        return f->e[i].e->f;
    }
    return 0;
}

void flip(Face2* f0, int i0, Face2* f1, int i1) {
    Point2* p0 = f0->p[i0];
    Point2* p1 = f1->p[cw_next(i1)];
    Point2* p2 = f1->p[i1];
    Point2* p3 = f0->p[cw_next(i0)];
    Edge2* e0 = f1->e[ccw_next(i1)].e;
    Edge2* e1 = f1->e[cw_next(i1)].e;
    Edge2* e2 = f0->e[ccw_next(i0)].e;
    Edge2* e3 = f0->e[cw_next(i0)].e;
    disconnect(f0);
    disconnect(f1);
    *f0 = Face2(p1, p3, p0);
    *f1 = Face2(p3, p1, p2);
    connect(f0, 0, f1, 0);
    if (e0) connect(f0, 2, e0->f, e0->i);
    if (e1) connect(f1, 1, e1->f, e1->i);
    if (e2) connect(f1, 2, e2->f, e2->i);
    if (e3) connect(f0, 1, e3->f, e3->i);
}

void split(Face2* f, Point2* p, Face2** out0, Face2** out1, Face2** out2) {
    Point2* p0 = f->p[0];
    Point2* p1 = f->p[1];
    Point2* p2 = f->p[2];
    Edge2* e0 = f->e[0].e;
    Edge2* e1 = f->e[1].e;
    Edge2* e2 = f->e[2].e;
    disconnect(f);
    *out0 = new Face2(p0, p1, p);
    *out1 = new Face2(p1, p2, p);
    *out2 = new Face2(p2, p0, p);
    connect(*out0, 1, *out1, 2);
    connect(*out1, 1, *out2, 2);
    connect(*out2, 1, *out0, 2);
    if (e0) connect(*out0, 0, e0->f, e0->i);
    if (e1) connect(*out1, 0, e1->f, e1->i);
    if (e2) connect(*out2, 0, e2->f, e2->i);
}

void split_edge(Face2* f, int i, Point2* p, Face2** out0, Face2** out1) {
    int i0 = i;
    int i1 = ccw_next(i0);
    int i2 = ccw_next(i1);
    Point2* p0 = f->p[i0];
    Point2* p1 = f->p[i1];
    Point2* p2 = f->p[i2];
    Edge2* e1 = f->e[i1].e;
    Edge2* e2 = f->e[i2].e;
    disconnect(f);
    *out0 = new Face2(p0, p, p2);
    *out1 = new Face2(p, p1, p2);
    connect(*out0, 1, *out1, 2);
    if (e1) connect(*out1, 1, e1->f, e1->i);
    if (e2) connect(*out0, 2, e2->f, e2->i);
}

Point2* support(std::vector<Point2*> const& points, Vector2 const& direction) {
    Point2* result = points[0];
    float d_max = dot(direction, points[0]->l);
    for (int i = 1; i < points.size(); ++i) {
        float d = dot(direction, points[i]->l);
        if (d > d_max) {
            d_max = d;
            result = points[i];
        }
    }
    return result;
}

std::vector<Point2*> convex_hull(std::vector<Point2*> const& points) {
    std::vector<Point2*> hull{support(points, Vector2(-1.0f, 0.0f)), support(points, Vector2(1.0f, 0.0f))};
    int i0 = 0;
    do {
        int i1 = i0+1 < hull.size() ? i0+1 : 0;
        Vector2 n = vector_normal(hull[i1]->l - hull[i0]->l);
        Vector2 direction = -Vector2(-n[1], n[0]);
        Point2* s = support(points, direction);
        if (s != hull[i0] && s != hull[i1]) {
            hull.insert(hull.begin()+(i0+1), s);
        } else {
            i0++;
        }
    } while (i0 < hull.size());
    return hull;
}

std::vector<Face2*> triangulate_convex(std::vector<Point2*> const& hull) {
    std::vector<Face2*> result;
    std::vector<Point2*> sub_hull = hull;
    std::vector<Face2*> neighbours(hull.size(), 0);
    while (sub_hull.size() > 3) {
        for (int i0 = 0; i0 < sub_hull.size(); ++i0) {
            int i1 = i0+1 < sub_hull.size() ? i0+1 : 0;
            int i2 = i1+1 < sub_hull.size() ? i1+1 : 0;
            if (area(sub_hull[i0], sub_hull[i1], sub_hull[i2]) > 0.0f) {
                result.push_back(new Face2(sub_hull[i0], sub_hull[i1], sub_hull[i2]));
                if (neighbours[i0]) connect(result.back(), 0, neighbours[i0], 2);
                if (neighbours[i1]) connect(result.back(), 1, neighbours[i1], 2);
                neighbours[i0] = result.back();
                neighbours.erase(neighbours.begin()+i1);
                sub_hull.erase(sub_hull.begin()+i1);
                break;
            }
        }
    }
    result.push_back(new Face2(sub_hull[0], sub_hull[1], sub_hull[2]));
    if (neighbours[0]) connect(result.back(), 0, neighbours[0], 2);
    if (neighbours[1]) connect(result.back(), 1, neighbours[1], 2);
    if (neighbours[2]) connect(result.back(), 2, neighbours[2], 2);
    return result;
}

std::vector<Vector3> vertex_data(std::vector<Face2*> const faces) {
    std::vector<Vector3> vertices(faces.size() * 3);
    for (int i = 0; i < faces.size(); ++i) {
        vertices[i * 3 + 0] = Vector3(faces[i]->p[0]->l[0], 0.0f, faces[i]->p[0]->l[1]);
        vertices[i * 3 + 1] = Vector3(faces[i]->p[1]->l[0], 0.0f, faces[i]->p[1]->l[1]);
        vertices[i * 3 + 2] = Vector3(faces[i]->p[2]->l[0], 0.0f, faces[i]->p[2]->l[1]);
    }
    return vertices;
}

bool intersection(Vector2 const& p0, Vector2 const& n0, Vector2 const& p1, Vector2 const& n1, Vector2& result) {
    Vector2 vs(-n1[1], n1[0]);
    Vector2 w = p0 - p1;
    float d = dot(vs, n0);
    if (d == 0.0f) return false;
    float t = -dot(vs, w)/d;
    result = p0 + t * n0;
    return true;
}

bool segment_intersection(Vector2 const& p0, Vector2 const& p1, Vector2 const& q0, Vector2 const& q1, Vector2& result, bool incl) {
    if (!incl && squared_length(p0 - q0) == 0) return false;
    if (!incl && squared_length(p1 - q0) == 0) return false;
    if (!incl && squared_length(p0 - q1) == 0) return false;
    if (!incl && squared_length(p1 - q1) == 0) return false;
    Vector2 n0 = p1 - p0;
    Vector2 n1 = q1 - q0;
    Vector2 us(-n0[1], n0[0]);
    Vector2 vs(-n1[1], n1[0]);
    Vector2 w = p0 - q0;
    float d = dot(vs, n0);
    if (d == 0.0f) return false;
    float t = -dot(vs, w)/d;
    float s = -dot(us, w)/d;
    result = p0 + t * n0;
    return (t >= 0.0f) && (t <= 1.0f) && (s >= 0.0f) && (s <= 1.0f);
}

std::vector<Vector2> cut(std::vector<Vector2> const& a, std::vector<Vector2> const& b) {
    std::vector<Vector2> result;
    std::vector<Vector2> br = b;
    std::reverse(br.begin(), br.end());
    
    struct Intersection {
        Vector2 pos;
        bool start;
        int i0;
        int i1;
        int j0;
        int j1;
    };
    std::vector<Intersection> intersections;
    
    for (int i0 = 0; i0 < a.size(); ++i0) {
        int i1 = i0+1 < a.size() ? i0+1 : 0;
        for (int j0 = 0; j0 < br.size(); ++j0) {
            int j1 = j0+1 < br.size() ? j0+1 : 0;
            
            Intersection inter;
            inter.i0 = i0;
            inter.i1 = i1;
            inter.j0 = j0;
            inter.j1 = j1;
            if (segment_intersection(a[i0], a[i1], br[j0], br[j1], inter.pos, true)) {
                inter.start = area(a[i0], a[i1], br[j0]) < 0.0f;
                intersections.push_back(inter);
            }
        }
    }
    
    if (intersections.size() > 0) {
        if (intersections[0].start == false) {
            intersections.push_back(intersections.front());
            intersections.erase(intersections.begin());
        }
        
        
    } else {
        int i = 0;
        int j = 0;
        bool found = false;
        for (i = 0; i < a.size(); ++i) {
            for (j = 0; j < br.size(); ++j) {
                // is this a valid cut?
                bool valid = true;
                Vector2 n = vector_normal(a[i] - br[j]);
                for (Vector2 const& p : br) {
                    if (dot(n, p - br[j]) > 0.0f) {
                        valid = false;
                        break;
                    }
                }
                if (!valid) {
                    continue;
                }
                for (int k0 = 0; k0 < a.size(); ++k0) {
                    int k1 = k0+1 < a.size() ? k0+1 : 0;
                    
                    if (k0 != i && k1 != i) {
                        Vector2 tmp;
                        
                        if (segment_intersection(br[j], a[i], a[k0], a[k1], tmp, true)) {
                            valid = false;
                            break;
                        }
                    }
                }
                if (valid) {
                    found = true;
                    break;
                }
            }
            if (found) {
                break;
            }
        }
        
        if (found) {
            result = std::vector<Vector2>(a.begin(), a.begin()+i+1);
            result.insert(result.end(), br.begin()+j, br.end());
            result.insert(result.end(), br.begin(), br.begin()+j+1);
            result.insert(result.end(), a.begin()+i, a.end());
        }
    }
    
    return result;
}

std::vector<Vector2> triangulate(std::vector<Vector2> const& points) {
    std::vector<Vector2> triangulation;
    if (points.size() < 3) return triangulation;
    std::vector<Vector2> remaining = points;
    while (remaining.size() > 3) {
        for (int i0 = 0; i0 < remaining.size(); ++i0) {
            int i1 = i0+1 < remaining.size() ? i0+1 : 0;
            int i2 = i1+1 < remaining.size() ? i1+1 : 0;
            if (area(remaining[i0], remaining[i1], remaining[i2]) > 0.0f) {
                //std::cout << remaining.size() << std::endl;
                //std::cout << i0 << " " << i1 << " " << i2 << std::endl;
                bool valid = true;
                for (int j0 = 0; j0 < remaining.size(); ++j0) {
                    int j1 = j0+1 < remaining.size() ? j0+1 : 0;
                    Vector2 tmp;
                    if (segment_intersection(remaining[i0], remaining[i2],
                                             remaining[j0], remaining[j1], tmp)) {
                        valid = false;
                        break;
                    }
                }
                if (valid) {
                    //std::cout << "create" << std::endl;
                    triangulation.push_back(remaining[i0]);
                    triangulation.push_back(remaining[i1]);
                    triangulation.push_back(remaining[i2]);
                    remaining.erase(remaining.begin()+i1);
                    
                    /*if (remaining.size() == 23) {
                        std::cout << "remaining:" << std::endl;
                        for (Vector2 const& p : remaining) {
                            std::cout << p << std::endl;
                        }
                        return triangulation;
                    }*/
                    
                    break;
                }
            }
        }
    }
    triangulation.push_back(remaining[0]);
    triangulation.push_back(remaining[1]);
    triangulation.push_back(remaining[2]);
    return triangulation;
}

std::vector<Vector2> circle(Vector2 center, float r, int d) {
    std::vector<Vector2> points;
    float dt = 2.0f * PI / (float)std::max(3, d);
    for (int i = 0; i < std::max(3, d); ++i) {
        points.push_back(center + Vector2(r * cosf(i * dt), r * sinf(i * dt)));
    }
    return points;
}

std::vector<Vector3> voronoi_cell_mesh(float width, float height, VoronoiCell2* cell) {
    std::vector<Vector3> result;

    std::vector<Vector2> convex;
    convex.resize(4);
    convex[0] = Vector2(-0.5f * width, -0.5f * height);
    convex[1] = Vector2(0.5f * width, -0.5f * height);
    convex[2] = Vector2(0.5f * width, 0.5f * height);
    convex[3] = Vector2(-0.5f * width, 0.5f * height);
    for (VoronoiCell2* p : cell->n) {
        Vector2 cut = vector_normal(p->p->l - cell->p->l);
        float cut_d = dot(cut, p->p->l - cell->p->l) * 0.5f - 0.01f;
        std::vector<Vector2> tmp_convex;
        for (int i0 = 0; i0 < convex.size(); ++i0) {
            int i1 = i0+1 < convex.size() ? i0+1 : 0;
            float d0 = dot(cut, convex[i0] - cell->p->l) - cut_d;
            float d1 = dot(cut, convex[i1] - cell->p->l) - cut_d;
            if (d0 <= 0) {
                tmp_convex.push_back(convex[i0]);
            }
            if (sign(d0) != sign(d1)) {
                Vector2 res;
                intersection(convex[i0], convex[i1] - convex[i0],
                             cell->p->l + cut * cut_d, Vector2(-cut[1], cut[0]), res);
                tmp_convex.push_back(res);
            }
        }
        convex = tmp_convex;
    }
    
    for (int i = 1; i < (int)convex.size()-1; ++i) {
        result.push_back(Vector3(convex[0][0], 0.0f, convex[0][1]));
        result.push_back(Vector3(convex[i][0], 0.0f, convex[i][1]));
        result.push_back(Vector3(convex[i+1][0], 0.0f, convex[i+1][1]));
    }
    
    return result;
}
