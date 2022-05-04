#pragma once

#include <ctime>
#include <random>
#include <algorithm>

#include "simulation.h"
#include "agent.h"
#include "food.h"
#include "SDL.h"
#include "utils/vec2.h"

#define TICKS_PER_SECOND 300.0
// #define CLOCKS_PER_TICK 1.0 / (TICKS_PER_SECOND * CLOCKS_PER_SEC)
#define CLOCKS_PER_TICK (clock_t)(CLOCKS_PER_SEC / (TICKS_PER_SECOND))


void Simulation::init() {
    SDL_Init(SDL_INIT_VIDEO);

    // Create window
    SDL_Window *win = SDL_CreateWindow("Natural Selection",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          width,
                                          height,
                                          0);
    if(!win) {
        printf("Window could not be created!\n"
               "SDL_Error: %s\n", SDL_GetError());
    }
    else {
        // Create renderer
        SDL_Renderer *rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
        if(!rend) {
            printf("Renderer could not be created!\n"
                   "SDL_Error: %s\n", SDL_GetError());
        }
        window = win;
        renderer = rend;
        // Set renderer to blank canvas
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }
}

// Runs one iteration of the simulation (all agents make one step)
// Agent decision process: (might want to add running from larger agents) (could add randomness as trait to decide where to go)
// 1) Check for food or smaller agents nearby. If so then take step towards one of them (must decide priority)
// 2) If no food/smaller agent is found, then move randomly based on speed
void Simulation::update() {
    // TODO update/change energy, add consideration in for speed
    // TODO make sure that all the movements and considerations of other agents is based off their old positions when parallelizing? 
    // This may need to be true even for the sequential version as well. (it does not currently account for this).
    for(auto agent : agents) {
        if (agent->energy <= 0.) continue;

        // get closest uneaten food
        Food* closest_food = nullptr;
        double food_dist = INT_MAX;
        for(auto cfood : food) {
            if (!cfood->eaten && (cfood->pos - agent->pos).l2() < food_dist) {
                closest_food = cfood;
                food_dist = (cfood->pos - agent->pos).l2();
            }
        }

        // eat food if in range
        if(closest_food && (closest_food->pos - agent->pos).l2() < agent->size + 2) {
            agent->eatFood(closest_food);
        }


        // get closest living agent
        Agent *closest_agent = nullptr;
        double agent_dist = INT_MAX;
        for(auto ag : agents) {
            if(ag->pos == agent->pos) continue;
            if(ag->energy >= 0 && (ag->pos - agent->pos).l2() < agent_dist) {
                closest_agent = ag;
                agent_dist = (ag->pos - agent->pos).l2();
            }
        }

        // eat closest agent if possible
        if(closest_agent && agent->canEat(closest_agent)) {
            agent->eatAgent(closest_agent);

            continue;
        } 
        
        // if closest agent can eat us then run
        if(closest_agent && closest_agent->canEat(agent)) {
            Vec2 dir = agent->pos - closest_agent->pos;
            agent->moveDir(dir.toDir());
        }
        // if in vision range move towards closer target
        else if(closest_agent && (closest_agent->pos - agent->pos).l2() <= agent->vision && agent->canEat(closest_agent)) {
            // move towards the agent
            Vec2 dir = (closest_agent->pos - agent->pos).toDir();
            agent->moveDir(dir);
        } else if (closest_food && (closest_food->pos - agent->pos).l2() <= agent->vision) {
            // move towards the food
            Vec2 dir = (closest_food->pos - agent->pos).toDir();
            agent->moveDir(dir);
        } else {
            // move in a random direction
            Vec2 dir = agent->randomNextWeightedDir();
            agent->moveDir(dir);
        }

        // if(closest_food && (closest_food->pos - agent->pos).l2() <= agent->vision) {
        //      if(closest_food) {
        //         // move towards the food
        //         Vec2 dir = (closest_food->pos - agent->pos).toDir();
        //         agent->moveDir(dir);
        //     }
        // } else {
        //     // move in a random direction
        //     Vec2 dir = agent->randomNextWeightedDir();
        //     agent->moveDir(dir);
        // }
        
        // out of bounds check & scroll over
        if(agent->pos.x < 0) {
            agent->pos.x = width - 1;
            agent->oldPos = agent->pos;
        }
        if(agent->pos.x >= width) { 
            agent->pos.x = 0;
            agent->oldPos = agent->pos;
        }
        if(agent->pos.y < 0) {
            agent->pos.y = height - 1;
            agent->oldPos = agent->pos;
        }
        if(agent->pos.y >= height) {
            agent->pos.y = 0;
            agent->oldPos = agent->pos;
        }
    }    
}

void Simulation::render1(Image &I) {
    for (auto a : agents) {
        a->drawAgent(I);
    }

    for (auto f : food) {
        I.drawCircle(f->pos, f->radius, f->color);
    }

    I.exportFile("/out");
    I.clearImage();
}

void Simulation::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    // Use agent color
    if(agents.size()) {
        auto a = agents[0];
        SDL_SetRenderDrawColor(renderer, a->color.r, a->color.g, a->color.b, a->color.a);
        for (auto a : agents) {
            a->render(renderer);
        }
    }
    
    // Use food color
    if(food.size()) {
        auto f = food[0];
        SDL_SetRenderDrawColor(renderer, f->color.r, f->color.g, f->color.b, f->color.a);
        for (auto f : food) {
            f->render(renderer);
        }
    }

    // Update screen
    SDL_RenderPresent(renderer);
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
            if(agents.size() == 0) return;
            render();
            stepsTaken++;
        }
    }
}

void Simulation::destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Simulation::repositionAgents() {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    // the range on this is inclusive on both ends
    std::uniform_int_distribution<> Hdistr(0, width - 1); 
    std::uniform_int_distribution<> Wdistr(0, height - 1);

    for(auto ag : agents) {
        ag->setPosition(Wdistr(gen), Hdistr(gen));
    }

}

void Simulation::resetFood() {

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    // the range on this is inclusive on both ends
    std::uniform_int_distribution<> Hdistr(0, width - 1); 
    std::uniform_int_distribution<> Wdistr(0, height - 1);

    for(auto fd : food) {
        fd->setPosition(Wdistr(gen), Hdistr(gen));
    }

}

// currently no genetic algorithm, will simply propogate agents that survive, 
// with a chance of mutation for their offspring. 
void Simulation::finishRound() {

    // remove the agents which died (ran out of energy)
    // TODO because we used new, we need to deallocate deleted agents first with delete. 
    for(int i = 0; i < agents.size(); i++) {
        if(agents[i]->energy <= 0) {
            delete agents[i];
            agents[i] = nullptr;
        }
    }
    auto it = std::remove_if(agents.begin(), agents.end(), [](const Agent* agent) { return !agent; });
    agents.erase(it, agents.end());

    // reserve twice the remaining amount (each living agent will duplicate)
    agents.reserve(2 * agents.size());

    // create new agents, and reset old agent values such as energy
    size_t range = agents.size();
    for(size_t i = 0; i < range; i++) {

        // reset current agents values
        agents[i]->resetEnergy();

        // get child of current agent, add to agent list
        Agent* child = agents[i]->makeChild();
        agents.push_back(child);

    }

    // reset agent positions
    repositionAgents();

    // reset food positions & eaten status
    resetFood();

}

