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

#include "SimXMLLoader.h"
#include "XMLSimulatorBase.h"
#include <iostream>
#include <list>
#include <vector>
#include "BaseAgent.h"
#include "SimulatorState.h"
#include "Elevations/ElevationDatabase.h"
#include "SpatialQueries/SpatialQueryDatabase.h"
#include "AgentGenerators/AgentGeneratorDatabase.h"
#include "ProfileSelectors/ProfileSelectorDatabase.h"
#include "StateSelectors/StateSelectorDatabase.h"
#include "ObstacleSets/ObstacleSetDatabase.h"
#include "AgentInitializer.h"
#include "os.h"
#include "Core.h"

namespace Menge {

	namespace Agents {

		////////////////////////////////////////////////////////////////////
		//			Implementation of SimXMLLoader
		////////////////////////////////////////////////////////////////////

		SimXMLLoader::SimXMLLoader( XMLSimulatorBase * sim ):_sceneFldr("."),_sim(sim), _agtCount(0) {
		}

		////////////////////////////////////////////////////////////////////

		bool SimXMLLoader::loadFromXML( const std::string & filename, AgentInitializer * agentInit, bool verbose ) {
			// COnfirms file is
			//	a) available for reading
			//	b) valid xml
			//	c) An "Experiment"
			if ( verbose ) logger << Logger::INFO_MSG << "Loading from xml: " << filename << ".";
			TiXmlDocument xml( filename );
			bool loadOkay = xml.LoadFile();

			if ( !loadOkay ) {	// load xml file
				logger << Logger::ERR_MSG << "Could not load simulation configuration xml (" << filename << ") due to xml syntax errors.\n";
				logger << "\t" << xml.ErrorDesc();
				return false;
			}

			TiXmlElement* experimentNode = xml.RootElement();	
			if( ! experimentNode ) {
				logger << Logger::ERR_MSG << "Scene configuration (" << filename << ") does not contain a root element.";
				return false;
			}

			if( experimentNode->ValueStr () != "Experiment" ) {
				logger << Logger::ERR_MSG << "Scene configuration (" << filename << ")'s root element is not \"Experiment\".";
				return false;
			}

			std::string absPath;
			os::path::absPath( filename, absPath );
			std::string junk;
			os::path::split( absPath, _sceneFldr, junk );
			logger << Logger::INFO_MSG << "Scene root: " << _sceneFldr << ".";		

			bool commonDone = false;	// common experiment parameters parsed
			bool targetDone = !_sim->hasExpTarget();	// target experiment parameters parsed
			bool spatialQueryDone = false;	// spatial query must be finished before obstacles and agents can be created

			// Tags I'm not ready to parse - only parse agent sets and obstacles AFTER experiment
			//	parameters
			std::list< TiXmlElement * > tagQueue;
			
			TiXmlElement* child;
			for( child = experimentNode->FirstChildElement(); child; child = child->NextSiblingElement()) {
				if ( child->ValueStr() == "Common" ) {
					// Currently the only "common" experiment parameter is the time step
					TiXmlAttribute * attr;
					for ( attr = child->FirstAttribute(); attr; attr = attr->Next() ) {
						try {
							if ( !_sim->setExpParam( attr->Name(), attr->ValueStr() ) ) {
								logger << Logger::WARN_MSG << "Unrecognized parameter in the global \"Common\" parameters (" << attr->Name() << ") on line " << child->Row() << "\n";
							}
						} catch ( XMLParamException e ) {
							logger << Logger::ERR_MSG << e.what();
							return false;
						}
					}
					commonDone = true;
				} else if ( child->ValueStr() == "AgentProfile" ) {
					if ( !parseAgentProfile( child, agentInit ) ) {
						return false;
					}
				} else if ( child->ValueStr() == "AgentGroup" ) {
					if ( ! ( commonDone || targetDone || spatialQueryDone ) ) {
						tagQueue.push_back( child );
					} else {
						if ( !parseAgentGroup( child, agentInit ) ) {
							return false;
						}
					}
				} else if ( child->ValueStr() == "ObstacleSet" ) {
					if ( ! ( commonDone || targetDone || spatialQueryDone) ) {
						tagQueue.push_back( child );
					} else {
						if ( ! parseObstacleSet( child ) ) {
							return false;
						}
					}
				} else if ( child->ValueStr() == "Elevation" ) {
					if ( _sim->hasElevation() ) {
						logger << Logger::ERR_MSG << "More than one elevation has been specified.  Found redundant elevation specification on line " << child->Row() << ".";
						return false;
					}
					Elevation * elevation = ElevationDB::getInstance( child, _sceneFldr );
					if ( elevation == 0x0 ) {
						logger << Logger::ERR_MSG << "Unable to instantiate elevation specifcation on line " << child->Row() << ".";
						return false;
					} else {
						_sim->setElevationInstance( elevation );
					}
					Menge::ELEVATION = elevation;
				} else if ( child->ValueStr() == "SpatialQuery" ) {
					if ( _sim->hasSpatialQuery() ) {
						logger << Logger::ERR_MSG << "More than one spatial query implementation has been specified.  Found redundant spatial query specification on line " << child->Row() << ".";
						return false;
					}
					SpatialQuery * spQuery = SpatialQueryDB::getInstance( child, _sceneFldr );
					if ( spQuery == 0x0 ) {
						logger << Logger::ERR_MSG << "Unable to instantiate spatial query specifcation on line " << child->Row() << ".";
						return false;
					} else {
						_sim->setSpatialQuery( spQuery );
						spatialQueryDone = true;
					}
				} else {	// target parameter 
					if ( !targetDone && _sim->isExpTarget( child->ValueStr() ) ) {
						// Parse the target
						TiXmlAttribute * attr;
						for ( attr = child->FirstAttribute(); attr; attr = attr->Next() ) {
							try {
								if ( ! _sim->setExpParam( attr->Name(), attr->ValueStr() ) ) {
									logger << Logger::WARN_MSG << "Unrecognized parameter in the global \"" << child->ValueStr() << "\" parameters (" << attr->Name() << ") on line " << child->Row() << "\n";
								}
							} catch ( XMLParamException e ) {
								logger << Logger::ERR_MSG << e.what() << " (on line " << child->Row() << ")";
								return false;
							}
						}
						targetDone = true;
					}
				} 
			}
			if ( !targetDone || !commonDone || !spatialQueryDone) {
				logger << Logger::ERR_MSG << "Missing required experiment parameters: \n";
				if ( !targetDone ) logger << "\tmodel simulation parameters ";
				if ( !commonDone ) logger << "\tcommon simulation parameters ";
				if ( !spatialQueryDone ) logger << "\tSpatial Query ";
				return false;
			}
			// Now parse any of the tags that were skipped while waiting for experiment configuration
			std::list< TiXmlElement * >::iterator tagItr = tagQueue.begin();
			for ( ; tagItr != tagQueue.end(); ++tagItr ) {
				TiXmlElement * child = *tagItr;
				if ( child->ValueStr() == "AgentGroup" ) {
					if ( !parseAgentGroup( child, agentInit ) ) {
						return false;
					}
				} else if ( child->ValueStr() == "ObstacleSet" ) {
					if ( ! parseObstacleSet( child ) ) {
							return false;
						}
				} else {
					logger << Logger::ERR_MSG << "XML contains an invalid tag: " << child->ValueStr() << " on line " << child->Row() << ".";
					return false;
				}
			}

			if ( _agtCount == 0 ) {
				// TODO: Change this test when agent sources are introduced
				//	in this case, it is possible to start with no agents and then add them
				//	w.r.t. time.
				logger << Logger::ERR_MSG << "No agents defined in simulation.";
				return false;
			}

			// free up the profiles
			//	TODO: I'll need to save these when I have AgentSources.
			for ( HASH_MAP< std::string, AgentInitializer * >::iterator itr = _profiles.begin();
				itr != _profiles.end();
				++itr ) 
			{
				delete itr->second;
			}
			_profiles.clear();

			return _sim->initSpatialQuery();
		}

		////////////////////////////////////////////////////////////////////

		bool SimXMLLoader::parseAgentGroup( TiXmlElement * node, AgentInitializer * agentInit ) {
			// 2-pass approach
			// Pass 1 get the profile selector
			// Pass 2 initialize AgentGenerator (Generator for short)

			// Get the profile selector - 
			TiXmlElement* child;
			ProfileSelector * profileSel = 0x0;
			StateSelector * stateSel = 0x0;
			// First pass, skip Agents
			for( child = node->FirstChildElement(); child; child = child->NextSiblingElement()) {
				if ( child->ValueStr() == "ProfileSelector" ) {
					if ( profileSel != 0x0 ) {
						// There should be only one.  If there are multiple, only the first will have an effect.
						logger << Logger::WARN_MSG << "Found multiple ProfileSelector tags in the AgentGroup on line " << node->Row() << ".  Only the first will be used.";
						continue;
					}
					profileSel = ProfileSelectorDB::getInstance( child, _sceneFldr );
					if ( profileSel == 0x0 ) {
						logger << Logger::ERR_MSG << "Unable to instantiate the profile selector specification line " << child->Row() << ".";
						return false;
					}
					if ( ! profileSel->cacheProfiles( _profiles ) ) {
						logger << Logger::ERR_MSG << "ProfileSelector on line " << child->Row() << " was unable to find a named profile.";
						return false;
					}
				} else if ( child->ValueStr() == "StateSelector" ) {
					if ( stateSel != 0x0 ) {
						// There should be only one.  If there are multiple, only the first will have an effect.
						logger << Logger::WARN_MSG << "Found multiple StateSelector tags in the AgentGroup on line " << node->Row() << ".  Only the first will be used.";
						continue;
					}
					stateSel = StateSelectorDB::getInstance( child, _sceneFldr );
					if ( stateSel == 0x0 ) {
						logger << Logger::ERR_MSG << "Unable to instantiate the state selector specification line " << child->Row() << ".";
						return false;
					}
				}
			}
			if ( profileSel == 0x0 ) {
				logger << Logger::ERR_MSG << "No profile selector defined for the AgentGroup on line " << node->Row() << ".";
				return false;
			}
			if ( stateSel == 0x0 ) {
				logger << Logger::ERR_MSG << "No state selector defined for the AgentGroup on line " << node->Row() << ".";
				return false;
			}

			// Second pass, parse Generators
			for( child = node->FirstChildElement(); child; child = child->NextSiblingElement()) {
				if ( child->ValueStr() == "Generator" ) {
					AgentGenerator * generator = AgentGeneratorDB::getInstance( child, _sceneFldr );

					if ( generator == 0x0 ) {
						logger << Logger::ERR_MSG << "Unable to instantiate agent generator specifcation on line " << child->Row() << ".";
						return false;
					}
					// Now instantiate the agents
					const size_t AGT_COUNT = generator->agentCount();
					for ( size_t i = 0; i < AGT_COUNT; ++i ) {
						Vector2 p = generator->agentPos( i );
						BaseAgent * agent = _sim->addAgent( p, profileSel->getProfile() );
						_sim->getInitialState()->setAgentState( agent->_id, stateSel->getState() );
					}
					_agtCount += (unsigned int) AGT_COUNT;

					generator->destroy();
				}
			}
			
			return true;
		}

		
		////////////////////////////////////////////////////////////////////

		bool SimXMLLoader::parseObstacleSet( TiXmlElement * node) {
			//pass through, try to get a generator, and then use it
			ObstacleSet * obSet = ObstacleSetDB::getInstance( node, _sceneFldr );
			if ( obSet == 0x0 ) {
				logger << Logger::ERR_MSG << "Unable to instantiate obstacle set specifcation on line " << node->Row() << ".";
				return false;
			} else {
				//_sim->setSpatialQuery( spQuery );
				size_t index = 0;
				for (;index < obSet->obstacleCount();++index){
					_sim->getSpatialQuery()->addObstacle(obSet->getObstacle(index));
				}

				obSet->destroy();
				return true;
			}
		}

		////////////////////////////////////////////////////////////////////

		bool SimXMLLoader::parseAgentProfile( TiXmlElement * node, AgentInitializer * agentInit ) {
			// Extract the name
			const char * nameCStr = node->Attribute( "name" );
			if ( nameCStr == 0x0 ) {
				logger << Logger::ERR_MSG << "The AgentProfile defined on line " << node->Row() << " is missing the required \"name\" attribute.";
				return false;
			}
			std::string name( nameCStr );
			if ( _profiles.find( name ) != _profiles.end() ) {
				logger << Logger::ERR_MSG << "The AgentProfile defined on line " << node->Row() << " has a name value (\"" << name << "\")that has previously been used.";
				return false;
			}

			AgentInitializer * init;
			// test inheritance
			const char * parentCStr = node->Attribute( "inherits" );
			if ( parentCStr ) {
				std::string pName( parentCStr );
				HASH_MAP< std::string, AgentInitializer * >::iterator itr = _profiles.find( pName );
				if ( itr == _profiles.end() ) {
					logger << Logger::ERR_MSG << "The AgentProfile on line " << node->Row() << " inherits from the undefined AgentProfile \"" << pName << "\".  Make sure the parent profile is defined <i>before</i> the child profile.";
					return false;
				} else {
					init = itr->second->copy();
				}
			} else {
				init = agentInit->copy();
				init->setDefaults();
			}
			_profiles[ name ] = init;

			for( TiXmlElement * child = node->FirstChildElement(); child; child = child->NextSiblingElement()) {
				if ( ! init->parseProperties( child, _sceneFldr) ) {
					logger << Logger::ERR_MSG << "Error parsing AgentProfile properties from line " << child->Row() << ".";
					return false;
				}
			}
			return true;
		}
	}	// namespace Agents
}	// namespace Menge