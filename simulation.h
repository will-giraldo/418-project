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


#define SIZE 4
#define VISION 150
#define SPEED 2
#define FOOD_VALUE 30 // amount of energy which food replenishes when eaten


class Simulation {
public:
    std::vector<Agent*> agents;
    std::vector<Food*> food;
    int width;
    int height;
    bool isRender;
    bool allowCrossover = true;
    SDL_Window* window;
    SDL_Renderer* renderer; 

    Simulation(int numAgents, int numFood, int _width, int _height, bool _isRender=true);

    // Class functions
    // Initialize SDL renderer
    void init(int num_threads);

    // Update agent parameters based on position change and food consumption
    void update();

    // Run a single round for a certain number of steps, and render (does not reset field)
    void runRound(int steps);

    // Renders agents and food
    void render();

    // Prepares stage and agents for next round
    void finishRound();

    // Randomly re-distributes agent positions in grid
    void repositionAgents();

    // Randomly re-distributes food positions in grid
    void resetFood();

    // Destroys SDL renderer/window
    void destroy();

    // Old renderer
    Image I;
    void render1(Image &I);

    // For runtime 
    double closestATime;
    double closestFTime;
};

Simulation::Simulation(int numAgents, int numFood, int _width, int _height, bool _isRender) {
    width = _width;
    height = _height;
    agents = std::vector<Agent*>(numAgents);
    food = std::vector<Food*>(numFood);
    isRender = _isRender;

    // random number code from https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    // these are inclusive ranges [0, height/width]
    std::uniform_int_distribution<> Wdistr(0, width - 1); 
    std::uniform_int_distribution<> Hdistr(0, height - 1); 


    for(int i = 0; i < numAgents; i++) {
        Agent *ag = new Agent(SIZE, VISION, SPEED, Wdistr(gen), Hdistr(gen));
        agents[i] = ag;
    }

    for(int i = 0; i < numFood; i++) {
        Food *fd = new Food(Vec2(Wdistr(gen), Hdistr(gen)), FOOD_VALUE);
        food[i] = fd;
    }

    closestATime = 0.;
    closestFTime = 0.;
}