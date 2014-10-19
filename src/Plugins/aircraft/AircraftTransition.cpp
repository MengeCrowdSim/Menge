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

/*!
 *	@file		AircraftTransition.h
 *	@brief		Definition of transition condition used in aircraft
 *				loading and unloading.
 */

#include "AircraftConfig.h"
#include "AircraftTransition.h"
#include "Transitions/ConditionFactory.h"
#include "FSMEnumeration.h"
#include <map>
#include "BaseAgent.h"
#include "Core.h"
#include "SimulatorInterface.h"

namespace Aircraft {
	
	/////////////////////////////////////////////////////////////////////////
	//                   Implementation of ClearAABBCondition
	/////////////////////////////////////////////////////////////////////////

	ClearAABBCondition::ClearAABBCondition( const ClearAABBCondition & cond ): BFSM::Condition(cond), _relative(cond._relative), _agentClass(cond._agentClass), _baseBox(cond._baseBox) {		
	}

	/////////////////////////////////////////////////////////////////////////

	ClearAABBCondition::~ClearAABBCondition() {
		
	}

	/////////////////////////////////////////////////////////////////////////

	bool ClearAABBCondition::conditionMet( Agents::BaseAgent * agent, const BFSM::Goal * goal ){
		// Determine if the agent's AABB is clear of the target class of agents
		//std::cout << "ClearAABBCondition on agent " << agent->_id << "\n";
		Vector2 offset(0.f,0.f);
		if ( _relative ) {
			//std::cout << "\tRelative to position: " << agent->_pos << "\n";
			offset.set( agent->_pos );
		}
		AABBShape activeBox( _baseBox, offset );

		//TODO: OPTIMIZE THIS
		const size_t NUM_AGENT = Menge::SIMULATOR->getNumAgents();
		for ( size_t i = 0; i < NUM_AGENT; ++i ) {
			const Agents::BaseAgent * testAgent = Menge::SIMULATOR->getAgent( i );
			//if this agent is in my box
			if ( testAgent->_id != agent->_id && 
				( _agentClass == -1 || _agentClass == testAgent->_class )
			   ){
				if ( activeBox.containsPoint(testAgent->_pos ) ) {
					return false;
				}
			}
		}

		return true;
	};

	/////////////////////////////////////////////////////////////////////////

	BFSM::Condition * ClearAABBCondition::copy() {
		return new ClearAABBCondition( *this );
	}

	/////////////////////////////////////////////////////////////////////////

	void ClearAABBCondition::setParams(float xMin, float xMax, float yMin, float yMax, bool relative, int agentClass){
	    //set the properties of this box
		_relative = relative;
		_agentClass = (size_t)agentClass;
		_baseBox.set( Vector2( xMin, yMin ), Vector2( xMax, yMax ) );
	};
	
	///////////////////////////////////////////////////////////////////////////
	//                   Implementation of ClearAABBCondFactory
	///////////////////////////////////////////////////////////////////////////

	bool ClearAABBCondFactory::setFromXML( BFSM::Condition * condition, TiXmlElement * node, const std::string & behaveFldr ) const {
		ClearAABBCondition * cond = dynamic_cast< ClearAABBCondition * >( condition );
		assert( cond != 0x0 && "Trying to set the properties of a ClearAABB condition on an incompatible object" );

		if ( !BFSM::ConditionFactory::setFromXML( cond, node, behaveFldr ) ) {
			return false;
		}
		// dimensions
		bool valid = true;
		double xMin, xMax, yMin, yMax;
		int relative, agentClass;

		// determine if it is active inside or outside
		if ( ! node->Attribute( "relative", &relative ) ) {
			logger << Logger::ERR_MSG << "The clear_AABB condition on line " << node->Row() << " is missing the \"relative\" attribute.";
			return false;
		}
		if ( ! node->Attribute( "min_x", &xMin ) ) {
			logger << Logger::ERR_MSG << "The clear_AABB condition on line " << node->Row() << " is missing the \"min_x\" property.";
			valid = false;
		}
		if ( ! node->Attribute( "max_x", &xMax ) ) {
			logger << Logger::ERR_MSG << "The clear_AABB condition on line " << node->Row() << " is missing the \"max_x\" property.";
			valid = false;
		}
		if ( ! node->Attribute( "min_y", &yMin ) ) {
			logger << Logger::ERR_MSG << "The clear_AABB condition on line " << node->Row() << " is missing the \"min_y\" property.";
			valid = false;
		}
		if ( ! node->Attribute( "max_y", &yMax ) ) {
			logger << Logger::ERR_MSG << "The clear_AABB condition on line " << node->Row() << " is missing the \"max_y\" property.";
			valid = false;
		}
		if (!node->Attribute( "agent_class", &agentClass ) ) {
			logger << Logger::WARN_MSG << "The clear_AABB condition on line " << node->Row() << " did not define \"agentClass\" property. Using -1.";	
			agentClass = -1;
		}
		if ( ! valid ) return false;

		cond->setParams( (float)xMin, (float)xMax, (float)yMin, (float)yMax, relative != 0, agentClass );

		return true;
	}
}	// namespace Aircraft

