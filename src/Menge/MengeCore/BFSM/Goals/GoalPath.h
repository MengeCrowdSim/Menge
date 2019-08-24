/*
 Menge Crowd Simulation Framework

 Copyright and trademark 2019 Sean Curtis

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
 @file    GoalPath.h
 @brief   The definition of a BFSM goal that moves along a path.
 */

#pragma once

#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/BFSM/Goals/GoalFactory.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/CoreConfig.h"

namespace Menge {
namespace BFSM {

/*!
 @brief    A goal that moves with constant speed over a pre-determined path.

 The %PathGoal is similar to other goals in that it has a geometric region. However, that region
 moves with a constant speed a long a piecewise-linear trajectory. The geometry, speed, and
 trajectory are defined in the behavior specification. The path doesn't re-orient while following
 the path -- it slides along the path with a fixed orientation with respect to the world frame.

 <h3>XML specification</h3>

 To specify a path goal, use the following syntax:

 @code{xml}
 <Goal type="path" speed="float" closed="int" shape={"point"|"circle"|"aabb"|"obb"}
       [shape parameters]>
   <Point x="float" y="float"/>
   ...
   <Point x="float" y="float"/>
 </Goal>
 @endcode

 The parameters have the following interpretations:

   - `speed` is the speed that the goal moves along the trajectory.
   - `closed` is a boolean (0 = false, everything else = true). If true, the path points are
     interpreted as a closed path (i.e., there is an implicit edge between the first and last
     way points. If false, the goal will move back and forth along the path.
   - `shape` is a string indicating the goal geometry. For the given shape type, it must also
     include the parameters that specify that shape (see PointGoal, CircleGoal, AABBGoal, or OBBGoal
     for details on those parameters).
   - The path is specified by a sequence of 2D points. The goal begins the simulation at the the
     first way point `<Point>` value and moves with constant speed along the straightline paths
     connecting sequential points. There must be *at least* two points.
 */
class MENGE_API PathGoal final : public Goal {
 public:
  // Inherits docs from Goal::getStringId().
  std::string getStringId() const final { return NAME; }

  // Inherits docs from Goal::moves().
  bool moves() const final { return true; }

  // Inherits docs from Goal::move().
  void move(float time_step) final;

  /** @brief  Reports the position of the origin of the geometry frame, Go, measured and
              expressed in the world frame.
   */
  const Math::Vector2& origin() const { return _p_WGo; }

  /*! The unique identifier used to register this type with run-time components. */
  static const std::string NAME;

 private:
  friend class PathGoalFactory;

  // Configures the path goal. The goal takes ownership of the given geometry.
  void configure(float speed, bool closed, Geometry2D* geometry, std::vector<Math::Vector2>&& path);

  // Set velocity based on current position and current waypoint. Assumes a non-trivial distance
  // between current way point position and current goal position.
  void set_velocity();

  // See documentation in Goal::worldToGeometry();
  Math::Vector2 worldToGeometry(const Math::Vector2& p_WQ) const final {
    // This transform is translation only, because the goal doesn't change orientation.
    return p_WQ - _p_WGo;
  }

  // See documentation in Goal::geometryToWorld();
  Math::Vector2 geometryToWorld(const Math::Vector2& p_GQ) const final {
    // This transform is translation only, because the goal doesn't change orientation.
    return p_GQ + _p_WGo;
  }

  // A sequence of points (each measured and expressed in the world frame) which defines the
  // piecewise-linear goal path.
  std::vector<Math::Vector2> _way_points;
  // The "position" (or origin) of the geometry's frame measured and expressed in the world frame.
  Math::Vector2 _p_WGo;
  // The current velocity of the goal.
  Math::Vector2 _v_W;
  bool _closed{};
  // The direction we're moving along the path -- if positive, it's advancing through the waypoints.
  // If negative, it's moving backwards through the way points.
  int _path_direction{1};
  int _current_waypoint{-1};
  float _speed{0.0};
};

/*!
 @brief    Factory for the PathGoal.
 */
class MENGE_API PathGoalFactory : public GoalFactory {
 public:
  PathGoalFactory();

  // Inhertis docs from GoalFactory::name().
  const char* name() const override { return PathGoal::NAME.c_str(); }

  // Inherits docs from GoalFactory::description().
  const char* description() const override {
    return "An agent goal consisting of a specified geometry shape that moves along a fixed path";
  };

 protected:
  /*!
   @brief    Create an instance of this class's goal.

   @returns    A pointer to a newly instantiated Goal class.
   */
  Goal* instance() const { return new PathGoal(); }

  // Inherits docs from GoalFactory::setFromXML().
  bool setFromXML(Goal* goal, TiXmlElement* node, const std::string& behaveFldr) const override;

  /*!
   @brief    The identifier for the "speed" float attribute.
   */
  size_t _speedId;

  /*!
   @brief    The identifier for the "closed" bool attribute.
   */
  size_t _closedId;

  /*!
   @brief    The identifier for the "shape" stringattribute.
   */
  size_t _shapeId;
};
}  // namespace BFSM
}  // namespace Menge
