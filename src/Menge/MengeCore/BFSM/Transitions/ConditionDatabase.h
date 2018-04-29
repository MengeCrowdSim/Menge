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
 @file    ConditionDatabase.h
 @brief    Central database for querying available behavior conditions.

 For conditions to be used in the finite state machine, they must register themselves into the
 ConditionDatabase.  This is done via the PluginEngine.
 */

#ifndef __CONDITION_DATABASE_H__
#define __CONDITION_DATABASE_H__

#include "MengeCore/BFSM/Transitions/Condition.h"
#include "MengeCore/BFSM/Transitions/ConditionFactory.h"
#include "MengeCore/PluginEngine/ElementDatabase.h"

namespace Menge {

namespace BFSM {
/*!
 @brief    The database of registered condition implementations.
 */
typedef ElementDB<ConditionFactory, Condition> ConditionDB;

}  // namespace BFSM

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/*!
 @brief    Explicit specialization of addBuiltins for the Condition Database
 */
template <>
void ElementDB<BFSM::ConditionFactory, BFSM::Condition>::addBuiltins();

/*!
 @brief    Explicit specialization of getElementName for the Condition Database
 */
template <>
std::string ElementDB<BFSM::ConditionFactory, BFSM::Condition>::getElementName();

#endif  // DOXYGEN_SHOULD_SKIP_THIS

}  // namespace Menge
#endif  // __CONDITION_DATABASE_H__
