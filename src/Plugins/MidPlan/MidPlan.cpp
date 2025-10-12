/*

Sergio Lozano Mateos
UNED, Spain

*/

/*!
 *	@file		MidPlan.cpp
 *	@brief		Plugin for Mid-term planning
 */

#if defined(_MSC_VER)
#define PLUGIN_EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#define PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

#include "MidPlanConfig.h"
#include "MidPlanModifier.h"
#include "MengeCore/PluginEngine/CorePluginEngine.h"

using Menge::PluginEngine::CorePluginEngine;

extern "C" {

/*!
 *	@brief		Retrieves the name of the plug-in.
 *
 *	@returns	The name of the plug in.
 */
extern "C" MPMODIFIER_API const char* getName() { return "midterm_planning"; }

/*!
 *	@brief		Description of the plug-in.
 *
 *	@returns	A description of the plugin.
 */
extern "C" MPMODIFIER_API const char* getDescription() {
  return "Velocity Modifier plugin for adherence to EACS algorithm"
         "including the following:\n"
         "\tModifier \"midterm_planning\" - adjusts the agent's preferred velocity to match the "
         "Mid-term Plan adaptation. ";
}

/*!
 *	@brief		Registers the plug-in with the PluginEngine
 *
 *	@param		engine		A pointer to the plugin engine.
 */
extern "C" MPMODIFIER_API void registerCorePlugin(CorePluginEngine* engine) {
  engine->registerVelModFactory(new MPModifier::MPModifierFactory());
}
}