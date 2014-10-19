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

#include "State.h"
#include "BaseAgent.h"
#include "FSM.h"
#include "GoalSelectors/GoalSelector.h"
#include <sstream>

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of State
		/////////////////////////////////////////////////////////////////////

		size_t State::COUNT = 0;
		Vector2 State::NULL_POINT;

		/////////////////////////////////////////////////////////////////////

		State::State( const std::string & name ): _velComponent(0x0), transitions_(),actions_(), _final(false), _goalSelector(0x0), _goals(), _name(name) {
			_id = COUNT++;
		}

		/////////////////////////////////////////////////////////////////////

		State::~State() {
			for ( size_t i = 0; i < actions_.size(); ++i ) {
				actions_[i]->destroy();
			}
			_velComponent->destroy();
			if ( _goalSelector ) {
				_goalSelector->destroy();
				_goalSelector = 0x0;
			}
		}

		/////////////////////////////////////////////////////////////////////

		void State::getTasks( FSM * fsm ) {
			fsm->addTask( _goalSelector->getTask() );

			for ( size_t i = 0; i < actions_.size(); ++i ) {
				fsm->addTask( actions_[i]->getTask() );
			}

			fsm->addTask( _velComponent->getTask() );
			
			for ( size_t i = 0; i < transitions_.size(); ++i ) {
				transitions_[i]->getTasks( fsm );
			}

			for ( size_t i = 0; i < velModifiers_.size(); ++i ) {
				//velModifiers_[i]->getTask( fsm );
				fsm->addTask(velModifiers_[i]->getTask());
			}
		}

		/////////////////////////////////////////////////////////////////////

		//change this to accept a velPref reference
		void State::getPrefVelocity( Agents::BaseAgent * agent, Agents::PrefVelocity &velocity ) {
			Goal * goal;
			_goalLock.lockRead();
			goal = _goals[ agent->_id ];
			_goalLock.releaseRead();

			//this needs to get changed. Create a copy of the VelPref. Pass that in, and then pass it back

			_velComponent->setPrefVelocity( agent, goal, velocity);

			//apply my velocity modifiers now
			std::vector< VelModifier * >::iterator vItr = velModifiers_.begin();
			for ( ; vItr != velModifiers_.end(); ++vItr ) {
				(*vItr)->adaptPrefVelocity(agent, velocity);
			}
		}

		/////////////////////////////////////////////////////////////////////

		State * State::testTransitions( Agents::BaseAgent * agent ) {
			std::set< State * > visited;
			State * newNode = testTransitions( agent, visited );
			return newNode;
		}

		/////////////////////////////////////////////////////////////////////

		State * State::testTransitions( Agents::BaseAgent * agent, std::set< State * > &visited ) {
	#ifdef _DEBUG
			_goalLock.lockRead();
			assert( _goals.count( agent->_id ) == 1 && "Testing transitions for an agent without a goal!" );
			_goalLock.releaseRead();
	#endif

			if ( visited.find( this ) != visited.end() ) return 0x0;
			visited.insert( this );

			_goalLock.lockRead();
			Goal * goal = _goals[ agent->_id ];
			_goalLock.releaseRead();
			
			for ( size_t i = 0; i < transitions_.size(); ++i ) {
				State * next = transitions_[i]->test( agent, goal );
				if ( next ) {
					leave( agent );	// a transition has come back true, leaving this state
					next->enter( agent );
					State * test = next->testTransitions( agent, visited );
					if ( test ) {
						return test;
					} else {
						return next;
					}
				}
			}
			return 0x0;
		}

		/////////////////////////////////////////////////////////////////////

		void State::enter( Agents::BaseAgent * agent ) {
			for ( size_t i = 0; i < actions_.size(); ++i ) {
				actions_[i]->onEnter( agent );
			}

			Goal * goal = 0x0;
			try {
				goal = _goalSelector->assignGoal( agent );
			} catch ( GoalSelectorException ) {
				logger << Logger::ERR_MSG << "State " << _name << " was unable to assign a goal to agent " << agent->_id << ".";
				throw StateException();
			}

			_goalLock.lockWrite();
			_goals[ agent->_id ] = goal;
			_goalLock.releaseWrite();

			_velComponent->onEnter( agent );
			for ( size_t i = 0; i < transitions_.size(); ++i ) {
				transitions_[i]->onEnter( agent );
			}

			//velmodifiers
			for ( size_t i = 0; i < velModifiers_.size(); ++i ) {
				velModifiers_[i]->onEnter( agent );
			}
		}

		/////////////////////////////////////////////////////////////////////

		void State::leave( Agents::BaseAgent * agent ) {
			_goalSelector->freeGoal( agent, _goals[ agent->_id ] );
			
			_goalLock.lockWrite();
			_goals.erase( agent->_id );
			_goalLock.releaseWrite();

			for ( size_t i = 0; i < actions_.size(); ++i ) {
				actions_[i]->onLeave( agent );
			}
			_velComponent->onExit( agent );
			for ( size_t i = 0; i < transitions_.size(); ++i ) {
				transitions_[i]->onLeave( agent );
			}

			for ( size_t i = 0; i < velModifiers_.size(); ++i ) {
				velModifiers_[i]->onLeave( agent );
			}
		}

		/////////////////////////////////////////////////////////////////////

		size_t State::getPopulation() const {
			// It is assumed that every agent actually in the state has a
			//	representation in _goals.
			//	This works because goal persistence is stored in the goal
			//	selector.
			_goalLock.lockRead();
			size_t size = _goals.size();
			_goalLock.releaseRead();
			return size;
		}

		/////////////////////////////////////////////////////////////////////

		void State::setGoalSelector( GoalSelector * selector ) {
			if ( _goalSelector != 0x0 ) {
				logger << Logger::ERR_MSG << "The state \"" << _name << "\" has been assigned multiple goal selectors.";
				throw GoalSelectorException();
			}
			_goalSelector = selector;
		}

		/////////////////////////////////////////////////////////////////////

		void State::clearGoalSelector() {
			if ( _goalSelector ) {
				_goalSelector->destroy();
				_goalSelector = 0x0;
			}
		}

	}	// namespace BFSM
}	// namespace Menge