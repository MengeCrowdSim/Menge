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
 @file    GoalSelectorDatabase.h
 @brief    Central database for querying available agent goals.
 */

#ifndef __GOAL_SELECTOR_DATABASE_H__
#define __GOAL_SELECTOR_DATABASE_H__

#include "MengeCore/BFSM/GoalSelectors/GoalSelector.h"
#include "MengeCore/BFSM/GoalSelectors/GoalSelectorFactory.h"
#include "MengeCore/PluginEngine/ElementDatabase.h"

namespace Menge {

namespace BFSM {
/*!
 @brief   The element database for GoalSelectors
 */
typedef ElementDB<GoalSelectorFactory, GoalSelector> GoalSelectorDB;

}  // namespace BFSM

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/*!
 @brief    Explicit specialization of addBuiltins for the GoalSelector Database
 */
template <>
void ElementDB<BFSM::GoalSelectorFactory, BFSM::GoalSelector>::addBuiltins();

/*!
 @brief    Explicit specialization of getElementName for the GoalSelector Database
 */
template <>
std::string ElementDB<BFSM::GoalSelectorFactory, BFSM::GoalSelector>::getElementName();

#endif  // DOXYGEN_SHOULD_SKIP_THIS

}  // namespace Menge

#endif  // __GOAL_SELECTOR_DATABASE_H__
