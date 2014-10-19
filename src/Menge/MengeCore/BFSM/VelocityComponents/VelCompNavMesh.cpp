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

#include "VelocityComponents/VelCompNavMesh.h"
#include "Tasks/NavMeshLocalizerTask.h"
#include "NavMeshNode.h"
#include "NavMeshEdge.h"
#include "BaseAgent.h"
#include "os.h"
#include "Logger.h"
#include "PathPlanner.h"
#include "PortalPath.h"
#include "Route.h"
#include "Goals/Goal.h"

#include <sstream>
#include <iomanip>

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of NavMeshVelComponent
		/////////////////////////////////////////////////////////////////////

		NavMeshVelComponent::NavMeshVelComponent():VelComponent(),_headingDevCos(-1.f),_navMesh(0x0),_localizer(0x0) {
		}

		/////////////////////////////////////////////////////////////////////

		void NavMeshVelComponent::onExit( Agents::BaseAgent * agent ) {
			_localizer->clearPath( agent->_id );
		}

		/////////////////////////////////////////////////////////////////////

		void NavMeshVelComponent::setHeadingDeviation( float angle ) {
			_headingDevCos = cos( angle );
		}

		/////////////////////////////////////////////////////////////////////

		void NavMeshVelComponent::setPrefVelocity( const Agents::BaseAgent * agent, const Goal * goal, Agents::PrefVelocity & pVel ) {
			// If agent does not have a path
			PortalPath * path = _localizer->getPath( agent->_id );
			if ( path == 0x0 ) {
				// Get the route
				Vector2 goalPoint = goal->getCentroid();
				unsigned int goalNode = _localizer->getNode( goalPoint );
				if ( goalNode == NavMeshLocation::NO_NODE ) {
					throw VelCompFatalException( "Can't compute a path to a goal outside of the navigation mesh.  Bad NavMeshVelComponent!" );
				}
				unsigned int agtNode = _localizer->getNode( agent );
				PortalRoute * route = _localizer->getPlanner()->getRoute( agtNode, goalNode, agent->_radius * 2.f );
				// compute the path
				path = new PortalPath( agent->_pos, goal, route, agent->_radius );
				// assign it to the localizer
				_localizer->setPath( agent->_id, path );
			}
			pVel.setSpeed( agent->_prefSpeed );
			path->setPreferredDirection( agent, _headingDevCos, pVel );
		}

		/////////////////////////////////////////////////////////////////////
		
		VelCompContext * NavMeshVelComponent::getContext() {
			return new NavMeshVCContext( this );
		}

		/////////////////////////////////////////////////////////////////////
		
		BFSM::Task * NavMeshVelComponent::getTask() {
			return new NavMeshLocalizerTask( _navMesh->getName(), true /*usePlanner*/ );
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of NavMeshVCContext
		/////////////////////////////////////////////////////////////////////

		NavMeshVCContext::NavMeshVCContext( NavMeshVelComponent * vc ):VelCompContext(),_vc(vc),_drawCenters(false),_drawNodeIDs(false) {
		}

		/////////////////////////////////////////////////////////////////////

		std::string NavMeshVCContext::getUIText( const std::string & indent ) const {
			std::stringstream ss;
			ss << indent << "Navigation mesh velocity component";
			ss << std::setiosflags(std::ios::fixed) <<  std::setprecision( 2 );
			const NavMeshPtr navMesh = _vc->_navMesh;
			ss << "\n" << indent << "    Num. vertices: " << navMesh->getVertexCount();
			ss << "\n" << indent << "    Num. polygons: " << navMesh->getNodeCount();
			ss << "\n" << indent << "    Num. edges: " << navMesh->getEdgeCount();
			if ( _drawCenters ) {
				ss << "\n" << indent << "    Hide polygon (Ctrl-c)enters";
			} else {
				ss << "\n" << indent << "    Draw polygon (Ctrl-c)enters";
			}
			if ( _drawNodeIDs ) {
				ss << "\n" << indent << "    Hide polygon (Ctrl-i)ds";
			} else {
				ss << "\n" << indent << "    Draw polygon (Ctrl-i)ds";
			}
			return ss.str();
		}

		/////////////////////////////////////////////////////////////////////

		SceneGraph::ContextResult NavMeshVCContext::handleKeyboard( SDL_Event & e ) { 
			SceneGraph::ContextResult result( false, false ); 

			SDLMod mods = e.key.keysym.mod;
			bool hasCtrl = ( mods & KMOD_CTRL ) > 0;
			bool hasAlt = ( mods & KMOD_ALT ) > 0;
			bool hasShift = ( mods & KMOD_SHIFT ) > 0;
			bool noMods = !(hasCtrl || hasAlt || hasShift );

			if ( e.type == SDL_KEYDOWN ) {
				if ( hasCtrl && !hasAlt && !hasShift ) {
					if ( e.key.keysym.sym == SDLK_c ) {
						_drawCenters = !_drawCenters;
						result.set( true, true );
					} else if ( e.key.keysym.sym == SDLK_i ) {
						_drawNodeIDs = ! _drawNodeIDs;
						result.set( true, true );
					}
				}
			}

			return result;
		} 

		/////////////////////////////////////////////////////////////////////

		void NavMeshVCContext::draw3DGL( const Agents::BaseAgent * agt, const Goal * goal ) {
			getOpenGLView();
			drawNavMesh();
			drawPath( agt, goal );

			// draw the preferred velocity
			NavMeshPtr navMesh = _vc->getLocalizer()->getNavMesh();
			unsigned int NODE_ID = _vc->getLocalizer()->getNode( agt );
			float elevation = navMesh->getElevation( NODE_ID, agt->_pos );
			Agents::PrefVelocity pVel;
			_vc->setPrefVelocity( agt, goal, pVel );
			drawPrefVel( pVel, agt->_pos, elevation );
		}

		////////////////////////////////////////////////////////////////////////////

		void NavMeshVCContext::drawNavMesh() const {
			const NavMeshPtr navMesh = _vc->_navMesh;
			unsigned int nCount = static_cast< unsigned int >( navMesh->getNodeCount() );

			const Vector2 * vertices = navMesh->getVertices();
			glPushAttrib( GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT | GL_POINT_BIT | GL_DEPTH_BUFFER_BIT );

			// shaded regions
			glDepthMask( GL_FALSE );
			glEnable( GL_BLEND );
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f( 0.5f, 0.8f, 1.f, 0.25f );
			for ( unsigned int n = 0; n < nCount; ++n ) {
				const NavMeshNode & node = navMesh->getNode( n );
				size_t vCount = node.getVertexCount();
				glBegin( GL_POLYGON );
				for ( size_t v = 0; v < vCount; ++ v ) {
					unsigned int vID = node.getVertexID( v );
					const Vector2 & p = vertices[ vID ];
					float elevation = node.getElevation( p );
					glVertex3f( p.x(), elevation, p.y() );
				}
				glEnd();
			}

			// outlines
			glDisable( GL_BLEND );
			// edges
			unsigned int eCount = static_cast< unsigned int >( navMesh->getEdgeCount() );
			glBegin( GL_LINES );
			for ( unsigned int e = 0; e < eCount; ++e ) {
				const NavMeshEdge & edge = navMesh->getEdge( e );
				const NavMeshNode * node = edge.getFirstNode();
				Vector2 p0 = edge.getP0();
				float h = node->getElevation( p0 );
				glVertex3f( p0.x(), h, p0.y() );
				Vector2 p1 = edge.getP1();
				h = node->getElevation( p1 );
				glVertex3f( p1.x(), h, p1.y() );
			}
			glEnd();


			if ( _drawCenters ) {
				//centers
				glPushAttrib( GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT | GL_POINT_BIT );
				glDepthMask( GL_FALSE );
				glDisable( GL_DEPTH_TEST );
				glPointSize( 5.f );
				glBegin( GL_POINTS );
				for ( unsigned int n = 0; n < nCount; ++n ) {
					const NavMeshNode & node = navMesh->getNode( n );
					Vector2 p = node.getCenter();
					float elevation = node.getElevation( p );
					glVertex3f( p.x(), elevation, p.y() );
				}
				glEnd();
				glPopAttrib();
			}

			if ( _drawNodeIDs ) {
				drawNodeIDs();
			}

			// obstacles
			glColor3f( 1.f, 0.1f, 0.1f );
			unsigned int oCount = static_cast< unsigned int >( navMesh->getObstacleCount() );
			glBegin( GL_LINES );
			for ( unsigned int o = 0; o < oCount; ++o ) {
				const NavMeshObstacle & obst = navMesh->getObstacle( o );
				const NavMeshNode * node = obst.getNode();
				Vector2 p0 = obst.getP0();
				float h = node->getElevation( p0 );
				glVertex3f( p0.x(), h, p0.y() );
				Vector2 p1 = obst.getP1();
				h = node->getElevation( p1 );
				glVertex3f( p1.x(), h, p1.y() );
			}
			glEnd(); 

			glPopAttrib();
		}

		////////////////////////////////////////////////////////////////////////////

		void NavMeshVCContext::drawPath( const Agents::BaseAgent * agt, const Goal * goal ) const {

			const size_t ID = agt->_id;
			PortalPath * path = _vc->getLocalizer()->getPath( ID );
			if ( path ) {
				NavMeshPtr navMesh = _vc->getNavMesh();
				unsigned int NODE_ID = _vc->getLocalizer()->getNode( agt );
				Vector2 prev( agt->_pos );
				float height = navMesh->getElevation( NODE_ID, prev );
				glPushAttrib( GL_LINE_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT );
				glDepthMask( GL_FALSE );
				
				glColor3f( 1.f, 1.f, 0.f );
				glLineWidth( 3.f );
				glBegin( GL_LINE_STRIP );
				glVertex3f( prev.x(), height, prev.y() );
				// iterate through the rest
				const size_t P_COUNT = path->getWayPointCount();
				size_t i = path->getCurrentPortal();
				if ( i < P_COUNT ) {
					NODE_ID = path->getNode( i );
					prev.set( path->getWayPoint( i ) );
					height = navMesh->getElevation( NODE_ID, prev );
					++i;
				}
				Vector2 p;
				for ( ; i < P_COUNT; ++i ) {
					NODE_ID = path->getNode( i );
					p.set( path->getWayPoint( i ) );
					float h = navMesh->getElevation( NODE_ID, p );
					if ( absSq( p - prev ) > 0.001f ) {
						glVertex3f( prev.x(), height, prev.y() );
					}
					prev.set( p );
					height = h;
				}
				glVertex3f( prev.x(), height, prev.y() );

				const BFSM::Goal * goal = path->getGoal();
				Vector2 goalPoint = goal->getTargetPoint( prev, agt->_radius );
				height = navMesh->getElevation( path->getEndNode(), goalPoint );
				glVertex3f( goalPoint.x(), height, goalPoint.y() );

				glEnd();
				glPopAttrib();	

				// draw goal
				Vector3 goal3D( goalPoint.x(), height, goalPoint.y() );
				drawGoal( goal3D, agt->_radius );
			}
		}

		////////////////////////////////////////////////////////////////////////////

		void NavMeshVCContext::drawNodeIDs() const {
			const NavMeshPtr navMesh = _vc->_navMesh;
			unsigned int nCount = static_cast< unsigned int >( navMesh->getNodeCount() );
			
			glColor4f( 0.5f, 0.8f, 1.f, 1.f );
			for ( unsigned int n = 0; n < nCount; ++n ) {
				std::stringstream ss;
				ss << n;
				const NavMeshNode & node = navMesh->getNode( n );
				Vector3 p = node.getCenter3D();
				writeText( ss.str(), p, true/*currColor*/ );
			}
		}
		
		/////////////////////////////////////////////////////////////////////
		//                   Implementation of NavMeshVCFactory
		/////////////////////////////////////////////////////////////////////

		NavMeshVCFactory::NavMeshVCFactory() : VelCompFactory() {
			_fileNameID = _attrSet.addStringAttribute( "file_name", true /*required*/ );
			_headingID = _attrSet.addFloatAttribute( "heading_threshold", false /*required*/, 180.f );
		}

		/////////////////////////////////////////////////////////////////////

		bool NavMeshVCFactory::setFromXML( VelComponent * vc, TiXmlElement * node, const std::string & behaveFldr ) const {
			NavMeshVelComponent * nmvc = dynamic_cast< NavMeshVelComponent * >( vc );
			assert( nmvc != 0x0 && "Trying to set attributes of a navigation mesh velocity component on an incompatible object" );
			
			if ( ! VelCompFactory::setFromXML( nmvc, node, behaveFldr ) ) return false;

			// get the absolute path to the file name

			std::string fName;
			std::string path = os::path::join( 2, behaveFldr.c_str(), _attrSet.getString( _fileNameID ).c_str() );
			os::path::absPath( path, fName );
			// nav mesh
			NavMeshPtr nmPtr;
			try {
				nmPtr = loadNavMesh( fName );
			} catch ( ResourceException ) {
				logger << Logger::ERR_MSG << "Couldn't instantiate the navigation mesh referenced on line " << node->Row() << ".";
				return false;
			}
			nmvc->setNavMesh( nmPtr );
			// nav mesh localizer
			NavMeshLocalizerPtr nmlPtr;
			try {
				nmlPtr = loadNavMeshLocalizer( fName, true );
			} catch ( ResourceException ) {
				logger << Logger::ERR_MSG << "Couldn't instantiate the navigation mesh localizer required by the velocity component on line " << node->Row() << ".";
				return false;
			}
			nmvc->setNavMeshLocalizer( nmlPtr );
			nmvc->setHeadingDeviation( _attrSet.getFloat( _headingID ) * DEG_TO_RAD  );

			return true;
		}
		
	}	// namespace BFSM
}	// namespace Menge