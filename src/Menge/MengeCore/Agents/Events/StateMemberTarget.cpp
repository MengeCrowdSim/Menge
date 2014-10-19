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

#include "StateMemberTarget.h"
#include "Core.h"
#include "FSM.h"
#include "State.h"
#include "EventSystem.h"
#include "SimulatorInterface.h"
#include <sstream>

namespace Menge {

	/////////////////////////////////////////////////////////////////////
	//					Implementation of NamedStateMemberTarget
	/////////////////////////////////////////////////////////////////////

	void NamedStateMemberTarget::finalize() {
		_state = ACTIVE_FSM->getNode( _stateName );
		if ( _state == 0x0 ) {
			std::stringstream ss;
			ss << "Event target unable to to locate state " << _stateName << ".";
			EventSystem::finalizeException( ss.str() );
		}
	}

	/////////////////////////////////////////////////////////////////////

	void NamedStateMemberTarget::update() {
		if ( _lastUpdate != SIM_TIME ) {
			_elements.clear();
			const size_t AGENT_COUNT = SIMULATOR->getNumAgents();
			for ( size_t i = 0; i < AGENT_COUNT; ++i ) {
				Agents::BaseAgent * agent = SIMULATOR->getAgent( i );
				BFSM::State * state = ACTIVE_FSM->getCurrentState( agent );
				if ( ( state != _state ) ^ _inState ) {
					_elements.push_back( agent );
				}
			}
			AgentEventTarget::update();
		}
	}

	/////////////////////////////////////////////////////////////////////
	//					Implementation of NamedStateMemberTargetFactory
	/////////////////////////////////////////////////////////////////////

	NamedStateMemberTargetFactory::NamedStateMemberTargetFactory() : AgentEventTargetFactory() {
		_stateID = _attrSet.addStringAttribute( "state", true, "" );
		_isMemberID = _attrSet.addBoolAttribute( "is_member", true, true );
	}

	/////////////////////////////////////////////////////////////////////

	bool NamedStateMemberTargetFactory::setFromXML( EventTarget * target, TiXmlElement * node, const std::string & specFldr ) const {
		NamedStateMemberTarget * sTarget = dynamic_cast< NamedStateMemberTarget * >( target );
		assert( sTarget != 0x0 && "Trying to set attributes of a state-membership event target on an incompatible object" );

		if ( ! AgentEventTargetFactory::setFromXML( target, node, specFldr ) ) return false;

		sTarget->_stateName = _attrSet.getString( _stateID );
		sTarget->_inState = _attrSet.getBool( _isMemberID );

		return true;
	}

}	// namespace Menge