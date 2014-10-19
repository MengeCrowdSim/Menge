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
 *	@file		VelCompGoal.h
 *	@brief		Provides the definition of the simple velocity component
 *				that computes a preferred velocity directly towards the goal.
 */

#ifndef __VEL_COMP_GOAL_H__
#define __VEL_COMP_GOAL_H__

#include "CoreConfig.h"
#include "VelocityComponents/VelComponent.h"
#include "VelocityComponents/VelComponentFactory.h"
#include "VelocityComponents/VelCompContext.h"

namespace Menge {

	namespace BFSM {

		/*!
		 *	@brief		A velocity component that returns a preferred velocity whose direction
		 *				points from the current agent position toward the goal point.
		 */
		class MENGE_API GoalVelComponent: public VelComponent {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			GoalVelComponent();

			/*!
			 *	@brief		Computes and sets the agent's preferred velocity.
			 *
			 *	The velocity component directly sets preferred velocity values in the
			 *	the provided preferred velocity instance.  See Agents::PrefVelocity for details.
			 *	Rather than setting the agent's preferred velocity value directly, a reference
			 *	to a preferred velocity instance is passed in to make the use more general.
			 *	This allows the computation of the preferred velocity for the agent, without
			 *	necessarily making changes to it.
			 *
			 *	@param		agent		The agent for which a preferred velocity is computed.
			 *	@param		goal		The agent's goal (although this may be ignored).
			 *	@param		pVel		The instance of Agents::PrefVelocity to set.
			 */
			virtual void setPrefVelocity( const Agents::BaseAgent * agent, const Goal * goal, Agents::PrefVelocity & pVel );

			/*!
			 *	@brief		Provides a display context for interacting with this velocity component.
			 *
			 *	It is the responsibility of the caller to delete the provided context.
			 *
			 *	@returns	A pointer to a context for this vel component.
			 */
			virtual VelCompContext * getContext();
		};

		//////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The context for the GoalVelComponent.
		 */
		class MENGE_API GoalVCContext : public VelCompContext {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		vc			A pointer to the underlying fsm velocity component.
			 *							The context will *not* delete the velocity component.
			 */
			GoalVCContext( GoalVelComponent * vc );

			/*!
			 *	@brief		Provides a string to be printed in the display as a UI element
			 *				detailing velocity component information.
			 *
			 *	@param		indent		An optional string representing indentation to be
			 *							applied to the text.  It is prefixed at the start
			 *							of each line.
			 *	@returns	The string for printing on the UI layer.
			 */
			virtual std::string getUIText( const std::string & indent="" ) const;

			/*!
			 *	@brief		Draw context elements into the 3D world.
			 *
			 *	This should never be called in select mode.
			 *
			 *	@param		agt			The particular agent for which the FSM is being visualized.
			 *	@param		goal		The agent's goal (although this may be ignored).
			 */
			virtual void draw3DGL( const Agents::BaseAgent * agt, const Goal * goal );

		protected:
			/*!
			 *	@brief		The underlying finite state machine velocity component.
			 */
			GoalVelComponent * _vc;
		};

		//////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Factory for the GoalVelComponent.
		 */
		class MENGE_API GoalVCFactory : public VelCompFactory {
		public:
			/*!
			 *	@brief		The name of the velocity component.
			 *
			 *	The velocity component's name must be unique among all registered 
			 *	velocity components.  Each velocity component factory must override this function.
			 *
			 *	@returns	A string containing the unique velocity component name.
			 */
			virtual const char * name() const { return "goal"; }

			/*!
			 *	@brief		A description of the velocity component.
			 *
			 *	Each velocity component factory must override this function.
			 *
			 *	@returns	A string containing the velocity component description.
			 */
			virtual const char * description() const {
				return "Provides a preferred velocity which always aims directly toward the goal (at the agent's preferred speed)"\
					" unless it will overstep the goal in a single time step, then it is scaled down.";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's velocity component.
			 *
			 *	All VelCompFactory sub-classes must override this by creating (on the heap)
			 *	a new instance of its corresponding velocity component type.  The various field values
			 *	of the instance will be set in a subsequent call to VelCompFactory::setFromXML.
			 *	The caller of this function takes ownership of the memory.
			 *
			 *	@returns		A pointer to a newly instantiated VelComponent class.
			 */
			VelComponent * instance() const { return new GoalVelComponent(); }
		};

	}	// namespace BFSM
}	// namespace Menge

#endif	 // __VEL_COMP_GOAL_H__