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

#include "StateEvtTrigger.h"
#include "Core.h"
#include "FSM.h"
#include "State.h"
#include "EventSystem.h"
#include <sstream>

namespace Menge {

	/////////////////////////////////////////////////////////////////////
	//					Implementation of StateEvtTrigger
	/////////////////////////////////////////////////////////////////////

	StateEvtTrigger::StateEvtTrigger() : EventTrigger(), _stateName(), _state(0x0) {
	}

	/////////////////////////////////////////////////////////////////////

	void StateEvtTrigger::finalize() {
		_state = ACTIVE_FSM->getNode( _stateName );
		if ( _state == 0x0 ) {
			std::stringstream ss;
			ss << "Event trigger unable to to locate state " << _stateName << ".";
			EventSystem::finalizeException( ss.str() );
		}
	}
	
	/////////////////////////////////////////////////////////////////////
	//					Implementation of StateEvtTriggerFactory
	/////////////////////////////////////////////////////////////////////

	StateEvtTriggerFactory::StateEvtTriggerFactory() : EventTriggerFactory() {
		_stateID = _attrSet.addStringAttribute( "state", true, "" );
	}

	/////////////////////////////////////////////////////////////////////

	bool StateEvtTriggerFactory::setFromXML( EventTrigger * trigger, TiXmlElement * node, const std::string & specFldr ) const {
		StateEvtTrigger * sTrigger = dynamic_cast< StateEvtTrigger * >( trigger );
		assert( sTrigger != 0x0 && "Trying to set attributes of a state event trigger on an incompatible object" );

		if ( ! EventTriggerFactory::setFromXML( sTrigger, node, specFldr ) ) return false;

		sTrigger->_stateName = _attrSet.getString( _stateID );

		return true;
	}

	/////////////////////////////////////////////////////////////////////
	//					Implementation of StatePopIncreaseTrigger
	/////////////////////////////////////////////////////////////////////

	// _lastPop is initialized to a ridiculously high number in order to keep
	//	the event from triggering upon initialization.  The first call to
	//	testCondition will bring it back down.
	StatePopIncreaseTrigger::StatePopIncreaseTrigger() : StateEvtTrigger(), _lastPop(100000000) {
	}

	/////////////////////////////////////////////////////////////////////

	void StatePopIncreaseTrigger::finalize() {
		StateEvtTrigger::finalize();
		_lastPop = _state->getPopulation();
	}

	/////////////////////////////////////////////////////////////////////

	bool StatePopIncreaseTrigger::testCondition() {
		size_t currPop = _state->getPopulation();
		bool increased = currPop > _lastPop;
		_lastPop = currPop;
		return increased;
	}

}	// namespace Menge 