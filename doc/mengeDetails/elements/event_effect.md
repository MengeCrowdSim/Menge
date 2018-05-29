Event Effect {#page_EventEffect}
============

@section sec_EE_overview Overview

A %Menge event is comprised of three components: a trigger, an effect, and a target. The 
Menge::EventEffect is the interface through which the effect of an event is defined. Because %Menge's
event system is intended to be as open as possible,the interface for the event *effect* is quite
simple. The @ref Menge::EventEffect "event effect" works in close conjunction with the
@ref Menge::EventTarget "event target". The effect defines an operation, and a target defines the
operand(s). The interface of the effect reflects this relationship.

There are XXXX importat methods for an implementation of an @ref Menge::EventEffect "EventEffect" to
implment: @ref Menge::EventEffect::isCompatible "isCompatible", @ref Menge::EventEffect::apply
"apply", and @ref Menge::EventEffect::finalize "finalize".

@ref Menge::EventEffect::isCompatible "isCompatible" takes a pointer to a single EventTarget. It
reports if this effect is "compatible" with the given target. This is necessary because of the
openness of the event system. One example could change the properties of a state, another event
could change the property of an agent. Those two effects must be different. The work required to
change the property of a state is different from the work required to change the property of an
agent. That means, those effects must be partnered with implementations of EventTarget that provide
the right *kind* of target object. This class will detect and report this compatibility.

@ref Menge::EventEffect::apply "apply" is the "operation" of the effect. Given a
compatible EventTarget, this method performs the work necessary to modify the target.

@ref Menge::EventEffect::finalize "finalize" gives the effect the chance to do any work between
parsing/initialization and execution. This method is invoked after the simulator has been fully
invoked. It gives the effect instance to complete its initialization if some part of its state
depends on other entities in the simulator.

@subsection subsec_EE_implementations Implementations

This version of %Menge includes the following implementations of event effects:

- The @ref Menge::SetAgentPropertyEffectFactory "*set* agent property effect" modifies agent
properties by applying a new value (in a similar way as the @ref Menge::BFSM::SetPropertyActFactory
"set property action" works). It works with Menge::AgentEventTarget instances.
- The @ref Menge::OffsetAgentPropertyEffectFactory "*offset* agent property effect" modifies agent
properties by adding delta to the current value (in a similar way as the 
@ref Menge::BFSM::OffsetPropertyActFactory "offset property action" works).
It works with Menge::AgentEventTarget instances.
- The @ref Menge::ScaleAgentPropertyEffectFactory "*scale* agent property effect" modifies agent
properties by multiplying a factor to the current value (in a similar way as the 
@ref Menge::BFSM::ScalePropertyActFactory "scale property action" works).
It works with Menge::AgentEventTarget instances.
- The @ref Menge::EventEffectAgentState "agent state effect" will change the state an agent is in.
It works with Menge::AgentEventTarget instances.

