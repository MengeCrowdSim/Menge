%Menge's Event System   {#page_EventSystem}
====================

The event system is comprised of two components: the underlying event mechanism (see 
Menge::EventSystem) and the definition of events.

@section sec_EventSystem Events in the simulation pipeline

@todo Provide links to each of these phases.

Every simulation time step has multiple phases:
	- evaluation of events
	- test and execute FSM transitions
	- compute preferred velocity
	- evaluate pedestrian models
	- evaluation of @ref page_Tasks
	
The very first thing that happens in each time step is that the events are evaluated. If any events
are "triggered", the events have the opportunity to change the state of the simulator before
doing anything else.

It is important to note, that events are _not_ evaluated during time steps. Advancing the time of
the simulator is treated like an atomic operation; every agent will advance in time based on its
initial state. Only after _all_ agents have advanced, well the events be evaluated again and,
potentially, have the chance to change simulator state.

@section sec_EventArchitecture Event architecture

In traditional systems, events are statically defined types. For example, in a GUI API, tapping a
key or clicking a mouse button are defined as known event types at compile time. Your code would
process the event and invoke a response to change the state of the application. %Menge uses a 
different approach.

%Menge's two driving design goals are modularity and extensibility. It's possible that a single
system event could trigger multiple changes in one simulation, but only a single change in another
simulation. We can also reverse that; where a single change could be triggered by any number of
unique conditions in the simulator. %Menge's events have been decomposed into _elements_ that can
be reassembled in many different ways to achieve arbitrarily complex event results.

A single Event, is made up of three different types of elements:
	- An @ref page_EventTrigger,
	- One or more @ref page_EventEffect "Event Effects", and
	- One or more @ref page_EventTarget "Event Targets".
	
Each element type plays a very specific role in the Event. The Event serves as the glue. 

@ref page_EventTrigger "Event triggers" define the condition under which an event is considered
"active". They encode a boolean test which gets evaluated at every time step. If the test evalutes
to true, the _effect_ of the event is fired off. See the discussion of @ref page_EventTrigger for more
details.

@ref page_EventEffect "Event effects" define how the simulation changes as a result of the event's
firing. It could be, as in the previous example, moving an agent to a new state. It could be changing
an agent's property. It could be changing the structure of the FSM itself (like adding a transition).
See the discussion of @ref page_EventEffect for more details.

@ref page_EventTarget "Event targets" define the _scope_ of the event's effect. The effect may say that
it moves an agent to a particular state, but it doesn't know _which_ agent. The event target defines
which agent. As a %Menge element, the logic for selecting the target can be arbitrarily complex.
It could be as simple as a fixed agent, to something more complex such as all the agents in a single
state, or all the agents _not_ in a single state, or all the agents in a particular region. And, again,
the target doesn't necessarily have to be an agent. Other components of the simulator could also
be targets. See the discussion of @ref page_EventTarget for more details.

@section sec_EventSpec Specifying Events in XML

Events are specified in the @ref page_BehaveSpec "behavior XML". As previously indicated, a single
event is defined with one *trigger*, and one or more *effects* and *targets*. Triggers are defined
uniquely for each event. However, effects and targets are defined independent of the events and
the specification of a single event merely *references* effects and targets that have already been
declared. 

This example shows a representative portion of a behavior specification, including only the 
declaration of the event system and *incomplete* events.

@code{xml}
1 <?xml version="1.0"?>
2 <BFSM>
3 	<EventSystem conservative="1">
4		<Target name="my_target" type="target_element" ... />
5		<Effect name="my_effect" type="effect_element" ... />
6		<Event name="my_event">
7			<Trigger name="something_happened" type="trigger_element" />
8			<Response effect="my_effect" target="my_target" />
9		</Event>
10	</EventSystem>
	...
</BFSM>
@endcode

The important features of this example are on the following line numbers:
	- Line 3: The declaration of the Menge::EventSystem. Some important notes:
	  - There can be *at most* a single declaration of the `<EventSystem>` tag.
	  - The `<EventSystem>` tag has a single boolean-valued parameter: `conservative`. See @ref
	  Menge::EventSystem for details on this flag.
	- Line 6: This declares an actual event. It provides a *unique* name for the event: "my_event".
	All events must have unique names.
	- Line 7: The event's trigger is defined in a child tag: `<Trigger>`. An `<%Event>` tag must
	  have one, and only one, `<Trigger>` child. See
	  Menge::EventTrigger for details on instantiating particular triggers.
	- Line 8: An event can have *multiple* responses. Each response is indicated by a child 
	`<Response>` tag. The `<Response>` tag references previously declared `<Target>` and `<Effect>`
	tags through their unique names. In this example, the event has only a *single* response and the
	declared target and effects are on lines 4 and 5, respectively.
	- Line 4: This is the declaration of an event *target*. It is given a unique name, "my_target".
	This name is referenced in the `<Response>` tag on line 8. See Menge::EventTarget for details on
	how to instantiate particular event target types.
	- Line 5: This is the declaration of an event "effect". It is given a unique name, "my_effect".
	This name is referenced in the `<Response>` tag on line 8. See Menge::EventEffect for details on
	how to instnatiate particular event target types.
	
@section sec_ExtendingEvents Extending Events in Plugins

@todo Write this up.
