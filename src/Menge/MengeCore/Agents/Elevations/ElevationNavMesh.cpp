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

#include "Elevations/ElevationNavMesh.h"
#include "Tasks/NavMeshLocalizerTask.h"
#include "BaseAgent.h"
#include "tinyxml.h"
#include "os.h"

namespace Menge {

	namespace Agents {

		////////////////////////////////////////////////////////////////
		//					Implementation of NavMeshElevation
		////////////////////////////////////////////////////////////////

		NavMeshElevation::NavMeshElevation(): Elevation(), _navMesh(0x0), _localizer(0x0) {
		}

		////////////////////////////////////////////////////////////////

		float NavMeshElevation::getElevation( const Vector2 & point ) const {
			unsigned int nodeID = _localizer->getNode( point );
			if ( nodeID == NavMeshLocation::NO_NODE ) {
				return 0.f;
			} else {
				return _navMesh->getElevation( nodeID, point );
			}
		}

		////////////////////////////////////////////////////////////////

		float NavMeshElevation::getElevation( const BaseAgent * agent ) const {
			unsigned int nodeID = _localizer->getNode( agent );
			if ( nodeID == NavMeshLocation::NO_NODE ) {
				return 0.f;
			} else {
				return _navMesh->getElevation( nodeID, agent->_pos );
			}
		}

		////////////////////////////////////////////////////////////////

		Vector2 NavMeshElevation::getGradient( const BaseAgent * agent ) const {
			unsigned int nodeID = _localizer->getNode( agent );
			if ( nodeID == NavMeshLocation::NO_NODE ) {
				return Vector2( 0.f, 0.f );
			} else {
				return _navMesh->getGradient( nodeID, agent->_pos );
			}
		}

		////////////////////////////////////////////////////////////////

		Vector2 NavMeshElevation::getGradient( const Vector2 & point ) const {
			unsigned int nodeID = _localizer->getNode( point );
			if ( nodeID == NavMeshLocation::NO_NODE ) {
				return Vector2( 0.f, 0.f );
			} else {
				return _navMesh->getGradient( nodeID, point );
			}
		}

		/////////////////////////////////////////////////////////////////////
		
		BFSM::Task * NavMeshElevation::getTask() {
			return new BFSM::NavMeshLocalizerTask( _navMesh->getName(), false /*usePlanner*/ );
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of NavMeshVCFactory
		/////////////////////////////////////////////////////////////////////

		NavMeshElevationFactory::NavMeshElevationFactory() : ElevationFactory() {
			_fileNameID = _attrSet.addStringAttribute( "file_name", true /*required*/ );
		}

		/////////////////////////////////////////////////////////////////////

		bool NavMeshElevationFactory::setFromXML( Elevation * e, TiXmlElement * node, const std::string & specFldr ) const {
			NavMeshElevation * nme = dynamic_cast< NavMeshElevation * >( e );
			assert( nme != 0x0 && "Trying to set attributes of a navigation mesh elevation component on an incompatible object" );

			if ( ! ElevationFactory::setFromXML( nme, node, specFldr ) ) return false;

			// get the file name
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
			nme->setNavMesh( nmPtr );
			// nav mesh localizer
			NavMeshLocalizerPtr nmlPtr;
			try {
				nmlPtr = loadNavMeshLocalizer( fName, true );
			} catch ( ResourceException ) {
				logger << Logger::ERR_MSG << "Couldn't instantiate the navigation mesh localizer required by the elevation on line " << node->Row() << ".";
				return false;
			}
			nme->setNavMeshLocalizer( nmlPtr );

			return true;
		}
		
	}	// namespace Agents
}	// namespace Menge
