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
 @file    TaskFactory.h
 @brief    The factory for parsing xml data and instantiating tasks.
 */

#ifndef __TASK_FACTORY_H__
#define __TASK_FACTORY_H__

#include "MengeCore/BFSM/Tasks/Task.h"
#include "MengeCore/CoreConfig.h"
#include "MengeCore/PluginEngine/ElementFactory.h"

#include <string>

namespace Menge {

namespace BFSM {
/*!
 @brief    A class for parsing the xml description of a task and instantiating particular instances.
 */
class MENGE_API TaskFactory : public ElementFactory<Task> {};
}  // namespace BFSM
}  // namespace Menge
#endif  // __TASK_FACTORY_H__
