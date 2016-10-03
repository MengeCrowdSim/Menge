/*!
 *	@file		STRESSGlobals.cpp
 *	@brief		Defines a static global STRESS manager
*/
#ifndef __STRESS_GLOBALS_CPP__
#define __STRESS_GLOBALS_CPP__

#include "StressGlobals.h"

//Define the global static proxy unit
namespace StressGAS {
	StressManager *STRESS_MANAGER = new StressManager();
}

#endif
