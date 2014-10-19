/*

License

Menge
Copyright © and trademark ™ 2012-14 University of North Carolina at Chapel Hill. 
All rights reserved.

Permission to use, copy, modify, and distribute this software and its documentation 
for educational, research, and non-profit purposes, without fee, and without a 
written agreement is hereby granted, provided that the above copyright notice, 
this paragraph, and the following four paragraphs appear in all copies.

This software program and documentation are copyrighted by the University of North 
Carolina at Chapel Hill. The software program and documentation are supplied "as is," 
without any accompanying services from the University of North Carolina at Chapel 
Hill or the authors. The University of North Carolina at Chapel Hill and the 
authors do not warrant that the operation of the program will be uninterrupted 
or error-free. The end-user understands that the program was developed for research 
purposes and is advised not to rely exclusively on the program for any reason.

IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE AUTHORS 
BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS 
DOCUMENTATION, EVEN IF THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE 
AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY 
DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY STATUTORY WARRANTY 
OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND 
THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS HAVE NO OBLIGATIONS 
TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

Any questions or comments should be sent to the authors {menge,geom}@cs.unc.edu

*/

//#define PLUGIN_EXPORT 1

#include <iostream>
#include "PluginEngine.h"
#include "os.h"
#include "SimulatorDB.h"
#include "Orca/ORCADBEntry.h"
#include "PedVO/PedVODBEntry.h"
#include "Actions/ActionDatabase.h"
#include "Transitions/ConditionDatabase.h"
#include "Transitions/TargetDatabase.h"
#include "VelocityComponents/VelComponentDatabase.h"
#include "VelocityModifiers/VelModifierDatabase.h"
#include "Tasks/TaskDatabase.h"
#include "Goals/GoalDatabase.h"
#include "GoalSelectors/GoalSelectorDatabase.h"
#include "Elevations/ElevationDatabase.h"
#include "SpatialQueries/SpatialQueryDatabase.h"
#include "AgentGenerators/AgentGeneratorDatabase.h"
#include "ProfileSelectors/ProfileSelectorDatabase.h"
#include "StateSelectors/StateSelectorDatabase.h"
#include "Events/EventTriggerDB.h"
#include "Events/EventEffectDB.h"
#include "Events/EventTargetDB.h"
#include "ObstacleSets/ObstacleSetDatabase.h"
#include "Logger.h"

namespace Menge {

	/////////////////////////////////////////////////////////////////////
	//					Implementation of PluginEngine
	/////////////////////////////////////////////////////////////////////

	PluginEngine::PluginEngine( SimulatorDB	* simDB ): _plugins(), _simDB(simDB) {
		registerModelDBEntry( new ORCA::DBEntry() );
		registerModelDBEntry( new PedVO::DBEntry() );
		BFSM::ActionDB::initialize();
		BFSM::ConditionDB::initialize();
		BFSM::TargetDB::initialize();
		BFSM::VelCompDB::initialize();
		BFSM::VelModDB::initialize();
		BFSM::TaskDB::initialize();
		BFSM::GoalDB::initialize();
		BFSM::GoalSelectorDB::initialize();
		Agents::ElevationDB::initialize();
		Agents::SpatialQueryDB::initialize();
		Agents::AgentGeneratorDB::initialize();
		Agents::ObstacleSetDB::initialize();
		Agents::ProfileSelectorDB::initialize();
		Agents::StateSelectorDB::initialize();
		EventEffectDB::initialize();
		EventTriggerDB::initialize();
		EventTargetDB::initialize();
	}

	/////////////////////////////////////////////////////////////////////

	PluginEngine::~PluginEngine() {
	}

	/////////////////////////////////////////////////////////////////////

	size_t PluginEngine::loadPlugins( const std::string & pluginFldr ) {
		//	1. get all files in pluginFldr that conform to a platform-dependent
		//		naming convention.
		//	2. For each one, try to open it and determine if it has the interface
		//		expected for this plugin engine
		//	3. If it does, register it with the appropriate manager.
		//			(NOt entirely sure what this means yet...ultimately, the manager
		//			has to be able to provide information about the entity in the
		//			plug-in and provide the ability to insantiate it.  "information"
		//			may also include the ability to parse itself from the xml.

		StringList files;
	#ifdef _MSC_VER
		std::string extension( "*.dll" );
	#else
		std::string extension( "*.so" );
	#endif
		if ( !os::listdir( pluginFldr, files, extension ) ) {
			return 0;
		}

		StringListCItr itr = files.begin();
		for ( ; itr != files.end(); ++itr ) {
			std::string fullPath;
			if ( ! os::path::absPath( os::path::join( 2, pluginFldr.c_str(), (*itr).c_str() ), fullPath ) ) {
				logger << Logger::ERR_MSG << "Unable to get absolute path for " << (*itr);
				continue;
			}
			Plugin * plugin;
			try {
				plugin = new Plugin( fullPath );
			} catch ( std::exception & ) {
				logger << Logger::ERR_MSG << "Failed loading " << fullPath << " as a plug-in";
				continue;
			}
			logger << Logger::INFO_MSG << "Loaded: " << plugin->getName() << "\n";;
			logger << "\t" << plugin->getDescription();
			plugin->registerPlugin( this );
			_plugins.insert( PluginMap::value_type( (*itr), plugin ) );
		}

		return files.size();
	}

	/////////////////////////////////////////////////////////////////////

	void PluginEngine::registerModelDBEntry( SimulatorDBEntry * dbEntry ) {
		_simDB->registerEntry( dbEntry );
	}

	/////////////////////////////////////////////////////////////////////

	void PluginEngine::registerActionFactory( BFSM::ActionFactory * factory ) {
		BFSM::ActionDB::addFactory( factory );
	}

	/////////////////////////////////////////////////////////////////////

	void PluginEngine::registerConditionFactory( BFSM::ConditionFactory * factory ) {
		BFSM::ConditionDB::addFactory( factory );
	}

	/////////////////////////////////////////////////////////////////////

	void PluginEngine::registerTargetFactory( BFSM::TargetFactory * factory ) {
		BFSM::TargetDB::addFactory( factory );
	}

	/////////////////////////////////////////////////////////////////////

	void PluginEngine::registerVelCompFactory( BFSM::VelCompFactory * factory ) {
		BFSM::VelCompDB::addFactory( factory );
	}

	/////////////////////////////////////////////////////////////////////

	void PluginEngine::registerVelModFactory( BFSM::VelModFactory * factory ) {
		BFSM::VelModDB::addFactory( factory );
	}

	/////////////////////////////////////////////////////////////////////

	void PluginEngine::registerTaskFactory( BFSM::TaskFactory * factory ) {
		BFSM::TaskDB::addFactory( factory );
	}

	/////////////////////////////////////////////////////////////////////

	void PluginEngine::registerGoalFactory( BFSM::GoalFactory * factory ) {
		BFSM::GoalDB::addFactory( factory );
	}

	/////////////////////////////////////////////////////////////////////

	void PluginEngine::registerGoalSelectorFactory( BFSM::GoalSelectorFactory * factory ) {
		BFSM::GoalSelectorDB::addFactory( factory );
	}

	/////////////////////////////////////////////////////////////////////

	void PluginEngine::registerElevationFactory( Agents::ElevationFactory * factory ) {
		Agents::ElevationDB::addFactory( factory );
	}

	/////////////////////////////////////////////////////////////////////

	void PluginEngine::registerSpatialQueryFactory( Agents::SpatialQueryFactory * factory ) {
		Agents::SpatialQueryDB::addFactory( factory );
	}

	/////////////////////////////////////////////////////////////////////

	void PluginEngine::registerAgentGeneratorFactory( Agents::AgentGeneratorFactory * factory ) {
		Agents::AgentGeneratorDB::addFactory( factory );
	}

	/////////////////////////////////////////////////////////////////////

	void PluginEngine::registerObstacleSetFactory( Agents::ObstacleSetFactory * factory ) {
		Agents::ObstacleSetDB::addFactory( factory );
	}

	/////////////////////////////////////////////////////////////////////

	void PluginEngine::registerProfileSelectorFactory( Agents::ProfileSelectorFactory * factory ) {
		Agents::ProfileSelectorDB::addFactory( factory );
	}

	/////////////////////////////////////////////////////////////////////

	void PluginEngine::registerStateSelectorFactory( Agents::StateSelectorFactory * factory ) {
		Agents::StateSelectorDB::addFactory( factory );
	}

	/////////////////////////////////////////////////////////////////////

	void PluginEngine::registerEventEffectFactory( EventEffectFactory * factory ) {
		EventEffectDB::addFactory( factory );
	}

	/////////////////////////////////////////////////////////////////////

	void PluginEngine::registerEventTriggerFactory( EventTriggerFactory * factory ) {
		EventTriggerDB::addFactory( factory );
	}

	/////////////////////////////////////////////////////////////////////

	void PluginEngine::registerEventTargetFactory( EventTargetFactory * factory ) {
		EventTargetDB::addFactory( factory );
	}
}	// namespace Menge