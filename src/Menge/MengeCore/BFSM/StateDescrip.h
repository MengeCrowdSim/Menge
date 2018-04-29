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
 @file    StateDescrip.h
 @brief    The state (and state goal) specification in the behavior configuration file.
 */

#ifndef __STATE_DESCRIP_H__
#define __STATE_DESCRIP_H__

#include "MengeCore/BFSM/fsmCommon.h"

#include <list>
#include <string>

// forward declarations
class TiXmlElement;

namespace Menge {

namespace BFSM {
// forward declarations
class Action;
class GoalSelector;
class VelComponent;
class VelModifier;

////////////////////////////////////////////////////////////////////////////////

/*!
 @brief    The description of a state  (see State).
 */
class StateDescrip {
 public:
  /*!
   @brief    Constructor.

   @param    name    The name of the state.
   @param    isFinal    Determines if this state is a final state (true) or not (false).
   */
  StateDescrip(const std::string& name, bool isFinal);

  /*!
   @brief    Destructor
   */
  ~StateDescrip();

  /*!
   @brief    Friend operator for printing the state description to an output stream.

   @param    out      The output stream.
   @param    state    A state description to write to the stream.
   @returns  The provided output stream.
   */
  friend Logger& operator<<(Logger& out, const StateDescrip& state);

  /*!
   @brief    The name of the state.
   */
  std::string _name;

  /*!
   @brief    Dictates whether this state is considered a "final" state (true) or not (false).
   */
  bool _isFinal;

  /*!
   @brief    The description of the goal selector used for this state.
   */
  GoalSelector* _goalSelector;

  /*!
   @brief    The velocity component for this state.
   */
  VelComponent* _velComponent;

  /*!
   @brief    The set of actions for this state.
   */
  std::list<Action*> _actions;

  /*!
   @brief    The set of actions for this state.
   */
  std::vector<VelModifier*> _velModifiers;  // TODO: list or vector?
};

/*!
 @brief    Parses a TinyXML element containing a state description

 @param    node        The TinyXML element.
 @param    behaveFldr  The folder in which the behavior is defined -- all resources are defined
                      relative to this folder.
 @param    states      List of state descriptions. The new state description is appended to the end
                      of this list.
 @returns  A boolean reporting succes (true) or failure (false).
 */
bool parseState(TiXmlElement* node, const std::string& behaveFldr,
                std::list<StateDescrip*>& states);

}  // namespace BFSM
}  // namespace Menge

#endif  // __STATE_DESCRIP_H__
