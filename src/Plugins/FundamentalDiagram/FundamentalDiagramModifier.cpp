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

#ifndef __FORMATIONS_MODIFIER_CPP__
#define __FORMATIONS_MODIFIER_CPP__

#include "FundamentalDiagramModifier.h"
#include "BaseAgent.h"
#include "Obstacle.h"

namespace FDModifier {
	/////////////////////////////////////////////////////////////////////
	//                   Implementation of FDModifier
	/////////////////////////////////////////////////////////////////////

	FDModifier::FDModifier(): BFSM::VelModifier(), _bufferGen(0x0), _factorGen(0x0), _sigmaAgent(1.5f), _sigmaObstacle(0.75f) {
	}

	/////////////////////////////////////////////////////////////////////

	FDModifier::FDModifier( FloatGenerator * buffer, FloatGenerator * factor, float sigmaAgent, float sigmaObstacle ): BFSM::VelModifier(), _bufferGen(buffer), _factorGen(factor), _sigmaAgent(sigmaAgent), _sigmaObstacle(sigmaObstacle) {
	}

	/////////////////////////////////////////////////////////////////////

	BFSM::VelModifier* FDModifier::copy() const{
		return new FDModifier( _bufferGen->copy(), _factorGen->copy(), _sigmaAgent, _sigmaAgent );
	};

	/////////////////////////////////////////////////////////////////////

	void FDModifier::adaptPrefVelocity(const Agents::BaseAgent * agent, Agents::PrefVelocity & pVel ){
		float strideConst, speedConst;
		_paramLock.lock();
		HASH_MAP< size_t, FDParam >::iterator itr = _strideParams.find( agent->_id );
		if ( itr == _strideParams.end() ) {
			FDParam param( _factorGen->getValue(), _bufferGen->getValue() );
			_strideParams[ agent->_id ] = param;
			strideConst = param._strideConst;
			speedConst = param._speedConst;
		} else {
			strideConst = itr->second._strideConst;
			speedConst = itr->second._speedConst;
		}
		_paramLock.release();

		//define temporary vars
		float prefSpeed = pVel.getSpeed();	
		Vector2 prefDir( pVel.getPreferred() );
		float availSpace = 1e6f;	// start assuming there is infinite space

		const float testDistance = 1.f;	// The distance (in meters) in the direction of preferred travel to test
									// the density.
		Vector2 critPt = agent->_pos + testDistance * prefDir;

		float density = 0.f;
		// For now, assume some constants
		const float areaSq2Inv = 1.f / ( 2 * _sigmaAgent * _sigmaAgent );
		const float sqrt2Pi = sqrtf( TWOPI );
		const float norm = 1.f / ( _sigmaAgent * sqrt2Pi );

		// AGENTS
		for ( size_t i = 0; i < agent->_nearAgents.size(); ++i ) {
			const Agents::BaseAgent* const other = agent->_nearAgents[i].agent;
			Vector2 critDisp = other->_pos - critPt;
			Vector2 yComp = ( critDisp * prefDir ) * prefDir;	// dot project gets projection, in the preferred direction
			Vector2 xComp = ( critDisp - yComp ) * 2.5f;			// penalize displacement perpindicular to the preferred direction
			critDisp.set( xComp + yComp );
			float distSq = absSq( critDisp );
			density += norm * expf( -distSq * areaSq2Inv );	
		}

		//// OBSTACLES
		const float OBST_AREA_SQ_INV = 1.f / ( 2 * _sigmaObstacle * _sigmaObstacle );
		const float OBST_NORM = 1.f / ( _sigmaObstacle * sqrt2Pi );
		const float OBST_SCALE = norm;// * 6.25f;	// what is the "density" of an obstacle?
		for ( size_t i = 0; i < agent->_nearObstacles.size(); ++i ) {
			const Agents::Obstacle* const obst = agent->_nearObstacles[i].obstacle;
			Vector2 nearPt;
			float distSq;	// set by distanceSqToPoint
			if ( obst->distanceSqToPoint( critPt, nearPt, distSq ) == Agents::Obstacle::LAST ) continue;

			if ( ( nearPt - agent->_pos ) * prefDir < 0.f ) continue;
			density += OBST_SCALE * expf( -distSq * OBST_AREA_SQ_INV );
		}

		const float	AGENT_WIDTH = 0.48f;
		if ( density < 0.001f ) {
			availSpace = 100.f;
		} else {
			availSpace = AGENT_WIDTH / density ;
		}

		// Compute the maximum speed I could take for the available space
		float maxSpeed = speedConst * availSpace * availSpace;
		if ( maxSpeed < prefSpeed ) pVel.setSpeed( maxSpeed );
	}

	/////////////////////////////////////////////////////////////////////
	//                   Implementation of FDModFactory
	/////////////////////////////////////////////////////////////////////

	FDModifierFactory::FDModifierFactory():BFSM::VelModFactory() {
		_factorID = _attrSet.addFloatDistAttribute( "factor_", true, 0.f, 1.f );
		_bufferID = _attrSet.addFloatDistAttribute( "buffer_", true, 0.f, 1.f );
		_sigmaAgentID = _attrSet.addFloatAttribute( "sigma_agent", false, 1.5f );
		_sigmaObstacleID = _attrSet.addFloatAttribute( "sigma_obstacle", false, 0.75f );
	}

	/////////////////////////////////////////////////////////////////////

	bool FDModifierFactory::setFromXML( BFSM::VelModifier * modifier, TiXmlElement * node, const std::string & behaveFldr ) const { 
		FDModifier * FDMod = dynamic_cast<FDModifier *>( modifier );
        assert( FDMod != 0x0 && "Trying to set property modifier properties on an incompatible object" );

		if ( ! BFSM::VelModFactory::setFromXML( modifier, node, behaveFldr ) ) return false;
		
		//set the params we need
		FDMod->setBuffer( _attrSet.getFloatGenerator( _bufferID ) );
		FDMod->setFactor( _attrSet.getFloatGenerator( _factorID ) );
		FDMod->setSigmaAgent( _attrSet.getFloat( _sigmaAgentID ) );
		FDMod->setSigmaObstacle( _attrSet.getFloat( _sigmaObstacleID ) );
		return true;
	}
};

#endif
