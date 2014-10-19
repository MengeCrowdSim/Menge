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

#include "KaramouzasInitializer.h"
#include "KaramouzasAgent.h"
#include "Math/RandGenerator.h"

namespace Karamouzas {
	////////////////////////////////////////////////////////////////
	//			Implementation of Karamouzas::AgentInitializer
	////////////////////////////////////////////////////////////////

	// Default values
	const float PER_SPACE = 1.f;			///< The default personal space for the agent.
	const float ANTICIPATION = 3.f;			///< The anticipation time of the agent.
	
	////////////////////////////////////////////////////////////////

	AgentInitializer::AgentInitializer() : Agents::AgentInitializer() { 
		_perSpace = new ConstFloatGenerator( PER_SPACE );
		_anticipation = new ConstFloatGenerator( ANTICIPATION );
	}

	////////////////////////////////////////////////////////////////

	AgentInitializer::AgentInitializer( const AgentInitializer & init ) : Agents::AgentInitializer(init) { 
		_perSpace = init._perSpace->copy();
		_anticipation = init._anticipation->copy();
	}

	////////////////////////////////////////////////////////////////

	AgentInitializer::~AgentInitializer() {
		delete _perSpace;
		delete _anticipation;
	}

	////////////////////////////////////////////////////////////////

	bool AgentInitializer::setProperties( Agents::BaseAgent * agent ) {
		Agent * a = dynamic_cast< Agent * >( agent );
		if ( a == 0x0 ) return false;
		a->_perSpace = _perSpace->getValue();
		a->_anticipation = _anticipation->getValue();

		return Agents::AgentInitializer::setProperties( agent );
	}

	////////////////////////////////////////////////////////////////

	bool AgentInitializer::isRelevant( const ::std::string & tagName ) {
		return ( tagName == "Karamouzas" ) || Agents::AgentInitializer::isRelevant( tagName );
	}

	////////////////////////////////////////////////////////////////

	Agents::AgentInitializer::ParseResult AgentInitializer::setFromXMLAttribute( const ::std::string & paramName, const ::std::string & value ) {
		ParseResult result = IGNORED;
		if ( paramName == "personal_space" ) {
			result = constFloatGenerator( _perSpace, value );
		} else if ( paramName == "anticipation" ) {
			result = constFloatGenerator( _anticipation, value );
		} 

		if ( result == FAILURE ) {
			logger << Logger::WARN_MSG << "Attribute \"" << paramName << "\" had an incorrectly formed value: \"" << value << "\".  Using default value.";
			result = ACCEPTED;
		} else if ( result == IGNORED ){
			return Agents::AgentInitializer::setFromXMLAttribute( paramName, value );
		} 
		return result;
	}

	////////////////////////////////////////////////////////////////

	AgentInitializer::ParseResult AgentInitializer::processProperty( ::std::string propName, TiXmlElement * node ) {
		ParseResult result = IGNORED;
		if ( propName == "personal_space" ) {
			result = getFloatGenerator( _perSpace, node );
		} else if ( propName == "anticipation" ) {
			result = getFloatGenerator( _anticipation, node );
		} 

		if ( result == FAILURE ) {
			logger << Logger::ERR_MSG << "Error extracting value distribution from Property " << propName << ".";
			return result;
		} else if ( result == IGNORED ) {
			return Agents::AgentInitializer::processProperty( propName, node );
		}
		return result;
	}

	////////////////////////////////////////////////////////////////

	void AgentInitializer::setDefaults() {
		if ( _perSpace ) delete _perSpace;
		_perSpace = new ConstFloatGenerator( PER_SPACE );
		if ( _anticipation ) delete _anticipation;
		_anticipation = new ConstFloatGenerator( ANTICIPATION );

		Agents::AgentInitializer::setDefaults();
	}

	////////////////////////////////////////////////////////////////

}	// namespace Karamouzas
