/*!
 *	@file		StressGAS.cpp  
 *	@brief		Plugin for StressGAS 
 */

#include "StressGasConfig.h"
#include "PluginEngine.h"
#include "StressActions.h"

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
			"\tAction \"general_stress\" - registers an agent for general stress as opposed to positional or temporal stress\n " \
			"Stress is applied to agents each second, independent of the simulation timestep, using a task.";
}

/*!
 *	@brief		Registers the plug-in with the PluginEngine
 *
 *	@param		engine		A pointer to the plugin engine.
 */
extern "C" EXPORT_API void registerPlugin( PluginEngine * engine ) {
	engine->registerActionFactory( new StressGAS::RegisterStressAgentActionFactory() );
}
