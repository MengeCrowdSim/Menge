/*!
 *	@file		ProxyGlobals.h
 *	@brief		Defines a static global proxyagent manager
*/
#ifndef __STRESS_GLOBALS_H__
#define __STRESS_GLOBALS_H__

#include "StressGasConfig.h"
#include "StressManager.h"


//Define the global static proxy unit
namespace StressGAS {
	extern EXPORT_API StressManager *STRESS_MANAGER;
}

#endif
