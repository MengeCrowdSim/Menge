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
 @file    TargetProb.h
 @brief    Defines a transition target probabilistically.
 */

#ifndef __TARGET_PROB_H__
#define __TARGET_PROB_H__

#include "MengeCore/BFSM/Transitions/Target.h"
#include "MengeCore/BFSM/Transitions/TargetFactory.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/CoreConfig.h"
#include "MengeCore/Math/RandGenerator.h"

#include <list>

namespace Menge {

namespace BFSM {

// forward declarations
class State;
class ProbTargetFactory;

///////////////////////////////////////////////////////////////////////////

/*!
 @brief    The definition of the probabalistic target.

 Typically, the FSM will be deterministic in that if an agent is in a current state, with a
 particualr condition being true, it will transition to a deterministic state. The probabalistic
 target allows the transition to be implicitly connected to a number of states. Upon transitioning,
 the destination state is computed randomly from the set.
 */
class MENGE_API ProbTarget : public TransitionTarget {
 public:
  /*!
   @brief    Constructor
   */
  ProbTarget();

  /*!
   @brief    Copy constructor.

   @param    tgt    The target to copy.
   */
  ProbTarget(const ProbTarget& tgt);

  /*!
   @brief    Determines the next state for the given agent.

   This function defines the main functionality of the target. Given the current agent and the
   class's internal state, it determines The state to move the agent into. This should be overridden
   by each sub-class.

   @param    agent    The agent to test the transition for.
   @returns  A pointer to the next state.
   */
  virtual State* nextState(Agents::BaseAgent* agent);

  /*!
   @brief    Performs any necessary connections to the target state(s).

   Sub-classes must override this because a transition must transition to a state. See the
   sub-classes SingleTransition for an example.

   @param    stateMap    A mapping from state names to state pointers.
   @returns  True if connection was successful, false otherwise.
   */
  virtual bool connectStates(std::map<std::string, State*>& stateMap);

  /*!
   @brief    Create a copy of this target.

   It is the responsibility of the caller to delete the object.

   @returns   A "deep copy" of this target - such that there is no shared objects between this and
              its copy.
   */
  virtual TransitionTarget* copy();

  friend class ProbTargetFactory;

 protected:
  /*!
   @brief    The random number generator for selecting the next state.
   */
  Menge::Math::UniformFloatGenerator _randNum;

  /*!
   @brief    The total weight of all the target states.

   This gets set after a call to connectStates.
   */
  float _totalWeight;

  /*!
   @brief    The set of target state *names* and their relative weights.
   */
  std::list<std::pair<float, std::string> > _targetNames;

  /*!
   @brief    The set of target states and their corresponding relative weights.
   */
  std::map<State*, float> _targets;
};

///////////////////////////////////////////////////////////////////////////

/*!
 @brief    The factory for creating the ProbTarget
 */
class MENGE_API ProbTargetFactory : public TargetFactory {
 public:
  /*!
   @brief    The name of the action.

   The action's name must be unique among all registered actions. Each action factory must override
   this function.

   @returns  A string containing the unique action name.
   */
  virtual const char* name() const { return "prob"; }

  /*!
   @brief    A description of the action.

   Each action factory must override this function.

   @returns  A string containing the action description.
   */
  virtual const char* description() const {
    return "The probabalistic transition target.  This allows a state to"
           " transition to a randomly selected member of a set of states. "
           "The state selected is based on weighted probabilities.";
  }

 protected:
  /*!
   @brief    Create an instance of this class's condition.

   All ConditionFactory sub-classes must override this by creating (on the heap) a new instance of
   its corresponding condition type. The various field values of the instance will be set in a
   subsequent call to ConditionFactory::setFromXML. The caller of this function takes ownership of
   the memory.

   @returns    A pointer to a newly instantiated TransitionTarget class.
   */
  virtual TransitionTarget* instance() const { return new ProbTarget(); }

  /*!
   @brief    Given a pointer to an Condition instance, sets the appropriate fields from the provided
            XML node.

   It is assumed that the value of the `type` attribute is this Tarnsitions's type (i.e.
   ConditionFactory::thisFactory has already been called and returned true). If sub-classes of
   ConditionFactory introduce *new* Condition parameters, then the sub-class should override this
   method but explicitly call the parent class's version.

   @param    target        A pointer to the transition target whose attributes are to be set.
   @param    node          The XML node containing the condition attributes.
   @param    behaveFldr    The path to the behavior file.  If the condition references resources in
                          the file system, it should be defined relative to the behavior file
                          location. This is the folder containing that path.
   @returns  A boolean reporting success (true) or failure (false).
   */
  virtual bool setFromXML(TransitionTarget* target, TiXmlElement* node,
                          const std::string& behaveFldr) const;
};

}  // namespace BFSM
}  // namespace Menge
#endif  // __TARGET_PROB_H__
