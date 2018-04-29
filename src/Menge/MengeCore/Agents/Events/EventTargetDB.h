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
 @file    EventTargetDB.h
 @brief    Central database for querying available event target implementations.

 For event targets to be used in simulation, they must register themselves into the EventTargetDB.
 This is done via the PluginEngine.
 */

#ifndef __EVENT_TARGET_DATABASE_H__
#define __EVENT_TARGET_DATABASE_H__

#include "MengeCore/Agents/Events/EventTarget.h"
#include "MengeCore/Agents/Events/EventTargetFactory.h"
#include "MengeCore/PluginEngine/ElementDatabase.h"

namespace Menge {
/*!
 @brief    The database of registered event targets implementations.
 */
typedef ElementDB<EventTargetFactory, EventTarget> EventTargetDB;

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template <>
void ElementDB<EventTargetFactory, EventTarget>::addBuiltins();
template <>
std::string ElementDB<EventTargetFactory, EventTarget>::getElementName();

#endif  // DOXYGEN_SHOULD_SKIP_THIS

}  // namespace Menge

#endif  // __EVENT_TARGET_DATABASE_H__
