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

#include "StateDescrip.h"
#include "tinyxml.h"
#include "Actions/Action.h"
#include "GoalSelectors/GoalSelector.h"
#include "State.h"
#include "os.h"

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//					Implementation of StateDescrip
		/////////////////////////////////////////////////////////////////////

		StateDescrip::StateDescrip( const std::string & name, bool isFinal ): _name(name), _isFinal(isFinal), _goalSelector(0x0), _velComponent(0x0) {
		}

		/////////////////////////////////////////////////////////////////////

		StateDescrip::~StateDescrip() {
			if ( _velComponent ) _velComponent->destroy();

			std::list< Action * >::const_iterator aItr = _actions.begin();
			for ( ; aItr != _actions.end(); ++aItr ) {
				(*aItr)->destroy();
			}

			if ( _goalSelector ) {
				_goalSelector->destroy();
			}
		}

		/////////////////////////////////////////////////////////////////////

		Logger & operator<<( Logger & out, const StateDescrip & state ) {
			out << "\t\tState( " << state._name << " ), ";
			if ( state._isFinal ) {
				out << " -- FINAL state";
			}
			out << "\nVelocity component streaming not supported yet.";
			// todo: out << "\n" << (*state->_velComponent);

			out << "\nAction streaming not supported yet.";
			// TODO: realize this
			//std::list< Action * >::const_iterator aItr = state._actions.begin();
			//for ( ; aItr != state._actions.end(); ++aItr ) {
			//	out << "\n" << (**aItr);
			//}
			return out;
		}

		/////////////////////////////////////////////////////////////////////

		bool parseState( TiXmlElement * node, const std::string & behaveFldr, std::list< StateDescrip * > & states ) {
			StateDescrip * s = 0x0;
			const char * cName = node->Attribute( "name" );
			if ( cName == 0x0 ) {
				logger << Logger::ERR_MSG << "State tags require a name\n";
				return false;
			}
			std::string name( cName );
			if ( name == "" ) {
				logger << Logger::ERR_MSG << "Undefined state name!\n";
				return false;
			}

			int i;
			bool isFinal = false;
			if ( ! node->Attribute( "final", &i ) ) {
				logger << Logger::ERR_MSG << "State must specify whether it is final or not\n";
				return false;
			} else {
				isFinal = i != 0;
			}

			s = new StateDescrip( name, isFinal );

			for ( TiXmlElement * gchild = node->FirstChildElement(); gchild; gchild = gchild->NextSiblingElement() ) {
				if ( gchild->ValueStr() == "Action" ) {
					Action * action = parseAction( gchild, behaveFldr );
					if ( action == 0x0 ) {
						delete s;
						return false;
					}
					s->_actions.push_back( action );
				} else if ( gchild->ValueStr() == "VelComponent" ) {
					if ( s->_velComponent != 0x0 ) {
						logger << Logger::ERR_MSG << "Multiple velocity components defined for the state (" << s->_name << ") on line " << gchild->Row() << ".  Only one VelComponent can be defined per state.";
						delete s;
						return false;
					}
					s->_velComponent = parseVelComponent( gchild, behaveFldr );
					if ( s->_velComponent == 0x0 ) {
						logger << Logger::ERR_MSG << "Unable to instantiate a velocity component for state " << s->_name << ".  Simulation cannot proceed.";
						delete s;
						return false;
					}
				} else if ( gchild->ValueStr() == "GoalSelector" ) {
					s->_goalSelector = parseGoalSelector( gchild, behaveFldr );
					if ( s->_goalSelector == 0x0 ) {
						logger << Logger::ERR_MSG << "Unable to instantiate a goal selector for state " << s->_name << ".  Simulation cannot proceed.";
						// This doesn't result in a default -- this is an error.  A Goal selector MUST be specified.
						delete s;
						return false;
					}
				} else if ( gchild->ValueStr() == "VelModifier" ) {
					VelModifier *vel = parseVelModifier( gchild, behaveFldr );
					if ( vel == 0x0 ) {
						delete s;
						return false;
					} else {
						s->_velModifiers.push_back(vel);
					}
				} else {
					logger << Logger::ERR_MSG << "State contains an improper child element: " << gchild->ValueStr() << ".";
					return false;
				}
			}
			states.push_back( s );

			return true;
		}

	}	// namespace BFSM 
}	// namespace Menge