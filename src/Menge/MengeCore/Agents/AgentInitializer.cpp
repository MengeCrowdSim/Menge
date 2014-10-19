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

#include "AgentInitializer.h"
#include "BaseAgent.h"
#include "Math/RandGenerator.h"
#include "tinyxml.h"
#include "Utils.h"
#include <sstream>

namespace Menge {

	namespace Agents {

		////////////////////////////////////////////////////////////////
		//			Implementation of AgentInitializer
		////////////////////////////////////////////////////////////////

		// Default values
		const float MAX_SPEED = 2.5f;				///< The default maximum speed
		const float MAX_ACCEL = 2.f;				///< The default maximum acceleration.
		const float PREF_SPEED = 1.34f;				///< The default preferred speed
		const int MAX_NEIGHBORS = 10;				///< The default maximum number of neighbors
		const float NEIGHBOR_DIST = 5.f;			///< The default neighbor distance
		const float RADIUS = 0.2f;					///< The default radius
		const size_t CLASS = 0;						///< The default class
		const float PRIORITY = 0.f;					///< The default priority
		const float MAX_ANGLE_VEL = TWOPI;			///< The default maximum angular velocity
		const size_t OBSTACLE_SET = 0xFFFFFFFF;		///< The default obstacle set (all obstacles)

		////////////////////////////////////////////////////////////////
		
		// TODO: Set this value with the VERBOSE flag in simple.exe
		bool AgentInitializer::VERBOSE = false;

		////////////////////////////////////////////////////////////////

		AgentInitializer::AgentInitializer() {
			// Values for distribution
			_maxSpeed = new ConstFloatGenerator( MAX_SPEED );
			_maxAccel = new ConstFloatGenerator( MAX_ACCEL );
			_prefSpeed = new ConstFloatGenerator( PREF_SPEED );
			_maxNeighbors = new ConstIntGenerator( MAX_NEIGHBORS );
			_neighborDist = new ConstFloatGenerator( NEIGHBOR_DIST );
			_radius = new ConstFloatGenerator( RADIUS );
			_maxAngVel = new ConstFloatGenerator( MAX_ANGLE_VEL );
			// single values
			_obstacleSet = OBSTACLE_SET;
			_priority = PRIORITY;
			_class = CLASS;
		}

		////////////////////////////////////////////////////////////////
		
		AgentInitializer::AgentInitializer( const AgentInitializer & init ) {
			_maxSpeed = init._maxSpeed->copy();
			_maxAccel = init._maxAccel->copy();
			_prefSpeed = init._prefSpeed->copy();
			_maxNeighbors = init._maxNeighbors->copy();
			_neighborDist = init._neighborDist->copy();
			_radius = init._radius->copy();
			_maxAngVel = init._maxAngVel->copy();
			_obstacleSet = init._obstacleSet;
			_priority = init._priority;
			_class = init._class;

			std::vector< BFSM::VelModifier * >::const_iterator vItr = init._velModifiers.begin();
			for ( ; vItr != init._velModifiers.end(); ++vItr ) {
				_velModifiers.push_back((*vItr)->copy());
				
			}

		}

		////////////////////////////////////////////////////////////////
		
		AgentInitializer::~AgentInitializer() {
			delete _maxSpeed;
			delete _maxAccel;
			delete _prefSpeed;
			delete _maxNeighbors;
			delete _neighborDist;
			delete _radius;
			delete _maxAngVel;
		}

		////////////////////////////////////////////////////////////////
		
		void AgentInitializer::setDefaults() {
			if ( _maxSpeed ) delete _maxSpeed;
			if ( _maxAccel ) delete _maxAccel;
			if ( _prefSpeed ) delete _prefSpeed;
			if ( _maxNeighbors ) delete _maxNeighbors;
			if ( _neighborDist ) delete _neighborDist;
			if ( _radius ) delete _radius;
			if ( _maxAngVel ) delete _maxAngVel;

			// Values for distribution
			_maxSpeed = new ConstFloatGenerator( MAX_SPEED );
			_maxAccel = new ConstFloatGenerator( MAX_ACCEL );
			_prefSpeed = new ConstFloatGenerator( PREF_SPEED );
			_maxNeighbors = new ConstIntGenerator( MAX_NEIGHBORS );
			_neighborDist = new ConstFloatGenerator( NEIGHBOR_DIST );
			_radius = new ConstFloatGenerator( RADIUS );
			_maxAngVel = new ConstFloatGenerator( MAX_ANGLE_VEL );
			// single values
			_priority = PRIORITY;
			_obstacleSet = OBSTACLE_SET;
			_class = CLASS;
		}

		////////////////////////////////////////////////////////////////
		
		bool AgentInitializer::parseProperties( TiXmlElement * node, const std::string & sceneFldr) {
			//first let's decide if this is a velocity modifier
			if (node->ValueStr() == "VelModifier"){
				//this is, we need to find out if we can parse it
				BFSM::VelModifier *vel = BFSM::parseVelModifier( node, sceneFldr);
				if ( vel == 0x0 ) {
					return false;
				} else {
					_velModifiers.push_back(vel);
				}
			} else if ( isRelevant( node->ValueStr() ) ) {
				// Extract the attributes of the tag
				TiXmlAttribute * attr;
				for ( attr = node->FirstAttribute(); attr; attr = attr->Next() ) {
					ParseResult result = setFromXMLAttribute( attr->Name(), attr->ValueStr() );
					if ( result == FAILURE ) {
						return false;
					} else if ( result == IGNORED ) {
						logger << Logger::WARN_MSG << "Encountered an unexpected per-agent attribute (" << attr->Name() << ") on line " << attr->Row() << ".";
					}
				}
				// Now look for advanced property specifications
				TiXmlElement* child;
				for( child = node->FirstChildElement(); child; child = child->NextSiblingElement()) {
					if ( ! parsePropertySpec( child ) ) {

						return false;
					}
				}
			}
			// Irrelevant nodes are, by definition, successful.
			return true;
		}

		////////////////////////////////////////////////////////////////
		
		bool AgentInitializer::setProperties( BaseAgent * agent ) {
			agent->_maxSpeed = _maxSpeed->getValue();
			agent->_maxAccel = _maxAccel->getValue();
			agent->_prefSpeed = _prefSpeed->getValue();
			agent->_maxNeighbors = _maxNeighbors->getValue();
			agent->_neighborDist = _neighborDist->getValue();
			agent->_radius = _radius->getValue();
			agent->_maxAngVel = _maxAngVel->getValue();
			agent->_obstacleSet = _obstacleSet;
			agent->_priority = _priority;
			agent->_class = _class;
			
			std::vector< BFSM::VelModifier * >::iterator vItr = _velModifiers.begin();
			for ( ; vItr != _velModifiers.end(); ++vItr ) {
				BFSM::VelModifier *newVel = (*vItr)->copy(); 
				agent->addVelModifier(newVel);
				newVel->registerAgent(agent);
			}
			return true;
		}
			
		////////////////////////////////////////////////////////////////
		
		bool AgentInitializer::isRelevant( const ::std::string & tagName ) {
			return tagName == "Common";
		}

		////////////////////////////////////////////////////////////////
		
		AgentInitializer::ParseResult AgentInitializer::setFromXMLAttribute( const ::std::string & paramName, const ::std::string & value ) {
			ParseResult result = IGNORED;

			if ( paramName == "neighbor_dist" ) {
				result = constFloatGenerator( _neighborDist, value );
			} else if ( paramName == "pref_speed" ) {
				result = constFloatGenerator( _prefSpeed, value );
			} else if ( paramName == "max_speed" ) {
				result = constFloatGenerator( _maxSpeed, value );
			} else if ( paramName == "max_accel" ) {
				result = constFloatGenerator( _maxAccel, value );
			} else if ( paramName == "max_neighbors" ) {
				result = constIntGenerator( _maxNeighbors, value );
			} else if ( paramName == "r" ) {
				result = constFloatGenerator( _radius, value );
			} else if ( paramName == "max_angle_vel" ) {
				result = constFloatGenerator( _maxAngVel, value, DEG_TO_RAD );
			} else if ( paramName == "obstacleSet" ) {
				result = constSizet( _obstacleSet, value );
			} else if ( paramName == "class" ) {
				result = constSizet( _class, value );
			} else if ( paramName == "priority" ) {
				result = constFloat( _priority, value );
			}

			if ( result == FAILURE ) {
				// Found an expected parameter name, but got a bad value.  That is failure
				logger << Logger::WARN_MSG << "Attribute " << paramName << " had an incorrectly formed value: " << value << ".  Using default value.";
			}

			return result;
		}

		////////////////////////////////////////////////////////////////
		
		bool AgentInitializer::parsePropertySpec( TiXmlElement * node ) {
			if ( node->ValueStr() == "Property" ) {
				const char * cName = node->Attribute( "name" );
				if ( cName == 0x0 ) {
					logger << Logger::ERR_MSG << "AgentSet Property tag specified on line " << node->Row() << " without a \"name\" attribute.";
					return false;
				}
				::std::string propName( cName );
				return processProperty( propName, node ) != FAILURE;
			} else if ( VERBOSE ) {
				logger << Logger::WARN_MSG << "Unexpected tag when looking for a property of an AgentSet parameter set: " << node->ValueStr() << "\n";
				TiXmlAttribute * attr;
				for ( attr = node->FirstAttribute(); attr; attr = attr->Next() ) {
					if ( setFromXMLAttribute( attr->Name(), attr->ValueStr() ) == FAILURE ) {
						return false;
					}
				}
			}
			// Unexpected tags are ignored
			return true;
		}

		////////////////////////////////////////////////////////////////
		
		AgentInitializer::ParseResult AgentInitializer::processProperty( ::std::string propName, TiXmlElement * node ) {
			ParseResult result = IGNORED;
			if ( propName == "neighbor_dist" ) {
				result = getFloatGenerator( _neighborDist, node );
			} else if ( propName == "pref_speed" ) {
				result = getFloatGenerator( _prefSpeed, node );
			} else if ( propName == "max_speed" ) {
				result = getFloatGenerator( _maxSpeed, node );
			} else if ( propName == "max_accel" ) {
				result = getFloatGenerator( _maxAccel, node );
			} else if ( propName == "max_neighbors" ) {
				result = getIntGenerator( _maxNeighbors, node );
			} else if ( propName == "r" ) {
				result = getFloatGenerator( _radius, node );
			} else if ( propName == "max_angle_vel" ) {
				result = getFloatGenerator( _maxAngVel, node, DEG_TO_RAD );
			}
			if ( result == FAILURE ) {
				logger << Logger::ERR_MSG << "Error extracting value distribution from Property " << propName << ".";
				return result;
			} else if ( result == IGNORED ) {
				logger << Logger::WARN_MSG << "AgentSet Property had unexpected name: " << propName << ".  Ignored.\n";
			}
			return result;
		}

		////////////////////////////////////////////////////////////////
		
		AgentInitializer::ParseResult AgentInitializer::constFloatGenerator( FloatGenerator * & gen, const ::std::string & valueStr, float scale ) {
			try {
				float f = toFloat( valueStr );
				if ( gen ) delete gen;
				gen = new ConstFloatGenerator( f * scale );
				return ACCEPTED;
			} catch ( UtilException ) {
				return FAILURE;
			}
		}

		////////////////////////////////////////////////////////////////
		
		AgentInitializer::ParseResult AgentInitializer::constFloat( float & numValue, const ::std::string & valueStr, float scale ) {
			try {
				float f = toFloat( valueStr );
				numValue = f * scale;
				return ACCEPTED;
			} catch ( UtilException ) {
				return FAILURE;
			}
		}

		////////////////////////////////////////////////////////////////
		
		AgentInitializer::ParseResult AgentInitializer::constIntGenerator( IntGenerator * & gen, const ::std::string & valueStr ) {
			try {
				int i = toInt( valueStr );
				if ( gen ) delete gen;
				gen = new ConstIntGenerator( i );
				return ACCEPTED;
			} catch ( UtilException ) {
				return FAILURE;
			}
		}

		////////////////////////////////////////////////////////////////
		
		AgentInitializer::ParseResult AgentInitializer::constSizet( size_t & numValue, const ::std::string & valueStr ) {
			try {
				size_t i = toSize_t( valueStr );
				numValue = i;
				return ACCEPTED;
			} catch ( UtilException ) {
				return FAILURE;
			}
		}

		////////////////////////////////////////////////////////////////
		
		AgentInitializer::ParseResult AgentInitializer::getFloatGenerator( FloatGenerator * & gen, TiXmlElement * node, float scale ) {
			FloatGenerator * newGen = createFloatGenerator( node, scale );
			if ( newGen ) {
				if ( gen ) delete gen;
				gen = newGen;
				return ACCEPTED;
			} else {
				return FAILURE;
			}
		}

		////////////////////////////////////////////////////////////////
		
		AgentInitializer::ParseResult AgentInitializer::getIntGenerator( IntGenerator * & gen, TiXmlElement * node ) {
			IntGenerator * newGen = createIntGenerator( node );
			if ( newGen ) {
				if ( gen ) delete gen;
				gen = newGen;
				return ACCEPTED;
			} else {
				return FAILURE;
			}
		}
	}	// namepsace Agents
}	// namespace Menge