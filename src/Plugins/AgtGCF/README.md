# Generalized Centrifugal Force Model

[Read the paper](https://arxiv.org/pdf/1008.4297.pdf)

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

1. Inexplicably, in the `navMeshPlacement.xml` project, the GCF agents do *not* get properly placed.
2. GCF agents suffer with navigation meshes.  They can't walk near obstacles (because of the 
   repulsive forces).  However, the path planning assumes they can get as close as their width. This
   leads to inefficiencies when crossing interior portals.
  1. This *might* be addressed by having the navigation mesh correct the funnel algorithm by 
	looking down the hallways to the most distant, visible funnel.
  2. Alternatively, the GCF agent should assume the preferred velocity is collision free and only
	   compute obstacle forces if there are agent forces acting on it (or possibly if they are
	   colliding with the obstacle.)
  3. This problem manifests itself in the maze scenarios.
3. This is sensitive to obstacle forces so, currently, obstacles are being included which aren't
   actually visible.  This is bad.
4. Agents occasionally get in situations where they spin at high speed (see `event.xml`).
5. Agent's can disappear due to bad orientation values.  This is apparent at the end of `4square.xml`.
   This should be considered a *global* problem.  It merely becomes visible in the drawing of GCF
   agents because their drawing depends on good orientation values.  Orientaiton and preferred
   velocity print as "-1.#IO".  This crops up when there's *no* preferred direction and no speed.
   
### Bad scenarios
	
- `core/4square.xml` : the agents disappear (intermittently) upon reaching their goal.
- `core/event.xml` : as agents cluster near goal, forces ripple through that cause fast moving
	and spinning agents.
- `core/globalNavSwap.xml` : agents in initial condition immediately pop into walls.
- `core/globalNavSwap-roadmap.xml` : agents get caught in the environment and can't make progress.
- `core/globalNavSwap-vectofrield.xml` : agents get accelerated into high speed spins.
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
- `core/swap.xml` : Agents don't get resized by action.
	