#pragma once

#include <iostream>
#include <chrono>

#include "simulation.h"
#include "simulation.cpp"
#include "SDL.h"

#define WIDTH 700
#define HEIGHT 700
#define NUM_ROUNDS 5000
#define STEPS_PER_ROUND 400


int main(int argc, char* argv[]) {
    // timing setup
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    std::vector<double> round_times(NUM_ROUNDS, 0.);
    
    // Initialize render window 
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "SDL2Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH,
        HEIGHT,
        0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    

    // Initialize simulation
    int numAgents = 12;
    int numFood = 20;
    auto t1 = high_resolution_clock::now();
    Simulation sim(numAgents, numFood, WIDTH, HEIGHT, renderer);
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> construct_time = t2 - t1;


    // Set renderer to blank canvas
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);


    // TODO: remove dead code
    // for(int i = 0; i < NUM_ROUNDS; i++) {
    // SDL_Rect rect;
    // rect.x = -900;
    // rect.y = -200;
    // rect.w = 100;
    // rect.h = 50;
    // SDL_SetRenderDrawColor(renderer, 140, 200, 5, 255);
    // SDL_RenderFillRect(renderer, &rect);
    // SDL_Delay(300);
    // return 0;
    // }
    // SDL_RenderPresent(renderer);
    // SDL_Delay(1000);


    // Run simulation
    double total_time = 0.;
    // sim.init();
    sim.agents[0]->size = 7;
    sim.agents[0]->energy *= 2;
    for(int r = 0; r < NUM_ROUNDS; r++) {
        if (sim.agents.size() == 0) break;
        // std::cout << r << std::endl;
        std::cout << sim.agents.size() << std::endl;
        t1 = high_resolution_clock::now();

        sim.runRound(STEPS_PER_ROUND);
        sim.finishRound();
        // TODO: remove delay here and figure out how to render at 30 fps
        // SDL_Delay(5);
        t2 = high_resolution_clock::now();
        duration<double, std::milli> round_time = t2 - t1;
        round_times[r] = round_time.count();
        total_time += round_times[r];
    }

    std::cout << "Total time was " << total_time << " ms\n";

    SDL_Delay(10000);

    // Destroy SDL parameters
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}