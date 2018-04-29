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
 @file    GoalDatabase.h
 @brief    Central database for querying available agent goals.
 */

#ifndef __GOAL_DATABASE_H__
#define __GOAL_DATABASE_H__

#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/BFSM/Goals/GoalFactory.h"
#include "MengeCore/PluginEngine/ElementDatabase.h"

namespace Menge {

namespace BFSM {

/*!
 @brief    The database of registered goal implementations.
 */
typedef ElementDB<GoalFactory, Goal> GoalDB;

}  // namespace BFSM

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/*!
 @brief    Explicit specialization of addBuiltins for the Goal Database
 */
template <>
void ElementDB<BFSM::GoalFactory, BFSM::Goal>::addBuiltins();

/*!
 @brief    Explicit specialization of getElementName for the Goal Database
 */
template <>
std::string ElementDB<BFSM::GoalFactory, BFSM::Goal>::getElementName();

#endif  // DOXYGEN_SHOULD_SKIP_THIS

}  // namespace Menge

#endif  // __GOAL_DATABASE_H__
