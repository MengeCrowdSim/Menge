/*!
 *	@file		StressGlobals.h
 *	@brief		Defines a static global proxyagent manager
*/
#ifndef __STRESS_GLOBALS_H__
#define __STRESS_GLOBALS_H__

#include "StressGasConfig.h"
#include "StressManager.h"

//Define the global static proxy unit
namespace StressGAS {
	// TODO(curds01) 10/4/2016 - This is not particularly thread safe
	/**
	 *	@brief		Manager accessible to all StressGas members
	 */
	extern EXPORT_API StressManager *STRESS_MANAGER;
}	// namespace StressGAS

#endif
