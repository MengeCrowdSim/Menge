/*!
 *	@file		VisPluginEngine.h
 *	@brief		Definition of the plugin engine used for visualization Menge plugins.
 */

#ifndef __VIS_PLUGIN_ENGINE_H__
#define __VIS_PLUGIN_ENGINE_H__

#include "MengeCore/PluginEngine/BasePluginEngine.h"
#include "MengeCore/PluginEngine/Plugin.h"
#include "MengeVis/VisConfig.h"

/*!
 *	@namespace	MengeVis
 *	@brief		The namespace that contains the supplementary visualization functionality.
 */
namespace MengeVis {
	// forward declarations
	namespace Runtime {
		class BaseAgentContext;
		namespace GoalVis {
			class GoalRenderer;
		}
		namespace VCContext {
			class VelCompContext;
		}
	}

	/*!
	 *	@namespace	MengeVis::PluginEngine
	 *	@brief		The namespace that contains the visualization plugin functionality..
	 */
	namespace PluginEngine {
		/*!
		 *	@brief		The definition of the plugin engine for Menge's core simulation
		 *				functionality.
		 */
		class MENGEVIS_API VisPluginEngine : 
			public Menge::PluginEngine::BasePluginEngine<VisPluginEngine, 
							               Menge::PluginEngine::Plugin<VisPluginEngine> > {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			VisPluginEngine();

			/*!
			 *	@brief		Virtual destructor.
			 */
			virtual ~VisPluginEngine();

			/*!
			 *	@brief		Register an AgentContext instance.
			 *
			 *	@param		context		An instance of an agent context with a unique element name.
			 */
			void registerAgentContext( Runtime::BaseAgentContext * context );

			/*!
			 *	@brief		Register a VelCompContext instance.
			 *
			 *	@param		context		An instance of a velocity component context with a globally
			 *							unique element name.
			 */
			void registerVCContext( Runtime::VCContext::VelCompContext * context );

			/*!
			 *	@brief		Register a GoalRenderer instance.
			 *
			 *	@param		renderer	An instance of a goal renderer with a globally unique
			 *							element name.
			 */
			void registerGoalRenderer( Runtime::GoalVis::GoalRenderer * renderer );

		protected:
			/*!
			 *	@brief		Provides the string that will be written to the logger as an info
			 *				at the beginning of plugin loading.  It should be specialized for each
			 *				type of plugin engine.
			 */
			virtual std::string getIntroMessage();
		};
	}	// namespace PluginEngine
}	// namespace Menge

/////////////////////////////////////////////////////////////////////
//					Implementation of Plugin
/////////////////////////////////////////////////////////////////////

namespace Menge {
	namespace PluginEngine {
		template<>
		const char * 
		Plugin< MengeVis::PluginEngine::VisPluginEngine >::getRegisterName() const {
			return "registerVisPlugin";
		}
	}  // namespace PluginEngine
}  // namespace Menge

#endif // __VIS_PLUGIN_ENGINE_H__
