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

#include "PedVOInitializer.h"
#include "PedVOAgent.h"
#include "Math/RandGenerator.h"

using namespace Menge::Math;

namespace PedVO {

	////////////////////////////////////////////////////////////////
	//			Implementation of ORCA::AgentInitializer
	////////////////////////////////////////////////////////////////

	// Default values
	const float TAU = 2.5f;				///< The default time horizon for predicting agent collisions
	const float TAU_OBST = 0.15f;		///< The default time horizon for predicting obstacle collisions
	const float TURNING_BIAS = 1.f;		///< The default turn bias
	const float STRIDE_FACTOR = 1.57f;			///< The default stride factor
	const float STRIDE_BUFFER = 0.9f;			///< The default stride buffer
		
	////////////////////////////////////////////////////////////////

	AgentInitializer::AgentInitializer() : Menge::Agents::AgentInitializer() { 
		_timeHorizon = new ConstFloatGenerator( TAU );
		_timeHorizonObst = new ConstFloatGenerator( TAU_OBST );
		_turningBias = new ConstFloatGenerator( TURNING_BIAS );
		_strideFactor = new ConstFloatGenerator( STRIDE_FACTOR );
		_strideBuffer = new ConstFloatGenerator( STRIDE_BUFFER );
		_denseAware = true;
	}

	////////////////////////////////////////////////////////////////

	AgentInitializer::AgentInitializer( const AgentInitializer & init ) : Menge::Agents::AgentInitializer(init) { 
		_timeHorizon = init._timeHorizon->copy();
		_timeHorizonObst = init._timeHorizonObst->copy();
		_turningBias = init._turningBias->copy();
		_strideFactor = init._strideFactor->copy();
		_strideBuffer = init._strideBuffer->copy();
		_denseAware = init._denseAware;
	}

	////////////////////////////////////////////////////////////////

	AgentInitializer::~AgentInitializer() {
		delete _timeHorizon;
		delete _timeHorizonObst;
		delete _turningBias;
		delete _strideFactor;
		delete _strideBuffer;
	}

	////////////////////////////////////////////////////////////////

	bool AgentInitializer::setProperties( Menge::Agents::BaseAgent * agent ) {
		Agent * a = dynamic_cast< Agent * >( agent );
		if ( a == 0x0 ) return false;
		a->_timeHorizon = _timeHorizon->getValue();
		a->_timeHorizonObst = _timeHorizonObst->getValue();
		a->_turningBias = _turningBias->getValue();
		float factor = _strideFactor->getValue();
		float buffer = _strideBuffer->getValue();
		a->setStrideParameters( factor, buffer );
		a->_denseAware = _denseAware;

		return Menge::Agents::AgentInitializer::setProperties( agent );
	}

	////////////////////////////////////////////////////////////////

	bool AgentInitializer::isRelevant( const ::std::string & tagName ) {
		return ( tagName == "PedVO" ) || Menge::Agents::AgentInitializer::isRelevant( tagName );
	}

	////////////////////////////////////////////////////////////////

	Menge::Agents::AgentInitializer::ParseResult AgentInitializer::setFromXMLAttribute( const ::std::string & paramName, const ::std::string & value ) {
		Menge::Agents::AgentInitializer::ParseResult result = IGNORED;
		if ( paramName == "tau" ) {
			result = constFloatGenerator( _timeHorizon, value );
		} else if ( paramName == "tauObst" ) {
			result = constFloatGenerator( _timeHorizonObst, value );
		} else if ( paramName == "turningBias" ) {
			result = constFloatGenerator( _turningBias, value );
		} else if ( paramName == "factor" ) {
			result = constFloatGenerator( _strideFactor, value );
		} else if ( paramName == "buffer" ) {
			result = constFloatGenerator( _strideBuffer, value );
		} else if ( paramName == "density_aware" ) {
			_denseAware = atoi( value.c_str() ) != 0;
		}

		if ( result == FAILURE ) {
			Menge::logger << Menge::Logger::WARN_MSG << "Attribute \"" << paramName << "\" had an incorrectly formed value: \"" << value << "\".  Using default value.";
			result = ACCEPTED;
		} else if ( result == IGNORED ){
			return Menge::Agents::AgentInitializer::setFromXMLAttribute( paramName, value );
		} 
		return result;
	}

	////////////////////////////////////////////////////////////////

	Menge::Agents::AgentInitializer::ParseResult AgentInitializer::processProperty( ::std::string propName, TiXmlElement * node ) {
		Menge::Agents::AgentInitializer::ParseResult result = IGNORED;
		if ( propName == "tau" ) {
			result = getFloatGenerator( _timeHorizon, node );
		} else if ( propName == "tauObst" ) {
			result = getFloatGenerator( _timeHorizonObst, node );
		} else if ( propName == "turningBias" ) {
			result = getFloatGenerator( _turningBias, node );
		} else if ( propName == "factor" ) {
			result = getFloatGenerator( _strideFactor, node );
		} else if ( propName == "buffer" ) {
			result = getFloatGenerator( _strideBuffer, node );
		}

		if ( result == FAILURE ) {
			Menge::logger << Menge::Logger::ERR_MSG << "Error extracting value distribution from Property " << propName << ".";
			return result;
		} else if ( result == IGNORED ) {
			return Menge::Agents::AgentInitializer::processProperty( propName, node );
		}
		return result;
	}

	////////////////////////////////////////////////////////////////

	void AgentInitializer::setDefaults() {
		if ( _timeHorizon ) delete _timeHorizon;
		_timeHorizon = new ConstFloatGenerator( TAU );
		if ( _timeHorizonObst ) delete _timeHorizonObst;
		_timeHorizonObst = new ConstFloatGenerator( TAU_OBST );
		if ( _strideFactor ) delete _strideFactor;
		_strideFactor = new ConstFloatGenerator( STRIDE_FACTOR );
		if ( _strideBuffer ) delete _strideBuffer;
		_strideBuffer = new ConstFloatGenerator( STRIDE_BUFFER );
		_denseAware = true;

		Menge::Agents::AgentInitializer::setDefaults();
	}

	////////////////////////////////////////////////////////////////

}	// namespace ORCA
