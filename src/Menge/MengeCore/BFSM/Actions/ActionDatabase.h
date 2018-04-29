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
 @file    ActionDatabase.h
 @brief    Central database for querying available behavior actions.

 For actions to be used in the finite state machine, they must register themselves into the
 ActionDatabase. This is done via the PluginEngine.
 */

#ifndef __ACTION_DATABASE_H__
#define __ACTION_DATABASE_H__

#include "MengeCore/BFSM/Actions/Action.h"
#include "MengeCore/BFSM/Actions/ActionFactory.h"
#include "MengeCore/PluginEngine/ElementDatabase.h"

namespace Menge {

namespace BFSM {

/*!
 @brief    The database of registered action implementations.
 */
typedef ElementDB<ActionFactory, Action> ActionDB;
}  // namespace BFSM

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/*!
 @brief    Explicit specialization of addBuiltins for the Action Database
 */
template <>
void ElementDB<BFSM::ActionFactory, BFSM::Action>::addBuiltins();

/*!
 @brief    Explicit specialization of getElementName for the Action Database
 */
template <>
std::string ElementDB<BFSM::ActionFactory, BFSM::Action>::getElementName();

#endif  // DOXYGEN_SHOULD_SKIP_THIS

}  // namespace Menge

// Declarations of explicit specialization

#endif  // __ACTION_DATABASE_H__
