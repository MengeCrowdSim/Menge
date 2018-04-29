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
 @file    GoalSelectorExplicit.h
 @brief    The definition of the explicit goal selector - specifying a target goal from a set
          explicitly.
 */

#ifndef __GOAL_SELECTOR_EXCPLICIT_H__
#define __GOAL_SELECTOR_EXCPLICIT_H__

#include "MengeCore/BFSM/GoalSelectors/GoalSelector.h"
#include "MengeCore/BFSM/GoalSelectors/GoalSelectorFactory.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/CoreConfig.h"

namespace Menge {

namespace BFSM {
// forward declaration
class Goal;

/*!
 @brief    The explicit goal selector makes extracts a specific goal from a specific goal set -- both
          explicitly stated.
 */
class MENGE_API ExplicitGoalSelector : public GoalSelector {
 public:
  /*!
   @brief    Default constructor
   */
  ExplicitGoalSelector();

  /*!
   @brief    Interface function for acquiring per-agent goals.

   @param    agent    The agent for whom a goal is extracted.
   @returns  A pointer to a goal.
   // TODO: Figure out who owns this goal.
   */
  virtual Goal* getGoal(const Agents::BaseAgent* agent) const;

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

  /*!
   @brief    Sets the goal identifier.

   @param    id    The goal's id that belongs to this selector.
   */
  void setGoalID(size_t id) { _goalID = id; }

 protected:
  /*!
   @brief    The id of the goal set to draw from.
   */
  size_t _goalSetID;

  /*!
   @brief    The goal associated with this goal selector.

   During parsing, it contains the id of the goal. After FSM construction, it contains a pointer to
   the actual goal.
   */
  union {
    size_t _goalID;  ///< The identifier for the goal
    Goal* _goal;     ///< The pointer to the goal.
  };
};

/*!
 @brief    Factory for the ExplicitGoalSelector.
 */
class MENGE_API ExplicitGoalSelectorFactory : public GoalSelectorFactory {
 public:
  /*!
   @brief    Constructor.
   */
  ExplicitGoalSelectorFactory();

  /*!
   @brief    The name of the goal selector type.

   The goal selector's name must be unique among all registered goal selectors. Each goal selector
   factory must override this function.

   @returns  A string containing the unique goal selector name.
   */
  virtual const char* name() const { return "explicit"; }

  /*!
   @brief    A description of the goal selector.

   Each goal selector factory must override this function.

   @returns  A string containing the goal selector description.
   */
  virtual const char* description() const {
    return "A goal selector.  The goal an agent gets is "
           "explicitly enumeraged, goal set and goal id.";
  };

 protected:
  /*!
   @brief    Create an instance of this class's goal selector.

   @returns    A pointer to a newly instantiated GoalSelector class.
   */
  GoalSelector* instance() const { return new ExplicitGoalSelector(); }

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

  /*!
   @brief    The identifier for the "goal" size_t attribute.
   */
  size_t _goalID;
};

}  // namespace BFSM
}  // namespace Menge

#endif  // __GOAL_SELECTOR_EXCPLICIT_H__
