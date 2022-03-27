# Parallelizing Natural Selection Simulation  -- Hridayesh Joshi (hvj), William Giraldo (wgiraldo)


## Summary
For our final project we are going to be constructing and parallelizing a simulation of natural selection, with the goal of creating a more complex simulation which will iterate and converge to more optimal solutions as fast as possible. Our work will consist of finding axises of parallelism on which to speed up computation, as well as ensuring that the simulated agents can all be run in parallel and interact with each other while maintaining the integrity of the solution/simulation. We will be implementing simulations primarily on the CPU using OpenMP, and time allowing create a GPU implementation to compare performance with. 

## Background
Such a simulation can be a fairly compute intensive program. It requires simulating potentially thousands (if not millions) of agents who can all interact with each other, and repetition of this for multiple generations while using methods such as genetic algorithms to simulate the surival of the agents with more desireable traits for the given environment. 
There are two parts of the application which we see benefiting from parallelism which we will look to accelerate, and which we will be implementing. First is the actual simulation, given independent agents, we will simulate their interactions and their survival. Secondly between generations of agents we will need to complete the natural selection process, and create new agents for the next generation. 
Doing each of these parts seperately in parallel will primarily allow for faster computation times to select for the most desireable traits, as well as allowing for much larger simulations on potentially different, more complex environments to complete in a reasonable amount of time. 
### General pseudocode
```
While running simulation:
  Create initial population
  Run simulation (parallel)
  For all remaining agents: (parallel)
    Assign performance rating
    Crossover with highly-rated agents
    Randomly mutate some traits with low probability
  Create new population with propagated traits
```

## The Challenge
The parallel challenge is running the simulation. While the selection and crossover step is done in parallel as well, this area shouldn't be as difficult to parallelize as there aren't necissarily dependincies between iterations, as long as agents are able to be selected for crossover properly. 
The simulation on the other hand requires potentially complex interactions between the agents themselves and the environment. Agents need to be able to interact with each other and make decisions based on other agents and the changing environment as the simulation runs. 
In terms of computation and memory, because agents can potentially take very different actions because of their traits, there is a large potential for diverging computation. Additionally, although we plan to work primarily under the shared-memory model, there will be frequent updates to the environment, which will require lots of changes in caches and memory, which may potentially slow the simulation significantly (think lots of global memory accesses in the GPU for example). 
Some constraints that the shared memory model imposes on the solution is that while memory can be shared, frequent access and modification to it may not be cheap. There will likely have to be some additional work to reduce the need to constantly reach out to global/shared memory while running the simulation. 


## Resources
For our project we will initially only be working on running the code on the CPU side, so will start by simply running on our own machines. However this may progress to testing our implementation on the GHC machines as well. If we manage to reach our goal of creating a GPU implementation, then we will be working on the GHC machines.
We will be starting from scratch for the main part of our project, but when we create a graphical model of our simulation, we will likely be using a 3rd party program such as Unity to handle the rendering and modeling of the agents/environment. 
These resources should be sufficient for the majority of our project, but time allowing it may be interesting to scale up the size of the problem (working with millions of agents) and run on something such as the Bridges machines. 


## Goals and Delivereables
### 75% Goal
- Finish a sequential implementation of the agents, which consists of the individualistic evolutionary traits such as speed and size. 
- Parallelize our implementation with OpenMP and analyze its effectiveness.
### 100% Goal
- Implement the rendering component with Unity.
- Add more complex traits to the agents. In particular, we will add the ability to crossover, which is when two agents can breed to form another agent rather than duplicating. Adding this will heighten the complexity of our simulation and will require a higher level of communication across the agents.
### 125% Goal
- Employ CUDA to parallelize our simulation and analyze its effectiveness.
- Include additional traits for the agents to add complexity to the population's evolution. 
- Improve the quality of rendering component to be more visually appealing. 
### Poster Session Demo
Ideally, we would like to have a visually appealing rendering of our simulation to demonstrate the effectiveness of parallelizing our code. We would run through multiple different versions that consist of varying the individual traits' values and the addition of crossover traits. Additionally, we will present graphs that measure the speedup and other relevant metrics. 

## Platform
We will first begin our implementation in C++ and utilize OpenMP to parallelize our CPU code as we believe our natural selection simulation will stand to gain more from using a shared memory protocol. Next, for our visual rendering aspect of the project, we plan to use Unity, a game rendering engine. Since rendering is not a core 
element of our project (i.e. not the part we want to analyze its speedup gains from parallelizing it), Unity is a simple and effective choice for rendering. 

We will analyze our performances and measure speedup and other relevant metrics on the GHC clusters as well as the PSC-Bridges 2 supercomputers. 

If we surpass our 100% Goals, we will attempt an implementation using CUDA. Since we will have thousands, potentially millions of agents in our simulation, we stand to gain a lot in speedup if we run our implementation on a GPU. 

## Schedule

| Week      | Goals |
| ----------- | ----------- |
| 3/21 - 3/28 | Project proposal and brainstorm methods to parallelize agents effectively      |
| 3/28 - 4/04 | Start and finish sequential implementation of agents, including core evolutionary traits       |
| 4/04 - 4/11 | Finish OpenMP implementation, Collect data on OpenMP performance, Milestone Report        |
| 4/11 - 4/18 | Complete rendering component; Add crossover ability among agents        |
| 4/18 - 4/25 | Finish crossover traits; Begin CUDA Implementation (if there is time) and collect relevant data        |
| 4/25 - 4/29 | Final Report         |
