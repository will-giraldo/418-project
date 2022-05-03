#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <utility>

#include "agent.h"
#include "food.h"
#include "SDL.h"
#include "./utils/image.h"
#include "./utils/vec2.h"


#define SIZE 4.0
#define VISION 5
#define SPEED 1


class Simulation {
public:
    std::vector<Agent*> agents;
    std::vector<Food*> food;
    int width;
    int height;

    Simulation(int numAgents, int numFood, int _width, int _height);

    // Class functions
    void init();
    void update();
    void runRound(int steps);
    void destroy();
    
    // SDL renderer parameters
    SDL_Window* window;
    SDL_Renderer* renderer; 
    void render2();


    // Old renderer
    Image I;
    void render(Image &I);
};

Simulation::Simulation(int numAgents, int numFood, int _width, int _height) {
    width = _width;
    height = _height;
    agents = std::vector<Agent*>(numAgents);
    food = std::vector<Food*>(numFood);

    // random number code from https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    // these are inclusive ranges [0, height/width]
    std::uniform_int_distribution<> Hdistr(0, width - 1); 
    std::uniform_int_distribution<> Wdistr(0, height - 1); 


    for(int i = 0; i < numAgents; i++) {
        Agent ag = Agent(SIZE, VISION, SPEED, Wdistr(gen), Hdistr(gen));
        agents[i] = &ag;
        if(agents[i] == nullptr) std::cout << "NULL AGENT"; 
    }

    for(int i = 0; i < numFood; i++) {
        Food fd = Food(Vec2(Wdistr(gen), Hdistr(gen)));
        food[i] = &fd;
        if(food[i] == nullptr) std::cout << "NULL FOOD"; 

    }
}