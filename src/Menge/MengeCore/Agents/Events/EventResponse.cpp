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

#include "EventResponse.h"
#include "EventEffect.h"
#include "EventTarget.h"
#include "Logger.h"
#include "EventException.h"
#include "EventSystem.h"
#include "Core.h"
#include <sstream>
#include <cassert>

namespace Menge {

	/////////////////////////////////////////////////////////////////////
	//					Implementation of EventResponse
	/////////////////////////////////////////////////////////////////////

	EventResponse::EventResponse() : _effectName(), _effect(0x0), _targetName(), _target(0x0) {
	}

	/////////////////////////////////////////////////////////////////////

	EventResponse::EventResponse( const std::string & effect, const std::string & target ) : _effectName(effect), _effect(0x0), _targetName(target), _target(0x0) {
	}

	/////////////////////////////////////////////////////////////////////

	void EventResponse::finalize() {
		// Find the target and effect from the event system
		HASH_MAP< std::string, EventEffect * >::iterator eItr = EVENT_SYSTEM->_effects.find( _effectName );
		// Only asserting this reasonable because I tested the names
		//	at parse time.
		assert ( eItr != EVENT_SYSTEM->_effects.end() && "Missing effect at response finalization" );

		HASH_MAP< std::string, EventTarget * >::iterator tItr = EVENT_SYSTEM->_targets.find( _targetName );
		assert ( tItr != EVENT_SYSTEM->_targets.end() && "Missing target at response finalization" );

		_effect = eItr->second;
		_target = tItr->second;
		// confirm compatability
		if ( ! _effect->isCompatible( _target ) ) {
			std::stringstream ss;
			ss << "The target " << _targetName << " is incompatible with the event effect " << _effectName << ".";
			EventSystem::finalizeException( ss.str() );
		}
	}

	/////////////////////////////////////////////////////////////////////

	void EventResponse::apply() {
		_target->update();
		_effect->apply( _target );

	}

	/////////////////////////////////////////////////////////////////////

	

}	//namespace Menge