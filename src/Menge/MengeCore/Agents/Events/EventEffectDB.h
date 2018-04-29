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
 @file    EventEffectDB.h
 @brief    Central database for querying available event effect implementations.

 For event effects to be used in simulation, they must register themselves into the EventEffectDB.
 This is done via the PluginEngine.
 */

#ifndef __EVENT_EFFECT_DATABASE_H__
#define __EVENT_EFFECT_DATABASE_H__

#include "MengeCore/Agents/Events/EventEffect.h"
#include "MengeCore/Agents/Events/EventEffectFactory.h"
#include "MengeCore/PluginEngine/ElementDatabase.h"

namespace Menge {
/*!
 @brief    The database of registered event effects implementations.
 */
typedef ElementDB<EventEffectFactory, EventEffect> EventEffectDB;

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template <>
void ElementDB<EventEffectFactory, EventEffect>::addBuiltins();
template <>
std::string ElementDB<EventEffectFactory, EventEffect>::getElementName();

#endif  // DOXYGEN_SHOULD_SKIP_THIS
}  // namespace Menge

#endif  // __EVENT_EFFECT_DATABASE_H__
