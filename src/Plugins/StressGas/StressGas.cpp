/*!
 *	@file		StressGAS.cpp  
 *	@brief		Plugin for StressGAS 
 */

#include "StressGasConfig.h"
#include "PluginEngine.h"
#include "TimedStressAction.h"
#include "DistanceStressAction.h"

/*!
 *	@brief		Retrieves the name of the plug-in.
 *
 *	@returns	The name of the plug in.
 */
extern "C" EXPORT_API const char * getName() {
	return "StressGAS_utils";
}

/*!
 *	@brief		Description of the plug-in.
 *
 *	@returns	A description of the plugin.
 */
extern "C" EXPORT_API const char * getDescription() {
	return	"Utilties for modelling stress according to the GAS model " \
		"including the following:\n"\
		"\tAction \"timed_stress\" - registers an agent to accumulate stress up to 100% stressed" \
		" in a specified number of seconds.";
}

/*!
 *	@brief		Registers the plug-in with the PluginEngine
 *
 *	@param		engine		A pointer to the plugin engine.
 */
extern "C" EXPORT_API void registerPlugin( PluginEngine * engine ) {
	engine->registerActionFactory( new StressGAS::TimedStressActionFactory() );
	engine->registerActionFactory( new StressGAS::DistanceStressActionFactory() );
}
