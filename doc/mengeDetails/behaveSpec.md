Behavior Specification                {#page_BehaveSpec}
======================

@section sec_behaveSummary Summary

The behavior specification ultimately how the simulation's state evolves with respect to time.
The @ref page_SceneSpec determines the simulations initial conditions -- the population and each
agent's properties, the obstacles, etc. The behavior defines the rules for what an agent seeks to
achieve at any given moment, how it goes about achieving that, and even how the agent changes as
time progresses. The behavior specification encompasses eight of the element types defined in %Menge.

@section sec_behaveFSM Behavior as a Finite State Machine

%Menge uses the concept of a Finite State Machine (FSM) to encode behaviors. At any given moment, an
agent has certain characteristics, is trying to achieve some goal, and employs a particular strategy
, in conjunction with its characteristics, to achieve that goal. As simulation time passes, an
agents goal can change, its strategy can change, and even its properties can change. Sometimes that
change arises from the agent's actions (such as achieving a goal) and other times it can arise from
independent events which nevertheless affect the agent. The behavior FSM provides mechanisms for
both.

Let's consider a concrete example: taking a subway ride. We'll followa single passenger and observe
his changing behaviors as he goes through a typical subway experience. First, the passenger enters
the station. We'll assume he's a tourist and has to buy a subway ticket. So, he goes to the ticket
kiosks. Because it's mid-morning, there are a number of other passengers and he waits in line to buy
a ticket. On his turn, he spends twenty seconds or so buying his ticket. With ticket in hand, he
passes through the turnstiles and starts heading towards his platform. On the way, and within sight
of his train platform, he sees a busker with a saxophone playing his favorite song. He pauses to listen. Suddenly, he sees the train coming and runs to the platform. He arrives just as the doors
open and stands to the side while passengers disembark, and then boards the train, taking a seat.

Let's break that down:
	1. Initially, the passenger was headed to the kiosk. If we assume a big open plaza, they probably
	   planned to walk straight toward it (adapting to other people as he went).
	2. Upon reaching the kiosk, the passenger got it in a line. The goal was still the same, the
  	   kiosk, but the strategy has changed. Now the passenger waits behind the person who was in the
	   back of the queue when the passenger entered the line. He patiently follows the person in 
	   front as they work their way toward the kiosk in an orderly manner.
	3. When the passenger reaches the kiosk, he spends some time buying the ticket.
	4. Ticket purchased (and assuming the passenger knows the way to the train platform), he has
	   a path in mind from the kiosk to the platform.
	5. Near the platform, the music distracts him and he stops to listen.
	6. The arrival of the train causes the passenger to panic and he quickly runs from the hallway
	   to the platform.
	7. Arriving on time, and with the goal of entering the train and taking a seat, the passenger 
	   still waits his turn, allowing other passengers to disembark, before boarding the train and
	   taking his seat.
	   
Each of those items represents a state.  In each state, the passenger has a particular objective and
a strategy for achieving it. In some cases, the state even affects the passenger's state of mind. 
We move from one state to another for one of two reasons: the passenger achieves his immediate goal
(i.e., successfuly bought a ticket) or because an external event influenced him (the unexpected arrival of the train). The behavior FSM, in conjunction with the event system, can model all of
these circumstances.

The following figure illustrates the the FSM of the passenger's experience. In this case, the FSM
is quite linear -- one state simply  moves to the next due to the right condition being met. In
other scenarios, we could introduce branches and alternate possibilities based on a more complex
relationship with the environment.

@image html specification/fsm_example.png "FSM Example: A passenger riding the subway" width=800px

The behavior scene specification XML has the following items:

@code{xml}
<?xml version="1.0"?>
<BFSM>
	<EventSystem conservative="1">
		<Target name="my_target" type="target_element" ... />
		<Effect name="my_effect" type="effect_element" ... />
		<Event name="my_event">
			<Trigger name="something_happened" type="trigger_element" />
			<Response effect="my_effect" target="my_target" />
		</Event>
	</EventSystem>
	
	<GoalSet id="0">
		<Goal type="goal_element" ... />
		<Goal type="goal_element" ... />
		<Goal type="goal_element" ... />
	</GoalSet>
	
	<State name="A" final="0" >
		<GoalSelector type="goal_selector_element" ... />
		<VelComponent type="velocity_component_element" ... />
		<Action type="action_element" ... />
	</State>
	
	<State name="B" final="1" >
		<GoalSelector type="goal_selector_element" ... />
		<VelComponent type="velocity_component_element" ... />
		<Action type="action_element" ... />
	</State>
	
	<Transition from="Home" >
		<Condition type="condition_element" ... />
		<Target type="transition_target_element" .../>
	</Transition>
</BFSM>
@endcode

The major elements of the behavior specification are:
	- The (optional) EventSystem (@ref sec_behaveEvent)
	- Specification of goals (@ref sec_behaveGoals)
	- State definitions (@ref sec_behaveStates)
	- Transition definitions (@ref sec_behaveTransitions)
	
Many of these sections are comprised of one or more %Menge elements. Each section is detailed below.

@section sec_behaveEvent The Event System

%Menge has an Event infrastructure. Events are the "hand of God" in %Menge. Through events, arbitrary
components can affect each other. In the subway example above, the arrival of the train is treated
as an event; we model the perception of the train and cause the agent to move from its current
state (listening to music) to a new state (running for the train).

As with all the major components in %Menge, the event system has been architected to be general and
extensible. @ref page_EventSystem provides the details on how the event system works.

@section sec_behaveGoals Defining Goals

@section sec_behaveStates Defining States

@section sec_behaveTransitions Defining Transitions

@note Don't forget to cover these.	
- Note the difference between event target and transition target.
- There is an abbreviated form of Transition where the target is implied.