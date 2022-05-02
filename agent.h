#pragma once

#include <random>
#include <cmath>

#include "./utils/image.h"
#include "./utils/vec2.h"

#define ENERGY 10
#define EAT_THRESHOLD 1.2


class Agent {
public:
    double size;
    double energy;
    int vision;
    int speed;
    int health;
    Vec2 pos;
    Vec2 oldPos; 

    Agent() {}
    Agent(double _size, int _vision, int _speed, int _x, int _y);
    void reduceEnergy();
    bool canEat(Agent* agent);
    void drawAgent(Image &I);


    // use these function to update position to ensure that oldPos is always properly updated
    void updatePos(Vec2 newPos) {
        oldPos = pos;
        pos = newPos;
    }
    void moveDir(Vec2 dir) {
        oldPos = pos;
        pos = pos + (dir.toDir()) * speed;
        reduceEnergy();
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
            int _x = (int) round(ndis(gen));
            int _y = (int) round(ndis(gen));
            return Vec2(_x, _y);
        }
    }

    Color agentColor = Color(100, 150, 237);

};

Agent::Agent(double _size, int _vision, int _speed, int _x, int _y) {
    size = _size;
    vision = _vision;
    speed = _speed;
    pos = Vec2(_x, _y);
    oldPos = Vec2(_x, _y);

    health = 0;
    energy = ENERGY;
}

bool Agent::canEat(Agent* agent) {
    return size > EAT_THRESHOLD * agent->size && (pos - agent->pos).l2() < size;
}

void Agent::reduceEnergy() {
    // TODO this will need tinkering to remove an appropriate amount of energy
    energy -= (std::pow(size, 3.) * std::pow(speed, 2.) + ((double) vision)) / ENERGY;

    // TODO should it set health below 0 so that other agents wont eat it if energy < 0?
    if(energy <= 0) {
        health = -1;
    }
}

void Agent::drawAgent(Image &I) {

}