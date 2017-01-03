/*

License

Menge
Copyright � and trademark � 2012-14 University of North Carolina at Chapel Hill. 
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

#include "MengeCore/BFSM/GoalSelectors/GoalSelectorNearest.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/BFSM/GoalSet.h"
#include "MengeCore/BFSM/Goals/Goal.h"

#include <cassert>

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of NearestGoalSelector
		/////////////////////////////////////////////////////////////////////
		
		Goal * NearestGoalSelector::getGoal( const Agents::BaseAgent * agent ) const {
			assert( agent != 0x0 && "NearestGoalGenerator requires a valid base agent!" );
			const size_t GOAL_COUNT = _goalSet->size();
			if ( GOAL_COUNT == 0 ) {
				logger << Logger::ERR_MSG;
				logger << "NearestGoalSelector was unable to provide a goal for agent ";
				logger << agent->_id << ".  There were no available goals in the goal set.";
				return 0x0;
			}
			const Vector2 p = agent->_pos;

			Goal * bestGoal;

			bestGoal = _goalSet->getIthGoal( 0 );
			Vector2 disp = bestGoal->getCentroid() - p;
			float bestDist = absSq( disp );
			for ( size_t i = 1; i < GOAL_COUNT; ++i ) {
				Goal * testGoal = _goalSet->getIthGoal( i );
				disp = testGoal->getCentroid() - p;
				float testDist = absSq( disp );
				if ( testDist < bestDist ) {
					bestDist = testDist;
					bestGoal = testGoal;
				}
			}

			return bestGoal;
		}
	}	// namespace BFSM
}	// namespace Menge
