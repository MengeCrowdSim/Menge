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
 *	@file	EventTriggerFactory.h
 *	@brief	The definition of the basic event trigger factory.
 */

#ifndef __EVENT_TRIGGER_FACTORY_H__
#define __EVENT_TRIGGER_FACTORY_H__

#include "MengeCore/CoreConfig.h"
#include "MengeCore/PluginEngine/ElementFactory.h"
#include "MengeCore/Agents/Events/EventTrigger.h"

namespace Menge {

	/*!
	 *	@brief		The base class for generating event triggers.
	 */
	class MENGE_API EventTriggerFactory : public ElementFactory< EventTrigger > {
	};

}	// namespace Menge
#endif	// __EVENT_TRIGGER_FACTORY_H__
