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
 @brief    The base, abstract class defining goals
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
   @brief    Reports the *squared* distance from the given point to the goal.

   @param    pt      The query point.
   @returns  The squared distance from the point to the goal.
   */
  float squaredDistance(const Math::Vector2& pt) const { return _geometry->squaredDistance(pt); }

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

   @param    q             The query point.
   @param    r             The radius of clearance.
   @param    directions    An instance of Agents::PrefVelocity.
   */
  void setDirections(const Math::Vector2& q, float r, Agents::PrefVelocity& directions) const {
    return _geometry->setDirections(q, r, directions);
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

   @param    q    The query point.
   @param    r    The radius of clearance.
   @returns  A 2D position representing the target point.
   */
  Math::Vector2 getTargetPoint(const Math::Vector2& q, float r) const {
    return _geometry->getTargetPoint(q, r);
  }

  /*!
   @brief    Return the centroid of the goal.
   */
  Math::Vector2 getCentroid() const { return _geometry->getCentroid(); }

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
