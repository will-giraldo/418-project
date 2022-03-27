# TITLE -- Hridayesh Joshi (hvj), William Giraldo (wgiraldo)


## Summary


## Background


## The Challenge

## Resources


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
