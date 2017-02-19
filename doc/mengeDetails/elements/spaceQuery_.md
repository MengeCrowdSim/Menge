Spatial Queries {#page_SpatialQuery}
=================

@section sec_SQ_overview Overview

The Spatial Query element provides a basis for answer various types of spatial queries.  It supports proximity and visibility queries.  It is initialized with the set of agents and obstacles for the scene and, at each time step, is given the opportunity to update its internal state based on the agents' new positions.

The Spatial Query element works in conjunction with the ProximityQuery class.  The spatial query merely provides the mechanism to do O(lg n) search operations, but the ProximityQuery provides the strategy (i.e., determining if it's nearest neighbor, or k-nearest neighbor).  This allows the spatial query to be used for arbitrary queries.  The BaseAgent class sub-classes the ProximityQuery class so that an agent can collect the nearest agents and obstacles.

TODO: Make sure code comments on ProximityQuery are clear -- i.e., what are the semantics of each of the functions that need to be implemented.


There are seven methods that need to be provided in any implementation: 
- [setAgents](@ref Menge::Agents::SpatialQuery::setAgents)
- [updateAgents](@ref Menge::Agents::SpatialQuery::updateAgents)
- [addObstacle](@ref Menge::Agents::SpatialQuery::addObstacle)
- [agentQuery](@ref Menge::Agents::SpatialQuery::agentQuery)
- [processObstacles](@ref Menge::Agents::SpatialQuery::processObstacles)
- [obstacleQuery](@ref Menge::Agents::SpatialQuery::obstacleQuery)
- [queryVisibility](@ref Menge::Agents::SpatialQuery::queryVisibility)

