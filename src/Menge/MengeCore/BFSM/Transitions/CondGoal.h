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
 @file    CondGoal.h
 @brief    The definition of the goal reached condition.
 */

#ifndef __COND_GOAL_H__
#define __COND_GOAL_H__

#include "MengeCore/BFSM/Transitions/Condition.h"
#include "MengeCore/BFSM/Transitions/ConditionFactory.h"
#include "MengeCore/CoreConfig.h"

namespace Menge {

namespace BFSM {

///////////////////////////////////////////////////////////////////////////

/*!
 @brief    A spatial transition based on individual goal positions.
 */
class MENGE_API GoalCondition : public Condition {
 public:
  /*!
   @brief    Constructor.
   */
  GoalCondition();

  /*!
   @brief    Copy Constructor.

   @param    cond    The condition to copy from.
   */
  GoalCondition(const GoalCondition& cond);

  /*!
   @brief    The GoalCondition's condition is met when the agent is within a distance to the goal.

   @param    agent    The agent to test the transition for.
   @param    goal    The agent's goal (although this may be ignored).
   @returns  True if the condition has been met.
   */
  virtual bool conditionMet(Agents::BaseAgent* agent, const Goal* goal);

  /*!
   @brief    Create a copy of this condition.

   It is the responsibility of the caller to delete the object.

   @returns   A "deep copy" of this condition - such that there is no shared objects between this
              and its copy.
   */
  virtual Condition* copy();

  /*!
   @brief    Set the minimum distance of approach.

   @param    dist  The minimum distance to the goal to consider the condition met.
   */
  void setMinDistance(float dist) { _distSq = dist * dist; }

 protected:
  /*!
   @brief    Minimum distance of approach (squared for efficiency).
   */
  float _distSq;
};

/*!
 @brief    The factory for creating the GoalCondition
 */
class MENGE_API GoalCondFactory : public ConditionFactory {
 public:
  /*!
   @brief    Constructor.
   */
  GoalCondFactory();

  /*!
   @brief    The name of the condition.

   The condition's name must be unique among all registered condition. Each condition factory must
   override this function.

   @returns  A string containing the unique condition name.
   */
  virtual const char* name() const { return "goal_reached"; }

  /*!
   @brief    A description of the action.

   Each action factory must override this function.

   @returns  A string containing the action description.
   */
  virtual const char* description() const {
    return "The goal condition.  It becomes active when an agent reaches "
           "a user-specified distance to the goal.";
  }

 protected:
  /*!
   @brief   Create an instance of this class's condition.

   All ConditionFactory sub-classes must override this by creating (on the heap) a new instance of
   its corresponding condition type.  The various field values of the instance will be set in a
   subsequent call to ConditionFactory::setFromXML. The caller of this function takes ownership of
   the memory.

   @returns    A pointer to a newly instantiated Action class.
   */
  virtual Condition* instance() const { return new GoalCondition(); }

  /*!
   @brief    Given a pointer to an Condition instance, sets the appropriate fields from the provided
            XML node.

   It is assumed that the value of the `type` attribute is this Tarnsitions's type (i.e.
   ConditionFactory::thisFactory has already been called and returned true). If sub-classes of
   ConditionFactory introduce *new* Condition parameters, then the sub-class should override this
   method but explicitly call the parent class's version.

   @param   condition   A pointer to the condition whose attributes are to be set.
   @param   node        The XML node containing the condition attributes.
   @param   behaveFldr  The path to the behavior file. If the condition references resources in the
                        file system, it should be defined relative to the behavior file location.
                        This is the folder containing that path.
   @returns  A boolean reporting success (true) or failure (false).
   */
  virtual bool setFromXML(Condition* condition, TiXmlElement* node,
                          const std::string& behaveFldr) const;

  /*!
   @brief    The identifier for the "distance" float attribute.
   */
  size_t _distanceID;
};

}  // namespace BFSM
}  // namespace Menge
#endif  // __COND_GOAL_H__
