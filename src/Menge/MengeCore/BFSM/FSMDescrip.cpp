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

#include "FSMDescrip.h"
#include "Logger.h"
#include "GoalSet.h"
#include "Goals/Goal.h"
#include "Tasks/Task.h"
#include "State.h"
#include "Transitions/Transition.h"
#include "Transitions/Target.h"
#include "Transitions/TargetDatabase.h"
#include "Transitions/ConditionDatabase.h"
#include "VelocityModifiers/VelModifier.h"
#include "VelocityModifiers/VelModifierDatabase.h"
#include "StateDescrip.h"
#include "Events/EventSystem.h"
#include "Events/Event.h"
#include "Core.h"

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of FSMDescrip
		/////////////////////////////////////////////////////////////////////

		FSMDescrip::FSMDescrip() {
		}

		/////////////////////////////////////////////////////////////////////

		FSMDescrip::~FSMDescrip() {
			std::list< StateDescrip * >::const_iterator sItr = _states.begin();
			for ( ; sItr != _states.end(); ++sItr ) {
				delete (*sItr );
			}
			std::map< std::string, std::list< Transition * > >::iterator stItr = _transitions.begin();
			for ( ; stItr != _transitions.end(); ++stItr ) {
				std::list< Transition * > & tList = stItr->second;
				std::list< Transition * >::iterator tItr = tList.begin();
				for ( ; tItr != tList.end(); ++tItr ) {
					delete ( *tItr );
				}
			}
			
			std::map< size_t, GoalSet * >::iterator gItr = _goalSets.begin();
			for ( ; gItr != _goalSets.end(); ++gItr ) {
				delete gItr->second;
			}

			std::list< Task * >::iterator tItr = _tasks.begin();
			for ( ; tItr != _tasks.end(); ++tItr ) {
				(*tItr)->destroy();
			}
		}

		/////////////////////////////////////////////////////////////////////

		State * FSMDescrip::addState( StateDescrip * sData ) {
			if ( _stateNameMap.find( sData->_name ) != _stateNameMap.end() ) {
				logger << Logger::ERR_MSG << "Found multiple states with the same name: \"" << sData->_name << "\".";
				return 0x0;
			}
			State * node = new State( sData->_name );
			node->setFinal( sData->_isFinal );
			_stateNameMap[ sData->_name ] = node;
			return node;
		}

		/////////////////////////////////////////////////////////////////////

		bool FSMDescrip::loadFromXML( const std::string & xmlName, bool verbose ) {
			logger << Logger::INFO_MSG << "Loading behavior from xml: " << xmlName;
			TiXmlDocument xml( xmlName );
			bool loadOkay = xml.LoadFile();

			if ( !loadOkay ) {
				logger << Logger::ERR_MSG << "Could not load behavior configuration xml (" << xmlName << ") due to xml syntax errors.\n";
				logger << "\t" << xml.ErrorDesc();
				return false;
			}

			TiXmlElement * popNode = xml.RootElement();
			if ( ! popNode ) {
				logger << Logger::ERR_MSG << "Root element does not exist.";
				return false;
			}
			if ( popNode->ValueStr() != "BFSM" ) {
				logger << Logger::ERR_MSG << "Root element value should be \"BFSM\".";
				return false;
			}

			std::string absPath;
			os::path::absPath( xmlName, absPath );
			std::string junk;
			os::path::split( absPath, _behaviorFldr, junk );
			logger << Logger::INFO_MSG << "Behavior root: " << _behaviorFldr;		

			TiXmlElement* child;
			for ( child = popNode->FirstChildElement(); child; child = child->NextSiblingElement() ) {
				if ( child->ValueStr() == "GoalSet" ) {
					int i;
					if ( !child->Attribute( "id", &i ) ) {
						logger << Logger::ERR_MSG << "GoalSet requires an \"id\" property.";
						return false;
					}
					size_t setID = static_cast< size_t >( i );
					// confirm that the id doesn't already exist
					if ( _goalSets.find( setID ) != _goalSets.end() ) {
						logger << Logger::WARN_MSG << "Found multiple GoalSets with the same id: " << setID << ".\n\tGoal definitions will be merged!";
					} else {
						_goalSets[ setID ] = new GoalSet();
					}
					TiXmlElement * goalNode;
					for ( goalNode = child->FirstChildElement(); goalNode; goalNode = goalNode->NextSiblingElement() ) {
						if ( goalNode->ValueStr() == "Goal" ) {
							Goal * goal = parseGoal( goalNode, _behaviorFldr );
							if ( goal == 0x0 ) {
								logger << Logger::ERR_MSG << "Error parsing a goal description.";
								return false;
							}
							// Make sure that this goal doesn't duplicate previous goal ids
							if ( ! _goalSets[ setID ]->addGoal( goal->getID(), goal ) ) {
								logger << Logger::ERR_MSG << "GoalSet " << setID << " has two goals with the identifier: " << goal->getID() << " (second appears on line " << goalNode->Row() << ").";
								return false;
							}
						} else {
							logger << Logger::WARN_MSG << "Found a child tag of the GoalSet that is not a \"Goal\" tag on line " << goalNode->Row() << ". It will be ignored.";
						}
					}

				} else if ( child->ValueStr() == "State" ) {
					if ( ! parseState( child, _behaviorFldr, _states ) ) {

						return false;
					}
				} else if ( child->ValueStr() == "Transition" ) {
					std::string from;
					Transition * trans = parseTransition( child, _behaviorFldr, from );
					if ( trans == 0x0 ) {
						return false;
					}

					addTransition( from, trans );
				} else if ( child->ValueStr() == "VelModifier" ) {
					VelModifier *vel = parseVelModifier( child, _behaviorFldr);
					if ( vel == 0x0 ) {
						return false;
					} else {
						_velModifiers.push_back(vel);
					}
				} else if ( child->ValueStr() == "Task" ) {

					Task * task = parseTask( child, _behaviorFldr );
					if ( task == 0x0 ) {
						logger << Logger::WARN_MSG << "User-specified Task on line " << child->Row() << " couldn't be instantiated.  It is being ignored.";
					} else {
						_tasks.push_back( task );
					}
				} else if ( child->ValueStr() == "EventSystem" ) {
					if ( ! EVENT_SYSTEM->parseEvents( child, _behaviorFldr ) ) {
						return false;
					}
				} else {
					logger << Logger::ERR_MSG << "Unrecognized tag as child of <Population>: <" << child->ValueStr() << ">.";
					return false;
				}
			}

			return true;
		}

		/////////////////////////////////////////////////////////////////////

		void FSMDescrip::addTransition( const std::string & name, Transition * trans ) {
			std::string tmpName( name );
			size_t n = tmpName.find_first_of( ',' );
			while ( n != std::string::npos ) {
				std::string sName = tmpName.substr( 0, n );
				_transitions[ sName ].push_back( trans->copy() );
				tmpName = tmpName.substr( n + 1 );
				n = tmpName.find_first_of( ',' );
			}

			_transitions[ tmpName ].push_back( trans );
		}

		/////////////////////////////////////////////////////////////////////

		Logger & operator<<( Logger & out, const FSMDescrip & fsmDescrip ) {
			out << "BFSM Configuration:";
			out << "\n\tStates:";
			std::list< StateDescrip * >::const_iterator sItr = fsmDescrip._states.begin();
			for ( ; sItr != fsmDescrip._states.end(); ++sItr ) {
				out << "\n" << (*(*sItr) );
			}
			out << "\n\tTransitions:";
			std::map< std::string, std::list< Transition * > >::const_iterator tItr = fsmDescrip._transitions.begin();
			for ( ; tItr != fsmDescrip._transitions.end(); ++tItr ) {
				out << "\nNo Transition output supported.";
			}
			return out;
		}

		/////////////////////////////////////////////////////////////////////
		
	}	// namespace BFSM
}	// namespace Menge