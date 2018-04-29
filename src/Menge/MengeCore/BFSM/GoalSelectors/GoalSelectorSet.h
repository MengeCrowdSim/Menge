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
 @file    GoalSelectorSet.h
 @brief    The definition of the base class for handling goal selectors which operate on single goal
          sets.
 */

#ifndef __GOAL_SELECTOR_SET_H__
#define __GOAL_SELECTOR_SET_H__

#include "MengeCore/BFSM/GoalSelectors/GoalSelector.h"
#include "MengeCore/BFSM/GoalSelectors/GoalSelectorFactory.h"
#include "MengeCore/BFSM/GoalSet.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/CoreConfig.h"

namespace Menge {

namespace BFSM {
/*!
 @brief    This is a base class -- not to be instantiated.
 
 It provides the functional basis for GoalSelectors which use a GoalSet as the underlying data
 structure.
 */
class MENGE_API SetGoalSelector : public GoalSelector {
 public:
  /*!
   @brief    Default constructor
   */
  SetGoalSelector();

  /*!
   @brief    Gives the instance the opportunity to set the goal set.

   This does nothing for instances which don't require a goal set. Otherwise, it extracts its
   required goal set.

   @param    goalSets  A mapping from goal set identifier to goal set pointers.
   */
  void setGoalSet(std::map<size_t, GoalSet*>& goalSets);

  /*!
   @brief    Sets the goal set identifier.

   @param    id    The goal set's id that belongs to this selector.
   */
  void setGoalSetID(size_t id) { _goalSetID = id; }

 protected:
  /*!
   @brief    Allows the goal selector to lock any resources it requires.

   This is primarily here so that GoalSelectors which use shared resources have a chance to lock
   them (see SetGoalSelector). A call to lockResources should always be followed by a call to
   releeaseResources().
   */
  virtual void lockResources() { _goalSet->lockRead(); }

  /*!
   @brief    Allows the goal selector to release previously locked resources.

   Should be used in conjunction with lockResources.
   */
  virtual void releaseResources() { _goalSet->releaseRead(); }

  /*!
   @brief    The goal set associated with this goal selector.

   During parsing, it contains the id of the goal set. After FSM construction, it contains a pointer
   to the actual goal set.
   */
  union {
    size_t _goalSetID;  ///< The identifier for the goal set
    GoalSet* _goalSet;  ///< The pointer to the goal set.
  };
};

/*!
 @brief    Factory for the SetGoalSelector.
 */
class MENGE_API SetGoalSelectorFactory : public GoalSelectorFactory {
 public:
  /*!
   @brief    Constructor.
   */
  SetGoalSelectorFactory();

 protected:
  /*!
   @brief    Given a pointer to a GoalSelector instance, sets the appropriate fields from the
            provided XML node.

   @param    selector    A pointer to the goal whose attributes are to be set.
   @param    node        The XML node containing the goal selector attributes.
   @param    behaveFldr  The path to the behavior file.  If the goal selector references resources in
                        the file system, it should be defined relative to the behavior file
                        location. This is the folder containing that path.
   @returns  A boolean reporting success (true) or failure (false).
   */
  virtual bool setFromXML(GoalSelector* selector, TiXmlElement* node,
                          const std::string& behaveFldr) const;

  /*!
   @brief    The identifier for the "goal_set" size_t attribute.
   */
  size_t _goalSetID;
};
}  // namespace BFSM
}  // namespace Menge

#endif  // __GOAL_SELECTOR_SET_H__
