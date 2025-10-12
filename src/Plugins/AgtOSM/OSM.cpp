/*
 OSM Simulation plugin

 Sergio Lozano Mateos
 UNED

*/

/*!
 *	@file		OSM.cpp
 *	@brief		Plugin for orca pedestrian.
 */

#include "OSMConfig.h"
#include "OSMDBEntry.h"
#include "MengeCore/PluginEngine/CorePluginEngine.h"

using Menge::PluginEngine::CorePluginEngine;

extern "C" {
/*!
 *	@brief		Retrieves the name of the plug-in.
 *
 *	@returns	The name of the plug in.
 */
OSM_API const char* getName() { return "Optimal Step Model 2012"; }

/*!
 *	@brief		Description of the plug-in.
 *
 *	@returns	A description of the plugin.
 */
OSM_API const char* getDescription() {
  return "A pedestrian plugin based on the model proposed in 2012 by M. J. Seitz and G. Köster";
}

/*!
 *	@brief		Registers the plug-in with the PluginEngine
 *
 *	@param		engine		A pointer to the plugin engine.
 */
OSM_API void registerCorePlugin(CorePluginEngine* engine) {
  engine->registerModelDBEntry(new OSM::DBEntry());
}
}
