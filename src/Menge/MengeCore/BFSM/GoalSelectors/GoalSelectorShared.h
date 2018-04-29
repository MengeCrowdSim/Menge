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
 @file    GoalSelectorShared.h
 @brief    The definition of the "shared" goal selector.
 */

#ifndef __GOAL_SELECTOR_SHARED_H__
#define __GOAL_SELECTOR_SHARED_H__

#include "MengeCore/BFSM/GoalSelectors/GoalSelector.h"
#include "MengeCore/BFSM/GoalSelectors/GoalSelectorFactory.h"
#include "MengeCore/BFSM/fsmCommon.h"

namespace Menge {

// forward declarations
namespace Agents {
class SimulatorInterface;
}

namespace BFSM {
// forward declarations
class SharedGoalSelectorFactory;
class FSM;
class FSMDescrip;
FSM* buildFSM(FSMDescrip& fsmDescrip, Agents::SimulatorInterface* sim, bool VERBOSE);

/*!
 @brief    The shared goal selector is a stub goal selector used strictly internally for bookkeeping.
 
 It is used to implement shared goal selectors between states. It is not intended to be sub-classed..
 */
class SharedGoalSelector : public GoalSelector {
 public:
  /*!
   @brief    Default constructor
   */
  SharedGoalSelector();

  /*!
   @brief    Interface function for acquiring per-agent goals.

   @param    agent    The agent for whom a goal is extracted.
   @returns  A pointer to a goal.
   */
  virtual Goal* getGoal(const Agents::BaseAgent* agent) const;

  friend class SharedGoalSelectorFactory;
  friend FSM* buildFSM(FSMDescrip& fsmDescrip, Agents::SimulatorInterface* sim, bool VERBOSE);

 protected:
  /*!
   @brief    The name of the state which will provide the actual goal selector instance.
   */
  std::string _stateName;

  /*!
   @brief    The line number this was defined on (for use with logger messages.
   */
  int _lineNo;
};

/*!
 @brief    Factory for the SharedGoalSelector.
 */
class SharedGoalSelectorFactory : public GoalSelectorFactory {
 public:
  /*!
   @brief    The name of the goal selector type.

   The goal selector's name must be unique among all registered goal selectors. Each goal selector
   factory must override this function.

   @returns  A string containing the unique goal selector name.
   */
  virtual const char* name() const { return "shared"; }

  /*!
   @brief    A description of the goal selector.

   Each goal selector factory must override this function.

   @returns  A string containing the goal selector description.
   */
  virtual const char* description() const {
    return "A goal selector stub.  Used to realize the \"shared\" "
           "goal selector semantics.";
  };

 protected:
  /*!
   @brief    Create an instance of this class's goal selector.

   @returns    A pointer to a newly instantiated GoalSelector class.
   */
  GoalSelector* instance() const { return new SharedGoalSelector(); }

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
};

}  // namespace BFSM
}  // namespace Menge

#endif  // __GOAL_SELECTOR_SHARED_H__
