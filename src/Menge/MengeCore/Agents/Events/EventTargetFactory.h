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
 @file  EventTargetFactory.h
 @brief  The definition of the basic event target factory.
 */

#ifndef __EVENT_TARGET_FACTORY_H__
#define __EVENT_TARGET_FACTORY_H__

#include "MengeCore/Agents/Events/EventTarget.h"
#include "MengeCore/CoreConfig.h"
#include "MengeCore/PluginEngine/ElementFactory.h"

namespace Menge {

/*!
 @brief    The base class for generating event targets.
 */
class MENGE_API EventTargetFactory : public ElementFactory<EventTarget> {};

}  // namespace Menge
#endif  // __EVENT_TARGET_FACTORY_H__
