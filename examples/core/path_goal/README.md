# Moving goals Example

This provides an example of four different moving goals (with particular emphasis of non-trivial
VelComponent implementations handling moving goals). In this example, there are four agents, each
a different class.

## Class 1

The agent in "group 1" (with agent class 1) is located in an open domain without obstacles. It has
a simple "goal" VelComponent. Its goal, however, is a "path" Goal that moves along a square path.
The path is closed so it loops. This agent will attempt to catch the moving target and will
eventually catch it after several loops (it is able to cut corners, eventually making up the
distance). This showcases the "path" Goal using a *closed* path.

## Class 2

The agent in "group 2" (with agent class 2) is likewise located in an open domain without obstacles.
Its goal is also a "path" Goal, but it has a simple *open* path. The goal moves left and right
along a simple line segment. The agent quickly catches the goal. This show cases the "path" Goal
using an *open* path.

## Class 3 and Class 4

The agents in "group 3" and "group 4", respectively, operate in an environment with obstacles which
form concave regions. The agent with class 3 uses a road map to traverse the region and
the agent with class 4 uses a navigation mesh to traverse the region. They start near each other
and are moving toward the same "path" Goal (which moves in a closed loop around the obstacle). These
two agents showcase how the "road_map" and "nav_mesh" VeclComponent implementations correctly handle
a moving goal.
