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
 @file    StateMemberTarget.h
 @brief    Defines an event effect target based on membership in state(s).
 */

#ifndef __STATE_MEMBER_TARGET_H__
#define __STATE_MEMBER_TARGET_H__

#include "MengeCore/Agents/Events/AgentEventTarget.h"

namespace Menge {
// forward declaration
class NamedStateMemberTargetFactory;

namespace BFSM {
class State;
}

//////////////////////////////////////////////////////////////////////////

/*!
 @brief    Defines the target of an event effect based on membership relationship to a single *named*
          state.
 */
class MENGE_API NamedStateMemberTarget : public AgentEventTarget {
 public:
  /*!
   @brief    Default constructor.
   */
  NamedStateMemberTarget() : AgentEventTarget() {}

  /*!
   @brief    Allows the event target to finish initializing itself from its parsed state to its
            running state.

   @throws    EventException if there is a problem finalizing.
   */
  virtual void finalize();

  /*!
   @brief    Gives the target a chance to update its knowledge of the target entities.

   If a sub-class needs to do particular computation to evaluate the target, it should be
   implemented here.
   */
  virtual void update();

  friend class NamedStateMemberTargetFactory;

 protected:
  /*!
   @brief    The name of the state to operate on.
   */
  std::string _stateName;

  /*!
   @brief    The state to operate on.
   */
  BFSM::State* _state;

  /*!
   @brief    Deterimines if membership in the specified state (true) or non-membership (false) makes
            an agent a target.
   */
  bool _inState;
};

/////////////////////////////////////////////////////////////////////////

/*!
 @brief    The factory to generate NamedStateMemberTarget instances.
 */
class MENGE_API NamedStateMemberTargetFactory : public AgentEventTargetFactory {
 public:
  /*!
   @brief    Constructor.
   */
  NamedStateMemberTargetFactory();

  /*!
   @brief    The name of the target.

   The target's name must be unique among all registered targets. Each target factory must override
   this function.

   @returns  A string containing the unique target name.
   */
  virtual const char* name() const { return "named_state_member"; }

  /*!
   @brief    A description of the target.

   Each target factory must override this function.

   @returns  A string containing the target description.
   */
  virtual const char* description() const {
    return "Defines an agent as a target based on its membership relationship to a "
           "single state.";
  };

 protected:
  /*!
   @brief    Create an instance of this class's target.

   All EventTargetFactory sub-classes must override this by creating (on the heap) a new instance of
   its corresponding event target type.  The various field values of the instance will be set in a
   subsequent call to EventTargetFactory::setFromXML. The caller of this function takes ownership of
   the memory.

   @returns    A pointer to a newly instantiated EventTarget class.
   */
  EventTarget* instance() const { return new NamedStateMemberTarget(); }

  /*!
   @brief    Given a pointer to an EventTarget instance, sets the appropriate fields from the
            provided XML node.

   It is assumed that the value of the `type` attribute is this EventTarget's type (i.e.
   EventTargetFactory::thisFactory has already been called and returned true). If sub-classes of
   EventTargetFactory introduce *new* EventTarget parameters, then the sub-class should override
   this method but explicitly call the parent class's version.

   @param    target      A pointer to the target whose attributes are to be set.
   @param    node        The XML node containing the target attributes.
   @param    behaveFldr  The path to the behavior file.  If the target references resources in the
                        file system, it should be defined relative to the behavior file location.
                        This is the folder containing that path.
   @returns  A boolean reporting success (true) or failure (false).
   */
  virtual bool setFromXML(EventTarget* target, TiXmlElement* node,
                          const std::string& behaveFldr) const;

  /*!
   @brief    The identifier for the "state" string attribute.
   */
  size_t _stateID;

  /*!
   @brief    The identifier for the "is_member" bool attribute.
   */
  size_t _isMemberID;
};

// TODO: Future state-based targets
//  Set of named states
//  membership based on state properties
//    final/not final
//    transitions to state X
//    etc.

}  // namespace Menge
#endif  // __STATE_MEMBER_TARGET_H__
