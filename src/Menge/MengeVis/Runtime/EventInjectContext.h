#pragma once
/*!
 *	@file	EventInjectContext.h
 *	@brief	The definition of a context that captures ExternalEventTriggers in the system
 *			and maps them to user actions.
 */

#include "MengeVis/Runtime/MengeContext.h"

// forward declarations
namespace Menge {
	namespace Agents {
		class SimulatorInterface;
	}
}

namespace MengeVis {
	namespace Runtime {
		/*!
		 *	@brief		A context that connects user actions to Menge::ExternalEventTrigger.
		 *
		 *	The EventInjectionContext serves as a layer between UI and the simulator. It keys on
		 *	ExternalEventTrigger instances with particular names. Each of the specific names is
		 *	triggered by a specific user action. The following table shows the actions and 
		 *	the name of the trigger that gets fired:
		 *
		 *	User Action               | Trigger Name   
		 *  --------------------------|---------------------
		 *  Press left arrow          | left_arrow 
		 *	Press right arrow         | right_arrow
		 *	Press up arrow            | up_arrow
		 *	Press down arrow          | down_arrow
		 *	Press spacebar            | spacebar
		 *	Click left mouse button   | left_mouse
		 *	Click right mouse button  | right_mouse
		 *	Click middle mouse button | middle_mouse
		 *	Roll mouse wheel up       | mouse_wheel_up
		 *	Roll mouse wheel down     | mouse_wheel_down
		 *
		 *	Note: no mouse data is provided (i.e., mouse position, state of modifiers, etc.)
		 *	The simple user action is sufficient to fire the trigger.
		 *
		 *	The EventInjectionContext also contains a child context. Events that trigger simulation
		 *	events are _also_ propagated down to the child context.
		 *
		 *	Finally, if the EvenEjectionContext finds appropriate external triggers, it will
		 *	display the valid event actions in the display (the display of which is toggle-able
		 *	by the user).
		 */
		class MENGEVIS_API EventInjectionContext : public MengeContext {
		public:
			/*!
			 *	@brief	Constructor.
			 *
			 *	@param	sim		The simulator being run.
			 *	@param	ctx		The optional pass-through context.
			 */
			EventInjectionContext( Menge::Agents::SimulatorInterface* sim,
								   MengeContext * ctx = nullptr );
		};
	}
}
