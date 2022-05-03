#pragma once

#include <random>
#include <cmath>
#include <iostream>

#include "SDL.h"
#include "food.h"
#include "./utils/image.h"
#include "./utils/vec2.h"

#define ENERGY 10
#define EAT_THRESHOLD 1.2
#define MUTATION_CHANCE .1
#define VAR_MOD .75 // modifier to get the variance for distribution
#define OLD_PROB .4 // probability that we move in the same direction as last movement


class Agent {
public:
    int size;
    double energy;
    int vision;
    int speed;
    Vec2 pos;
    Vec2 oldPos; 

    Agent() {}
    Agent(double _size, int _vision, int _speed, int _x, int _y);

    void reduceEnergy();
    bool canEat(Agent* agent);

    void drawAgent(Image &I);
    int render(SDL_Renderer* renderer);
    
    void eatFood(Food* food);
    void eatAgent(Agent* ag);
    void resetEnergy();
    Agent makeChild();
    void setPosition(int _x, int _y);


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

    Color color = Color(100, 150, 237, 255);

};

Agent::Agent(double _size, int _vision, int _speed, int _x, int _y) {
    // TODO maybe change initialization to be random over some range for these values
    size = _size;
    vision = _vision;
    speed = _speed;
    pos = Vec2(_x, _y);
    oldPos = Vec2(_x, _y);

    energy = ENERGY;
}

bool Agent::canEat(Agent* agent) {
    return agent->energy >= 0 && size > EAT_THRESHOLD * agent->size && (pos - agent->pos).l2() < size;
}

void Agent::reduceEnergy() {
    // TODO this will need tinkering to remove an appropriate amount of energy
    energy -= (std::pow(size, 3.) * std::pow(speed, 2.) + ((double) vision)) / ENERGY;
}

void Agent::eatFood(Food* food) {
    if(food->eaten) std::cout << "Agent tried to eat food which was already eaten\n";
    energy += food->value;
    food->eaten = true;
}

void Agent::eatAgent(Agent* ag) {
    if(ag->energy < 0) std::cout << "Tried to eat dead agent\n";
    energy += ag->energy; // TODO how much energy should an agent get for eating another
    ag->energy = -1;
}

// TODO should this function also reset the position of all the agents to the edges
void Agent::resetEnergy() {
    energy = ENERGY;
}

Agent Agent::makeChild() {
    Agent child(size, vision, speed, pos.x, pos.y);

    // randomly mutate parameters
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_real_distribution<> dis(0.0,1.0);

    if(dis(gen) < MUTATION_CHANCE) {
        // mutate size
        std::normal_distribution<> d(size, 3.); //TODO is this variance appropriate?
        child.size = abs(d(gen));
    }
    if(dis(gen) < MUTATION_CHANCE) {
        // mutate speed
        std::normal_distribution<> d(speed, 3.); //TODO is this variance appropriate?
        child.speed = (int) std::round(abs(d(gen)));
    }
    if(dis(gen) < MUTATION_CHANCE) {
        // mutate vision
        std::normal_distribution<> d(vision, 3.); //TODO is this variance appropriate?
        child.vision = (int) std::round(abs(d(gen)));
    }

    return child;

}

void Agent::setPosition(int _x, int _y) {
    Vec2 _pos(_x, _y);
    pos = _pos;
    oldPos = pos; // want these to be the same since the context for this is to change the position entirely
}

void Agent::drawAgent(Image &I) {

}

int Agent::render(SDL_Renderer* renderer) {
    SDL_Rect rect;
    rect.x = pos.x;
    rect.y = pos.y;
    rect.w = size * 5;
    rect.h = size * 5;
    SDL_RenderFillRect(renderer, &rect);

    return 0;
    // int x = pos.x;
    // int y = pos.y;
    // int offsetx, offsety, d;
    // int status;

    // // CHECK_RENDERER_MAGIC(renderer, -1);

    // offsetx = 0;
    // offsety = size;
    // d = size -1;
    // status = 0;

    // while (offsety >= offsetx) {
    //     status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
    //     status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
    //     status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
    //     status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
    //     status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
    //     status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
    //     status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
    //     status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

    //     if (status < 0) {
    //         status = -1;
    //         break;
    //     }

    //     if (d >= 2*offsetx) {
    //         d -= 2*offsetx + 1;
    //         offsetx +=1;
    //     }
    //     else if (d < 2 * (size - offsety)) {
    //         d += 2 * offsety - 1;
    //         offsety -= 1;
    //     }
    //     else {
    //         d += 2 * (offsety - offsetx - 1);
    //         offsety -= 1;
    //         offsetx += 1;
    //     }
    // }

    // return status;
}