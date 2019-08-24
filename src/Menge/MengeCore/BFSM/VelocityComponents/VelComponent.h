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
 @file    VelComponent.h
 @brief    The definition of how preferred velocity is computed in a state.
 */
#ifndef __VEL_COMPONENT_H__
#define __VEL_COMPONENT_H__

#include "MengeCore/Agents/PrefVelocity.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/MengeException.h"
#include "MengeCore/PluginEngine/Element.h"

#include <vector>

namespace Menge {

// forward declaration
template <class Element>
class ElementFactory;

namespace BFSM {

// FORWARD DECLARATIONS
class Goal;

/*!
 @brief    Base exception class for preferred velocity computation.
 */
class MENGE_API VelCompException : public virtual MengeException {
 public:
  /*!
   @brief    Default constructor.
   */
  VelCompException() : MengeException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  VelCompException(const std::string& s) : MengeException(s) {}
};

/*!
 @brief    Exception thrown when the velocity component has an error which cannot be recovered from.
 */
class VelCompFatalException : public VelCompException, public MengeFatalException {
 public:
  /*!
   @brief    Default constructor.
   */
  VelCompFatalException() : MengeException(), VelCompException(), MengeFatalException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  VelCompFatalException(const std::string& s)
      : MengeException(s), VelCompException(), MengeFatalException() {}
};

/*!
 @brief    The base class for computing an agent's preferred velocity.

 Velocity components each have weights. The relative weights determine how multiple velocity
 components combine inside a single state. Their share is its own weight divided by the summed
 weight of all velocity components.

 The %VelComponent is first introduced to a goal for an agent upon the invocation of its onEnter()
 method. This is where the initial work can be done in preparing the data structures necessary
 for setPrefVelocity(). However, this cannot be work that is *solely* done in onEnter(). If the goal
 is mobile (i.e., goal->moves() return `true`), then such one-time configuration may quickly become
 invalid. When computing a preferred velocity, it is important to account for moving goals such that
 the preferred velocity always reflects the goal's instantaneous position (see updateGoal() and,
 especially for derived classes, doUpdateGoal()).
 */
class MENGE_API VelComponent : public Element {
 public:
  /*!
   @brief    Default constructor.
   */
  VelComponent() : Element() {}

 protected:
  /*!
   @brief    Virtual destructor.
   */
  virtual ~VelComponent() {}

 public:
  /*!
   @brief    Called when the agent enters the state which possesses this velocity component.

   Gives the velocity component to intialize any agent-specific data.

   @param    agent    The agent who entered the state.
   */
  virtual void onEnter(Agents::BaseAgent* agent) {}

  /*!
   @brief    Called when the agent leaves the state which possesses this velocity component.

   Gives the velocity component to restore any agent-specific data it might have changed.

   @param    agent    The agent who left the state.
   */
  virtual void onExit(Agents::BaseAgent* agent) {}

  /*!
   @brief    Computes and sets the agent's preferred velocity.

   The velocity component directly sets preferred velocity values in the the provided preferred
   velocity instance. See Agents::PrefVelocity for details. Rather than setting the agent's
   preferred velocity value directly, a reference to a preferred velocity instance is passed in to
   make the use more general. This allows the computation of the preferred velocity for the agent,
   without necessarily making changes to it.

   @param    agent   The agent for which a preferred velocity is computed.
   @param    goal    The agent's goal (although this may be ignored).
   @param    pVel    The instance of Agents::PrefVelocity to set.
   */
  virtual void setPrefVelocity(const Agents::BaseAgent* agent, const Goal* goal,
                               Agents::PrefVelocity& pVel) const = 0;

  /*!
   @brief   Gives the velocity component the chance to update its internal state based on the
            properties of the given goal (e.g., it moves).

   @param   agent   The agent whose goal requires updating.
   @param   goal    The moving goal for `agent`.
   */
  void updateGoal(const Agents::BaseAgent* agent, const Goal* goal);

  /*!
   @brief    Used by the plugin system to know what artifacts to associate with agents of this type.
   Every sub-class of must return a globally unique value if it should be associated with unique
   artifacts.
   */
  virtual std::string getStringId() const = 0;

  friend class ElementFactory<VelComponent>;

 protected:
  /** @brief    The method invoked to give the velocity component the chance to adapt to moving
   goals. This will only be invoked if goal->moves() reports true. This will be called in a threaded
   context so accesses to underlying data structures must be protected.

   @param   agent   The agent whose goal requires updating.
   @param   goal    The moving goal for `agent`.
   */
  virtual void doUpdateGoal(const Agents::BaseAgent* agent, const Goal* goal){};
};

/*!
 @brief    Parses a TinyXML element containing a velocity component specification

 @param    node          The TinyXML element
 @param    behaveFldr    The folder in which the behavior is defined -- all resources are defined
                        relative to this folder.
 @returns  A pointer to the new velocity component implementation (NULL if no valid instance could
           be created).
 */
VelComponent* parseVelComponent(TiXmlElement* node, const std::string& behaveFldr);

}  // namespace BFSM
}  // namespace Menge
#endif  // __VEL_COMPONENT_H__
