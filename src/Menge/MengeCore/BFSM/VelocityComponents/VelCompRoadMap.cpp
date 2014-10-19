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

#include "VelocityComponents/VelCompRoadMap.h"
#include "BaseAgent.h"
#include "os.h"
#include "Graph.h"
#include "Goals/Goal.h"

#include <sstream>
#include <iomanip>

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of RoadMapVelComponent
		/////////////////////////////////////////////////////////////////////

		RoadMapVelComponent::RoadMapVelComponent():VelComponent(), _roadmap(0x0) {
		}

		/////////////////////////////////////////////////////////////////////

		RoadMapVelComponent::RoadMapVelComponent( const GraphPtr & graph ):VelComponent(), _roadmap(graph) {
		}

		/////////////////////////////////////////////////////////////////////

		RoadMapVelComponent::~RoadMapVelComponent() {
			PathMap::iterator itr = _paths.begin();
			for ( ; itr != _paths.end(); ++itr ) {
				delete itr->second;
			}
			_paths.clear();
		}

		/////////////////////////////////////////////////////////////////////

		void RoadMapVelComponent::onExit( Agents::BaseAgent * agent ) {
			// This test is necessary.  Because state advancement can jump MULTIPLE states in a single
			//	time step, it is possible to enter and exit a state with a roadmap velocity without
			//	ever actually calling "setPrefVelocity" on that velocity component.
			//
			//	Roadmap initializes the path in setPrefVelocity - so, things don't get properly
			//	initialized.
			_lock.lockWrite();
			PathMap::iterator itr = _paths.find( agent->_id );
			if ( itr != _paths.end() ) {
				delete itr->second;
				_paths.erase( agent->_id );
			}
			_lock.releaseWrite();
		}

		/////////////////////////////////////////////////////////////////////

		void RoadMapVelComponent::setPrefVelocity( const Agents::BaseAgent * agent, const Goal * goal, Agents::PrefVelocity & pVel ) {
			_lock.lockRead();
			PathMap::iterator itr = _paths.find( agent->_id );
			RoadMapPath * path = 0x0;
			if ( itr == _paths.end() ) {
				_lock.releaseRead();
				// compute the path and add it to the map
				//	Create path for the agent
				Vector2 goalPoint = goal->getCentroid();
				path = _roadmap->getPath( agent, goal ); 
				_lock.lockWrite();
				_paths[ agent->_id ] = path;
				_lock.releaseWrite();
			} else {
				path = itr->second;
				_lock.releaseRead();
			}
			pVel.setSpeed( agent->_prefSpeed );
			path->setPrefDirection( agent, pVel );
		}

		/////////////////////////////////////////////////////////////////////
		
		VelCompContext * RoadMapVelComponent::getContext() {
			return new RoadMapVCContext( this );
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of RoadMapVCContext
		/////////////////////////////////////////////////////////////////////

		RoadMapVCContext::RoadMapVCContext( RoadMapVelComponent * vc ):VelCompContext(),_vc(vc) {
		}

		/////////////////////////////////////////////////////////////////////

		std::string RoadMapVCContext::getUIText( const std::string & indent ) const {
			std::stringstream ss;
			ss << indent << "Roadmap velocity component";
			return ss.str();
		}

		/////////////////////////////////////////////////////////////////////

		void RoadMapVCContext::draw3DGL( const Agents::BaseAgent * agt, const Goal * goal ) {
			// draw target
			glPushAttrib( GL_POINT_BIT | GL_LINE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			glDepthMask( GL_FALSE );
			
			GraphPtr & graph = _vc->_roadmap;

			// draw edges
			glColor3f( 0.2f, 0.2f, 1.f );
			glLineWidth( 1.5f );
			glBegin( GL_LINES );
			const size_t V_COUNT = graph->getVertexCount();
			for ( size_t i = 0; i < V_COUNT; ++i ) {
				const GraphVertex * v = graph->getVertex( i );
				const size_t vID = v->getID();
				const Vector2 vPos = v->getPosition();
				const size_t N_COUNT = v->getNeighborCount();
				for ( size_t j = 0; j < N_COUNT; ++j ) {
					const GraphVertex * u = v->getNeighbor( j );
					if ( vID < u->getID() ) {
						glVertex3f( vPos.x(), 0.f, vPos.y() );
						Vector2 uPos = u->getPosition();
						glVertex3f( uPos.x(), 0.f, uPos.y() );
					}
				}
			}
			glEnd();

			// draw nodes
			glColor3f( 1.0f, 0.6f, 1.f );
			glPointSize( 5.f );
			glBegin( GL_POINTS );
			for ( size_t i = 0; i < V_COUNT; ++i ) {
				const GraphVertex * v = graph->getVertex( i );
				const Vector2 vPos = v->getPosition();
				glVertex3f( vPos.x(), 0.f, vPos.y() );
			}
			glEnd();

			// draw path
			Vector2 tgtPoint;
			PathMap::iterator itr = _vc->_paths.find( agt->_id );
			if ( itr != _vc->_paths.end() ) {
				const RoadMapPath * path = itr->second;
				const size_t WP_COUNT = path->getWayPointCount();

				// draw the goal
				if ( WP_COUNT > 0 ) {
					// nearest point to the goal from the last way point
					tgtPoint = goal->getTargetPoint( path->getWayPoint( WP_COUNT - 1 ), agt->_radius );
				} else {
					tgtPoint = goal->getTargetPoint( agt->_pos, agt->_radius );
				}

				const size_t TARGET = path->getTargetID();
				glColor3f( 1.f, 1.f, 0.5f );
				glLineWidth( 2.f );
				glBegin( GL_LINE_STRIP );
				glVertex3f( agt->_pos.x(), 0.f, agt->_pos.y() );
				for ( size_t i = TARGET; i < WP_COUNT; ++i ) {
					Vector2 p = path->getWayPoint( i );
					glVertex3f( p.x(), 0.f, p.y() );
				}
				glVertex3f( tgtPoint.x(), 0.f, tgtPoint.y() );
				glEnd();
			} else {
				tgtPoint = goal->getTargetPoint( agt->_pos, agt->_radius );
			}

			// draw goal
			//	TODO: REALLY draw the goal
			drawGoal( tgtPoint, agt );

			// draw the preferred velocity
			Agents::PrefVelocity pVel;
			_vc->setPrefVelocity( agt, goal, pVel );
			drawPrefVel( pVel, agt->_pos );
			
			glPopAttrib();
		}
		
		/////////////////////////////////////////////////////////////////////
		//                   Implementation of RoadMapVCFactory
		/////////////////////////////////////////////////////////////////////

		RoadMapVCFactory::RoadMapVCFactory() : VelCompFactory() {
			_fileNameID = _attrSet.addStringAttribute( "file_name", true /*required*/ );
		}

		/////////////////////////////////////////////////////////////////////

		bool RoadMapVCFactory::setFromXML( VelComponent * vc, TiXmlElement * node, const std::string & behaveFldr ) const {
			RoadMapVelComponent * rmvc = dynamic_cast< RoadMapVelComponent * >( vc );
			assert( rmvc != 0x0 && "Trying to set attributes of a road map velocity component on an incompatible object" );
			
			if ( ! VelCompFactory::setFromXML( rmvc, node, behaveFldr ) ) return false;

			// get the file name
			std::string fName;
			std::string path = os::path::join( 2, behaveFldr.c_str(), _attrSet.getString( _fileNameID ).c_str() );
			os::path::absPath( path, fName );
			GraphPtr gPtr;
			try {
				gPtr = loadGraph( fName );
			} catch ( ResourceException ) {
				logger << Logger::ERR_MSG << "Couldn't instantiate the road map referenced on line " << node->Row() << ".";
				return false;
			}
			rmvc->setRoadMap( gPtr );

			return true;
		}
		
	}	// namespace BFSM
}	// namespace Menge
