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
 @file    VelModifier.h
 @brief    The definition of how preferred velocity is modified by a filter.
 */
#ifndef __VEL_MODIFIER_H__
#define __VEL_MODIFIER_H__

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
class VelModContext;

/*!
 @brief    Base exception class for preferred velocity modification.
 */
class MENGE_API VelModException : public virtual MengeException {
 public:
  /*!
   @brief    Default constructor.
   */
  VelModException() : MengeException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  VelModException(const std::string& s) : MengeException(s) {}
};

/*!
 @brief    Exception thrown when the velocity modifier has an error which cannot be recovered from.
 */
class VelModFatalException : public VelModException, public MengeFatalException {
 public:
  /*!
   @brief    Default constructor.
   */
  VelModFatalException() : MengeException(), VelModException(), MengeFatalException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  VelModFatalException(const std::string& s)
      : MengeException(s), VelModException(), MengeFatalException() {}
};

/*!
 @brief    The base class for modifying preferred velocities

 Each velocity modifier is allowed to change the input preferred velocity without limit based on
 arbitrary criteria. The filteres are executed in sequence so that the input of each modifier is the
 output of the previous, making order very important.
 */
class MENGE_API VelModifier : public Element {
 public:
  /*!
   @brief    Default constructor.
   */
  VelModifier() : Element() {}

 protected:
  /*!
   @brief    Virtual destructor.
   */
  virtual ~VelModifier() {}

 public:
  /*!
   @brief       Modifies the input preferred velocity in place.

   The main function of the velocity modifier. Applies its own algorithms to the input preferred
   velocity, transforming it into a new velocity, in place.

   @param    agent    The agent for which a preferred velocity is adapted.
   @param    pVel    The instance of Agents::PrefVelocity to set.
   */
  virtual void adaptPrefVelocity(const Agents::BaseAgent* agent, Agents::PrefVelocity& pVel) = 0;

  /*!
   @brief       Registers an agent for use with the VelModifier.

   When a velocity modifier is assigned to the agent, the agent is "registered" to it. If the
   velocity modifier needs to maintain per-agent data, this is the time to instantiate that data.
   Default implementation does nothing.

   @param    agent    The agent to be registered
   */
  virtual void registerAgent(const Agents::BaseAgent* agent) {}

  /*!
   @brief       Unregisters a registed agent.

   If an velocity modifier is removed from an agent, this allows the velocity modifier to destroy
   any agent-specific data. Default implementation does nothing.

   @param    agent    The agent to be unregistered
   */
  virtual void unregisterAgent(const Agents::BaseAgent* agent) {}

  /*!
   @brief       Called when an agent enters the state with this velocity modifier.

   This function calls registerAgent. Entering a state which has one or more velocity modifiers
   assigned to it, causes the velocity modifiers to be temporarily assigned to the agent.

   @param    agent    The agent entering the state
   */
  void onEnter(const Agents::BaseAgent* agent);

  /*!
   @brief       Called when an agent exits the state with this velocity modifier.

   The dual of onEnter(). This function calls unregisterAgent(). Agents departing a state with one
   or more velocity modifiers will be unregistered.

   @param    agent    The agent entering the state
   */
  void onLeave(const Agents::BaseAgent* agent);

  /*!
   @brief    Creates a copy of this velocity modifier.

   @returns  A unique, deep copy of this velocity modifier.
   */
  virtual VelModifier* copy() const = 0;
#if 0
  /*!
    @brief    Provides a display context for interacting with this velocity modifier.

    It is the responsibility of the caller to delete the provided context.

    @returns  A pointer to a context for this velocity modifier
    */
  virtual VelModContext * getContext();
#endif
  friend class ElementFactory<VelModifier>;
};

/*!
 @brief    Parses a TinyXML element containing a velocity modifier specification.

 @param    node          The TinyXML element
 @param    behaveFldr    The folder in which the behavior is defined -- all resources are defined
                        relative to this folder.
 @returns  A pointer to the new velocity modifier implementation (NULL if no valid instance could be
          created).
 */
VelModifier* parseVelModifier(TiXmlElement* node, const std::string& behaveFldr);

}  // namespace BFSM
}  // namespace Menge

#endif  // __VEL_MODIFIER_H__
