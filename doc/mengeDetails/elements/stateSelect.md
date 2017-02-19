State Selector {#page_StateSelect}
=================

@section sec_SS_overview Overview

A StateSelector is a simple machine.  Provided a set of states defined in the BFSM (see @ref page_BehaveSpec), it selects an *initial* state for each agent in the corresponding `<AgentGroup>`.  The algorithm can be as simple or as complex as the scenario requires.  For example, at its simplest, all agents in an `<AgentGroup>` could start from the same [BFSM State](@ref Menge::BFSM::State).  Alternatively, their initial States could be drawn from a set of BFSM States, either with uniform probability or with weighted probability.  Then again, the initial state could be based on the agents initial position.

The StateSelector works in conjunction with the [ProfileSelector](@ref page_ProSelect) and [AgentGenerator](@ref page_AgentGen) elements to define an `<AgentGroup>`.  An `<AgentGroup>` can only contain a single StateSelector instance.  The order of the elements in the `<AgentGroup>` tag do not matter.  When the StateSelector is called for an agent, its internal state will have been completely initialized as per the [ProfileSelector](@ref page_ProSelect).

There is a single method which must be implemented by a novel implementation of the StateSelector: [getState](@ref Menge::Agents::StateSelector::getState).

[getState](@ref Menge::Agents::StateSelector::getState) reports the *name* of the BFSM State that this agent will start in.  The name is only validated after the behavior specification has been read.  The name returned by the StateSelector must be a valid BFSM State name, otherwise an error is returned and the simulation ends.

@subsection subsec_SS_implementations Implementations

This version of Menge includes a single implementation of the StateSelector:

- The [const selector](@ref Menge::Agents::ConstStateSelector).

See the class documentation for details on its semantics and how to instantiate this profile selector.

Future versions of Menge will include additional StateSelector implementations.
