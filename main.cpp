#pragma once

#include <iostream>
#include <chrono>
#include <omp.h>

#include "simulation.h"
#include "simulation.cpp"
#include "SDL.h"

#define WIDTH 700
#define HEIGHT 700
#define NUM_ROUNDS 5000
#define STEPS_PER_ROUND 400


int main(int argc, char* argv[]) {
    // Timing setup
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    std::vector<double> round_times(NUM_ROUNDS, 0.);
    

    // Initialize simulation
    int numAgents = 12;
    int numFood = 20;
    auto t1 = high_resolution_clock::now();
    Simulation sim(numAgents, numFood, WIDTH, HEIGHT);
    sim.init();

    // Initialize clock
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> construct_time = t2 - t1;

    // Run simulation
    double total_time = 0.;
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
    sim.destroy();

    return 0;
}