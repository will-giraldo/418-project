#pragma once

#include <ctime>

#include "simulation.h"
#include "agent.h"
#include "util/vec2.h"
#include "food.h"

#define TICKS_PER_SECOND 30.0
#define CLOCKS_PER_TICK 1.0 / TICKS_PER_SECOND * CLOCKS_PER_SECOND


// Runs one iteration of the simulation (all agents make one step)
// Agent decision process: (might want to add running from larger agents) (could add randomness as trait to decide where to go)
// 1) Check for food or smaller agents nearby. If so then take step towards one of them (must decide priority)
// 2) If no food/smaller agent is found, then move randomly based on speed
void Simulation::update() {
    // TODO update/change energy, add consideration in for speed
    // TODO make sure that all the movements and considerations of other agents is based off their old positions when parallelizing? 
    // This may need to be true even for the sequential version as well. (it does not currently account for this).
    for(auto agent : agents) {
        if(agent->health < 0 || agent->energy <= 0.) continue;

        // get closest uneaten food
        Food closest_food;
        double food_dist = INT_MAX;
        for(auto cfood : food) {
            if (!cfood->eaten && (cfood->pos - agent->pos).l2() < dist) {
                closest_food = cfood;
                dist = (cfood - agent->pos).l2();
            }
        }

        // eat food if in range
        if((closest_food - agent->pos).l2() < agent->size) {
            closest_food->eaten = true;
            agent->health++;

            continue;
        }


        // get closest living agent
        Agent *closest_agent;
        double agent_dist = INT_MAX;
        for(auto ag : agents) {
            if(ag->health >= 0 && (ag->pos - agent->pos).l2() < agent_dist) {
                closest_agent = ag;
                agent_dist = (ag->pos - agent->pos).l2();
            }
        }

        // eat closest agent if possible
        if(agent->canEat(closest_agent)) {
            closest_agent->health = -1;
            agent->health++;

            continue;
        } 
        
        // if closest agent can eat us then run
        if(closest_agent->canEat(agent)) {
            Vec2 dir = agent->pos - closest_agent->pos;
            agent->moveDir(dir.toDir());
        }
        // if in vision range move towards closer target
        else if(closest_agent->pos.l1() <= agent->vision || closest_food.l1() <= agent->vision) {
            if(agent_dist < food_dist) {
                // move towards the agent
                Vec2 dir = (closest_agent->pos - agent->pos).toDir();
                agent->moveDir(dir);
            } else {
                // move towards the food
                Vec2 dir = (closest_food - agent->pos).toDir();
                agent->moveDir(dir);
            }
        } else {
            // move in a random direction
            agent->moveDir(agent->randomNextWeightedDir());
        }
    }

    // out of bounds check & scroll over
    if(agent->pos.x < 0) {
        agent->pos.x = width - 1;
    }
    if(agent->pos.x >= width) {
        agent->pos.x = 0;
    }
    if(agent->pos.y < 0) {
        agent->pos.y = height - 1;
    }
    if(agent->pos.y >= height) {
        agent->pos.y = 0;
    }

    
}

void Simulation::render(Image &I) {
    for (auto a : agents) {
        a->drawAgent(I);
    }

    for (auto f : food) {
        I.drawCircle(f, foodRadius, foodColor);
    }

    I.exportFile("/out");
    I.clearImage();
}

void Simulation::runRound(int steps) {
    clock_t time;
    time = clock();

    int stepsTaken = 0;
    // execute loop code TICKS_PER_SECOND times per second
    while(stepsTaken < steps) {
        if (clock() > time) {
            time += CLOCKS_PER_TICK;
            update();
            render();
            stepsTaken++;
        }
    }
}

