Elements                {#page_Elements}
======================

@section sec_elemSummary Element Summary

%Menge is comprised of *xx* different element types.  Each serves a particular purpose in the overall crowd simulation task.  By implementing each element's unique interface, the %Menge framework can simulate scenarios across a vast behavioral space.

@section sec_elemTypes Element Types

The element types are as follows:
  - Agent initial conditions

    Each agent's initial condition is uniquely defined by where it is in space, its properties, and its current behavior state.  These three characteristics are defined by three elements: the agent generator, the profile selector, and the state selector, respectively.      
    + [Agent Generators](@ref page_AgentGen): Mechanism by which the number and position of all agents in the scene are determined.
    + [Profile Selectors](@ref page_ProSelect): Mechanism by which agent properties (defined by an "agent profile") are assigned to the agents created by the Agent Generator.
    + [State Selectors](@ref page_StateSelect): Mechanism by which each agent is assigned an initial state in the [Behavioral Finite State Machine (BFSM)](@ref sec_Arch_BFSM).
  - Simulator Components

    The simulator uses various elements to satisfy queries and define the domain.  

	+ [Elevations](@ref page_Elevation): Mechanism by which agents can move in domains that are not flat planes.
	+ [Obstacle Sets](@ref page_ObstSet): Mechanism by which obstacles are introduced into the scene.
	+ [Spatial Queries](@ref page_SpatialQuery): Mechanism by which spatial queries (e.g., k-nearest neighbors) are satisfied.
	+ [Tasks](@ref page_Tasks): Mechanism by which arbitrary computation can be introduced into the %Menge paradigm.  

  - [Behavioral Finite State Machine](@ref sec_Arch_BFSM)

    The behavior of each agent can change as the simulation progresses.  We accomplish this by use of a finite state machine.  An agent exists in a single state at any given time.  The state defines the agent's *preferred velocity*.  As the agent moves from state to state, its preferred velocity (and even how it is computed) can radically change.  Agents can transition from one state to another state based on FSM *structural* transitions or external events.  As agents enter and leave states, actions can be applied to the agent, changing (permanently *or* temporarily) properties of the agent.

	+ [Actions](@ref page_Actions): Actions assigned to a state are applied to agents upon entering that state and can optionally be undone when the agent leaves the state.
	+ [Events](@ref page_EventSystem)
		
		%Menge supports an event system for affecting agent behaviors.  The event system is comprised of three elements: an event trigger, effect, and target.
		- [Event Triggers](@ref page_EventTrigger): The definition of a condition on simulation state which causes an event to be applied to the simulation.
		- [Event Effects](@ref page_EventEffect): The definition of a response to an event (such as changing agent states, changing the simulation domain, etc.)
		- [Event Targets](@ref page_EventTarget): The definition of the simulation entity that will be affected by the Event Effect.

	+ [Goals](@ref page_Goal): The definition of a region in space the agent wants to reach.
	+ [Goal Selectors](@ref page_GoalSelect): The mechanism by which the agent selects a goal.
	+ Transitions

		These are the FSM transitions.  They define the circumstances, based on agent state, which cause an agent to transition from one state to another.
		- [Conditions](@ref sec_Trans_Condition): The definition of the condition which causes an agent to move to a connected state.
		- [Targets](@ref sec_Trans_Target): The definition of the connected state to which an agent moves.
	+ [Velocity Components](@ref page_VelComp): The mechanism which computes an agent's instantaneous preferred velocity.
	+ [Velocity Modifiers](@ref page_VelMod): A mechanism through which an agent's preferred velocity can be modified based on aribtrary simulation state.
	- Future Elements
	
		There are plans to extend %Menge's expressive ability by adding new element types.  We are always open to suggestions which will increase %Menge's usefulness.
		
		- Agent Sources: Entities which can dynamically introduce new agents into the simulation at arbitrary times rather than purely as initial conditions.
