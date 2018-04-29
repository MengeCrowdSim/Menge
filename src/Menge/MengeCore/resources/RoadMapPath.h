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
 @file    RoadMapPath.h
 @brief    A path along a roadmap.
 */

#ifndef __ROADMAP_PATH_H__
#define __ROADMAP_PATH_H__

#include "MengeCore/mengeCommon.h"

namespace Menge {

class RoadMapPath;

/*!
 @brief    A map from agent id to its path.
 */
typedef HASH_MAP<size_t, RoadMapPath*> PathMap;

namespace Agents {
class BaseAgent;
class PrefVelocity;
}  // namespace Agents

namespace BFSM {
class Goal;
}

/*!
 @brief    A path on a roadmap between vertices
 */
class MENGE_API RoadMapPath {
 public:
  /*!
   @brief    Constructor

   @param    pointCount    The number of waypoints in the path.
   */
  RoadMapPath(size_t pointCount);

  /*!
   @brief    Destructor.
   */
  ~RoadMapPath();

  /*!
   @brief    Sets the position of the ith waypoint.

   @param    i      The index of the waypoint.
   @param    pos    The location of the ith waypoint.
   */
  void setWayPoint(size_t i, const Math::Vector2& pos);

  /*!
   @brief    Sets the ultimate goal.

   @param    goal    The ultimate goal
   */
  inline void setGoalPos(const BFSM::Goal* goal) { _goal = goal; }

  /*!
   @brief      Sets the direction of the preferred velocity (and target).

   @param      agent     The agent to compute the preferred direciton for.
   @param      pVel      The preferred velocity to set.
   @returns    false if the preferred velocity could not be set and the agent must plan a new path.
   */
  bool setPrefDirection(const Agents::BaseAgent* agent, Agents::PrefVelocity& pVel);

  /*!
   @brief    Reports the number of waypoints in the path.

   @returns  The number of way points. This does *not* include the final goal point. So, at its
            simplest, this could be zero.
   */
  inline size_t getWayPointCount() const { return _wayPointCount; }

  /*!
   @brief    Returns the position of the ith waypoint.

   The index i is only validated in debug mode with an assertion. The ultimate goal is not included
   in the count.

   @param    i    The index of the desired waypoint.
   @returns  The position of the ith waypoint.
   */
  Math::Vector2 getWayPoint(size_t i) const;

  /*!
   @brief    Returns the ultimate goal.

   @returns  A pointer to the ultimate goal.
   */
  const BFSM::Goal* getGoal() const { return _goal; }

  /*!
   @brief    Returns the current target waypoint index

   @returns  The index of the target waypoint.
   */
  inline size_t getTargetID() const { return _targetID; }

 protected:
  /*!
   @brief    The ultimate goal.
   */
  const BFSM::Goal* _goal;

  /*!
   @brief    The last valid position -- validity means the target goal was visible.
   */
  Math::Vector2 _validPos;

  /*!
   @brief    The index of the current target.
   */
  size_t _targetID;

  /*!
   @brief    The number of way points in the path.
   */
  size_t _wayPointCount;

  /*!
   @brief    The way points along the path.
   */
  Math::Vector2* _wayPoints;
};
}  // namespace Menge
#endif  // __ROADMAP_PATH_H__
