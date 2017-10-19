Menge Summary {#mainpage}
=================

@section sec_overview Overview

Menge is a framework for simulating crowds and developing novel pedestrian models and techniques.  The framework decomposes crowd simulation into computational components, called *elements*.  Each type of element solves one aspect of crowd simulation -- aspects such as nearest neighbor queries, global path planning, local collision avoidance, etc.  The Menge framework includes implementations of straightforward implementations of all of these elements.  It's strength for research and development is that novel implementations can be introduced via its plug-in architecture.  In other words, a simulation scenario can be created using a particular collection of element implementations.  One aspect of that simulation can be swapped with one or more different implementations and the results can be easily compared, knowing that all other aspects of the simulation are exactly the same.  

The framework comes with a number of example plug-ins; these examples illustrate how easy it is to extend Menge's fundamental capabilities and illustrate how various scenarios or techniques *might* be solved.  We hope to engender a Menge community through which users can submit their own plug-ins for other users to benefit from.  Our vision is that in the future, development of crowd simulation techniques can be accelerated by the use of a common framework and the ability to share and compare techniques.

For more details, see the <a href="https://collective-dynamics.eu/index.php/cod/article/view/A1">original paper</a>.

@subsection subsec_ModelFunction Pedestrian Model as a Function

@image html pipeline.png "Menge Architecture" width=800px

As previously mentioned, the Menge framework decomposes the problem of simulating crowds into multiple computation components, called *elements*.  The element classes are uniquely defined by their interfaces -- they take a certain input and provide a particular output.  The implementation of that interface can lead to improved simulation -- either through improved performance or improved crowd behaviors.  We'l illustrate this with two simple examples.

Crowd simulation typically requires the ability to perform spatial queries; agents need to consider other near-by agents and obstacles.  This type of query is common in simulation and graphics and many different approaches have been proposed, each with their own unique costs and benefits.  For example, the queries could be accelerated by use of a uniform grid decomposition of the simulation domain, a dynamic bounding volume hierarchy, binary space partitions, kd-trees, or irregular polygonal decompositions (to name a few).  Picking the right algorithm can greatly impact the performance of the simulator.  The *best* spatial query algorithm for crowd simulation may differ from simulation scenario to scenario.  The Menge framework allows for the implementation of arbitrary solutions to the spatial query algorithm and the *right* algorithm can be selected for each simulation scenario as appropriate.

The second example illustrates how novel element implementations can introduce novel crowd behaviors.  One of the included example simulation scenarios shows agents disembarking a plane.  In this scenario, a small number of agents represent infirm passengers.  These passengers must wait to disembark until the plane is mostly clear.  Menge uses a Behavioral Finite State Machine (BFSM) to allow agents to change what they are seeking to accomplish (their goals) and how they go about achieving this (their behavior).  We realized these infirm passengers by introducing a novel Transition Condition element.  Each infirm passenger only transitions from a *waiting* state to a *exiting* state when a specified region, forward of the passengers seat, is clear of other passengers.  This custom implementation of the condition element provides the unique behavior we sought in this scenario.  Far more complex behaviors can be introduced by creating a suitable implementation of the appropriate Menge element (see @ref page_Elements for more details).

@subsection subsec_usage Intended usage

Menge can be used in two ways.  Simply exploiting its current functionality to model new simulation scenarios or to extend its functionality to include novel pedestrian models, spatial query algorithms, behavioral mechanisms, global planning algorithms, etc. 

@subsubsection subsubsec_running Running a Simulation

Using %Menge as a black box simulator is straight-forward.  To simulate a new scenario we requires two input files: a scene specification and a behavior specification.  The scene specification describes the environment and population, including the global and per-agent simulation parameters (see @ref page_SceneSpec); essentially, it defines the simulation initial conditions.  The behavior specification defines how each agent defines its overall behaviors -- its goals, strategies, and responses to changes in the environment (see @ref page_BehaveSpec).  These two files are provided as input to the program, specifying the pedestrian model to use. These files, as well as other relevant simulation parameters can be specified in a [project specification file](@ref page_ProjectSpec) and overrideen on the [command line](@ref page_CommandLine). The results of the simulation can can be displayed interactively (see @ref page_ViewSpec) and/or the per-agent trajectories can be written to a cached file for later analysis or visualization (see @ref page_outSpec).

@subsubsection subsubsec_Extending Extending the Simulator

More generally, we have released Menge as open-source software to encourage extension of its basic functionality.  As previously stated, the Menge simulation architecture is comprised of various, sub-classable elements.  Creating a new element implementation simply requires implementing the Menge element and plug-in interface.  This interface includes the ability to extend the scene and behavior specifications such that novel components can easily be referenced in the simulation specification.
For full instructions, see @ref page_extension.