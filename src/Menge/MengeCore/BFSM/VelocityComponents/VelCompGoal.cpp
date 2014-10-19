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

#include "VelocityComponents/VelCompGoal.h"
#include "BaseAgent.h"
#include "PrefVelocity.h"
#include "Goals/Goal.h"
#include "Core.h"

#include <sstream>
#include <iomanip>

namespace Menge {
	
		namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of GoalVelComponent
		/////////////////////////////////////////////////////////////////////

		GoalVelComponent::GoalVelComponent(): VelComponent() {
		}

		/////////////////////////////////////////////////////////////////////

		void GoalVelComponent::setPrefVelocity( const Agents::BaseAgent * agent, const Goal * goal, Agents::PrefVelocity & pVel ) {
			// directions
			goal->setDirections( agent->_pos, agent->_radius, pVel );
			
			// speed
			Vector2 goalPoint = pVel.getTarget();
			Vector2 disp = goalPoint - agent->_pos;
			const float distSq = absSq( disp );
			float speed = agent->_prefSpeed;

			if ( distSq <= 0.0001f ) {
				// I've basically arrived -- speed should be zero.
				speed = 0.f;
			} else {
				const float speedSq = speed * speed;
				const float TS_SQD = SIM_TIME_STEP * SIM_TIME_STEP;
				if ( distSq / speedSq < TS_SQD ) {
					// The distance is less than I would travel in a single time step.
					speed = sqrtf( distSq ) / SIM_TIME_STEP;
				}
			}
			pVel.setSpeed( speed );
		}

		/////////////////////////////////////////////////////////////////////
		
		VelCompContext * GoalVelComponent::getContext() {
			return new GoalVCContext( this );
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of GoalVCContext
		/////////////////////////////////////////////////////////////////////

		GoalVCContext::GoalVCContext( GoalVelComponent * vc ):VelCompContext(),_vc(vc) {
		}

		/////////////////////////////////////////////////////////////////////

		std::string GoalVCContext::getUIText( const std::string & indent ) const {
			std::stringstream ss;
			ss << indent << "Goal velocity component";
			return ss.str();
		}

		/////////////////////////////////////////////////////////////////////

		void GoalVCContext::draw3DGL( const Agents::BaseAgent * agt, const Goal * goal ) {
			// draw goal point
			// TODO: Get the elevation of the goalPoint
			Vector2 goalPoint = goal->getTargetPoint( agt->_pos, agt->_radius );
			drawGoal( goalPoint, agt );
			
			// draw preferred velocity
			Agents::PrefVelocity pVel;
			_vc->setPrefVelocity( agt, goal, pVel );
			drawPrefVel( pVel, agt->_pos );
		}
		
	} // namespace BFSM
}	// namespace Menge