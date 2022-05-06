#include <cmath>
#include <climits> 
#include <ctime>
#include <chrono>
#include <random>
#include <iostream>
#include <algorithm>
#include <stdlib.h> 
#include <cstdlib>
#include <stdio.h>
#include <omp.h>

#include "simulation.h"
#include "agent.h"
#include "food.h"
#include "SDL.h"
#include "utils/vec2.h"

#define TICKS_PER_SECOND 300.0
#define CLOCKS_PER_TICK (clock_t)(CLOCKS_PER_SEC / (TICKS_PER_SECOND))
#define NUM_AGENTS 500
#define NUM_FOOD 700

void Simulation::init(int num_threads) {
    // Set numthreads
    omp_set_num_threads(num_threads);

    if (!isRender) return;

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
        // Setting simulation parameters
        window = win;
        renderer = rend;

        // Set renderer to blank canvas
        SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }
}

// Runs one iteration of the simulation (all agents make one step)
// Agent decision process: (might want to add running from larger agents) (could add randomness as trait to decide where to go)
// 1) Check for food or smaller agents nearby. If so then take step towards one of them (must decide priority)
// 2) If no food/smaller agent is found, then move randomly based on speed
void Simulation::update() {
    // Runtime calculations
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    #pragma omp parallel for schedule(static)
    for(int i = 0; i < agents.size(); i++) {
        auto agent = agents[i];
        if (agent->energy <= 0.) continue;

        // auto foodComp = [&](Food* a, Food* b) {
        //     if(!a) return b;
        //     if(!b) return a;
        //     double adist = (a->pos - agent->pos).l2();
        //     double bdist = (b->pos - agent->pos).l2();
        //     return adist < bdist ? a : b;
        // };

        // #pragma omp declare reduction (                      \
        //                                 foodRed :            \
        //                                 Food*:omp_out=foodComp(omp_out,omp_in)) initializer (omp_priv=omp_orig)

        // Food* (*cf)(Food*)(Food*)= &Agent::foodComp;

        // #pragma omp declare reduction(                                           \  
        //                                 foodRed :                                \
        //                                 Food* :                                  \
        //                                 cf(omp_out, omp_in)                      \
        //                              )                                           \
        //                         initializer (omp_priv=omp_orig)


        // get closest uneaten food
        auto t1 = high_resolution_clock::now();
        Food* closest_food = nullptr;
        double food_dist = INT_MAX;
        // #pragma omp parallel for reduction (foodRed : closest_food)
        for(int j = 0; j < food.size(); j++) {
            auto cfood = food[j];
            if (!cfood->eaten && (cfood->pos - agent->pos).l2() < food_dist) {
                closest_food = cfood;
                food_dist = (cfood->pos - agent->pos).l2();
            }
        }

        auto t2 = high_resolution_clock::now();
        duration<double, std::milli> f_time = t2 - t1;
        closestFTime += f_time.count();

        // eat food if in range
        if(closest_food && (closest_food->pos - agent->pos).l2() < agent->size + 2) {
            #pragma omp critical
            {agent->eatFood(closest_food);}
        }


        // get closest living agent
        auto t3 = high_resolution_clock::now();

        Agent *closest_agent = nullptr;
        double agent_dist = INT_MAX;
        for(auto ag : agents) {
            if(ag == agent) continue;
            if(ag->energy >= 0 && (ag->pos - agent->pos).l2() < agent_dist) {
                closest_agent = ag;
                agent_dist = (ag->pos - agent->pos).l2();
            }
        }

        auto t4 = high_resolution_clock::now();
        duration<double, std::milli> a_time = t4 - t3;
        closestATime += a_time.count();

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
        
        // out of bounds check & scroll over
        if(agent->pos.x < 0) {
            agent->newPos.x = width - 1;
        }
        if(agent->pos.x >= width) { 
            agent->newPos.x = 0;
        }
        if(agent->pos.y < 0) {
            agent->newPos.y = height - 1;
        }
        if(agent->pos.y >= height) {
            agent->newPos.y = 0;
        }
    }

    #pragma omp parallel for schedule(static)
    for(int i = 0; i < agents.size(); i++) {
        auto agent = agents[i];
        agent->pos = agent->newPos;
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    // Use agent color
    if(agents.size()) {
        auto a = agents[0];
        SDL_SetRenderDrawColor(renderer, a->color.r, a->color.g, a->color.b, a->color.a);
        for (int i = 0; i < agents.size(); i++) {
            agents[i]->render(renderer);
        }
    }
    
    // Use food color
    if(food.size()) {
        auto f = food[0];
        SDL_SetRenderDrawColor(renderer, f->color.r, f->color.g, f->color.b, f->color.a);
        for (int i = 0; i < food.size(); i++) {
            food[i]->render(renderer);
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
        if (!isRender || clock() > time) {
            time += CLOCKS_PER_TICK;
            update();
            if(agents.size() == 0) return;
            if (isRender) render();
            stepsTaken++;
        }
    }
}

void Simulation::destroy() {
    if (!isRender) return;
    SDL_Delay(5000);
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

    #pragma omp parallel for schedule(static)
    for(int i = 0; i < agents.size(); i++) {
        agents[i]->setPosition(Wdistr(gen), Hdistr(gen));
    }
}

void Simulation::resetFood() {

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    // the range on this is inclusive on both ends
    std::uniform_int_distribution<> Hdistr(0, width - 1); 
    std::uniform_int_distribution<> Wdistr(0, height - 1);

    #pragma omp parallel for schedule(static)
    for(int i = 0; i < food.size(); i++) {
        food[i]->setPosition(Wdistr(gen), Hdistr(gen));
    }
}

// currently no genetic algorithm, will simply propogate agents that survive, 
// with a chance of mutation for their offspring. 
void Simulation::finishRound() {
    // Remove the agents which died (ran out of energy)
    #pragma omp parallel for schedule(static)
    for(int i = 0; i < agents.size(); i++) {
        if(agents[i]->energy <= 0) {
            delete agents[i];
            agents[i] = nullptr;
        } else {
            // Increase numRounds
            agents[i]->numRounds++;
        }
    }

    auto it = std::remove_if(agents.begin(), agents.end(), [](const Agent* agent) { return !agent; });
    agents.erase(it, agents.end());
    int agentsSize = agents.size();

    // Reserve twice the remaining amount (each living agent will duplicate)
    agents.reserve(2 * agents.size());

    // Create new agents, and reset old agent values such as energy
    if(allowCrossover && agents.size() > 1) {
        // compute fitness scores
        #pragma omp parallel for schedule(static) 
        for(int i = 0; i < agents.size(); i++) {
            agents[i]->computeFitnessScore();
        }
        
        // Number of agents that will crossover and reproduce with another agent
        int numFittestParents = (int) agents.size() / 4;
        std::vector<bool> visited(agents.size(), false);
        std::sort(agents.begin(), agents.end());
        int startInd = agents.size() - 1;
        int endInd = std::max(1, startInd - numFittestParents);
        // If not, remove private(seed)
        unsigned seed;
        #pragma omp parallel for schedule(static) private(seed) 
        for (int i = startInd; i >= endInd; i--) {
            // Move onto next agent if we have already procreated with this agent
            if (visited[i]) continue;
            // Mark agent as visited
            visited[i] = 1;
            
            // Check if all other agents have already procreated
            int allVisited = true;
            for(int k = 0; k <= startInd; k++) {
                allVisited = allVisited && visited[k];
            }
            if (allVisited) continue;

            // Generate random number
            std::random_device rd; 
            std::mt19937 gen(rd()); 
            std::uniform_int_distribution<> d(0, i-1);

            // Get agent that hasn't been visited 
            #pragma omp critical
            {int j;
            while (true) {
                j = d(gen);
                if (!visited[j]) break; 
            }

            // Perform crossover and child to agents vector
            Agent* child = agents[i]->crossover(agents[j]);
            agents.push_back(child);
        
            // Mark j as visited
            visited[j] = true;}
        }

    } else {
        #pragma omp parallel for schedule(static)
        for(int i = 0; i < agentsSize; i++) {

            // Get child of current agent, add to agent list
            Agent* child = agents[i]->makeChild();
            #pragma omp critical
            {agents.push_back(child);}
        }
    }

    if (agents.size() > NUM_AGENTS) {
        for(int i = NUM_AGENTS; i < agents.size(); i++) {
            delete agents[i];
            agents[i] = nullptr;
        }
        agents.resize(NUM_AGENTS);

    }
    for(int i = agents.size(); i < NUM_AGENTS; i++) {
        Agent* child = new Agent(SIZE, VISION, SPEED, 0, 0);
        agents.push_back(child);
    }

    // Mutate newly created agents and reset energy
    #pragma omp parallel for schedule(static)
    for(int i = agentsSize; i < agents.size(); i++) {
        agents[i]->mutate();
        agents[i]->resetEnergy();
    }

    // reset agent positions
    repositionAgents();

    // reset food positions & eaten status
    resetFood();
}
