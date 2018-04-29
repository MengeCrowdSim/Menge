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
 @file    GoalSelectorWeighted.h
 @brief    The definition of the weighted random goal selector.
 */

#ifndef __GOAL_SELECTOR_WEIGHTED_H__
#define __GOAL_SELECTOR_WEIGHTED_H__

#include "MengeCore/BFSM/GoalSelectors/GoalSelectorSet.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/CoreConfig.h"

namespace Menge {

namespace BFSM {
/*!
 @brief    The weighted goal selector selects a goal from a goal set with weighted probability (i.e.,
          it uses the user-specified goal weights).
 */
class MENGE_API WeightedGoalSelector : public SetGoalSelector {
 public:
  /*!
   @brief    Interface function for acquiring per-agent goals.

   @param    agent    The agent for whom a goal is extracted.
   @returns  A pointer to a goal.
   // TODO: Figure out who owns this goal.
   */
  virtual Goal* getGoal(const Agents::BaseAgent* agent) const;
};

/*!
 @brief    Factory for the WeightedGoalSelector.
 */
class MENGE_API WeightedGoalSelectorFactory : public SetGoalSelectorFactory {
 public:
  /*!
   @brief    The name of the goal selector type.

   The goal selector's name must be unique among all registered goal selectors. Each goal selector
   factory must override this function.

   @returns  A string containing the unique goal selector name.
   */
  virtual const char* name() const { return "weighted"; }

  /*!
   @brief    A description of the goal selector.

   Each goal selector factory must override this function.

   @returns  A string containing the goal selector description.
   */
  virtual const char* description() const {
    return "A goal selector.  Assigns the agent a goal drawn randomly "
           "from a goal set with weighted  probability.";
  };

 protected:
  /*!
   @brief    Create an instance of this class's goal selector.

   @returns    A pointer to a newly instantiated GoalSelector class.
   */
  GoalSelector* instance() const { return new WeightedGoalSelector(); }
};
}  // namespace BFSM
}  // namespace Menge

#endif  // __GOAL_SELECTOR_WEIGHTED_H__
