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

#include "ExplicitAgentGenerator.h"
#include "tinyxml.h"
#include "Logger.h"

namespace Menge {

	namespace Agents {

		////////////////////////////////////////////////////////////////////////////
		//			Implementation of ExplicitGenerator
		////////////////////////////////////////////////////////////////////////////

		ExplicitGenerator::ExplicitGenerator() : AgentGenerator(), _positions() {
		}

		////////////////////////////////////////////////////////////////////////////

		Vector2 ExplicitGenerator::agentPos( size_t i ) {
			if ( i >= _positions.size() ) {
				throw AgentGeneratorFatalException("ExplicitGenerator trying to access an agent outside of the specified population");
			}
			return _positions[ i ];
		}

		////////////////////////////////////////////////////////////////////////////

		void ExplicitGenerator::addPosition( const Vector2 & p ) {
			_positions.push_back( addNoise( p ) );
		}

		////////////////////////////////////////////////////////////////////////////
		//			Implementation of ExplicitGeneratorFactory
		////////////////////////////////////////////////////////////////////////////

		bool ExplicitGeneratorFactory::setFromXML( AgentGenerator * gen, TiXmlElement * node, const std::string & specFldr ) const {
			ExplicitGenerator * eGen = dynamic_cast< ExplicitGenerator * >( gen );
			assert( eGen != 0x0 && "Trying to set attributes of an explicit agent generator component on an incompatible object" );

			if ( ! AgentGeneratorFactory::setFromXML( eGen, node, specFldr ) ) return false;

			for( TiXmlElement * child = node->FirstChildElement(); child; child = child->NextSiblingElement()) {
				if ( child->ValueStr() == "Agent" ) {
					try {
						Vector2 p = parseAgent( child );
						eGen->addPosition( p );
					} catch ( AgentGeneratorException ) {
						return false;
					}
				} else {
					logger << Logger::WARN_MSG << "Found an unexpected child tag in an AgentGroup on line " << node->Row() << ".  Ignoring the tag: " << child->ValueStr() << ".";
				}
			}

			return true;
		}

		////////////////////////////////////////////////////////////////////////////

		Vector2 ExplicitGeneratorFactory::parseAgent( TiXmlElement * node ) const {
			float x, y;
			double dVal;
			bool valid = true;
			if (node->Attribute( "p_x", &dVal ) ) {
				x = (float)dVal;
			} else {
				valid = false;
			}
			if (node->Attribute( "p_y", &dVal ) ) {
				y = (float)dVal;
			} else {
				valid = false;
			}
			if ( ! valid ) {
				logger << Logger::ERR_MSG << "Agent on line " << node->Row() << " didn't define position!";
				throw AgentGeneratorFatalException( "Agent in explicit generator didn't define a position" );
			}
			return Vector2( x, y );
		}

	}	// namespace Agents
}	// namespace Menge

