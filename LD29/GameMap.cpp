//
//  GameMap.cpp
//  LD29
//
//  Created by Kristof Niederholtmeyer on 30.04.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//

#include "GameMap.h"
#include "DelaunayTriangulation.h"
#include <map>
#include <random>

bool neighbours(Tile* t0, Tile* t1) {
    return std::find(t0->neighbours.begin(), t0->neighbours.end(), t1) != t0->neighbours.end();
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

bool valid_cell(VoronoiCell2* c, float cutoff) {
    if (c->p->l[0] < -cutoff || c->p->l[0] > cutoff || c->p->l[1] < -cutoff || c->p->l[1] > cutoff) {
        return false;
    }
    return true;
}

GameMap::GameMap() {
    std::mt19937 rand_engine;
    
    float const size = 7.5f;
    
    std::vector<Vector2> points;
    
    std::uniform_real_distribution<float> dist(-size*0.5f,size*0.5f);
    for (int i = 0; i < 200; ++i) {
        points.push_back(Vector2(dist(rand_engine), dist(rand_engine)));
    }
    for (int i = 0; i < 10; ++i) {
        points = smooth(size, size, points);
    }
    std::vector<Point2*> ppoints;
    for (Vector2 const& p : points) {
        ppoints.push_back(new Point2{p});
    }
    
    {
        VoronoiDiagram vd(size, size, ppoints);
        
        // Generate a map:
        float cutoff = 0.5f * size - 0.5f;
        
        // 1.) find a valid seed cell
        std::set<VoronoiCell2*> map;
        for (VoronoiCell2* c : vd.cells()) {
            if (valid_cell(c, cutoff)) {
                map.insert(c);
                break;
            }
        }
        
        // 2.) expand the map starting from the seed. All cells have to be connected.
        for (int i = 0; i < 100; ++i) {
            bool found = false;
            for (VoronoiCell2* c : vd.cells()) {
                if (valid_cell(c, cutoff) && map.find(c) == map.end()) {
                    for (VoronoiCell2* m : map) {
                        if (std::find(m->n.begin(), m->n.end(), c) != m->n.end()) {
                            found = true;
                            map.insert(c);
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
        
        // 3.) Generate tiles
        std::map<VoronoiCell2*, Tile*> cell_tiles;
        for (VoronoiCell2* cell : map) {
            _tiles.push_back(new Tile());
            cell_tiles.insert(std::make_pair(cell, _tiles.back()));
            _tiles.back()->center = Vector3(cell->p->l[0], 0.0f, cell->p->l[1]);
            _tiles.back()->shape = voronoi_cell_mesh(size, size, cell);
        }
        
        // 4.) Connect tiles
        for (auto it : cell_tiles) {
            Tile* tile = it.second;
            VoronoiCell2* cell = it.first;
            for (VoronoiCell2* neighbour : cell->n) {
                if (map.find(neighbour) != map.end()) {
                    tile->neighbours.push_back(cell_tiles[neighbour]);
                }
            }
        }
    }
    
    // free the points
    for (Point2* p : ppoints) {
        delete p;
    }
}

GameMap::~GameMap() {
    
}

std::vector<Tile*> const& GameMap::tiles() const {
    return _tiles;
}