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

#include "FSM.h"

#include "State.h"
#include "Transitions/Transition.h"
#include "Tasks/Task.h"
#include "Core.h"
#include "FsmContext.h"
#include "StateContext.h"
#include "GoalSet.h"
#include "PrefVelocity.h"
#include "Events/EventSystem.h"

#include "BaseAgent.h"
#include "SimulatorInterface.h"

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of FSM
		/////////////////////////////////////////////////////////////////////

		FSM::FSM( Agents::SimulatorInterface * sim ):_sim(sim), _agtCount(0), _currNode(0x0) {	
			setAgentCount( sim->getNumAgents() );
		}

		/////////////////////////////////////////////////////////////////////

		FSM::~FSM() {
			if ( _currNode ) {
				delete [] _currNode;
			}
			// TODO: Why doesn't this delete the States and transitions?
			std::map< size_t, GoalSet * >::iterator gsItr = _goalSets.begin();
			for ( ; gsItr != _goalSets.end(); ++gsItr ) {
				delete gsItr->second;
			}
			for ( size_t i = 0; i < _tasks.size(); ++i ) {
				_tasks[ i ]->destroy();
			}
			_tasks.clear();
		}

		/////////////////////////////////////////////////////////////////////
		 
		void FSM::collectTasks() {
			const size_t STATE_COUNT = _nodes.size();
			for ( size_t i = 0; i < STATE_COUNT; ++i ) {
				_nodes[ i ]->getTasks( this );
			}

			//now collect the velocity modifiers tasks
			std::vector< VelModifier * >::iterator vItr = _velModifiers.begin();
			for ( ; vItr != _velModifiers.end(); ++vItr ) {   //TODO: replace global vel mod initalizer
				addTask((*vItr)->getTask());
			}

			//iterate over agents

		}

		/////////////////////////////////////////////////////////////////////
		 
		void FSM::addTask( Task * task ) {
			if ( task ) {
				for ( size_t i = 0; i < _tasks.size(); ++i ) {
					if ( task->isEquivalent( _tasks[i] ) ) {
						task->destroy();
						return;
					}
				}
				_tasks.push_back( task );
			}
		}

		/////////////////////////////////////////////////////////////////////
		 
		void FSM::setAgentCount( size_t count ) {
			if ( _currNode ) {
				delete [] _currNode;
				_currNode = 0x0;
			}
			_agtCount = count;
			_currNode = new State * [ count ];
			memset( _currNode, 0x0, count * sizeof( State * ) );
		}

		/////////////////////////////////////////////////////////////////////
		 
		void FSM::advance( Agents::BaseAgent * agent ) {
			const size_t ID = agent->_id;
			// Evaluate the current state's transitions
			State * newNode = _currNode[ ID ]->testTransitions( agent );
			if ( newNode ) {
				_currNode[ ID ] = newNode;
			}
		}

		/////////////////////////////////////////////////////////////////////
		 
		void FSM::computePrefVelocity( Agents::BaseAgent * agent ) {
			const size_t ID = agent->_id;
			// Evalute the new state's velocity

			//generate a preferred velocity for passing around
			Agents::PrefVelocity newVel;

			_currNode[ ID ]->getPrefVelocity( agent, newVel);

			//TODO: My velocity modifiers here
			 
			std::vector< VelModifier * >::iterator vItr = _velModifiers.begin();
			for ( ; vItr != _velModifiers.end(); ++vItr ) {   //TODO: replace global vel mod initalizer
				(*vItr)->adaptPrefVelocity(agent, newVel);
			}


			//agent will now have a set preferred velocity method
			agent->setPreferredVelocity(newVel);
		}

		/////////////////////////////////////////////////////////////////////

		State * FSM::getNode( const std::string & name ) {
			const size_t STATE_COUNT = _nodes.size();
			for ( size_t i = 0; i < STATE_COUNT; ++i ) {
				if ( _nodes[i]->getName() == name ) {
					return _nodes[i];
				}
			}
			return 0x0;
		}

		/////////////////////////////////////////////////////////////////////

		size_t FSM::addNode( State *node ) {
			if ( _currNode[0] == 0x0 ) {
				for ( size_t i = 0; i < _agtCount; ++i ) {
					_currNode[i] = node;
				}
			}
			_nodes.push_back( node );
			return _nodes.size() - 1;
		}

		/////////////////////////////////////////////////////////////////////

		bool FSM::addTransition( size_t fromNode, Transition * t ) {
			if ( fromNode >= _nodes.size() ) return false;
			State * from = _nodes[ fromNode ];
			from->addTransition( t );
			return true;
		}

		/////////////////////////////////////////////////////////////////////

		bool FSM::addGoal( size_t goalSet, size_t goalID, Goal * goal ) {
			if ( _goalSets.find( goalSet ) == _goalSets.end() ) {
				_goalSets[ goalSet ] = new GoalSet();
			}
			return _goalSets[ goalSet ]->addGoal( goalID, goal );
		}

		/////////////////////////////////////////////////////////////////////

		Goal * FSM::getGoal( size_t goalSet, size_t goalID ) {
			if ( _goalSets.find( goalSet ) == _goalSets.end() ) {
				return 0x0;
			}
			return _goalSets[ goalSet ]->getGoalByID( goalID );
		}

		/////////////////////////////////////////////////////////////////////

		GoalSet * FSM::getGoalSet( size_t goalSetID ) {
			if ( _goalSets.find( goalSetID ) == _goalSets.end() ) {
				return 0x0;
			}
			return _goalSets[ goalSetID ];
		}
		 
		/////////////////////////////////////////////////////////////////////

		void FSM::setCurrentState( Agents::BaseAgent * agent, size_t currNode ) {
			assert( currNode < _nodes.size() && "Set invalid state as current state" );
			_currNode[ agent->_id ] = _nodes[ currNode ];
		}

		/////////////////////////////////////////////////////////////////////

		State * FSM::getCurrentState( const Agents::BaseAgent * agt ) const {
			return _currNode[ agt->_id ];
		}

		/////////////////////////////////////////////////////////////////////

		size_t FSM::getAgentStateID( const Agents::BaseAgent * agent ) const { 
			return _currNode[ agent->_id ]->getID(); 
		}

		/////////////////////////////////////////////////////////////////////

		size_t FSM::getAgentStateID( size_t agentID ) const { 
			return _currNode[ agentID ]->getID(); 
		}

		/////////////////////////////////////////////////////////////////////

		bool FSM::allFinal() const {
			// NOTE: This assumes that there are no holes in this memory
			for ( size_t a = 0; a < _agtCount; ++a ) {
				if ( !_currNode[ a ]->getFinal() ) return false;
			}
			return true;
		}

		/////////////////////////////////////////////////////////////////////

		bool FSM::doStep() {
			// NOTE: This is a cast from size_t to int to be compatible with older implementations
			//		of openmp which require signed integers as loop variables
			SIM_TIME = this->_sim->getGlobalTime();
			EVENT_SYSTEM->evaluateEvents();
			int agtCount = (int)this->_sim->getNumAgents();
			size_t exceptionCount = 0;
			#pragma omp parallel for reduction(+:exceptionCount)
			for ( int a = 0; a < agtCount; ++a ) {
				Agents::BaseAgent * agt = this->_sim->getAgent( a );
				try {
					advance( agt );
					this->computePrefVelocity( agt );
				} catch ( StateException & e ) {
					logger << Logger::ERR_MSG << e.what() << "\n";
					++exceptionCount;
				}
			}
			if ( exceptionCount > 0 ) {
				throw FSMFatalException();
			}
			return this->allFinal();
		}

		/////////////////////////////////////////////////////////////////////

		void FSM::doTasks() {
			for ( size_t i = 0; i < this->_tasks.size(); ++i ) {
				try {
					this->_tasks[i]->doWork( this );
				} catch ( TaskFatalException ) {
					logger << Logger::ERR_MSG << "Fatal error in FSM task: " << _tasks[i]->toString() << "\n";
					throw FSMFatalException();
				} catch ( TaskException ) {
					logger << Logger::ERR_MSG << "Error in FSM task: " << _tasks[i]->toString() << "\n";
				}
			}
		}

		/////////////////////////////////////////////////////////////////////

		void FSM::finalize() {
			EVENT_SYSTEM->finalize();
			doTasks();
		}

		/////////////////////////////////////////////////////////////////////

		FsmContext * FSM::getContext() {
			FsmContext * ctx = new FsmContext( this );
			// TODO: Populate the context
			for ( size_t i = 0; i < _nodes.size(); ++i ) {
				StateContext * sCtx = new StateContext( _nodes[ i ] );
				ctx->addStateContext( _nodes[i]->getID(), sCtx );
			}
			
			return ctx;
		}
	}	// namespace BFSM
}	// namespace Menge
