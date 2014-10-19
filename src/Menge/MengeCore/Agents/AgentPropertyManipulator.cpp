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

#include "AgentPropertyManipulator.h"
#include "BaseAgent.h"

namespace Menge {

	/////////////////////////////////////////////////////////////////////
	//                   Implementation of helper function
	/////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		Helper function for parsing property actions
	 *
	 *	It converts the xml string indicating the property to an 
	 *	enumerated value
	 *
	 *	@param		opName		The name of the operand as specified in the
	 *							xml specification.
	 *	@returns	The corresponding enumeration.  If there is no corresponding
	 *				enumeration, NO_PROPERTY is returned.
	 */
	BFSM::PropertyOperand parsePropertyName( const std::string & opName ) {
		if ( opName == "max_speed" ) {
			return BFSM::MAX_SPEED;
		} else if ( opName == "max_accel" ) {
			return BFSM::MAX_ACCEL;
		} else if ( opName == "pref_speed" ) {
			return BFSM::PREF_SPEED;
		} else if ( opName == "max_angle_vel" ) {
			return BFSM::MAX_ANGLE_VEL;
		} else if ( opName == "neighbor_dist" ) {
			return BFSM::NEIGHBOR_DIST;
		} else if ( opName == "priority" ) {
			return BFSM::PRIORITY;
		} else if ( opName == "r" ) {
			return BFSM::RADIUS;
		}
		return BFSM::NO_PROPERTY;
	}
	
	/////////////////////////////////////////////////////////////////////
	//					Implementation of AgentPropertyManipulator
	/////////////////////////////////////////////////////////////////////

	AgentPropertyManipulator::AgentPropertyManipulator(): Element(), _operandGen(0x0),_property(BFSM::NO_PROPERTY),_originalMap() {
	}

	/////////////////////////////////////////////////////////////////////

	AgentPropertyManipulator::~AgentPropertyManipulator() {
		// Is this delete safe?  This may require a destroy method if it is
		//	instantiated in MengeCore and used in external dll
		if ( _operandGen ) delete _operandGen;
		_originalMap.clear();
	}

	/////////////////////////////////////////////////////////////////////

	void AgentPropertyManipulator::manipulate( Agents::BaseAgent * agent ) {
		_lock.lock();
		switch ( _property ) {
			case BFSM::MAX_SPEED:
				_originalMap[ agent->_id ] = agent->_maxSpeed;
				agent->_maxSpeed = newValue( agent->_maxSpeed, agent->_id );
				break;
			case BFSM::MAX_ACCEL:
				_originalMap[ agent->_id ] = agent->_maxAccel;
				agent->_maxAccel = newValue( agent->_maxAccel, agent->_id );
				break;
			case BFSM::PREF_SPEED:
				_originalMap[ agent->_id ] = agent->_prefSpeed;
				agent->_prefSpeed = newValue( agent->_prefSpeed, agent->_id );
				break;
			case BFSM::MAX_ANGLE_VEL:
				_originalMap[ agent->_id ] = agent->_maxAngVel;
				agent->_maxAngVel = newValue( agent->_maxAngVel, agent->_id );
				break;
			case BFSM::NEIGHBOR_DIST:
				_originalMap[ agent->_id ] = agent->_neighborDist;
				agent->_neighborDist = newValue( agent->_neighborDist, agent->_id );
				break;
			case BFSM::PRIORITY:
				_originalMap[ agent->_id ] = agent->_priority;
				agent->_priority = newValue( agent->_priority, agent->_id );
				break;
			case BFSM::RADIUS:
				_originalMap[ agent->_id ] = agent->_radius;
				agent->_radius = newValue( agent->_radius, agent->_id );
				break;
		}
		_lock.release();
	}

	/////////////////////////////////////////////////////////////////////

	void AgentPropertyManipulator::restore( Agents::BaseAgent * agent ) {
		_lock.lock();
		std::map< size_t, float >::iterator itr = _originalMap.find( agent->_id );
		if ( itr == _originalMap.end() ) {
			_lock.release();
			return;
		}
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

	void AgentPropertyManipulator::setGenerator( FloatGenerator * gen ) {
		if ( _operandGen ) delete _operandGen;	// see note in destructor
		_operandGen = gen;
	}

	/////////////////////////////////////////////////////////////////////
	//					Implementation of SetPropertyManipulator
	/////////////////////////////////////////////////////////////////////

	float SetPropertyManipulator::newValue( float value, size_t agentID ) {
		return _operandGen->getValue();
	}

	/////////////////////////////////////////////////////////////////////
	//                   Implementation of OffsetPropertyManipulator
	/////////////////////////////////////////////////////////////////////

	float OffsetPropertyManipulator::newValue( float value, size_t agentID ) {
		return value + _operandGen->getValue();
	}

	/////////////////////////////////////////////////////////////////////
	//                   Implementation of ScalePropertyManipulator
	/////////////////////////////////////////////////////////////////////

	float ScalePropertyManipulator::newValue( float value, size_t agentID ) {
		return value * _operandGen->getValue();
	}
	
}	// namespace Menge