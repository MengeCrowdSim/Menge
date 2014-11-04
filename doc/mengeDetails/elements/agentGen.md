Agent Generators {#page_AgentGen}
=================

@section sec_AG_overview Overview

As the name suggest, an Agent Generator, generates agents.  It is responsible for determining the total number of agents in the `<AgentGroup>` and their positions.  In addition to whatever parameters the particular generator requires, all generators can be use a *noise* parameter.  The calculated agent position can be perturbed by a noise generator.

The noise generation is a simple displacement of the computed agent position.  The direction of displacement is random with all directions equally likely.  The magnitude of displacement is defined by the given distribution (more details can be seen in the documentation for the [AgentGenerator](@ref Menge::Agents::AgentGenerator) class).

There are two important methods for an implementation of an AgentGenerator to implement: [agentCount](@ref Menge::Agents::AgentGenerator::agentCount) and [computePos](@ref Menge::Agents::AgentGenerator::computePos).

[agentCount](@ref Menge::Agents::AgentGenerator::agentCount) reports the number of agents that will be created based on the parameters in the XML specification.  

[computePos](@ref Menge::Agents::AgentGenerator::computePos) takes as a parameter the index of one of the generated agents (it should lie in the range [0, N) where N is the value returned by [agentCount](@ref Menge::Agents::AgentGenerator::agentCount).  The algorithm should have a well-defined value for agent i.

@subsection subsec_AG_implementations Implementations

This version of Menge includes the following implementations of the agent generator:

- The [explicit generator](@ref Menge::Agents::ExplicitGenerator).  
- The [rectangular grid generator](@ref Menge::Agents::RectGridGenerator).
- The [hexagonal lattice generator](@ref Menge::Agents::HexLatticeGenerator)
	
See the class documentation for details on their semantics and how to instantiate this profile selector.

Future versions of Menge will include profile selectors which can operate on sets of profiles, e.g., assigning profiles randomly or based on weighted distributions.
