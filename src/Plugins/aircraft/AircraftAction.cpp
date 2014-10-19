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

#include "AircraftAction.h"
#include "Actions/PropertyAction.h"
#include "BaseAgent.h"
#include "SimulatorInterface.h"
#include "FSM.h"
#include "BFSM/GoalSet.h"
#include <iostream>

namespace Aircraft {

	/////////////////////////////////////////////////////////////////////
	//                   Implementation of PropertyXAction
	/////////////////////////////////////////////////////////////////////

	PropertyXAction::PropertyXAction():Action(),_xOrigin(0.f),_originValue(0.f),_scale(0.f),_property(BFSM::NO_PROPERTY),_originalMap() {
	}

	/////////////////////////////////////////////////////////////////////

	PropertyXAction::~PropertyXAction() {
		_originalMap.clear();
	}

	/////////////////////////////////////////////////////////////////////

	void PropertyXAction::onEnter( Agents::BaseAgent * agent ) {
		float value = ( agent->_pos.x() - _xOrigin ) * _scale + _originValue;
		_lock.lock();
		switch ( _property ) {
			case BFSM::MAX_SPEED:
				if ( _undoOnExit ) _originalMap[ agent->_id ] = agent->_maxSpeed;
				agent->_maxSpeed =	value;
				break;
			case BFSM::MAX_ACCEL:
				if ( _undoOnExit ) _originalMap[ agent->_id ] = agent->_maxAccel;
				agent->_maxAccel = value;
				break;
			case BFSM::PREF_SPEED:
				if ( _undoOnExit ) _originalMap[ agent->_id ] = agent->_prefSpeed;
				agent->_prefSpeed = value;
				break;
			case BFSM::MAX_ANGLE_VEL:
				if ( _undoOnExit ) _originalMap[ agent->_id ] = agent->_maxAngVel;
				agent->_maxAngVel = value;
				break;
			case BFSM::NEIGHBOR_DIST:
				if ( _undoOnExit ) _originalMap[ agent->_id ] = agent->_neighborDist;
				agent->_neighborDist = value;
				break;
			case BFSM::PRIORITY:
				if ( _undoOnExit ) _originalMap[ agent->_id ] = agent->_priority;
				agent->_priority = value;
				break;
			case BFSM::RADIUS:
				if ( _undoOnExit ) _originalMap[ agent->_id ] = agent->_radius;
				agent->_radius = value;
				break;
		}
		_lock.release();
	}

	/////////////////////////////////////////////////////////////////////

	void PropertyXAction::leaveAction( Agents::BaseAgent * agent ) {
		_lock.lock();
		std::map< size_t, float >::iterator itr = _originalMap.find( agent->_id );
		assert( itr != _originalMap.end() && "An agent is exiting a state that it apparently never entered" );
		float value = itr->second;
		_originalMap.erase( itr );
		_lock.release();
		switch ( _property ) {
			case BFSM::MAX_SPEED:
				agent->_maxSpeed = value;
				break;
			case BFSM::MAX_ACCEL:
				agent->_maxAccel = value;
				break;
			case BFSM::PREF_SPEED:
				agent->_prefSpeed = value;
				break;
			case BFSM::MAX_ANGLE_VEL:
				agent->_maxAngVel = value;
				break;
			case BFSM::NEIGHBOR_DIST:
				agent->_neighborDist = value;
				break;
			case BFSM::PRIORITY:
				agent->_priority = value;
				break;
			case BFSM::RADIUS:
				agent->_radius = value;
				break;
		}
	}

	/////////////////////////////////////////////////////////////////////
	//                   Implementation of PropertyXActFactory
	/////////////////////////////////////////////////////////////////////

	bool PropertyXActFactory::setFromXML( BFSM::Action * action, TiXmlElement * node, const std::string & behaveFldr ) const {
		PropertyXAction * pAction = dynamic_cast< PropertyXAction * >( action );
		assert( pAction != 0x0 && "Trying to set property action properties on an incompatible object" );
		if ( ! BFSM::ActionFactory::setFromXML( action, node, behaveFldr ) ) {
			return false;
		}
		// set the target property
		const char * pName = node->Attribute( "property" );
		if ( ! pName ) {
			logger << Logger::ERR_MSG << "The property action defined on line " << node->Row() << " did not define the \"property\" attribute\n";
			return false;
		}
		pAction->_property = Menge::parsePropertyName( pName );
		if ( pAction->_property == BFSM::NO_PROPERTY ) {
			logger << Logger::ERR_MSG  << "The set property x action defined online " << node->Row() << " specified an invalid value for the \"property\" attribute\n";
			return false;
		}
		
		double d;
		// Get the x-origin value
		if ( node->Attribute( "origin", &d ) ) {
			pAction->_xOrigin = (float) d;
		} else {
			logger << Logger::WARN_MSG << "The set property x action defined on line " << node->Row() << " did not define the \"origin\" attribute.  Using the default value 0.0\n";
			pAction->_xOrigin = 0.f;
		}

		// Get the origin value
		if ( node->Attribute( "origin_value", &d ) ) {
			pAction->_originValue = (float) d;
		} else {
			logger << Logger::WARN_MSG << "The set property x action defined on line " << node->Row() << " did not define the \"origin_value\" attribute.  Using the default value 0.0\n";
			pAction->_originValue = 0.f;
		}

		// Get the scale value
		if ( node->Attribute( "scale", &d ) ) {
			pAction->_scale = (float) d;
		} else {
			logger << Logger::WARN_MSG << "The set property x action defined on line " << node->Row() << " did not define the \"scale\" attribute.  Using the default value 0.0\n";
			pAction->_scale = 0.f;
		}


		return true;
	}

	
	



}	// namespace Aircraft
