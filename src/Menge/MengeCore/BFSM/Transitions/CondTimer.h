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
 @file    CondTimer.h
 @brief    The definition of the timer-based condition.
 */

#ifndef __COND_TIMER_H__
#define __COND_TIMER_H__

#include "MengeCore/BFSM/Transitions/Condition.h"
#include "MengeCore/BFSM/Transitions/ConditionFactory.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/CoreConfig.h"
#include "MengeCore/Runtime/ReadersWriterLock.h"

#include <map>  //TODO: Replace this with a hash_map

namespace Menge {

namespace BFSM {

// forward declarations
class TimerCondFactory;
///////////////////////////////////////////////////////////////////////////

/*!
 @brief    The definition of the timer condition.

 The time condition becomes active when the agent has been in the state at least the amount of time
 specified by the transition properties. The amount of time can be specified globally or per agent
 and, in the case of a per-agent duration, can be specified using the value distributions
 (FloatGenerator).
 */
class MENGE_API TimerCondition : public Condition {
 public:
  /*!
   @brief    Constructor.
   */
  TimerCondition();

  /*!
   @brief    Copy Constructor.

   @param    cond    The condition to copy from.
   */
  TimerCondition(const TimerCondition& cond);

  /*!
   @brief    Create a copy of this condition.

   It is the responsibility of the caller to delete the object.

   @returns   A "deep copy" of this condition - such that there is no shared objects between this
              and its copy.
   */
  virtual Condition* copy();

 protected:
  /*!
   @brief    Constructor.
   */
  ~TimerCondition();

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
   @brief    The AutoCondition is always met -- it is a tautology.

   @param    agent    The agent to test the transition for.
   @param    goal    The agent's goal (although this may be ignored).
   @returns  True if the condition has been met.
   */
  virtual bool conditionMet(Agents::BaseAgent* agent, const Goal* goal);

  friend class TimerCondFactory;

 protected:
  /*!
   @brief    The trigger time for agents currently effected by this transition.
   */
  std::map<size_t, float> _triggerTimes;

  /*!
   @brief    The generator for determining the per-agent duration.
   */
  FloatGenerator* _durGen;

  /*!
   @brief    Lock to protect _triggerTimes;
   */
  ReadersWriterLock _lock;
};

///////////////////////////////////////////////////////////////////////////

/*!
 @brief    The factory for creating the TimerCondition
 */
class MENGE_API TimerCondFactory : public ConditionFactory {
 public:
  /*!
   @brief    Constructor.
   */
  TimerCondFactory();

  /*!
   @brief    The name of the action.

   The action's name must be unique among all registered actions. Each action factory must override
   this function.

   @returns  A string containing the unique action name.
   */
  virtual const char* name() const { return "timer"; }

  /*!
   @brief    A description of the action.

   Each action factory must override this function.

   @returns  A string containing the action description.
   */
  virtual const char* description() const {
    return "The timer condition.  It becomes active when the agent has "
           "remained in the state at least a user-specified length of time.";
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
  virtual Condition* instance() const { return new TimerCondition(); }

  /*!
   @brief    Given a pointer to an Condition instance, sets the appropriate fields from the provided
            XML node.

   It is assumed that the value of the `type` attribute is this Tarnsitions's type (i.e.
   ConditionFactory::thisFactory has already been called and returned true). If sub-classes of
   ConditionFactory introduce *new* Condition parameters, then the sub-class should override this
   method but explicitly call the parent class's version.

   @param    condition    A pointer to the condition whose attributes are to be set.
   @param    node        The XML node containing the condition attributes.
   @param    behaveFldr  The path to the behavior file. If the condition references resources in the
                        file system, it should be defined relative to the behavior file location.
                        This is the folder containing that path.
   @returns  A boolean reporting success (true) or failure (false).
   */
  virtual bool setFromXML(Condition* condition, TiXmlElement* node,
                          const std::string& behaveFldr) const;

  /*!
   @brief    The identifier for the "per_agent" boolean attribute.
   */
  size_t _perAgentID;

  /*!
   @brief    The identifier for the duration float distribution attribute.
   */
  size_t _durGenID;
};

}  // namespace BFSM
}  // namespace Menge
#endif  // __COND_TIMER_H__
