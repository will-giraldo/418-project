
#include <iostream>
#include <chrono>

#include "simulation.h"
#include "simulation.cpp"
#include "SDL.h"

#define WIDTH 700
#define HEIGHT 700
#define NUM_ROUNDS 500
#define STEPS_PER_ROUND 500


int main(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "SDL2Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Delay(3000);

    SDL_DestroyWindow(window);
    SDL_Quit();


    // timing setup
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    std::vector<double> round_times(NUM_ROUNDS, 0.);

    // setup simulation
    int numAgents = 50;
    int numFood = 50;
    auto t1 = high_resolution_clock::now();
    Simulation simu(numAgents, numFood, WIDTH, HEIGHT);
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> construct_time = t2 - t1;

    // run simulation
    double total_time = 0.;
    for(int r = 0; r < NUM_ROUNDS; r++) {
        t1 = high_resolution_clock::now();

        simu.runRound(STEPS_PER_ROUND);
        simu.finishRound();

        t2 = high_resolution_clock::now();
        duration<double, std::milli> round_time = t2 - t1;
        round_times[r] = round_time.count();
        total_time += round_times[r];
    }

    std::cout << "Total time was " << total_time << " ms\n";

    return 0;

}