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

#include "ObstacleSets/NavMeshObstacleSet.h"
#include "tinyxml.h"
#include <vector>
#include "os.h"

namespace Menge {

	namespace Agents {
		
		////////////////////////////////////////////////////////////////////////////
		//			Implementation of NavMeshObstacleSet
		////////////////////////////////////////////////////////////////////////////

		NavMeshObstacleSet::NavMeshObstacleSet() : ListObstacleSet() {

		}

		////////////////////////////////////////////////////////////////////////////

		bool NavMeshObstacleSet::extractFromNavMesh(NavMeshPtr nm) {
			std::vector<ObstacleVertexList> obstacles = nm->getObstacles();
			std::vector<ObstacleVertexList>::iterator vItr = obstacles.begin();
			for (;vItr != obstacles.end(); ++vItr){
				addObstacle(*vItr);
			}

			return true;
		};

		////////////////////////////////////////////////////////////////////////////
		//			Implementation of ExplicitObstacleSetFactory
		////////////////////////////////////////////////////////////////////////////

		NavMeshObstacleSetFactory::NavMeshObstacleSetFactory() : ObstacleSetFactory() {
			_fileNameID = _attrSet.addStringAttribute( "file_name", true /*required*/ );
		}
		
		////////////////////////////////////////////////////////////////////////////

		bool NavMeshObstacleSetFactory::setFromXML( ObstacleSet * gen, TiXmlElement * node, const std::string & specFldr ) const {
			NavMeshObstacleSet * eSet = dynamic_cast< NavMeshObstacleSet * >( gen );
			assert( eSet != 0x0 && "Trying to set attributes of an explicit obstacle set on an incompatible object" );

			if ( ! ObstacleSetFactory::setFromXML( eSet, node, specFldr ) ) return false;

			//get the absolute path for the file name
			std::string fName;
			std::string path = os::path::join( 2, specFldr.c_str(), _attrSet.getString( _fileNameID ).c_str() );
			os::path::absPath( path, fName );
			// nav mesh
			NavMeshPtr nmPtr;
			try {
				nmPtr = loadNavMesh( fName );
			} catch ( ResourceException ) {
				logger << Logger::ERR_MSG << "Couldn't instantiate the navigation mesh referenced on line " << node->Row() << ".";
				return false;
			}

			return eSet->extractFromNavMesh(nmPtr);
		}

	}	// namespace Agents
}	// namespace Menge
