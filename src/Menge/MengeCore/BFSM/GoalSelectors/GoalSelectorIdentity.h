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
 @file    GoalSelectorIdentity.h
 @brief    The definition of the "identity" goal selector.
 */

#ifndef __GOAL_SELECTOR_IDENTITY_H__
#define __GOAL_SELECTOR_IDENTITY_H__

#include "MengeCore/BFSM/GoalSelectors/GoalSelector.h"
#include "MengeCore/BFSM/GoalSelectors/GoalSelectorFactory.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/CoreConfig.h"

namespace Menge {

namespace BFSM {
/*!
 @brief    The identity goal selector makes a goal out of the agent's current position when assigned.
 
 The goal is not associated with any set and should be destroyed upon being freed.
 */
class MENGE_API IdentityGoalSelector : public GoalSelector {
 public:
  /*!
   @brief    Default constructor
   */
  IdentityGoalSelector();

  /*!
   @brief    Interface function for acquiring per-agent goals.

   @param    agent    The agent for whom a goal is extracted.
   @returns  A pointer to a goal.
   // TODO: Figure out who owns this goal.
   */
  virtual Goal* getGoal(const Agents::BaseAgent* agent) const;
};

/*!
 @brief    Factory for the IdentityGoalSelector.
 */
class MENGE_API IdentityGoalSelectorFactory : public GoalSelectorFactory {
 public:
  /*!
   @brief    The name of the goal selector type.

   The goal selector's name must be unique among all registered goal selectors. Each goal selector
   factory must override this function.

   @returns  A string containing the unique goal selector name.
   */
  virtual const char* name() const { return "identity"; }

  /*!
   @brief    A description of the goal selector.

   Each goal selector factory must override this function.

   @returns  A string containing the goal selector description.
   */
  virtual const char* description() const {
    return "A goal selector.  The goal an agent gets is based on "
           "its current position when the goal is assigned.";
  };

 protected:
  /*!
   @brief    Create an instance of this class's goal selector.

   @returns    A pointer to a newly instantiated GoalSelector class.
   */
  GoalSelector* instance() const { return new IdentityGoalSelector(); }
};

}  // namespace BFSM
}  // namespace Menge

#endif  // __GOAL_SELECTOR_IDENTITY_H__
