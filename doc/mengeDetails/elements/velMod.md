Velocity Modifier {#page_VelMod}
=================

@section sec_VM_overview Overview

The [Velocity Modifier](@ref Menge::BFSM::VelModifier) operates between the [Velocity Component](@ref page_VelComp) and the agent.  The Velocity Component is responsible for computing an *optimal* path for the agent to travel.  However, the global navigation algorithm included in the Velocity Component may not take into account all factors which should affect an agent's preferred velocity; typically they only consider the static environment.  Velocity modifiers provide a mechanism for compositionally layering on additional algorithms and models to modify an agent's preferred velocity.

A velocity modifier can be thought as a velocity *filter*.  A preferred velocity is fed in as input and a, possibly, different preferred velocity is provided as output.  Multiple velocity modifiers can be chained together such that the output of one velocity modifier serves as input to the next. Finally, the output of the last velocity modifier is provided as preferred velocity to the agent.  A velocity modifier can make *arbitrary* changes to the input preferred velocity; it is distinctly possible to apply a velocity modifier that renders all previous preferred velocity calculations moot.

Velocity modifiers can be specified in three different locations in the simulation specification.  The location of the reference will change what its scope of application is.  Furthermore, the various scopes interact in a particular way.

- __Agent Profile__: A velocity modifier can be defined in an `<AgentProfile>` (see @ref page_SceneSpec).  These velocity modifiers affect *all* agents which belong to the profile.  The velocity modifier will be in effect during the entire duration of the simulation for those agents (unless some event otherwise disables it).
- __BFSM State__: A velocity modifier can be defined as an element of a [BFSM State](@ref Menge::BFSM::State).  These velocity modifiers will only affect agents who currently reside in the state.
- __BFSM__: Finally, a velocity modifier can be defined in the root of the Behavior FSM.  These velocity modifiers affect all agents, all the time.

If there is only ever a single velocity modifier affecting an agent, the order of operations is simple:

\image html specification/velMod_singleEval.png

The preferred velocity computed by the Velocity Component is fed into the single velocity modifier, the preferred velocity is modified in some way, and, finally, the resultant preferred velocity is provided to the pedestrian model.

Multiple velocity modifiers can be provided.  For example, an agent profile could specify two velocity modifiers as follows:

  <AgentProfile>
    ...
    <VelModifer type="firstMod" ... />
    <VelModifer type="secondMod" ... />
  </AgentProfile>

In this case, the velocity modifiers are composed in the order given:

\image html specification/velMod_doubleEval.png

Finally, velocity modifiers can be specified in all three locations.  The velocity modifiers are composed in a specific-to-general order.  When the preferred velocity is computed by the Velocity Component, it passes through the modifiers specified in the `<AgentProfile>`, then the BFSM State, and, finally, the BFSM scope, as shown here:

\image html specification/velMod_tripleEval.png


@subsection subsec_VM_implementations Implementations

- The [scale velocity modifier](@ref Menge::BFSM::ScaleVelModifier) which simply scales the magnitude of the preferred velocity.
- The [height field velocity modifier](@ref Terrain::HeightFieldModifier) which modifies the preferred velocity based on a height field, pushing agents away from walking straight up steep hills.
- The [fundamental diagram velocity modifier](@ref FDModifier::FDModifier) which modifies the speed of the preferred velocity to adhere to pedestrian's normal sensitivity to density.
- The [formation velocity modifier](@ref Formations::FormationModifier) which modifies the preferred velocity such that the agent maintains its position in a formation.

** TODO: THE LINKS TO PLUGINS DON'T WORK ** 