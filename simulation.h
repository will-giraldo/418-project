#include <vector>
#include "agent.h"
#pragma once


class Simulation {
public:
    std::vector<Agent> agents;
    std::vector<std::pair<int,int>> food;

    void init(int numAgents);
    void run();
    void update();
    void render();
}