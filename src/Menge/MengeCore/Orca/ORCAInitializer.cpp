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

#include "ORCAInitializer.h"
#include "ORCAAgent.h"
#include "Math/RandGenerator.h"

namespace ORCA {

	////////////////////////////////////////////////////////////////
	//			Implementation of ORCA::AgentInitializer
	////////////////////////////////////////////////////////////////

	// Default values
	const float TAU = 2.5f;				///< The default value for tau (the time horizon w.r.t. other agents).
	const float TAU_OBST = 0.15f;		///< The default value for tau obstacles (the time horizon w.r.t. obstacles).
	
	////////////////////////////////////////////////////////////////

	AgentInitializer::AgentInitializer() : Menge::Agents::AgentInitializer() { 
		_timeHorizon = new ConstFloatGenerator( TAU );
		_timeHorizonObst = new ConstFloatGenerator( TAU_OBST );
	}

	////////////////////////////////////////////////////////////////

	AgentInitializer::AgentInitializer( const AgentInitializer & init ) : Menge::Agents::AgentInitializer(init) { 
		_timeHorizon = init._timeHorizon->copy();
		_timeHorizonObst = init._timeHorizonObst->copy();
	}

	////////////////////////////////////////////////////////////////

	AgentInitializer::~AgentInitializer() {
		delete _timeHorizon;
		delete _timeHorizonObst;
	}

	////////////////////////////////////////////////////////////////

	bool AgentInitializer::setProperties( Menge::Agents::BaseAgent * agent ) {
		Agent * a = dynamic_cast< Agent * >( agent );
		if ( a == 0x0 ) return false;
		a->_timeHorizon = _timeHorizon->getValue();
		a->_timeHorizonObst = _timeHorizonObst->getValue();

		return Menge::Agents::AgentInitializer::setProperties( agent );
	}

	////////////////////////////////////////////////////////////////

	bool AgentInitializer::isRelevant( const ::std::string & tagName ) {
		return ( tagName == "ORCA" ) || Menge::Agents::AgentInitializer::isRelevant( tagName );
	}

	////////////////////////////////////////////////////////////////

	Menge::Agents::AgentInitializer::ParseResult AgentInitializer::setFromXMLAttribute( const ::std::string & paramName, const ::std::string & value ) {
		Menge::Agents::AgentInitializer::ParseResult result = IGNORED;
		if ( paramName == "tau" ) {
			result = constFloatGenerator( _timeHorizon, value );
		} else if ( paramName == "tauObst" ) {
			result = constFloatGenerator( _timeHorizonObst, value );
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
		Menge::Agents::AgentInitializer::setDefaults();
	}

	////////////////////////////////////////////////////////////////

}	// namespace ORCA
