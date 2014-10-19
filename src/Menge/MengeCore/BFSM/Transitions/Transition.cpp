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

#include "Transition.h"
#include "Transitions/Condition.h"
#include "Transitions/ConditionDatabase.h"
#include "Transitions/Target.h"
#include "Transitions/TargetDatabase.h"
#include "BaseAgent.h"
#include "FSM.h"
#include "tinyxml.h"

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//					Implementation of Transition
		/////////////////////////////////////////////////////////////////////

		Transition::Transition( const Transition & trans ):_condition(trans._condition->copy()), _target(trans._target->copy()) {
		}

		/////////////////////////////////////////////////////////////////////

		Transition::Transition( Condition * condition, TransitionTarget * target ):_condition(condition), _target(target) {
		}

		/////////////////////////////////////////////////////////////////////

		Transition::~Transition() {
			if ( _condition ) _condition->destroy();
			if ( _target ) _target->destroy();
		}

		/////////////////////////////////////////////////////////////////////

		bool Transition::connectStates( std::map< std::string, State * > & stateMap ) {
			assert( _target && "Connecting states for a transition with no target" );
			return _target->connectStates( stateMap );
		}

		/////////////////////////////////////////////////////////////////////

		void Transition::onEnter( Agents::BaseAgent * agent ) {
			assert( _condition && "Entering transition with no defined condition" );
			assert( _target && " Entering transition with no defined target" );
			_condition->onEnter( agent );
			_target->onEnter( agent );
		}

		/////////////////////////////////////////////////////////////////////

		void Transition::onLeave( Agents::BaseAgent * agent ) {
			assert( _condition && "Leaving transition with no defined condition" );
			assert( _target && " Leaving transition with no defined target" );
			_condition->onLeave( agent );
			_target->onLeave( agent );
		}

		/////////////////////////////////////////////////////////////////////

		State * Transition::test( Agents::BaseAgent * agent, const Goal * goal ) {
			if ( _condition->conditionMet( agent, goal ) ) {
				return _target->nextState( agent );
			}
			return 0x0;
		}

		/////////////////////////////////////////////////////////////////////

		void Transition::getTasks( FSM * fsm ) {
			fsm->addTask( _condition->getTask() );
			fsm->addTask( _target->getTask() );
		}
		
		/////////////////////////////////////////////////////////////////////
		
		Transition * Transition::copy() {
			return new Transition( *this );
		}

		/////////////////////////////////////////////////////////////////////
		
		Transition * parseTransition( TiXmlElement * node, const std::string & behaveFldr, std::string & fromName ) {
			// 1) test "from" name - store if valid
			bool valid = true;
			const char * fromCStr = node->Attribute( "from" );
			if ( fromCStr == 0x0 ) {
				logger << Logger::ERR_MSG << "Transition defined on line " << node->Row() << " missing \"from\" attribute.";
				valid = false;
			}
			fromName = fromCStr;
			
			// 2) test "to" name - create SingleTarget if valid (and store)
			Condition * condition = 0x0;
			TransitionTarget * target = 0x0;
			const char * toCStr = node->Attribute("to");
			if ( toCStr != 0x0 ) {
				target = new SingleTarget( std::string( toCStr ) );
			}
			// 3) Look for child tags: Condition and Target
			for ( TiXmlElement * child = node->FirstChildElement(); child; child = child->NextSiblingElement() ) {
				if ( child->ValueStr() == "Condition" ) {
					condition = ConditionDB::getInstance( child, behaveFldr );
				} else if ( child->ValueStr() == "Target" ) {
					if ( target ) target->destroy();
					target = TargetDB::getInstance( child, behaveFldr );
				} else {
					logger << Logger::ERR_MSG << "Unrecognized child tag of a Transition on line " << child->Row() << ": " << child->ValueStr() << ".";
					valid = false;
				}
			}

			valid = valid && condition != 0x0 && target != 0x0;
			// 4) If no Condition tag exists, it fails, if no Target tag exists && there was no to tag, it fails
			if ( ! valid ) {
				logger << Logger::ERR_MSG << "Missing target and/or condition specification for the Transition defined on line " << node->Row() << ".";
				if ( condition ) condition->destroy();
				if ( target ) target->destroy();
				return 0x0;
			}
			// If everything is valid, instantiate the transition, and the Transition
			return new Transition( condition, target );
		}
	}	// namespace BFSM
}	// namespace Menge