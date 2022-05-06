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
#define IS_RENDER true



int main(int argc, char* argv[]) {
    // Timing setup
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    std::vector<double> run_round_times(NUM_ROUNDS, 0.);
    std::vector<double> finish_round_times(NUM_ROUNDS, 0.);
    std::vector<double> round_times(NUM_ROUNDS, 0.);
    
    // Initialize simulation
    int numAgents = NUM_AGENTS;
    int numFood = NUM_FOOD;
    auto t1 = high_resolution_clock::now();

    Simulation sim(numAgents, numFood, WIDTH, HEIGHT, IS_RENDER);
    sim.init();

    // Initialize clock
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> construct_time = t2 - t1;

    // Run simulation
    double run_time = 0.;
    double finish_time = 0.;
    double total_time = 0.;
    for(int r = 0; r < NUM_ROUNDS; r++) {

        if (sim.agents.size() == 0) break;

        t1 = high_resolution_clock::now();

        sim.runRound(STEPS_PER_ROUND);
        
        // Run round time
        auto t3 = high_resolution_clock::now();
        duration<double, std::milli> run_round_time = t3 - t1;
        run_round_times[r] = run_round_time.count();
        run_time += run_round_times[r];

        sim.finishRound();

        // Finish round time
        auto t4 = high_resolution_clock::now();
        duration<double, std::milli> finish_round_time = t4 - t3;
        finish_round_times[r] = finish_round_time.count();
        finish_time += finish_round_times[r];

        // Total round time 
        t2 = high_resolution_clock::now();
        duration<double, std::milli> round_time = t2 - t1;
        round_times[r] = round_time.count();
        total_time += round_times[r];

    }

    // Print out run times of algorithm's components
    std::cout << "Run time: " << run_time << " ms\n";
    std::cout << "Finish time: " << finish_time << " ms\n";
    std::cout << "Total time: " << total_time << "ms\n";


    // Destroy SDL parameters
    sim.destroy();

    return 0;
}