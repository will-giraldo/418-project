#pragma once

#include <vector>
#include <random>
#include <utility>

#include "agent.h"
#include "./utils/image.h"
#include "./utils/vec2.h"
#include "food.h"

#define SIZE 4.0
#define VISION 5
#define SPEED 1
#define FOOD_VALUE 8 // amount of energy which food replenishes when eaten


class Simulation {
public:
    std::vector<Agent*> agents;
    std::vector<Food*> food;
    int width;
    int height;

    Simulation(int numAgents, int numFood, int _width, int _height);

    // run a single round for a certain number of steps, and render (does not reset field)
    void runRound(int steps);

    // prepares stage and agents for next round
    void finishRound();

    // randomly re-distributes agent positions in grid
    void repositionAgents();

    // randomly re-distributes food positions in grid
    void resetFood();

    Image I;
    Color foodColor = Color(100, 237, 132);
    int foodRadius = 1;

    void update();
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
    }

    for(int i = 0; i < numFood; i++) {
        Food fd = Food(Vec2(Wdistr(gen), Hdistr(gen)), FOOD_VALUE);
        food[i] = &fd;
    }

}