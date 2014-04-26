//
//  Geometry.h
//  LD29
//
//  Created by Kristof Niederholtmeyer on 20.04.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//

#ifndef __LD29__Geometry__
#define __LD29__Geometry__

#define TEST_STUFF

#include <vector>
#include "Types.h"

struct Edge2;

struct Point2 {
    Point2(Vector2 const& l) : l(l) {}
    
    Vector2 l; // location
    bool on_hull = false;
}; // Point2

struct Face2;

struct Edge2 {
    Face2* f; // parent face
    int i; // edge index in parent face
    Edge2* e; // neighbouring edge
}; // Edge2

struct Face2 {
    Face2(Point2* p0, Point2* p1, Point2* p2);
    Face2(Face2 const& f);
    Face2 const& operator = (Face2 const& f);

    Point2* p[3]; // corners
    Edge2 e[3]; // edges
}; // Face2

struct VoronoiCell2 {
    VoronoiCell2(Point2* p, std::vector<Point2*> const& n) : p(p), n(n) {}
    
    Point2* p;
    std::vector<Point2*> n;
    std::vector<Vector3> vertices;
    bool draw = true;
}; // VoronoiCell2

// next index in counter clockwise direction
int ccw_next(int i);
// next index in clockwise direction
int cw_next(int i);
bool ccw(Point2* p0, Point2* p1, Point2* p2);
float area(Point2* p0, Point2* p1, Point2* p2);
float area(Vector2 const& p0, Vector2 const& p1, Vector2 const& p2);
bool in_circle(Point2* p0, Point2* p1, Point2* p2, Point2* p3);
void connect(Face2* f0, int i0, Face2* f1, int i1);
void disconnect(Face2* f);
void disconnect(Face2* f, int i);
Face2* neighbour(Face2* f, int i);
void flip(Face2* f0, int i0, Face2* f1, int i1);
void split(Face2* f, Point2* p, Face2** out0, Face2** out1, Face2** out2);
void split_edge(Face2* f, int i, Point2* p, Face2** out0, Face2** out1);

Point2* support(std::vector<Point2*> const& points, Vector2 const& direction);
std::vector<Point2*> convex_hull(std::vector<Point2*> const& points);
std::vector<Face2*> triangulate_convex(std::vector<Point2*> const& hull);
std::vector<Vector3> vertex_data(std::vector<Face2*> const faces);

bool intersection(Vector2 const& p0, Vector2 const& n0, Vector2 const& p1, Vector2 const& n1, Vector2& result);
bool segment_intersection(Vector2 const& p0, Vector2 const& p1, Vector2 const& q0, Vector2 const& q1, Vector2& result, bool incl = false);
std::vector<Vector2> cut(std::vector<Vector2> const& a, std::vector<Vector2> const& b);
std::vector<Vector2> triangulate(std::vector<Vector2> const& points);

#endif /* defined(__LD29__Geometry__) */
