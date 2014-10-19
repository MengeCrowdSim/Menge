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

/*!
 *	@file		PluginEngine.h
 *	@brief		Definition of the plug-in architecture.
 */

#ifndef __PLUGIN_ENGINE_H__
#define __PLUGIN_ENGINE_H__

#include <string>
#include <map>
#include "Plugin.h"

namespace Menge {

	// Forward declaration
	class SimulatorDBEntry;
	class SimulatorDB;

	namespace BFSM {	
		class ActionFactory;
		class ConditionFactory;
		class TargetFactory;
		class VelCompFactory;
		class VelModFactory;
		class TaskFactory;
		class GoalFactory;
		class GoalSelectorFactory;
	}

	namespace Agents {
		class ElevationFactory;
		class SpatialQueryFactory;
		class AgentGeneratorFactory;
		class ProfileSelectorFactory;
		class StateSelectorFactory;
		class ObstacleSetFactory;
	}

	class EventTriggerFactory;
	class EventEffectFactory;
	class EventTargetFactory;

	/*!
	 *	@brief		Mapping from the file name of the dynamic library to a pointer to
	 *				the loaded plugin.
	 */
	typedef std::map< std::string, Plugin * > PluginMap;

	/*!
	 *	@brief		The class responsible for finding, managing, and deleting
	 *				plugins.
	 */
	class PluginEngine {
	public:
		// TODO: Make the make SimulatorDB a PedestrianDB and make it static, like
		//	the ActionFactory
		/*!
		 *	@brief		Default constructor.
		 *
		 *	@param		simDB		A simulator database for pedestrian model registration
		 */
		MENGE_API PluginEngine( SimulatorDB	* simDB );

		/*!
		 *	@brief		Destructor
		 */
		MENGE_API ~PluginEngine();

		/*!
		 *	@brief		Initialize plug-ins from the given directory.
		 *
		 *	@param		pluginFldr		The folder to search for plugins.
		 *	@returns	The number of plugins successfully loaded.
		 */
		MENGE_API size_t loadPlugins( const std::string & pluginFldr );

		/*!
		 *	@brief		Register a simulator database entry.
		 *
		 *	@param		dbEntry		An instance of a model database entry to register.
		 */
		MENGE_API void registerModelDBEntry( SimulatorDBEntry * dbEntry );

		/*!
		 *	@brief		Register an ActionFactory.
		 *
		 *	@param		factory		An instance of the ActionFactory to register
		 */
		MENGE_API void registerActionFactory( BFSM::ActionFactory * factory );

		/*!
		 *	@brief		Register a ConditionFactory.
		 *
		 *	@param		factory		An instance of the ConditionFactory to register
		 */
		MENGE_API void registerConditionFactory( BFSM::ConditionFactory * factory );

		/*!
		 *	@brief		Register a TargetFactory.
		 *
		 *	@param		factory		An instance of the TargetFactory to register
		 */
		MENGE_API void registerTargetFactory( BFSM::TargetFactory * factory );

		/*!
		 *	@brief		Register a VelCompFactory.
		 *
		 *	@param		factory		An instance of the VelCompFactory to register
		 */
		MENGE_API void registerVelCompFactory( BFSM::VelCompFactory * factory );

		/*!
		 *	@brief		Register a VelModFactory.
		 *
		 *	@param		factory		An instance of the VelModifier to register
		 */
		MENGE_API void registerVelModFactory( BFSM::VelModFactory * factory );

		/*!
		 *	@brief		Register a TaskFactory.
		 *
		 *	@param		factory		An instance of the TaskFactory to register
		 */
		MENGE_API void registerTaskFactory( BFSM::TaskFactory * factory );

		/*!
		 *	@brief		Register a GoalFactory.
		 *
		 *	@param		factory		An instance of the GoalFactory to register
		 */
		MENGE_API void registerGoalFactory( BFSM::GoalFactory * factory );

		/*!
		 *	@brief		Register a GoalSelectorFactory.
		 *
		 *	@param		factory		An instance of the GoalSelectorFactory to register
		 */
		MENGE_API void registerGoalSelectorFactory( BFSM::GoalSelectorFactory * factory );

		/*!
		 *	@brief		Register an ElevationFactory.
		 *
		 *	@param		factory		An instance of the ElevationFactory to register
		 */
		MENGE_API void registerElevationFactory( Agents::ElevationFactory * factory );

		/*!
		 *	@brief		Register an SpatialQueryFactory.
		 *
		 *	@param		factory		An instance of the SpatialQueryFactory to register
		 */
		MENGE_API void registerSpatialQueryFactory( Agents::SpatialQueryFactory * factory );

		/*!
		 *	@brief		Register an AgentGeneratorFactory.
		 *
		 *	@param		factory		An instance of the AgentGeneratorFactory to register
		 */
		MENGE_API void registerAgentGeneratorFactory( Agents::AgentGeneratorFactory * factory );

		/*!
		 *	@brief		Register an ObstacleSetFactory.
		 *
		 *	@param		factory		An instance of the ObstacleSetFactory to register
		 */
		MENGE_API void registerObstacleSetFactory( Agents::ObstacleSetFactory * factory );

		/*!
		 *	@brief		Register an ProfileSelectorFactory.
		 *
		 *	@param		factory		An instance of the ProfileSelectorFactory to register
		 */
		MENGE_API void registerProfileSelectorFactory( Agents::ProfileSelectorFactory * factory );

		/*!
		 *	@brief		Register an StateSelectorFactory.
		 *
		 *	@param		factory		An instance of the StateSelectorFactory to register
		 */
		MENGE_API void registerStateSelectorFactory( Agents::StateSelectorFactory * factory );

		/*!
		 *	@brief		Register an EventEffectFactory.
		 *
		 *	@param		factory		An instance of the EventEffectFactory to register
		 */
		MENGE_API void registerEventEffectFactory( EventEffectFactory * factory );

		/*!
		 *	@brief		Register an EventTriggerFactory.
		 *
		 *	@param		factory		An instance of the EventTriggerFactory to register
		 */
		MENGE_API void registerEventTriggerFactory( EventTriggerFactory * factory );

		/*!
		 *	@brief		Register an EventTargetFactory.
		 *
		 *	@param		factory		An instance of the EventTargetFactory to register
		 */
		MENGE_API void registerEventTargetFactory( EventTargetFactory * factory );

	protected:
		/*!
		 *	@brief		The loaded plugins.
		 */
		PluginMap	_plugins;

		/*!
		 *	@brief		The simulator database for registration.
		 */
		SimulatorDB	* _simDB;
	};
}	 // namespace Menge
#endif	 // __PLUGIN_ENGINE_H__