#pragma once
#define ENERGY 10

class Agent {
public:
    double size;
    double vision;
    double energy;
    int speed;
    int x;
    int y;
    int old_x;
    int old_y;   
}

Agent::Agent(double _size, double _vision, int _speed, int _x, int _y) {
    size = _size;
    vision = _vision;
    speed = _speed;
    x = _x;
    y = _y;
    old_x = x;
    old_y = y;

    energy = ENERGY;
}