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
 @file    CondBoolean.h
 @brief    Definition of boolean conditiosn for transition conditions.
 */

#ifndef __COND_BOOLEAN_H__
#define __COND_BOOLEAN_H__

#include "MengeCore/BFSM/Transitions/Condition.h"
#include "MengeCore/BFSM/Transitions/ConditionFactory.h"
#include "MengeCore/CoreConfig.h"

namespace Menge {

namespace BFSM {

// forward declarations
class Bool2CondFactory;

///////////////////////////////////////////////////////////////////////////

/*!
 @brief    The base class for binary boolean operand as a transition condition.
 */
class MENGE_API Bool2Condition : public Condition {
 public:
  /*!
   @brief    Constructor.
   */
  Bool2Condition();

  /*!
   @brief    Copy Constructor.

   @param    cond    The condition to copy from.
   */
  Bool2Condition(const Bool2Condition& cond);

 protected:
  /*!
   @brief    Protected destructor.
   */
  virtual ~Bool2Condition();

 public:
  /*!
   @brief    Called when an agent enters a state with this exiting transition.

   Sub-classes should use this function as the opportunity to cache any particular per-agent data.

   @param    agent    The agent who has entered the state which uses this transition.
   */
  virtual void onEnter(Agents::BaseAgent* agent);

  /*!
   @brief    Called when an agent exits the state with this transition.

   @param    agent    The agent who left the state.
   */
  virtual void onLeave(Agents::BaseAgent* agent);

  friend class Bool2CondFactory;

 protected:
  /*!
   @brief    The first boolean operand.
   */
  Condition* _op1;

  /*!
   @brief    The second boolean operand.
   */
  Condition* _op2;
};

///////////////////////////////////////////////////////////////////////////

/*!
 @brief    The factory for creating the Bool2Condition
 */
class MENGE_API Bool2CondFactory : public ConditionFactory {
 protected:
  /*!
   @brief    Given a pointer to an Condition instance, sets the appropriate fields from the provided
            XML node.

   It is assumed that the value of the `type` attribute is this Tarnsitions's type (i.e.
   ConditionFactory::thisFactory has already been called and returned true). If sub-classes of
   ConditionFactory introduce *new* Condition parameters, then the sub-class should override this
   method but explicitly call the parent class's version.

   @param    condition   A pointer to the condition whose attributes are to be set.
   @param    node        The XML node containing the condition attributes.
   @param    behaveFldr  The path to the behavior file. If the condition references resources in the
                        file system, it should be defined relative to the behavior file location.
                        This is the folder containing that path.
   @returns  Returns `true` indicating successful parsing.
   */
  virtual bool setFromXML(Condition* condition, TiXmlElement* node,
                          const std::string& behaveFldr) const;
};

///////////////////////////////////////////////////////////////////////////

/*!
 @brief    A condition based on the boolean and of two conditions.
 */
class MENGE_API AndCondition : public Bool2Condition {
 public:
  /*!
   @brief    Constructor.
   */
  AndCondition();

  /*!
   @brief    Copy constructor.

   @param    cond    The condition to copy from.
   */
  AndCondition(const AndCondition& cond);

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
};

///////////////////////////////////////////////////////////////////////////

/*!
 @brief    The factory for creating the AndCondition
 */
class MENGE_API AndCondFactory : public Bool2CondFactory {
 public:
  /*!
   @brief    The name of the condition.

   The condition's name must be unique among all registered condition. Each condition factory must
   override this function.

   @returns  A string containing the unique condition name.
   */
  virtual const char* name() const { return "and"; }

  /*!
   @brief    A description of the condition.

   Each condition factory must override this function.

   @returns  A string containing the condition description.
   */
  virtual const char* description() const {
    return "The and condition.  This condition depends on the evaluation of "
           "two child conditions.  If both are met, this condition is met.";
  }

 protected:
  /*!
   @brief    Create an instance of this class's condition.

   All ConditionFactory sub-classes must override this by creating (on the heap) a new instance of
   its corresponding condition type.  The various field values of the instance will be set in a
   subsequent call to ConditionFactory::setFromXML. The caller of this function takes ownership of
   the memory.

   @returns    A pointer to a newly instantiated Action class.
   */
  virtual Condition* instance() const { return new AndCondition(); }
};

///////////////////////////////////////////////////////////////////////////

/*!
 @brief    A condition based on the boolean OR of two conditions.
 */
class MENGE_API OrCondition : public Bool2Condition {
 public:
  /*!
   @brief    Constructor.
   */
  OrCondition();

  /*!
   @brief    Copy constructor.

   @param    cond    The condition to copy from.
   */
  OrCondition(const OrCondition& cond);

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
};

///////////////////////////////////////////////////////////////////////////

/*!
 @brief    The factory for creating the OrCondition
 */
class MENGE_API OrCondFactory : public Bool2CondFactory {
 public:
  /*!
   @brief    The name of the condition.

   The condition's name must be unique among all registered condition. Each condition factory must
   override this function.

   @returns  A string containing the unique condition name.
   */
  virtual const char* name() const { return "or"; }

  /*!
   @brief    A description of the condition.

   Each condition factory must override this function.

   @returns  A string containing the condition description.
   */
  virtual const char* description() const {
    return "The or condition.  This condition depends on the evaluation of "
           "two child conditions.  If either are met, this condition is met.";
  }

 protected:
  /*!
   @brief    Create an instance of this class's condition.

   All ConditionFactory sub-classes must override this by creating (on the heap) a new instance of
   its corresponding condition type. The various field values of the instance will be set in a
   subsequent call to ConditionFactory::setFromXML. The caller of this function takes ownership of
   the memory.

   @returns    A pointer to a newly instantiated Action class.
   */
  virtual Condition* instance() const { return new OrCondition(); }
};

///////////////////////////////////////////////////////////////////////////

// forward declaration
class NotCondFactory;

/*!
 @brief    A condition based on the boolean negation of a single condition.
 */
class MENGE_API NotCondition : public Condition {
 public:
  /*!
   @brief    Constructor.
   */
  NotCondition();

  /*!
   @brief    Copy Constructor.

   @param    cond    The condition to copy from.
   */
  NotCondition(const NotCondition& cond);

 protected:
  /*!
   @brief    Protected destructor.
   */
  virtual ~NotCondition();

 public:
  /*!
   @brief    Called when an agent enters a state with this exiting transition.

   Sub-classes should use this function as the opportunity to cache any particular per-agent data.

   @param    agent    The agent who has entered the state which uses this transition.
   */
  virtual void onEnter(Agents::BaseAgent* agent);

  /*!
   @brief    Called when an agent exits the state with this transition.

   @param    agent    The agent who left the state.
   */
  virtual void onLeave(Agents::BaseAgent* agent);

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

  friend class NotCondFactory;

 protected:
  /*!
   @brief    The boolean operand to negate.
   */
  Condition* _op;
};

///////////////////////////////////////////////////////////////////////////

/*!
 @brief    The factory for creating the NotCondition
 */
class MENGE_API NotCondFactory : public ConditionFactory {
 public:
  /*!
   @brief    The name of the condition.

   The condition's name must be unique among all registered condition. Each condition factory must
   override this function.

   @returns  A string containing the unique condition name.
   */
  virtual const char* name() const { return "not"; }

  /*!
   @brief    A description of the condition.

   Each condition factory must override this function.

   @returns  A string containing the condition description.
   */
  virtual const char* description() const {
    return "The not condition.  This condition depends on the evaluation of "
           "a single child condition.  This condition is met when the child is not "
           " and vice versa.";
  }

 protected:
  /*!
   @brief    Create an instance of this class's condition.

   All ConditionFactory sub-classes must override this by creating (on the heap) a new instance of
   its corresponding condition type. The various field values of the instance will be set in a
   subsequent call to ConditionFactory::setFromXML. The caller of this function takes ownership of
   the memory.

   @returns    A pointer to a newly instantiated Action class.
   */
  virtual Condition* instance() const { return new NotCondition(); }

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
   @returns  True if the parsing was successful.
   */
  virtual bool setFromXML(Condition* condition, TiXmlElement* node,
                          const std::string& behaveFldr) const;
};

}  // namespace BFSM
}  // namespace Menge

#endif  // __COND_BOOLEAN_H__
