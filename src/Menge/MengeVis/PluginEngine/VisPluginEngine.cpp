#include "MengeVis/PluginEngine/VisPluginEngine.h"

#include "MengeVis/Runtime/AgentContext/AgentContextDatabase.h"
#include "MengeVis/Runtime/AgentContext/BaseAgentContext.h"
#include "MengeVis/Runtime/GoalRenderer/GoalRenderer.h"
#include "MengeVis/Runtime/GoalRenderer/GoalRendererDatabase.h"
#include "MengeVis/Runtime/VCContext/VelCompContext.h"
#include "MengeVis/Runtime/VCContext/VelCompContextDatabase.h"

namespace MengeVis {
	namespace PluginEngine {

		using Menge::PluginEngine::BasePluginEngine;
		using Menge::PluginEngine::Plugin;
		using MengeVis::Runtime::AgentContextDB;
		using MengeVis::Runtime::BaseAgentContext;
		using MengeVis::Runtime::VCContextDB;
		using MengeVis::Runtime::VCContext::VelCompContext;
		using MengeVis::Runtime::GoalRendererDB;
		using MengeVis::Runtime::GoalVis::GoalRenderer;

		/////////////////////////////////////////////////////////////////////
		//					Implementation of VisPluginEngine
		/////////////////////////////////////////////////////////////////////

		VisPluginEngine::VisPluginEngine() : 
			BasePluginEngine<VisPluginEngine, Plugin<VisPluginEngine> >() {
			AgentContextDB::initialize();
			VCContextDB::initialize();
			GoalRendererDB::initialize();
		}

		/////////////////////////////////////////////////////////////////////

		VisPluginEngine::~VisPluginEngine() {
		}

		/////////////////////////////////////////////////////////////////////

		void VisPluginEngine::registerAgentContext( BaseAgentContext * context ) {
			AgentContextDB::addVisElement( context );
		}

		/////////////////////////////////////////////////////////////////////

		void VisPluginEngine::registerVCContext( VelCompContext * context ) {
			VCContextDB::addVisElement( context );
		}
		
		/////////////////////////////////////////////////////////////////////

		void VisPluginEngine::registerGoalRenderer( GoalRenderer * renderer ) {
			GoalRendererDB::addVisElement( renderer );
		}

		/////////////////////////////////////////////////////////////////////

		std::string VisPluginEngine::getIntroMessage() {
			return "Loading Menge visualization plugins...";
		}
	}
}	// namespace Menge
