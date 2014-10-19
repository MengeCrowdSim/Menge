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

#include "EventSystem.h"
#include "Event.h"
#include "EventTargetDB.h"
#include "EventEffectDB.h"
#include "EventException.h"
#include <sstream>

namespace Menge {
	/////////////////////////////////////////////////////////////////////
	//					Implementation of EventSystem
	/////////////////////////////////////////////////////////////////////

	bool EventSystem::CONSERVATIVE_SETUP = true;

	/////////////////////////////////////////////////////////////////////

	EventSystem::EventSystem(): _events() {
	}

	/////////////////////////////////////////////////////////////////////

	EventSystem::~EventSystem() {
		for ( size_t i = 0; i < _events.size(); ++i ) {
			delete _events[i];
		}
		for ( HASH_MAP< std::string, EventTarget * >::iterator itr = _targets.begin();
			itr != _targets.end(); ++itr ) {
			itr->second->destroy();
		}
		for ( HASH_MAP< std::string, EventEffect * >::iterator itr = _effects.begin();
			itr != _effects.end(); ++itr ) {
			itr->second->destroy();
		}
		
	}

	/////////////////////////////////////////////////////////////////////

	void EventSystem::evaluateEvents() {
		for ( size_t i = 0; i < _events.size(); ++i ) {
			_events[i]->evaluate();
		}
	}

	/////////////////////////////////////////////////////////////////////

	void EventSystem::finalize() {
		HASH_MAP< std::string, EventTarget * >::iterator tgtItr = _targets.begin();
		while ( tgtItr != _targets.end() ) {
			try {
				tgtItr->second->finalize();
				++tgtItr;
			} catch ( Menge::EventFatalException & ex ) {
				logger << Logger::ERR_MSG << "Fatal exception finalizing event target: " << tgtItr->first << "\n" << ex._msg;
				throw ex;
			} catch ( Menge::EventException & ex ) {
				logger << Logger::WARN_MSG << "Removing invalid event target " << tgtItr->first << "!\n" << ex._msg;
				tgtItr = _targets.erase( tgtItr );
			}
		}

		HASH_MAP< std::string, EventEffect * >::iterator effItr = _effects.begin();
		while ( effItr != _effects.end() ) {
			try {
				effItr->second->finalize();
				++effItr;
			} catch ( Menge::EventFatalException & ex ) {
				logger << Logger::ERR_MSG << "Fatal exception finalizing event effect: " << effItr->first << "\n" << ex._msg;
				throw ex;
			} catch ( Menge::EventException & ex ) {
				logger << Logger::WARN_MSG << "Removing invalid event effect " << effItr->first << "!\n" << ex._msg;
				effItr = _effects.erase( effItr );
			}
		}

		std::vector< Event * >::iterator itr = _events.begin(); 
		while ( itr != _events.end() ) {
			try {
				(*itr)->finalize();
				++itr;
			} catch ( Menge::EventFatalException & ex ) {
				logger << Logger::ERR_MSG << "Fatal exception finalizing events\n" << ex._msg;
				throw ex;
			} catch ( Menge::EventException & ex ) {
				logger << Logger::WARN_MSG << "Removing event " << (*itr)->_name << "!\n" << ex._msg;
				itr = _events.erase( itr );
			}
		}
	}

	/////////////////////////////////////////////////////////////////////

	void EventSystem::finalizeException( const std::string msg ) {
		if ( CONSERVATIVE_SETUP ) {
			logger << Logger::ERR_MSG << msg ;
			throw EventFatalException( msg );
		} else {
			logger << Logger::WARN_MSG << msg ;
			throw EventException( msg );
		}
	}

	/////////////////////////////////////////////////////////////////////

	bool EventSystem::parseEvents( TiXmlElement * node, const std::string & behaveFldr ) {
		// First test for "conservative" attribute
		int i;
		if ( node->Attribute("conservative", &i ) ) {
			Menge::EventSystem::CONSERVATIVE_SETUP = i != 0;
		}
		const int TARGET_PASS = 0;
		const int EFFECT_PASS = 1;
		const int EVENT_PASS = 2;

		const char * tags[] = { "Target", "Effect", "Event" };
		for ( int pass = 0; pass < 3; ++pass ) {
			std::string TAG( tags[ pass] );
			// then parse events
			TiXmlElement* child;
			for ( child = node->FirstChildElement(); child; child = child->NextSiblingElement() ) {
				if ( child->ValueStr() == TAG ) {
					if ( pass == TARGET_PASS ) {
						const char * cStr = child->Attribute( "name" );
						if ( cStr == 0x0 ) {
							logger << Logger::ERR_MSG << "Event target on line " << child->Row() << " requires a \"name\" attribute.";
							return false;
						}  else {
							std::string name( cStr );
							if ( _targets.find( name ) != _targets.end() ) {
								logger << Logger::ERR_MSG << "Found multiple event targets with the same name: " << name << ".";
								return false;
							}
							EventTarget * target = EventTargetDB::getInstance( child, behaveFldr );
							if ( target == 0x0 ) {
								return false;
							}
							_targets[ name ] = target;
						}
					} else if ( pass == EFFECT_PASS ) {
						const char * cStr = child->Attribute( "name" );
						if ( cStr == 0x0 ) {
							logger << Logger::ERR_MSG << "Event effect on line " << child->Row() << " requires a \"name\" attribute.";
							return false;
						}  else {
							std::string name( cStr );
							if ( _effects.find( name ) != _effects.end() ) {
								logger << Logger::ERR_MSG << "Found multiple event effects with the same name: " << name << ".";
								return false;
							}
							EventEffect * effect = EventEffectDB::getInstance( child, behaveFldr );
							if ( effect == 0x0 ) {
								return false;
							}
							_effects[ name ] = effect;
						}
					} else if ( pass == EVENT_PASS ) {
						Event * evt = parseEvent( child, behaveFldr );
						if ( evt == 0x0 ) {
							return false;
						}
						_events.push_back( evt );
					} else {
						std::stringstream ss;
						ss << "Found invalid child of EventSystem tag: " << child->ValueStr() << ".";
						if ( CONSERVATIVE_SETUP ) {
							logger << Logger::ERR_MSG << ss.str();
							throw EventFatalException( ss.str() );
						} else {
							// only print the message on the first pass
							if ( pass == 0 ) logger << Logger::WARN_MSG << ss.str();
						}
					}
				}
			}
		}
		return true;
	}

	
	
}