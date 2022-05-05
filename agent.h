#pragma once

#include <random>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "SDL.h"
#include "food.h"
#include "./utils/image.h"
#include "./utils/vec2.h"

#define ENERGY 15
#define EAT_THRESHOLD 1.2
#define MUTATION_CHANCE .3
#define VAR_MOD .75 // Modifier to get the variance for distribution
#define OLD_PROB .85 // Probability that we move in the same direction as last movement


class Agent {
public:
    int size;
    double energy;
    int vision;
    int speed;
    Vec2 pos;
    Vec2 newPos;
    bool hasEaten;
    Vec2 lastMove;
    double fitness;
    int numRounds;

    Agent() {}
    Agent(int _size, int _vision, int _speed, int _x, int _y);

    // Simulation round functions
    void reduceEnergy();
    bool canEat(Agent* agent);
    void eatFood(Food* food);
    void eatAgent(Agent* ag);
    void resetEnergy();

    // Rendering functions
    void drawAgent(Image &I);
    void render(SDL_Renderer* renderer);
    
    // Genetic functions
    void computeFitnessScore();
    Agent* makeChild();
    Agent* crossover(Agent* b);
    void mutate();
    void setPosition(int _x, int _y);


    // use these function to update position to ensure that oldPos is always properly updated
    void updateNewPos(Vec2 _newPos) {
        newPos = _newPos;
    }
    void moveDir(Vec2 dir) {
        newPos = pos + (dir.toDir()) * speed;
        lastMove = dir;
        reduceEnergy();
    }

    // this is a long name :(
    Vec2 randomNextWeightedDir() {
        std::random_device rd; 
        std::mt19937 gen(rd()); 
        std::uniform_real_distribution<> dis(0.0,1.0);
        std::uniform_int_distribution<> ndis(-1,1);

        if(lastMove != Vec2(0,0) && dis(gen) < OLD_PROB) {
            return lastMove.toDir();
        } else {
            int _x = 0;
            int _y = 0;
            while(_x == 0 && _y == 0) {
                _x = ndis(gen);
                _y = ndis(gen);
            }
            return Vec2(_x, _y);
        }
    }

    Color color = Color(100, 150, 237, 255);

    bool operator<(const Agent& b) {
        return fitness < b.fitness;
    }
};

Agent::Agent(int _size, int _vision, int _speed, int _x, int _y) {
    size = _size;
    vision = _vision;
    speed = _speed;
    pos = Vec2(_x, _y);
    newPos = Vec2(_x, _y);
    lastMove = Vec2();
    numRounds = 0;
    energy = ENERGY;
}

void Agent::setPosition(int _x, int _y) {
    Vec2 _pos(_x, _y);
    pos = _pos;
}

bool Agent::canEat(Agent* agent) {
    return agent->energy > 0 && size > EAT_THRESHOLD * agent->size && (pos - agent->pos).l2() < (size + agent->size);
}

void Agent::reduceEnergy() {
    // TODO this will need tinkering to remove an appropriate amount of energy
    energy -= (std::pow(size, 2.) + std::pow(speed, 2.) + ((double) vision)) / (ENERGY * 200);
}

void Agent::eatFood(Food* food) {
    if(food->eaten) return;
    energy += food->value;
    food->eaten = true;
}

void Agent::eatAgent(Agent* ag) {
    if(ag->energy < 0) std::cout << "Tried to eat dead agent\n";
    energy += .7 * ENERGY + .1 * ag->energy + .2 * ag->size; 
    ag->energy = -1;
}

void Agent::computeFitnessScore() {
    fitness = energy + numRounds;
}

Agent* Agent::makeChild() {
    Agent* child = new Agent(size, vision, speed, pos.x, pos.y);
    return child;
}

// In this version of the genetic algorithm, we will be implementing uniform crossover
Agent* Agent::crossover(Agent* b) {
    int s;
    double v;
    int sp;
    int x, y;

    // Set pos values to 'this'.pos - they will be reset randomly so it doesn't matter
    x = pos.x;
    y = pos.y;

    // Set up RNG 
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_real_distribution<> dis(0.0,1.0);

    // Size
    if (dis(gen) < 0.5) s = size;
    else s = b->size;

    // Vision
    if (dis(gen) < 0.5) v = vision;
    else v = b->vision;

    // Speed
    if (dis(gen) < 0.5) sp = speed;
    else sp = b->speed;

    Agent* child = new Agent(s, v, sp, x, y);
    return child;
}

void Agent::mutate() {
    // randomly mutate parameters
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_real_distribution<> dis(0.0,1.0);
    if(dis(gen) < MUTATION_CHANCE) {
        // Mutate size
        std::uniform_int_distribution<> d(2, 2 * size); 
        size = (int) std::round(abs(d(gen)));
    }
    if(dis(gen) < MUTATION_CHANCE) {
        // Mutate speed
        std::uniform_int_distribution<> d(1, 1.5 * speed); 
        speed = (int) std::round(abs(d(gen)));
    }
    if(dis(gen) < MUTATION_CHANCE) {
        // Mutate vision
        std::uniform_int_distribution<> d(2, 2 * vision); 
        vision = (int) std::round(abs(d(gen)));
    }
}


void Agent::resetEnergy() {
    energy = ENERGY;
}

void Agent::drawAgent(Image &I) {

}

void Agent::render(SDL_Renderer* renderer) {
    if(energy <= 0)  return;

    // Rectangle
    SDL_Rect rect;
    rect.x = pos.x;
    rect.y = pos.y;
    rect.w = size * 5;
    rect.h = size * 5;
    SDL_RenderFillRect(renderer, &rect);

    // return;

    // // Fill circle
    // int x = pos.x;
    // int y = pos.y;
    // int offsetx, offsety, d;
    // int status;
    // int radius, scaleFactor; 
    
    // scaleFactor = 5;
    // radius = scaleFactor * size;
    // offsetx = 0;
    // offsety = radius;
    // d = radius -1;
    // status = 0;

    // while (offsety >= offsetx) {

    //     status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
    //                                  x + offsety, y + offsetx);
    //     status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
    //                                  x + offsetx, y + offsety);
    //     status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
    //                                  x + offsetx, y - offsety);
    //     status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
    //                                  x + offsety, y - offsetx);

    //     if (status < 0) {
    //         status = -1;
    //         break;
    //     }

    //     if (d >= 2*offsetx) {
    //         d -= 2*offsetx + 1;
    //         offsetx +=1;
    //     }
    //     else if (d < 2 * (radius - offsety)) {
    //         d += 2 * offsety - 1;
    //         offsety -= 1;
    //     }
    //     else {
    //         d += 2 * (offsety - offsetx - 1);
    //         offsety -= 1;
    //         offsetx += 1;
    //     }
    // }


    // Circle
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

    // return;
}