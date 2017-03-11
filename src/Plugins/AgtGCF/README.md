# Generalized Centrifugal Force Model

This is inspired by the paper by Chraibi et al (Generalized Centrifugal Force model.
However, the implementation differs in several critical ways. See the documentation in 
`src/Plugins/GCFAgent/GCF.h` for details.

[Read the paper](https://arxiv.org/pdf/1008.4297.pdf)

This implementation makes use of the code for computing the distance of closest approach between
two ellipses provided at:

http://www.math.kent.edu/~zheng/ellipsoid.c

## Notes on using the model

This implementaiton does not deal well with dense, contradictory flow.  When simulated with
sufficient numerical accuracy (i.e., a *very* small time step), the net result is a stationary
traffic jam.

## Notes on implementation

The Generalized Centrifugal Force (GCF) model is a force model.  Numerically speaking, these models
tend to require smaller simulation time steps.  First, it's a second-order model (that is to say,
we are computing forces/accelerations that have to be integrated twice to get positions). Second,
the repulsive forces tend to be quite stiff when agents are near each other or obstacles.

One option is to set the time step to a smaller value (e.g., `Menge -t 0.01 ...`).  If visualzing
the result, or if it requires a smaller timestep than you really want to deal with (for example
in outputting the trajectories), you can use a *larger* time step in conjunction with the
`--subSteps` parameter.  A time step value of 0.1 with a sub steps value of 9 is equivalent
(integration-wise) to taking a time step of 0.01.  The difference is that ten smaller steps will
be taken between visualizer refresh and scb-writing.

## Known issues

1. GCF agents suffer with navigation meshes.  They can't walk near obstacles (because of the 
   repulsive forces).  However, the path planning assumes they can get as close as their width. This
   leads to inefficiencies when crossing interior portals.
  1. This would be *partially* alleviated by modifying the path following algorithm such that the
     segments of the funnel path only have vertices at meaningful turnings.  This would increase
	 the cost of the path following algorithm.
  2. Alternatively, the GCF agent should assume the preferred velocity is collision free and only
	   compute obstacle forces if there are agent forces acting on it (or possibly if they are
	   colliding with the obstacle.)
  3. This problem manifests itself in the maze scenarios.
2. This model is sensitive to obstacle forces so, currently, obstacles are being included which
   aren't actually visible.  This is bad.  In computing neighbor obstacles, this should omit 
   obstacles that are appreciable occluded.
3. Agents occasionally get in high-density situations where they spin at high speed (see 
   `event.xml`). Cause currently unknown.
4. Setting size properties on agent doesn't work. This requires a redefinition of agent "size" and
   "resizing" such that it can work with more arbitrary agent geometries.
   
### Bad scenarios
	
- `core/event.xml` : as agents cluster near goal, forces ripple through that cause fast moving
	and spinning agents.
- `core/globalNavSwap.xml` : agents in initial condition immediately pop into walls.
- `core/globalNavSwap-roadmap.xml` : agents get caught in the environment and can't make progress.
- `core/globalNavSwap-vectorfield.xml` : agents get accelerated into high speed spins.
- `core/goalDistance.xml` : agents exhibit navigation mesh problems of not passing through funnel
	goals.
- `core/maze*.xml` : agents suffer from obstacles they can't see.  Also has nav mesh problems.
- `core/obstacleSwitch.xml` : inconsistent behavior w.r.t. obstacles (related to decomposed
	obstacles).
- `core/office.xml` : unstable; causes the simulation to crash.
- `core/pedModelSwap.xml` : agents can't get past each other.
- `core/periodic.xml`: the agents can't pass each other.
- `core/randomGoal.xml` : the agents can't pass each other so end up forming a jam around goals.
- `core/sharedGoal.xml` : two agents promptly disappear and the agents don't resize.
- `core/soccer.xml` : Agents are not getting properly placed on the navigation mesh.
- `core/stadium.xml` : Agents are not getting properly placed on the navigation mesh.
  - This *may* be due to the fact that the ellipses are *not* drawn with the right position.
  - I'm setting the position *directly* but the agent has no chance to do anything to update the
	ellipse.  *HOWEVER* this doesn't address the issue of it not being drawn at the correct elevation.
  - Is it a problem in the visagent?
- `core/swap.xml` : Agents don't get resized by action.

	