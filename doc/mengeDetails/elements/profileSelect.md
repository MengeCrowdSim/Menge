Profile Selector {#page_ProSelect}
=================

@section sec_PS_overview Overview

A Profile Selector is a simple machine.  It is initialized with a set of profiles and each implementation encodes a particular algorithm for assigning a profile from the set to each agent in a sequence.  The algorithm can be as simple or as complex as the scenario requires.  For example, one implementation could assign profiles based on a round-robin strategy, another could use a purely random distribution.  Once the profile is selected, the profile's defined properties (see [agent profiles](@ref sec_sceneAgentProfile)) (including their variability) are applied to the agent.  The agents are processed serially and profile selectors do not need to make any effort to be thread safe.

There are two important methods for an implementation of a ProfileSelector to implement: [cacheProfiles](@ref Menge::Agents::ProfileSelector::cacheProfiles) and [getProfile](@ref Menge::Agents::ProfileSelector::getProfile).

[cacheProfiles](@ref Menge::Agents::ProfileSelector::cacheProfiles) allows the implementation of the profile selector to map its named profiles to the instantiated profiles.  The provided map includes all profiles in the scene specification; the class should convert names specified in the project XML to pointers to the proper profiles.  If the profile selector is expecting a named profile that cannot be found in the map, this is considered to be a fatal error; the problem should be logged and the appropriate value returned.

[getProfile](@ref Menge::Agents::ProfileSelector::getProfile) implements the algorithm of the selector.  This method is called once for each agent that is created.  A pointer to one of the cached profiles should be returned to be applied to the new agent.

@subsection subsec_PS_implementations Implementations

This version of Menge includes the following implementations of the profile selector:

- The [constant profile selector](@ref Menge::Agents::ConstProfileSelector). See the class 
documentation for details on its semantics and how to instantiate this profile selector.
- The [random profile selector](@ref Menge::Agents::ProfileSelectorRandom).  See the class 
documentation for details on its semantics and how to instantiate this profile selector.

Future versions of Menge will include profile selectors which can operate on sets of profiles, e.g., assigning profiles randomly or based on weighted distributions.
