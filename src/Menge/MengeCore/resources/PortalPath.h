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

#ifndef __PORTAL_PATH_H__
#define __PORTAL_PATH_H__

/*!
 @file      PortalPath.h
 @brief     The definition of a path through a navigation mesh.
            Each leg of the path is defined by a portal (the width of a shared edge.)
 */

#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/mengeCommon.h"
#include "MengeCore/resources/NavMesh.h"
#include "MengeCore/resources/Route.h"

namespace Menge {

// Forward Declaration
class NavMeshLocalizer;
class PathPlanner;
namespace Agents {
class BaseAgent;
}

/*!
 @brief    The definition of a path through space comprising of a sequence of portals.

 The path is repsonsible for computing instantaneous preferred velocity for an agent based on a
 PortalRoute and an optimized path through the portals.
 */
class MENGE_API PortalPath {
 public:
  /*!
   @brief    Constructor

   @param    startPos      The 2D position where the path starts
   @param    goal          The goal (whose centroid lies in the final polygon).
   @param    route          The route the path follows
   @param    agentRadius    The radius of the given agent.
   */
  PortalPath(const Math::Vector2& startPos, const BFSM::Goal* goal, const PortalRoute* route,
             float agentRadius);

  /*!
   @brief    Destructor
   */
  ~PortalPath();

  /*!
   @brief    Sets the directions in the preferred velocity from the path

   @param    agent          The agent for which the preferred direction should be computed.
   @param    headingCos    The cosine of the maximum allowable angular deviation of heading (between
                          planned and realized) before a new funnel algorithm is triggered to
                          improve the path.
   @param    pVel          The preferred velocity whose directions are to be set.
   */
  void setPreferredDirection(const Agents::BaseAgent* agent, float headingCos,
                             Agents::PrefVelocity& pVel);

  /*!
   @brief    Updates the location of the agent relative to the nav mesh.

   @param    agent        The agent whose location is updated.
   @param    navMesh      The navmesh for making queries.
   @param    localizer    The nav mesh localizer for identifying locations.
   @param    planner      The nav mesh path planner for creating new routes.
   @returns  The index of the node the agent is in.
   */
  unsigned int updateLocation(const Agents::BaseAgent* agent, const NavMeshPtr& navMesh,
                              const NavMeshLocalizer* localizer, PathPlanner* planner);

  /*!
  @brief    Updates the path based on a goal position that *may* have moved. 

  @param    agent       The agent to update that path for.
  @param    goal_node   The id of the node in which the goal lies.
  @param    planner     The nav mesh path planner for creating new routes.
  */
  void updateGoalLocation(const Agents::BaseAgent* agent, unsigned int goal_node,
                          PathPlanner* planner);

  /*!
   @brief    Reports the node the agent is currently in.

   @returns  The index of the node associated with this location. If the location is not on a node,
            PortalPath::NO_NODE is returned.
   */
  unsigned int getNode() const;

  /*!
   @brief    Reports the number of way points in the path (*not* including the goal)

   @return    The number of points (*not* including the goal).
   */
  inline size_t getWayPointCount() const { return _route->getPortalCount(); }

  /*!
   @brief    Returns the position of the ith way point.

   @param    i    The index of the desired way point.
   @returns  The 2d position of the desired way point.
   */
  Math::Vector2 getWayPoint(size_t i) const;

  /*!
   @brief    Returns the goal.

   @returns  The goal.
   */
  inline const BFSM::Goal* getGoal() const { return _goal; }

  /*!
   @brief    Returns the centroid of the goal.

   @returns  The goal's centroid.
   */
  inline Math::Vector2 getGoalCentroid() const { return _goal->getCentroid(); }

  /*!
   @brief    Returns the identifier for the destination node on the path.

   @returns  The destination node's index.
   */
  inline unsigned int getEndNode() const { return _route->getEndNode(); }

  /*!
   @brief    Returns the id of the node traveled for the ith way point.

   @param    i    The index of the ith way point.
   @returns  The identifier of the node the agent would be travelling on while making progress toward
            the ith point.
   */
  unsigned int getNode(size_t i) const;

  /*!
   @brief    Returns the index of the current portal (the portal the agent is currently headed
            towards.

   @returns  The index of the current portal.
   */
  inline size_t getCurrentPortal() const { return _currPortal; }

  /*!
   @brief    Reports the number of portals in the path)

   @return    The number of portals.
   */
  inline size_t getPortalCount() const { return _route->getPortalCount(); }

  /*!
   @brief    Returns the ith portal

   @param    i    The index of the desired portal
   @returns  A pointer to the ith portal.
   */
  inline const WayPortal* getPortal(size_t i) const { return _route->getPortal(i); }

  /*!
   @brief    Set the waypoints for all of the indicated way portals.

   @param    start    The index of the first portal to set the crossing point.
   @param    end      The index of the first portal to stop at. In other words, portals in the open
                    interval [start, end) will receive crossing values.  end will not be included.
   @param    p0      The way point.
   @param    dir      The expected direction toward the way point along the path.
   */
  void setWaypoints(size_t start, size_t end, const Math::Vector2& p0, const Math::Vector2& dir);

  /*!
   @brief   Updates the crossings based on a moving goal.

   @param  agent   The agent to whom this path belongs.
   */
  void updateCrossingFromMovingGoal(const Agents::BaseAgent& agent);

 protected:
  /*!
   @brief    The route to follow.
   */
  const PortalRoute* _route;

  /*!
   @brief    The ultimate goal.
   */
  const BFSM::Goal* _goal;

  /*!
   @brief    The index of the way portal currently serving as immediate goal. -1 means it's at the
            end.
   */
  size_t _currPortal;

  /*!
   @brief    Computes the crossing values for each portal

   @param    startPos      The 2D position where the path starts
   @param    agentRadius    The radius of the given agent.
   */
  void computeCrossing(const Math::Vector2& startPos, float agentRadius);

  /*!
   @brief    The sequence of way points.  Some way points will be duplicated.
   */
  Math::Vector2* _waypoints;

  /*!
   @brief    The original direction to the way point.
   */
  Math::Vector2* _headings;

  /*!
   @brief    Something has changed and the path has to replan.

   @param    startPos     The starting position.
   @param    startNode    The starting node (startPos should lie on this node.)
   @param    endNode      The ending node.
   @param    minWidth     The minimum width required for clearance.
   @param    planner      The planner.
   */
  void replan(const Math::Vector2& startPos, unsigned int startNode, unsigned int endNode,
              float minWidth, PathPlanner* planner);
};
}  // namespace Menge
#endif  // __PORTAL_PATH_H__
