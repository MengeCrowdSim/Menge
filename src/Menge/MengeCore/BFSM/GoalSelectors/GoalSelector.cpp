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

#include "GoalSelectors/GoalSelector.h"
#include "GoalSelectors/GoalSelectorDatabase.h"
#include "Goals/Goal.h"
#include "BaseAgent.h"

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//					Implementation of GoalSelector
		/////////////////////////////////////////////////////////////////////

		Goal *	GoalSelector::assignGoal( const Agents::BaseAgent * agent ) {
			Goal * goal = 0x0;
			if ( _persistent ) {
				_lock.lockRead();
				HASH_MAP< size_t, Goal * >::iterator itr = _assignedGoals.find( agent->_id );
				if ( itr != _assignedGoals.end() ) {
					goal = itr->second;
				}
				_lock.releaseRead();
				if ( goal ) return goal;
			}
			
			// Either not persistent, or no goal previously assigned
			lockResources();
			goal = getGoal( agent );
			if ( goal == 0x0 ) {
				logger << Logger::ERR_MSG << "Goal selector unable to create goal for agent " << agent->_id << ".";
				throw GoalSelectorException();
			}
			try {
				goal->assign( agent );
			} catch ( GoalException ) {
				logger << Logger::ERR_MSG << "Couldn't assign agent " << agent->_id << " to goal " << goal->getID() << ".";
				throw GoalSelectorException();
			}
			releaseResources();

	#ifdef _DEBUG
			// In debug mode, always store the assigned goal
			_lock.lockWrite();
			_assignedGoals[ agent->_id ] = goal;
			_lock.releaseWrite();
	#else
			// In release mode, only assign it if persistent
			if ( _persistent ) {
				_lock.lockWrite();
				_assignedGoals[ agent->_id ] = goal;
				_lock.releaseWrite();
			}
	#endif
			return goal;
		}

		/////////////////////////////////////////////////////////////////////
		
		void GoalSelector::freeGoal( const Agents::BaseAgent * agent, Goal * goal ) {
	#ifdef _DEBUG
			_lock.lockWrite();
			assert( _assignedGoals.count( agent->_id ) == 1 && "Trying to free a goal from an agent that hasn't actually been assigned." );
			assert( _assignedGoals[ agent->_id ] == goal && "Trying to free the wrong goal from the agent" );
			_lock.releaseWrite();
	#endif 
			if ( !_persistent ) {
				goal->free();
	#ifdef _DEBUG
				_lock.lockWrite();
				_assignedGoals.erase( agent->_id );
				_lock.releaseWrite();
	#endif
			}
		}
		
		/////////////////////////////////////////////////////////////////////
		//					Implementation of parsing function
		/////////////////////////////////////////////////////////////////////

		GoalSelector * parseGoalSelector( TiXmlElement * node, const std::string & behaveFldr ) {
			return GoalSelectorDB::getInstance( node, behaveFldr );
		}

	}	// namespace BFSM 
}	// namespace Menge