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
 @file       Goal.h
 @brief      Defines the goal classes for agent behaviors.
 */

#ifndef __GOALS_H__
#define __GOALS_H__

#include "MengeCore/Agents/PrefVelocity.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/CoreConfig.h"
#include "MengeCore/Math/Geometry2D.h"
#include "MengeCore/MengeException.h"
#include "MengeCore/PluginEngine/Element.h"
#include "MengeCore/Runtime/ReadersWriterLock.h"

// forward declaration
class TiXmlElement;

namespace Menge {

namespace Agents {
class PrefVelocity;
}

namespace BFSM {

// Forward declaration
class GoalSet;

/*!
 @brief    Exception class for BFSM goals.
 */
class MENGE_API GoalException : public virtual MengeException {
 public:
  /*!
   @brief    Default constructor.
   */
  GoalException() : MengeException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  GoalException(const std::string& s) : MengeException(s) {}
};

/*!
 @brief    Exception thrown when the goal has an error which cannot be recovered from.
 */
class GoalFatalException : public GoalException, public MengeFatalException {
 public:
  /*!
   @brief    Default constructor.
   */
  GoalFatalException() : MengeException(), GoalException(), MengeFatalException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  GoalFatalException(const std::string& s)
      : MengeException(s), GoalException(), MengeFatalException() {}
};

/////////////////////////////////////////////////////////////////////////////////////

/*!
 @brief    The base, abstract class defining goals.

 A goal defines a region in space that an agent should attempt to reach. The goal can be stationary
 or can move.

 The geometry associated with the region (e.g., point, sphere, AABB, OBB, etc.) is defined in
 the frame G. Typically, to express it in the world frame, it must be transformed to the world
 frame by X_WG. In the case of stationary goals, it is assumed that X_WG = I. However, if the goal
 can move, X_WG will not be the identity. Specifically, moving goals define the location of the
 *origin* of frame G measured and expressed in the world frame W. There is an important implication
 of this that we will illustrate.

 Assume we have a moving circle goal whose center C is defined to be at (10, 10). That is measured
 and expressed in the geometry frame, p_GC. The goal's movement logic places the origin of the G
 frame at the position p_WQ = (-1, -1). In the world frame, the center of the sphere will actually
 be located at (9, 9) in the world frame. Therefore, if a goal is to move along a path, *centered*
 on that path, it must be defined as being centered on the origin of frame G.

 @warning moving goals can only experience *translational* movement. For now, attempting to rotate a
 moving goal will lead to unintended and unexpected results.
 */
class MENGE_API Goal : public Element {
 public:
  /*!
   @brief    Basic constructor
   */
  Goal()
      : Element(),
        _weight(1.f),
        _capacity(MAX_CAPACITY),
        _id(-1),
        _goalSet(0x0),
        _population(0),
        _geometry(0x0) {}
  // -1 is the biggest value for size_t

 protected:
  /*!
   @brief    Destructor.
   */
  virtual ~Goal();

 public:
  /*!
   @brief    Used by the plugin system to know what artifacts to associate with agents of this type.
   
   Every sub-class of must return a globally unique value if it should be associated with unique
   artifacts.
   */
  virtual std::string getStringId() const = 0;

  /*!
   @brief  Returns true if this goal moves w.r.t. time.  Sub-classes for moving goals should
   override this.
   */
  virtual bool moves() const { return false; }

  // TODO(curds01): Have this method return a boolean indicating if actualy movement has taken
  //  place. This is merely an optimization for those velocity components that can save the effort
  //  if there exists a moving goal that hasn't actually moved.
  /*!
   @brief  Gives the goal the chance to update its position. This shouldn't do anything if 
           moves() return false.

   @param  time_step      The amount of time to advance the goal's position.
   */
  virtual void move(float time_step) {}

  /*!
   @brief    Reports the *squared* distance from a point Q to the goal.

   @param    p_WQ      The query point Q, measured and expressed in the world frame.
   @returns  The squared distance from the point to the goal.
   */
  float squaredDistance(const Math::Vector2& p_WQ) const {
    return _geometry->squaredDistance(worldToGeometry(p_WQ));
  }

  /*!
   @brief    Set the preferred velocity directions w.r.t. the goal: left, right, and preferred.

   The Agents::PrefVelocity class represents a span of velocities that will reach the goal. For a
   goal that covers a 2D region, the directions in the Agents::PrefVelocity should span the arc
   subtended by the goal from the query point's perspective. Furthermore, it should have sufficient
   clearance for a disk with the given radius to pass through. This should be overridden by
   subclasses to account for their unique geometry.

   The arc subtends an angle formed by a cone.  The apex of the cone is at the point `q`. The legs
   of the cone move from the apex in directions _towards_ the goal. The legs bound the minkowski sum
   of the goal geometry with a circle of radius `r`.

   There is a degenerate case when the cone apex, `q`, lies _inside_ the goal geometry. Directions
   are now ill-defined.  The goal can assign _any_ preferred velocity it considers appropriate as
   long as:

   -# The three directions (left, right, and preferred) are all valid unit vectors).
   -# The target point is `q`.

   @param    p_WQ          The query point measured and expressed in the world frame.
   @param    r             The radius of clearance.
   @param    directions    An instance of Agents::PrefVelocity.
   */
  void setDirections(const Math::Vector2& p_WQ, float r, Agents::PrefVelocity& directions) const {
    // Currently assuming that moving goals don't turn -- so, I don't need to transform the
    // direction vectors from the G frame to the W frame.
    _geometry->setDirections(worldToGeometry(p_WQ), r, directions);
    // TODO(curds01): Right now the pref velocity also has its *target* point T set. This is being
    // set as p_GT and I need it as p_WT. Handle this more gracefully.
    directions.setTarget(geometryToWorld(directions.getTarget()));
  }

  // TODO: Delete this function= transition uses it determine distance to goal
  //    I would be better off simply returning "squared distance to goal"
  /*!
   @brief    Returns the closest "target" point in the goal to the given query point.

   A "valid" target point is the nearest point to the query point that is sufficiently inside the
   goal region that a disk with the given radius is completely inside the goal. It need not be
   literally the *best* value, an approximation is sufficient.

   In the case where the goal region is too small to hold the agent, then the "deepest" point in the
   region is a good approximation.

   @param    p_WQ     The query point measured and expressed in the world frame.
   @param    r        The radius of clearance.
   @returns  A 2D position representing the target point.
   */
  Math::Vector2 getTargetPoint(const Math::Vector2& p_WQ, float r) const {
    return geometryToWorld(_geometry->getTargetPoint(worldToGeometry(p_WQ), r));
  }

  /*!
   @brief    Return the centroid of the goal.
   */
  Math::Vector2 getCentroid() const { return geometryToWorld(_geometry->getCentroid()); }

  /*!
   @brief    Reports if the goal still has capacity.

   @returns  True if the goal has remaining capacity, false otherwise.
   */
  bool hasCapacity() const;

  /*!
   @brief    Inform the goal that it has been assigned.

   @param    agent    The agent that has been assigned to this goal.
   */
  void assign(const Agents::BaseAgent* agent);

  /*!
   @brief    Inform the goal that an assignment has been removed.
   */
  void free();

  /*!
   @brief    Sets the goal's geometry.

   @param    geometry    The geometry for this goal; the goal takes *ownership.
   */
  void setGeometry(Math::Geometry2D* geometry);

  /*!
   @brief    Sets this goal's goal set.

   @param    goalSet    A pointer to the goal set to which this goal belongs.
   */
  inline void setGoalSet(GoalSet* goalSet) { _goalSet = goalSet; }

  /*!
   @brief    Returns a pointer to this agent's goal set.

   A dynamic goal (such as would be created by a hold-position goal will return NULL, which means it
   isn't owned by a goal set and should be destroyed when finished.

   // TODO: Handle goal sharing and persistence.

   @returns  A pointer to the goal set to which this goal belongs.
   */
  inline GoalSet* getGoalSet() { return _goalSet; }

  /*!
   @brief    Returns a const pointer to this agent's goal set.

   A dynamic goal (such as would be created by a hold-position goal will return NULL, which means it
   isn't owned by a goal set and should be destroyed when finished.

   // TODO: Handle goal sharing and persistence.

   @returns  A const pointer to the goal set to which this goal belongs.
   */
  inline const GoalSet* getGoalSet() const { return _goalSet; }

  /*!
   @brief    Sets the goal's weight.

   @param    weight    The weight value for this goal.
   */
  inline void setWeight(float weight) { _weight = weight; }

  /*!
   @brief    Retrieves the goal's weight.

   @returns  This goal's weight.
   */
  inline float getWeight() const { return _weight; }

  /*!
   @brief    Sets the goal's capacity.

   @param    capacity    The target capacity for this goal.
   */
  inline void setCapacity(size_t capacity) { _capacity = capacity; }

  /*!
   @brief    Returns this goal's capacity.

   @returns  The goal's capacity.
   */
  inline size_t getCapacity() const { return _capacity; }

  /*!
   @brief    Sets the id of the goal.

   @param    id    The goal's new id.
   */
  inline void setID(size_t id) { _id = id; }

  /*!
   @brief    Gets the id of the goal.

   @returns  The goal's id.
   */
  inline size_t getID() const { return _id; }

  /*!
   @brief    Provides access to the underlying geometry.

   @returns  The underlying geometry structure.
   */
  inline const Math::Geometry2D* getGeometry() const { return _geometry; }

  /*!
   @brief    The maximum capacity any goal can hold.
   */
  static const size_t MAX_CAPACITY;

  friend class GoalSet;

 protected:
  // For a point Q, maps the position vector measured and expressed in the world frame (p_WQ) to the
  // geometry frame (p_GQ). For goals that are stationary (i.e., moves() return false)
  // the transform X_WG is the identity. For moving goals, they need to apply this transform.
  virtual Math::Vector2 worldToGeometry(const Math::Vector2& p_WQ) const { return p_WQ; }

  // For a point Q, maps the position vector measured and expressed in the geometry frame (p_GQ) to
  // the world frame (p_WQ). For goals that are stationary (i.e., moves() return false)
  // the transform X_GW is the identity. For moving goals, they need to apply this transform.
  virtual Math::Vector2 geometryToWorld(const Math::Vector2& p_GQ) const { return p_GQ; }

  /*!
   @brief    The relative weight of this goal.
   */
  float _weight;

  /*!
   @brief    The maximum capacity of this goal.
   */
  size_t _capacity;

  /*!
   @brief    The id of this goal in its goal set.
   */
  size_t _id;

  /*!
   @brief    The goal set to which this goal belongs.
   */
  GoalSet* _goalSet;

  /*!
   @brief    The current "population" of this goal.

   In other words, it is the number of times "getGoalPoint" has been called on this goal.
   */
  mutable size_t _population;

  /*! @brief  The underlying geometry for the goal. */
  Math::Geometry2D* _geometry;

  /*!
   @brief    The lock to maintain readers-writer access to the structure which control available
            goals.
   */
  ReadersWriterLock _lock;
};

/*!
 @brief    Parses a TinyXML element containing a goal description

 @param    node          The TinyXML element
 @param    behaveFldr    The folder in which the behavior is defined -- all resources are defined
                        relative to this folder.
 @returns  A pointer to the new goal description (NULL if the definition is invalid).
 */
Goal* parseGoal(TiXmlElement* node, const std::string& behaveFldr);

}  // namespace BFSM
}  // namespace Menge
#endif  //__GOALS_H__
