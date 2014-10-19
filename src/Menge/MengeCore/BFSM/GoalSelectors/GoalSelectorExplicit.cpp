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

#include "GoalSelectors/GoalSelectorExplicit.h"
#include "Goals/Goal.h"
#include "GoalSet.h"
#include "BaseAgent.h"
#include <cassert>

namespace Menge {

	namespace BFSM {
		/////////////////////////////////////////////////////////////////////
		//                   Implementation of ExplicitGoalSelector
		/////////////////////////////////////////////////////////////////////

		ExplicitGoalSelector::ExplicitGoalSelector():GoalSelector() {}

		/////////////////////////////////////////////////////////////////////
		
		Goal * ExplicitGoalSelector::getGoal( const Agents::BaseAgent * agent ) const {
			return _goal;
		}

		/////////////////////////////////////////////////////////////////////
		
		void ExplicitGoalSelector::setGoalSet( std::map< size_t, GoalSet * > & goalSets ) {
			if ( goalSets.count( _goalSetID ) == 1 ) {
				GoalSet * gs = goalSets[ _goalSetID ];
				size_t gid = _goalID;
				_goal = gs->getGoalByID( gid );
				if ( _goal == 0x0 ) {
					std::stringstream ss;
					logger << Logger::ERR_MSG << "Goal Selector cannot find targeted goal (" << gid << ") in desired goal set (" << _goalSetID << ").";
					throw GoalSelectorException();
				}
			} else {
				logger << Logger::ERR_MSG << "Explicit goal selector tried accessing a goal set that doesn't exist: " << _goalSetID << "\n";
				throw GoalSelectorException();
			}
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of ExplicitGoalSelectorFactory
		/////////////////////////////////////////////////////////////////////

		ExplicitGoalSelectorFactory::ExplicitGoalSelectorFactory() : GoalSelectorFactory() {
			_goalSetID = _attrSet.addSizeTAttribute( "goal_set", true /*required*/ );
			_goalID = _attrSet.addSizeTAttribute( "goal", true /*required*/ );
		}

		/////////////////////////////////////////////////////////////////////

		bool ExplicitGoalSelectorFactory::setFromXML( GoalSelector * selector, TiXmlElement * node, const std::string & behaveFldr ) const {
			ExplicitGoalSelector * gs = dynamic_cast< ExplicitGoalSelector * >( selector );
			assert( gs != 0x0 && "Trying to set explicit goal selector attributes on an incompatible object." );

			if ( ! GoalSelectorFactory::setFromXML( gs, node, behaveFldr ) ) return false;

			gs->setGoalSetID( _attrSet.getSizeT( _goalSetID ) );
			gs->setGoalID( _attrSet.getSizeT( _goalID ) );

			return true;
		}
	}	// namespace BFSM
}	// namespace Menge
