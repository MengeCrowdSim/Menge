Scene Specification                {#page_SceneSpec}
======================

@section sec_sceneSummary Summary

The scene specification defines the *elements* of the simulation: the static obstacles, the elevation object, the spatial query mechanism, and, of course, the agent population.  All of this is specified via an XML file constructed from a pre-defined set of valid tags.  However, this set of tags can be extended when implementing new simulation elements by implementing the appropriate interface.

The basic scene specification XML has the following form:

@code{xml}
<?xml version="1.0"?>
<Experiment version="2.0">
	<Common ... />
	<Model1 ... />
	...
	<ModelN ... />
	
	<AgentProfile ... />
	...
	<AgentProfile ... />
	
	<AgentGroup ... />
	...
	<AgentGroup ... />
	
	<SpatialQuery ... />
	<Elevation ... />
	
	<ObstacleSet ... />
	...
	<ObstacleSet ... />
</Experiment>
@endcode
	
The major elements of the scene specification are:
  - The header: `<Experiment ... />` (@ref sec_sceneHeader)
  - Global simulation parameters: `<Common ... />` and `<Modeli ... />` (@ref sec_sceneSimParam )
  - Agent profile definitions: `<AgentProfile ... />` (@ref sec_sceneAgentProfile)
  - Agent group definitions: `<AgentGroup ... />` (@ref sec_sceneAgentGroup)
  - Spatial query object: `<SpatialQuery ... />` (@ref sec_sceneSpaceQuery)
  - Elevation specification: `<Elevation ... />` (@ref sec_sceneElevation)
  - Obstacle definition: `<ObstacleSet ... />` (@ref sec_sceneObstacles)

The order of the major elements does not matter.  Each section is detailed below.

@section sec_sceneHeader Header

The header is simple.  It is the same across all simulation scenarios.  In addition to the standard XML boilerplate (`<?xml version="1.0"?>`) it contains all of the simulation specification contained in an `<Experiment version="2.0">` tag.  For historical reasons, the file version is 2.0.  This must be specified, but it is conceivable that in the future alternative versions will be created to allow for changes which are *not* backwards compatible.

@section sec_sceneSimParam Global Simulation Parameters

Each pedestrian model requires values across a certain parameter space.  Some of these parameters are considered unique per agent.  Others are the same for all agents in the simulation -- these are considered *global* parameters.  Implementing the pedestrian model interface includes defining these global parameters.  All global parameter definitions are included as child tags of the `<Experiment>` tag.  There are some parameters which are common to all simulators (as all pedestrian models inherit from a common class).  These are contained in the `<Common>` tag. Global parameters unique to a particular model are contained in a uniquely defined tag.  This tag is a sibling to the global `<Common>` tag.

Currently, there is only one global simulation parameter: time step.  Thus a typical `<Common>` tag would like this:

    <Common time_step="0.1" />

It is worth noting that this simulation time step can be overridden on the [command line](@ref page_CommandLine) or in the [project specification](@ref page_ProjectSpec).

@section sec_sceneAgentProfile Agent Profile Definitions

%Menge allows for crowds made up of a heterogeneous population.  This heterogeneity can be realized using two complementary mechanisms: profiles and distributions.  An agent profile reflects the idea that there may be different classifications of agents (e.g., old/young, male/female, etc.)  These different classifications (or *profiles*) arise from the idea that the agents which belong to different profiles are possessed of quite different property values.  However, inside a single profile, there can still be variability across the agents.  This is done using *distributions*.  For example, agents modelling young male pedestrians may have a mean preferred walking speed of 1.5 m/s with a standard deviation of 0.1 m/s.  In contrast, old females would have a mean walking speed of 0.9 m/s and a standard deviation of 0.05 m/s.  

Ultimately, the agent profile is directly concerned with defining *per-agent* parameters.  We specify an agent profile in the following manner:

@code{xml}
<AgentProfile name="PROFILE_NAME">
	<Common max_angle_vel="360" max_neighbors="10" obstacleSet="1" neighbor_dist="5" r="0.19" class="1" pref_speed="1.04" max_speed="2" max_accel="5" />
	<Model1 prop1="value1" prop2="value2"/>
	...
	<ModelN prop1="value1" prop2="value2"/>
	<VelModifier type="vmName" ... />
	...
	<VelModifier type="vmName" ... />
</AgentProfile>
@endcode
	
The agent profile must be given a name -- this is how the profile is referred to later by the `<AgentGenerator>` tags.

As with the global simulation parameters, there are common per-agent parameters and model-specific parameters.  The common parameters are stored in the `<Common>` tag and the model-specific parameters are stored in a tag whose name is defined by the pedestrian model's implementation.

The `<Common>` tag has the following values (order doesn't matter and exclusion will cause them to be assigned a built-in, default value):

- `max_angle_vel`: the maximum angular velocity of the agent (in degrees/second).  The BaseAgent includes a method for smoothing the "orientation" of the agent.  It does so by limiting the speed at which it can turn.
- `max_neighbors`: the maximum number of nearby *agents* which will be computed for defining an agent response.  Smaller numbers lead to faster simulations but can lead to simulation artifacts.
- `obstacle_set`: All obstacles have a identifier (typically a power of two).  The agent's `obstacle_set` serves as a mask.  If the bit-wise and of the `obstacle_set` value and the obstacle's identifier is non-zero, the agent can "see" and respond to the obstacle, otherwise, it is ignored (see @ref page_ObstSet).
- `neighbor_dist`: the maximum distance an agent can be and still be a neighbor candidate.  
- `r`: in %Menge, agents are largely modeled as circular disks.  This is the radius of that disk.
- `class`: the visualization class of the agent.  Used by the visualizer.  Generally, agents with different class will be drawn differently.
- `pref_speed`: the agents preferred speed (in m/s).
- `max_speed`: the agents maximum speed of travel (in m/s).  The result of the pedestrian model's velocity computation will be clamped to this speed.
- `max_accel`: the maximum acceleration the agent can experience (in m/s^2).  This is a simple smoothing mechanism and doesn't make allowances for anisotropic behaviors.

A `<Model>` tag has a similar structure.  Each per-agent property will have a property name and value pair.  For specific pedestrian models, the tag name and property key-value pairs are defined in the model (see @ref page_PedModel).

Finally, a profile can consist of zero or more [velocity modifiers](@ref Menge::BFSM::VelModifier).  The velocity modifiers will affect all agents derived from this profile, regardless of what state they are in.  See the [Velocity Modifier](@ref page_VelMod) page for more details.

@subsection subsec_sceneProfVariability Property Variability

In the example above, every agent assigned the same profile would have the same property values.  %Menge provides a mechanism to define distributions of values for any or all of the properties contained in an `<AgentProfile>` child tag (both `<Common>` and `<Model>` tags.  It looks like this (illustrated with the *common* property `pref_speed`):

@code{xml}
<AgentProfile name="PROFILE_NAME">
	<Common max_angle_vel="360" max_neighbors="10" obstacleSet="1" neighbor_dist="5" r="0.19" class="1" pref_speed="1.04" max_speed="2" max_accel="5" >
		<Property name="pref_speed" dist="n" mean="1.3" std_dev="0.1" />
	</Common>
	<Model1 prop1="value1" prop2="value2"/>
	...
	<ModelN prop1="value1" prop2="value2"/>
</AgentProfile>
@endcode

Note, that the `<Common>` tag is no longer a self-contained tag.  It now has a child tag: `<Property>`.  The property tag is how we define variability for a particular property.  We do so by specifying the property name (`name`) and a distribution (`dist`).  In this case, we're defining a normal distribution with a mean value of 1.3 m/s and a standard deviation of 0.1 m/s.  As agents are assigned this profile, their preferred speed values will be assigned according to this distribution (see @ref page_Distribution for more details).  Any property with such a property tag will have its single value overridden by the distribution.

@subsection subsec_sceneProfInherit Profile Inheritance

It is quite common that multiple profiles will be largely similar.  In fact, one common use case is to have groups of identical agents who only differ in their visualization class (see the 4square example in @ref page_Examples).

The `<AgentProfile>` tag has an additional property: `inherits`.  One `<AgentProfile>` can refer to an earlier profile, inheriting all of its values (including property distributions).  Then, only those properties that are different need be specified.  Inheritance would look like this:

@code{xml}
<AgentProfile name="source">
	<Common max_angle_vel="360" max_neighbors="10" obstacleSet="1" neighbor_dist="5" r="0.19" class="1" pref_speed="1.04" max_speed="2" max_accel="5" >
		<Property name="pref_speed" dist="n" mean="1.3" std_dev="0.1" />
	</Common>
</AgentProfile>

<AgentProfile name="child" inherits="source">
	<Common class="2"/>
</AgentProfile>
@endcode

The agent profiles must appear in the *correct* order in the specification file.  The parent profile must appear before the profiles that inherit from it.

@section sec_sceneAgentGroup Agent Groups

The agent profile merely defines a type or class of agent; it does not instantiate agents in the simulation.  The `<Agent Group>` tag is responsible for instantiating agents.  An agent is instantiated by defining three aspects: its profile, its position, and its initial behavioral state.  Each of these three aspects is defined by its own element (see @ref page_Elements): a Profile Selector, an Agent Generator, and a State Selector.  The syntax of the `<AgentGroup>` tag looks like this:

@code{xml}
<AgentGroup>
	<ProfileSelector type="<typeName>" ... />
	<StateSelector type="<typeName>" ... />
	<Generator type="<typeName>" ... />
</AgentGroup>
@endcode

@subsection subsec_sceneProfSelect Profile Selector

The [Profile Selectors](@ref page_ProSelect) element encodes an algorithm which determines how a profile is assigned to each newly created agent in the group.  Each implementation of a [ProfileSelector](@ref Menge::Agents::ProfileSelector) defines a unique `type` name.  To declare a profile selector of that type, use the documented type name and provide its other arguments, as necessary.  See the documentation of various [profile selectors](@ref page_ProSelect) for details.

@subsection subsec_sceneAgentGen Agent Generator

The [Agent Generators](@ref page_AgentGen) element encodes an algorithm which determines the number and position of agents in the `Agent Group`.  Each implementation of an [AgentGenerator](@ref Menge::Agents::AgentGenerator) defines a unique `type` name.  To declare an agent generator of that type, use the documented type name and provide its other arguments, as necessary.  See the documentation of various [agent generators](@ref page_AgentGen) for details.

@subsection subsec_sceneStateSelect State Selector

The [State Selectors](@ref page_StateSelect) element encodes an algorithm which determines how a BFSM initial state is assigned to each newly created agent.  Each implementation of a [StateSelector](@ref Menge::Agents::StateSelector) defines a unique `type` name.  To declare a state selector of that type, use the documented type name and provide its other arguments, as necessary.  See the documentation of various [state selectors](@ref page_StateSelect) for details.


@section sec_sceneSpaceQuery Spatial Queries

Still to come...

@section sec_sceneElevation Elevation

Still to come...

@section sec_sceneObstacles Obstacle Sets

Still to come...
