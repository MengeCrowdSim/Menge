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
 @file       GoalSet.h
 @brief      Defines a set of goals
 */

#ifndef __GOALSET_H__
#define __GOALSET_H__

#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/Math/RandGenerator.h"
#include "MengeCore/Runtime/ReadersWriterLock.h"

#include <map>

namespace Menge {

namespace BFSM {

// Forward declaration
class Goal;

/////////////////////////////////////////////////////////////////////////////////////

/*!
 @brief    A set of goals.  Maps integer identifiers to instances of goals.
 */
class MENGE_API GoalSet {
 public:
  /*!
   @brief    Default constructor.
   */
  GoalSet();

  /*!
   @brief    Destructor.
   */
  ~GoalSet();

  /*!
   @brief    Add the given goal with the given id to the set.

   @param    id    The id for the given goal.
   @param    goal  The goal to add to the set.
   @returns  A boolean reporting succes on adding the goal. If the id does not already exist in the
            set, the goal is added and true is returned. Otherwise, nothing is changed and false is
            returned. Once a goal is added to the GoalSet, the GoalSet takes responsibility for
            freeing the memory.
   */
  bool addGoal(size_t id, Goal* goal);

  /*!
   @brief   Gives any moving goals a chance to update their position.

   @param time_step  The amount of time to advance the goals' positions.
   */
  void moveGoals(float time_step);

  /*!
   @brief    Returns the goal with the given user-defined identifier.

   This is the identifier given the behavior specification. This operation is not thread-safe. It
   should only be used in a context that is known to be "safe".

   @param    id    The identifier of the desired goal.
   @returns  A pointer to the desired goal. If the goal doesn't exist, NULL is returned. Also, if the
            goal has reached capacity, NULL is returned.
   */
  Goal* getGoalByID(size_t id);

  /*!
   @brief    Returns the goal with the given user-defined identifier.

   This is the identifier given the behavior specification. This operation is thread-safe. But it
   should not be called in the same thread that has already called GoalSet::lockRead.

   @param    id    The identifier of the desired goal.
   @returns  A pointer to the desired goal. If the goal doesn't exist, NULL is returned. Also, if the
            goal has reached capacity, NULL is returned.
   */
  Goal* getGoalByIDConcurrent(size_t id);

  /*!
   @brief    Returns the ith *available* goal (doesn't necessarily correlate with the user-defined
            identifier).
            
   Merely the order in which the goals are ordered in the set. This operation is not thread-safe. It
   should only be used in a context that is known to be "safe".

   @param    i    The ith goal in the set -- order is undefined.
   @returns  A pointer to the desired goal. NULL is returned if the index exceeds the number of
            *available* goals.
   */
  Goal* getIthGoal(size_t i);

  /*!
   @brief    Returns the ith *available* goal (doesn't necessarily correlate with the user-defined
            identifier).
            
   Merely the order in which the goals are ordered in the set. This operation is thread-safe. But it
   should not be called in the same thread that has already called GoalSet::lockRead().

   @param    i    The ith goal in the set -- order is undefined.
   @returns  A pointer to the desired goal. NULL is returned if the index exceeds the number of
            *available* goals.
   */
  Goal* getIthGoalConcurrent(size_t i);

  /*!
   @brief    Reports the number of goals in the set.  *Not* thread safe.

   @returns  The number of goals in the set.
   */
  size_t size() const { return _goalIDs.size(); }

  /*!
   @brief    Reports the number of goals in the set.  Thread safe version.

   @returns  The number of goals in the set.
   */
  size_t sizeConcurrent() const;

  /*!
   @brief    Select a goal randomly from the set with all having equal probability.

   @returns  A pointer to the randomly selected goal
   */
  Goal* getRandomGoal();

  /*!
   @brief    Select a goal randomly, based on the relative weights of the goals.

   @returns  A pointer to the randomly selected goal
   */
  Goal* getRandomWeightedGoal();

  /*!
   @brief    Locks the goal set for a read-only operations.
   */
  void lockRead() { _lock.lockRead(); }

  /*!
   @brief    Unlocks the goal set from read-only operations.
   */
  void releaseRead() { _lock.releaseRead(); }

  friend class Goal;

 protected:
  /*!
   @brief    Informs the goal set that the given goal has reached its capacity and should no longer
            be considered.
   */
  void setGoalFull(const Goal* goal) const;

  /*!
   @brief    Informs the goal set that the given goal, previously with full capacity, now has
            capacity to be assigned to a new agent.
   */
  void setGoalAvailable(const Goal* goal) const;

  /*!
   @brief    The underlying mapping from user-specified goal identifier to goal
   */
  std::map<size_t, Goal*> _goals;

  /*!
   @brief    A vector of valid identifiers in the goal set.
   */
  mutable std::vector<size_t> _goalIDs;

  /*!
   @brief    The sum of all goal weights
   */
  mutable float _totalWeight;

  /*!
   @brief    The random die to select random and weighted goals
   */
  Menge::Math::UniformFloatGenerator _randVal;

  /*!
   @brief    The lock to maintain readers-writer access to the structure which control available
            goals.
   */
  ReadersWriterLock _lock;
};

}  // namespace BFSM
}  // namespace Menge

#endif  //__GOALSET_H__
