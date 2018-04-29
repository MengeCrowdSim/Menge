/*
 Menge Crowd Simulation Framework

 Copyright and trademark 2012-17 University of North Carolina at Chapel Hill

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0
 or
    LICENSE.txt in the root of the Menge repository.

 Any questions or comments should be sent to the authors menge@cs.unc.edu

 <http://gamma.cs.unc.edu/Menge/>
*/

/*!
 @file      SpatialQuery.h
 @brief     The base class for all objects which support agent spatial queries including: k-nearest
            agent neighbor, k-nearest obstacles and visibility queries.
 */
#ifndef __SPATIAL_QUERY_H__
#define __SPATIAL_QUERY_H__

#include "MengeCore/Agents/Obstacle.h"
#include "MengeCore/Agents/SpatialQueries/ProximityQuery.h"
#include "MengeCore/CoreConfig.h"
#include "MengeCore/Math/Vector2.h"
#include "MengeCore/MengeException.h"
#include "MengeCore/PluginEngine/Element.h"

#include <vector>

namespace Menge {

namespace Agents {

/*!
 @brief    Exception class for spatial query operations
 */
class MENGE_API SpatialQueryException : public virtual MengeException {
 public:
  /*!
   @brief    Default constructor.
   */
  SpatialQueryException() : MengeException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  SpatialQueryException(const std::string& s) : MengeException(s) {}
};

/*!
 @brief    The fatal spatial query exception.
 */
class MENGE_API SpatialQueryFatalException : public SpatialQueryException,
                                             public MengeFatalException {
 public:
  /*!
   @brief    Default constructor.
   */
  SpatialQueryFatalException() : MengeException(), SpatialQueryException(), MengeFatalException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  SpatialQueryFatalException(const std::string& s)
      : MengeException(s), SpatialQueryException(), MengeFatalException() {}
};

// FORWARD DECLARATIONS
class BaseAgent;

/*!
 @brief    The base class for performing spatial queries.

 A spatial query implementation has to support several operations:
 - Given a maximum distance, find agents within that distance from a point
 - Given a maximum distance, find obstacles within that distance from a point
 - Given two points and a width, determines if a straight link between them is collision free (the
   link has the given width).
 - Updates its internal structure based on current agent state.
 // TODO: This should be done via a task.
 - The spatial query is also responsible for visiblity testing Spatial Queries work in tandem with a
   ProximityQuery. The query itself is agnostic of how the results are filtered.
 */
class MENGE_API SpatialQuery : public Element {
 public:
  /*!
   @brief    Constructor.
   */
  SpatialQuery();

 protected:
  /*!
   @brief    Virtual destructor.
   */
  virtual ~SpatialQuery() {}

  // Agent operations
 public:
  /*!
   @brief      Define the set of agents on which query class will operate.
   */
  virtual void setAgents(const std::vector<BaseAgent*>& agents) = 0;

  /*!
   @brief      Allows the spatial query structure to update its knowledge of the agent positions.
   */
// TODO: Replace this with a task.
  virtual void updateAgents() = 0;

  /*!
   @brief      Adds an obstacle to the internal list of the spatial query

   @param    obs    The obstacle to insert into the spatial query data structure.
   */
  virtual void addObstacle(Obstacle* obs);

  /*!
   @brief      Returns the collected obstacles of the spatial query for use in visualization.

   @returns    A const reference to the obstacle set
   */
  const std::vector<Obstacle*>& getObstacles() const { return _obstacles; }

  /*!
   @brief      Performs an agent based proximity query
   @param      query          A pointer for the proximity query to be performed
   */
  virtual void agentQuery(ProximityQuery* query) const = 0;

  // Obstacle operations

  /*!
   @brief      Do the necessary pre-computation to support obstacle definitions.
   
   Now uses the internal obstacle representation
   */
  virtual void processObstacles() = 0;

  /*!
   @brief      Performs an obstacle based proximity query.
   @param      query    A pointer for the proximity query to be performed.
   */
  virtual void obstacleQuery(ProximityQuery* query) const = 0;

  /*!
   @brief  Reports if an agent can traverse the straight-line path from `q1` to `q2`.

   This query performs a per-obstacle test to determine if the obstacle limits traversibility.
   This test is closely related to an intersection test between a line segment (an individual
   obstacle) and a capsule (the traversible link between `q1` and `q2` convolved with a disk of
   the given `radius`). Generally, if the line segment intersects the link, the link is _not_
   traversible. However, this isn't universally true.

   First, obstacles have "sides". An obstacle doesn't obstruct if the path from `q1` to `q2
   passes from _inside_ the obstacle to _outside_ the obstacle.

   Second, even an agent whose center lies _outside_ an obstacle (but otherwise intersects the
   obstacle) may not be considered obstructed. This query link represents the path of an agent.
   We cannot guarantee that the starting position is collision free. As such, an obstacle might
   intersect the capsule in an otherwise meaningless way. These meaningless collisions do not
   prevent the link from being traversible.

   To be a "meaningless" collision the following must be true:

     - The line segment must intersect a circle centered at `q1` with the given `radius`.
     - The direction of the link (`q2 - q1`) must point _away_ from the line segment.

  @param  q1      The start point of the link to be tested.
  @param  q2      The end point of the link to be tested.
  @param  radius  The radius of the agent to traverse the link.
  @returns  True if the link is deemed a traversible path.
  */
  virtual bool linkIsTraversible(const Math::Vector2& q1, const Math::Vector2& q2,
                                 float radius) const = 0;

  /*!
   @brief      Queries the visibility between two points within a specified radius.

   @param      q1       The first point between which visibility is to be tested.
   @param      q2       The second point between which visibility is to be tested.
   @param      radius   The radius within which visibility is to be tested.
   @returns    True if q1 and q2 are mutually visible within the radius.
   */
  virtual bool queryVisibility(const Math::Vector2& q1, const Math::Vector2& q2,
                               float radius) const = 0;

  /*!
   @brief    Sets the spatial query to include visibility in finding agent neighbors.

   @param    state    If true, the spatial query has to consider visibility.
   */
  virtual void setNeighborVisibleTest(bool state) {}

  /*!
   @brief    Sets the test visibility status of the neighbor functions.

   @param    status    The new status of the test visibility property.
   */
  inline void setTestVisibility(bool status) { _testVisibility = status; }

  /*!
   @brief    Reports the test visibility property of the spatial query instance.

   @returns    A boolean reporting if the spatial query instance uses visibility tests when computing
              nearby agents and obstacles (true) or not (false).
   */
  inline bool getTestVisibility() const { return _testVisibility; }

 protected:
  /*!
   @brief    Dictates if visibility queries should be used (true) for neighbor queries or not
            (false).
   */
  bool _testVisibility;

  /*!
   @brief    An internal central list of obstacles.
   */
  std::vector<Obstacle*> _obstacles;
};
}  // namespace Agents
}  // namespace Menge
#endif  // __SPATIAL_QUERY_H__
