#pragma once

#include <iostream>

#include "simulation.cpp"
#include "SDL.h"


int main(int argc, char* argv[]) {
    // Initialize simulation
    int numAgents = 100;
    int numFood = 50;
    int dim = 800;
    Simulation sim(numAgents, numFood, dim, dim);

    // Run simulation
    int numRounds = 20;
    int numSteps = 100;
    sim.init();
    if(sim.window == nullptr) std::cout << "HERE";
    if(sim.renderer == nullptr) std::cout << "HERE2";
    for(int round = 0; round < numRounds; round++) {
        sim.runRound(numSteps);
    }

    sim.destroy();

    // SDL_Init(SDL_INIT_VIDEO);

    // SDL_Window *window = SDL_CreateWindow(
    //     "SDL2Test",
    //     SDL_WINDOWPOS_UNDEFINED,
    //     SDL_WINDOWPOS_UNDEFINED,
    //     640,
    //     480,
    //     0
    // );

    // SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    // SDL_RenderClear(renderer);
    // SDL_RenderPresent(renderer);

    // SDL_Delay(3000);

    // SDL_DestroyWindow(window);
    // SDL_Quit();

    return 0;

}