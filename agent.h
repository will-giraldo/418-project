#pragma once

#include "vec2.h"

#define ENERGY 10


class Agent {
public:
    double size;
    double energy;
    int vision;
    int speed;
    Vec2 pos;
    Vec2 oldPos; 
}

Agent::Agent(double _size, int _vision, int _speed, int _x, int _y) {
    size = _size;
    vision = _vision;
    speed = _speed;
    pos = Vec2::Vec2(_x, _y);
    oldPos = Vec2::Vec2(_x, _y);

    energy = ENERGY;
}