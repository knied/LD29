//
//  DelaunayTriangulation.h
//  LD29
//
//  Created by Kristof Niederholtmeyer on 20.04.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//

#ifndef __LD29__DelaunayTriangulation__
#define __LD29__DelaunayTriangulation__

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include "Geometry.h"

class DelaunayTriangulation {
    std::vector<Face2*> _faces;
    
    std::vector<Face2*>::iterator find_triangle(Point2* p) {
        std::vector<Face2*>::iterator it;
        for (it = _faces.begin(); it != _faces.end(); ++it) {
            Face2* f = *it;
            if (ccw(f->p[0], f->p[1], p) &&
                ccw(f->p[1], f->p[2], p) &&
                ccw(f->p[2], f->p[0], p)) {
                return it;
            }
        }
        return it;
    }
    
public:
    DelaunayTriangulation(std::vector<Point2*> const& points) {
        // create initial convex shape surrounding all points
        std::vector<Point2*> hull = convex_hull(points);
        for (Point2* p : hull) {
            p->on_hull = true;
        }
        _faces = triangulate_convex(hull);
        
        // handle remaining points
        for (Point2* p : points) {
            if (std::find(hull.begin(), hull.end(), p) != hull.end()) continue;
            
            std::vector<Face2*>::iterator it = find_triangle(p);
            if (it == _faces.end()) {
                std::cout << "WARNING: No triangle found." << std::endl;
                continue;
            }
            
            // is this point part of the convex hull?
            int edge = -1;
            for (int i = 0; i < 3; ++i) {
                if (area((*it)->p[i], p, (*it)->p[ccw_next(i)]) == 0) {
                    edge = i;
                }
            }
            // insert point
            std::set<Face2*> stack;
            if (edge < 0) {
                Face2* new0, *new1, *new2;
                split(*it, p, &new0, &new1, &new2);
                delete *it;
                _faces.erase(it);
                _faces.push_back(new0);
                _faces.push_back(new1);
                _faces.push_back(new2);
                stack.insert(new0);
                stack.insert(new1);
                stack.insert(new2);
            } else {
                p->on_hull = true;
                Face2* new0, *new1;
                split_edge(*it, edge, p, &new0, &new1);
                delete *it;
                _faces.erase(it);
                _faces.push_back(new0);
                _faces.push_back(new1);
                stack.insert(new0);
                stack.insert(new1);
            }
            
            // make the mesh delaunay
            while (stack.size() > 0) {
                Face2* f = *stack.begin();
                stack.erase(stack.begin());
                for (int i0 = 0; i0 < 3; ++i0) {
                    Face2* n = neighbour(f, i0);
                    if (n) {
                        int j0 = f->e[i0].e->i;
                        int j1 = ccw_next(j0);
                        int j2 = cw_next(j0);
                        int i1 = ccw_next(i0);
                        int i2 = cw_next(i0);
                        if (in_circle(f->p[i0], f->p[i1], f->p[i2], n->p[j2])) {
                            Face2* f0 = neighbour(f, i1);
                            Face2* f1 = neighbour(f, i2);
                            Face2* f2 = neighbour(n, j1);
                            Face2* f3 = neighbour(n, j2);
                            flip(f, i0, n, j0);
                            if (f0) stack.insert(f0);
                            if (f1) stack.insert(f1);
                            if (f2) stack.insert(f2);
                            if (f3) stack.insert(f3);
                            break;
                        }
                    }
                }
            }
        }
    }

    ~DelaunayTriangulation() {
        for (Face2* f : _faces) {
            delete f;
        }
    }
    
    void vertex_data(std::vector<Vector3>& vertices) const {
        vertices.resize(_faces.size() * 3);
        for (int i = 0; i < _faces.size(); ++i) {
            vertices[i * 3 + 0] = Vector3(_faces[i]->p[0]->l[0], 0.0f, _faces[i]->p[0]->l[1]);
            vertices[i * 3 + 1] = Vector3(_faces[i]->p[1]->l[0], 0.0f, _faces[i]->p[1]->l[1]);
            vertices[i * 3 + 2] = Vector3(_faces[i]->p[2]->l[0], 0.0f, _faces[i]->p[2]->l[1]);
        }
    }
    
    std::vector<Face2*> const& faces() const { return _faces; }
};

class VoronoiDiagram {
    std::vector<VoronoiCell2*> _cells;
    
public:
    VoronoiDiagram(float width, float height, std::vector<Point2*> const& points) {
        DelaunayTriangulation dt(points);
        std::map<Point2*, std::set<Point2*>> neighbours;
        for (Face2* f : dt.faces()) {
            for (int i = 0; i < 3; ++i) {
                Point2* p0 = f->p[i];
                Point2* p1 = f->p[ccw_next(i)];
                Point2* p2 = f->p[cw_next(i)];
                neighbours[p0].insert(p1);
                neighbours[p0].insert(p2);
            }
        }
        
        std::map<Point2*, VoronoiCell2*> cells;
        for (auto it : neighbours) {
            cells.insert(std::make_pair(it.first, new VoronoiCell2(it.first)));
        }
        
        for (auto it : cells) {
            _cells.push_back(it.second);
            for (Point2* n : neighbours[it.first]) {
                _cells.back()->n.push_back(cells[n]);
            }
        }
    }
    ~VoronoiDiagram() {
        for (VoronoiCell2* c : _cells) {
            delete c;
        }
    }
    
    std::vector<VoronoiCell2*> const& cells() const { return _cells; }
};

#endif /* defined(__LD29__DelaunayTriangulation__) */
