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
 @file    CondAuto.h
 @brief    The definition of the automatic condition.
 */

#ifndef __COND_AUTO_H__
#define __COND_AUTO_H__

#include "MengeCore/BFSM/Transitions/Condition.h"
#include "MengeCore/BFSM/Transitions/ConditionFactory.h"
#include "MengeCore/CoreConfig.h"

namespace Menge {

namespace BFSM {

///////////////////////////////////////////////////////////////////////////

/*!
 @brief    The definition of the "auto" condition.

 The auto condition always evaluates to true.  It can be used to automatically progress from one
 state to another -- or as a default case when multiple transitions are possible and the transition
 with the auto condition should be taken when no other transition proved to be valid.
 */
class MENGE_API AutoCondition : public Condition {
 public:
  /*!
   @brief    The AutoCondition is always met -- it is a tautology.

   @param    agent    The agent to test the transition for.
   @param    goal    The agent's goal (although this may be ignored).
   @returns  True if the condition has been met.
   */
  virtual bool conditionMet(Agents::BaseAgent* agent, const Goal* goal) { return true; }

  /*!
   @brief    Create a copy of this condition.

   It is the responsibility of the caller to delete the object.

   @returns:  A "deep copy" of this condition - such that there is no shared objects between this
              and its copy.
   */
  virtual Condition* copy() { return new AutoCondition(); }
};

///////////////////////////////////////////////////////////////////////////

/*!
 @brief    The factory for creating the AutoCondition
 */
class MENGE_API AutoCondFactory : public ConditionFactory {
 public:
  /*!
   @brief    The name of the action.

   The action's name must be unique among all registered actions. Each action factory must override
   this function.

   @returns  A string containing the unique action name.
   */
  virtual const char* name() const { return "auto"; }

  /*!
   @brief    A description of the action.

   Each action factory must override this function.

   @returns  A string containing the action description.
   */
  virtual const char* description() const {
    return "The tautological condition.  It always evaluates to true.  "
           "If tested, the transition will be active.";
  }

 protected:
  /*!
   @brief    Create an instance of this class's condition.

   All ConditionFactory sub-classes must override this by creating (on the heap) a new instance of
   its corresponding condition type.  The various field values of the instance will be set in a
   subsequent call to ConditionFactory::setFromXML. The caller of this function takes ownership of
   the memory.

   @returns    A pointer to a newly instantiated Condition class.
   */
  virtual Condition* instance() const { return new AutoCondition(); }
};

}  // namespace BFSM
}  // namespace Menge

#endif  // __COND_AUTO_H__
