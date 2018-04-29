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
 @file    GoalSelectorOffset.h
 @brief    The definition of the "offset" goal selector.
 */

#ifndef __GOAL_SELECTOR_OFFSET_H__
#define __GOAL_SELECTOR_OFFSET_H__

#include "MengeCore/BFSM/GoalSelectors/GoalSelector.h"
#include "MengeCore/BFSM/GoalSelectors/GoalSelectorFactory.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/CoreConfig.h"

namespace Menge {

namespace BFSM {
/*!
 @brief    The offset goal selector makes a goal by applying an offset value to the agent's position
          at assignment time.
 
 The offsets can be defined with a distribution.
 */
class MENGE_API OffsetGoalSelector : public GoalSelector {
 public:
  /*!
   @brief    Default constructor
   */
  OffsetGoalSelector();

 protected:
  /*!
   @brief    Destructor.
   */
  ~OffsetGoalSelector();

 public:
  /*!
   @brief    Interface function for acquiring per-agent goals.

   @param    agent    The agent for whom a goal is extracted.
   @returns  A pointer to a goal.
   // TODO: Figure out who owns this goal.
   */
  virtual Goal* getGoal(const Agents::BaseAgent* agent) const;

  /*!
   @brief    Set the 2D number distribution for the goal selector.

   The goal selector takes ownership of the provided 2D number generator and will delete it upon its
   own destruction.

   @param    gen    A pointer to the 2D generator.
   */
  void setDistribution(Vec2DGenerator* gen) { _2DVel = gen; }

 protected:
  /*!
   @brief    A vector distribution for the offset value.
   */
  mutable Vec2DGenerator* _2DVel;
};

/*!
 @brief    Factory for the OffsetGoalSelector.
 */
class MENGE_API OffsetGoalSelectorFactory : public GoalSelectorFactory {
 public:
  /*!
   @brief    Constructor.
   */
  OffsetGoalSelectorFactory();

  /*!
   @brief    The name of the goal selector type.

   The goal selector's name must be unique among all registered goal selectors. Each goal selector
   factory must override this function.

   @returns  A string containing the unique goal selector name.
   */
  virtual const char* name() const { return "offset"; }

  /*!
   @brief    A description of the goal selector.

   Each goal selector factory must override this function.

   @returns  A string containing the goal selector description.
   */
  virtual const char* description() const {
    return "A goal selector.  The goal an agent gets is the agent's "
           "position offset by some user-specified 2D distribution.";
  };

 protected:
  /*!
   @brief    Create an instance of this class's goal selector.

   @returns    A pointer to a newly instantiated GoalSelector class.
   */
  GoalSelector* instance() const { return new OffsetGoalSelector(); }

  /*!
   @brief    Given a pointer to a GoalSelector instance, sets the appropriate fields
   from the provided XML node.

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
   @brief    The identifier for the 2D vector generator attribute.
   */
  size_t _offsetID;
};
}  // namespace BFSM
}  // namespace Menge

#endif  // __GOAL_SELECTOR_OFFSET_H__
