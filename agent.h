#pragma once

#include <random>

#include "image.h"
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

    Agent() {}
    Agent(double _size, int _vision, int _speed, int _x, int _y);

    // use these function to update position to ensure that oldPos is always properly updated
    void updatePos(Vec2 newPos) {
        oldPos = pos;
        pos = newPos;
    }
    void moveDir(Vec2 dir) {
        oldPos = pos;
        pos = pos + dir;
    }

    // this is a long name :(
    Vec2 randomNextWeightedDir() {
        std::random_device rd; 
        std::mt19937 gen(rd()); 
        std::uniform_real_distribution<> dis(0.0,1.0);
        std::uniform_real_distribution<> ndis(-1.0,1.0);

        if(dis(gen) < OLD_PROB) {
            return (pos - oldPos).toDir();
        } else {
            int _x = round(ndis(gen));
            int _y = round(ndis(gen));
            return Vec2(_x, _y);
        }
    }

    Color agentColor = Color(100, 150, 237);
    void drawAgent(Image &I) {};

};

Agent::Agent(double _size, int _vision, int _speed, int _x, int _y) {
    size = _size;
    vision = _vision;
    speed = _speed;
    pos = Vec2(_x, _y);
    oldPos = Vec2(_x, _y);

    energy = ENERGY;
}

void Agent::drawAgent(Image &I) {

}