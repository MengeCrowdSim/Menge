/*!
 *	@file		StressGlobals.cpp
 *	@brief		Defines a static global STRESS manager
 */

#include "StressGlobals.h"

namespace StressGAS {
// TODO(curds01) 10/4/16 - This is not particularly threadsafe.  However, it needs to be.
//	Agents can enter the stress manager at arbitrary times during the BFSM execution and
//	access to the maps can collide.
StressManager* STRESS_MANAGER = new StressManager();
}  // namespace StressGAS
