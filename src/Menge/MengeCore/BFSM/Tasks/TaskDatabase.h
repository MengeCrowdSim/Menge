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
 @file    TaskDatabase.h
 @brief    Central database for querying available behavior actions.

 For tasks to be used in the finite state machine, they must register themselves into the
 TaskDatabase. This is done via the PluginEngine.
 */

#ifndef __TASK_DATABASE_H__
#define __TASK_DATABASE_H__

#include "MengeCore/BFSM/Tasks/Task.h"
#include "MengeCore/BFSM/Tasks/TaskFactory.h"
#include "MengeCore/PluginEngine/ElementDatabase.h"

namespace Menge {

namespace BFSM {

/*!
 @brief    The database of registered task implementations.
 */
typedef ElementDB<TaskFactory, Task> TaskDB;

}  // namespace BFSM

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/*!
 @brief    Explicit specialization of addBuiltins for the Task Database
 */
template <>
void ElementDB<BFSM::TaskFactory, BFSM::Task>::addBuiltins();

/*!
 @brief    Explicit specialization of getElementName for the Task Database
 */
template <>
std::string ElementDB<BFSM::TaskFactory, BFSM::Task>::getElementName();

#endif  // DOXYGEN_SHOULD_SKIP_THIS

}  // namespace Menge
#endif  // __TASK_DATABASE_H__
