#pragma once

#include <ctime>

#include "simulation.h"
#include "agent.h"
#include "vec2.h"

#define TICKS_PER_SECOND 30.0
#define CLOCKS_PER_TICK 1.0 / TICKS_PER_SECOND * CLOCKS_PER_SECOND


// Runs one iteration of the simulation (all agents make one step)
// Agent decision process: (might want to add running from larger agents) (could add randomness as trait to decide where to go)
// 1) Check for food or smaller agents nearby. If so then take step towards one of them (must decide priority)
// 2) If no food/smaller agent is found, then move randomly based on speed
void update() {

    for(Agent cur : agents) {

        // get closest food
        Vec2 closest_food = Vec2::Vec2(-1, -1);
        double food_dist = INT_MAX;
        for(auto cfood : food) {
            if ((cfood - cur.pos).l2() < dist) {
                closest_food = cfood;
                dist = (cfood - cur.pos).l2();
            }
        }

        // get closest agent
        Agent closest_agent;
        double agent_dist = INT_MAX;
        for(auto ag : agents) {
            if((ag.pos - cur.pos).l2() < agent_dist) {
                closest_agent = ag;
                agent_dist = (ag.pos - cur.pos).l2();
            }
        }

        // if in vision range move towards closer target
        if(closest_agent.pos.l1() <= cur.vision || closest_food.l1() <= cur.vision) {
            if(agent_dist < food_dist) {
                // move towards the agent
                Vec2 dir = (closest_agent.pos - cur.pos).toDir();
                cur.moveDir(dir);
            } else {
                // move towards the food
                Vec2 dir = (closest_food - cur.pos).toDir();
                cur.moveDir(dir);
            }
        } else {
            // move in a random direction
            cur.moveDir(cur.randomNextWeightedDir());
        }
    }

}

void render() {

}

void run() {
    clock_t time;
    time = clock();
    // execute loop code TICKS_PER_SECOND times per second
    while(true) {
        if (clock() > time) {
            time += CLOCKS_PER_TICK;
            update();
            render();
        }
    }
}
