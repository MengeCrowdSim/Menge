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

#include "Event.h"
#include "EventTrigger.h"
#include "EventResponse.h"
#include "Logger.h"
#include "EventException.h"
#include "EventSystem.h"
#include "EventTriggerDB.h"
#include "Core.h"
#include "tinyxml.h"
#include <sstream>
#include <cassert>

namespace Menge {

	/////////////////////////////////////////////////////////////////////
	//					Implementation of Event
	/////////////////////////////////////////////////////////////////////

	Event::Event( const std::string & name ): _name(name), _trigger(0x0), _responses() {
	}

	/////////////////////////////////////////////////////////////////////

	Event::~Event() {
		if ( _trigger) _trigger->destroy();
		for ( size_t i = 0; i < _responses.size(); ++i ) {
			delete _responses[i];
		}
	}

	/////////////////////////////////////////////////////////////////////

	void Event::finalize() {
		Logger::LogType logType = EventSystem::CONSERVATIVE_SETUP ? Logger::ERR_MSG : Logger::WARN_MSG;
		bool error = false;
		if ( _trigger ) {
			try {
				_trigger->finalize();
			} catch ( EventException & e ) {
				_trigger->destroy();
				_trigger = 0x0;
				logger << logType << "Event " << _name << " had problems finalizing its target: " << e._msg << "\n";
				error = true;
			}
		} else {
			logger << logType << "Event " << _name << " is missing a trigger.\n";
			error = true;
		}

		if ( _responses.size() > 0 ) {
			std::vector< EventResponse * >::iterator itr = _responses.begin();
			while( itr != _responses.end() ) {
				try {
					(*itr)->finalize();
					++itr;
				} catch ( EventException & e ) {
					delete *itr;
					itr = _responses.erase( itr );
					logger << logType << "Event " << _name << " had problems finalizing a response: " << e._msg << "\n";
					error = true;
				}
			}
		} else {
			logger << logType << "Event " << _name << " has no responses defined.";
			error = true;
		}

		if ( error ) {
			std::stringstream ss;
			ss << "Finalization errors in event " << _name << ".";
			if ( EventSystem::CONSERVATIVE_SETUP ) {
				throw EventFatalException( ss.str() );
			} else {
				if ( _trigger == 0x0 || _responses.size() == 0x0 ) {
					throw EventException( ss.str() );
				}
			}
		}
	}

	/////////////////////////////////////////////////////////////////////

	void Event::evaluate() {
		assert( _responses.size() > 0 && "Evaluating an event with no responses!" );
		assert( _trigger != 0x0 && "Trying to evaluate an event with no trigger" );
		if ( _trigger->conditionMet() ) {
			_trigger->fired();
			for ( size_t i = 0; i < _responses.size(); ++i ) {
				_responses[i]->apply();
			}
		}
	}

	/////////////////////////////////////////////////////////////////////

	Event * parseEvent( TiXmlElement * node, const std::string & specFldr ) {
		// extract name
		const char * cName = node->Attribute( "name" );
		Event * evt = 0x0;
		if ( cName != 0x0 ) {
			std::string name( cName );
			evt = new Event( name );
		} else {
			logger << Logger::ERR_MSG << "Event defined on line " << node->Row() << " is missing the \"name\" attribute.";
			return 0x0;
		}
		
		TiXmlElement* child;
		for ( child = node->FirstChildElement(); child; child = child->NextSiblingElement() ) {
			if ( child->ValueStr() == "Trigger" ) {
				// parse the trigger
				evt->_trigger = EventTriggerDB::getInstance( child, specFldr );
				if ( evt->_trigger == 0x0 ) {
					delete evt;
					return 0x0;
				}
			} else if ( child->ValueStr() == "Response" ) {
				const char * eStr = child->Attribute( "effect" );
				if ( eStr == 0x0 ) {
					logger << Logger::ERR_MSG << "Event response on line " << child->Row() << " requires an \"effect\" attribute.";
					delete evt;
					return 0x0;
				}
				const char * tStr = child->Attribute( "target" );
				if ( tStr == 0x0 ) {
					logger << Logger::ERR_MSG << "Event response on line " << child->Row() << " requires an \"target\" attribute.";
					delete evt;
					return 0x0;
				}
				bool valid = true;
				// test the names against the database
				if ( EVENT_SYSTEM->_targets.find( tStr ) == EVENT_SYSTEM->_targets.end() ) {
					logger << Logger::ERR_MSG << "An event has been assigned a target which doesn't exist in the system: " << tStr << ".";
					valid = false;
				}
				if ( EVENT_SYSTEM->_effects.find( eStr ) == EVENT_SYSTEM->_effects.end() ) {
					logger << Logger::ERR_MSG << "An event has been assigned an effect which doesn't exist in the system: " << eStr << ".";
					valid = false;
				}
				if ( valid ) {
					evt->_responses.push_back( new EventResponse( eStr, tStr ) );
				} else {
					delete evt;
					return 0x0;
				}
			} else {
				logger << Logger::ERR_MSG << "Encountered unexpected child tag of Effects on line " << child->Row() << ": " << child->ValueStr() << ".";
				delete evt;
				return 0x0;
			}
		}
		return evt;
	}

}