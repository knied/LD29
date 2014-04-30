//
//  GameMap.h
//  LD29
//
//  Created by Kristof Niederholtmeyer on 30.04.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//

#ifndef __LD29__GameMap__
#define __LD29__GameMap__

#include <iostream>
#include <vector>
#include "Types.h"
#include "GameDraw.h"

struct Tile {
    Vector3 center;
    GLShape shape;
    std::vector<Tile*> neighbours;
    int building = 0;
    int coins = 0;
    int spawn = 0;
    int kingdom = -1;
};

bool neighbours(Tile* t0, Tile* t1);

class GameMap {
    std::vector<Tile*> _tiles;
    
public:
    GameMap();
    ~GameMap();
    
    std::vector<Tile*> const& tiles() const;
};

#endif /* defined(__LD29__GameMap__) */
