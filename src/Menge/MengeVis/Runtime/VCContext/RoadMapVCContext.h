/*!
 *	@file		RoadMapVCContext.h
 *	@brief		Definition of the context to use with road map velocity omponents.
 */

#ifndef __ROAD_MAP_VC_CONTEXT_H__
#define __ROAD_MAP_VC_CONTEXT_H__

#include "MengeVis/Runtime/VCContext/VelCompContext.h"

// forward declarations
namespace Menge {
	namespace Agents {
		class BaseAgent;
	}
	namespace BFSM {
		class RoadMapVelComponent;
		class Goal;
	}
}

namespace MengeVis {
	namespace Runtime {
		namespace VCContext {

			/*!
			*	@brief		The context for the RoadMapVelComponent.
			*/
			class MENGEVIS_API RoadMapVCContext : public VelCompContext {
			public:
				/*!
				 *	@brief		Constructor.
				 */
				RoadMapVCContext();

				/*!
				 *	@brief		Sets the agent for this context.
				 *
				 *	This method works in conjunction with the VisElementDatabase. When this
				 *	visualization element is triggered, the database will supply the triggering
				 *	element.
				 *
				 *	@param		agent		The agent to interact with.
				 */
				virtual void setElement( Menge::BFSM::VelComponent * vc );

				/*!
				 *	@brief		Provides a string to be printed in the display as a UI element
				 *				detailing velocity component information.
				 *
				 *	@param		indent		An optional string representing indentation to be
				 *							applied to the text.  It is prefixed at the start
				 *							of each line.
				 *	@returns	The string for printing on the UI layer.
				 */
				virtual std::string getUIText( const std::string & indent = "" ) const;

				/*!
				 *	@brief		Draw context elements into the 3D world.
				 *
				 *	This should never be called in select mode.
				 *
				 *	@param		agt			The particular agent for which the FSM is being
				 *							visualized.
				 *	@param		goal		The agent's goal (although this may be ignored).
				 */
				virtual void draw3DGL( const Menge::Agents::BaseAgent * agt,
									   const Menge::BFSM::Goal * goal );

			protected:
				/*!
				*	@brief		The underlying finite state machine velocity component.
				*/
				Menge::BFSM::RoadMapVelComponent * _vc;
			};
		}	// namespace VCContext
	}	// namespace Runtime
}	// namespace MengeVis

#endif	// __ROAD_MAP_VC_CONTEXT_H__
