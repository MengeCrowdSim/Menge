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

#include "JohanssonInitializer.h"
#include "JohanssonAgent.h"
#include "Math/RandGenerator.h"

namespace Johansson {
	////////////////////////////////////////////////////////////////
	//			Implementation of Johansson::AgentInitializer
	////////////////////////////////////////////////////////////////

	// Default values
	const float DIR_WEIGHT = 0.16f;			///<	The default field-of-view weight.
	
	////////////////////////////////////////////////////////////////

	AgentInitializer::AgentInitializer() : Agents::AgentInitializer() { 
		_dirWeight = new ConstFloatGenerator( DIR_WEIGHT );
	}

	////////////////////////////////////////////////////////////////

	AgentInitializer::AgentInitializer( const AgentInitializer & init ) : Agents::AgentInitializer(init) { 
		_dirWeight = init._dirWeight->copy();
	}

	////////////////////////////////////////////////////////////////

	AgentInitializer::~AgentInitializer() {
		delete _dirWeight;
	}

	////////////////////////////////////////////////////////////////

	bool AgentInitializer::setProperties( Agents::BaseAgent * agent ) {
		Agent * a = dynamic_cast< Agent * >( agent );
		if ( a == 0x0 ) return false;
		a->_dirWeight = _dirWeight->getValue();

		return Agents::AgentInitializer::setProperties( agent );
	}

	////////////////////////////////////////////////////////////////

	bool AgentInitializer::isRelevant( const ::std::string & tagName ) {
		return ( tagName == "Johansson" ) || Agents::AgentInitializer::isRelevant( tagName );
	}

	////////////////////////////////////////////////////////////////

	Agents::AgentInitializer::ParseResult AgentInitializer::setFromXMLAttribute( const ::std::string & paramName, const ::std::string & value ) {
		ParseResult result = IGNORED;
		if ( paramName == "fov_weight" ) {
			result = constFloatGenerator( _dirWeight, value );
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
		if ( propName == "fov_weight" ) {
			result = getFloatGenerator( _dirWeight, node );
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
		if ( _dirWeight ) delete _dirWeight;
		_dirWeight = new ConstFloatGenerator( DIR_WEIGHT );

		Agents::AgentInitializer::setDefaults();
	}

	////////////////////////////////////////////////////////////////

}	// namespace Johansson
