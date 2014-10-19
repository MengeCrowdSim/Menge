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

#include "ZanlungoAgentContext.h"
#include "ZanlungoAgent.h"
#include "VisAgent.h"
#include "shapes.h"
#include <iomanip>

namespace Zanlungo {

	////////////////////////////////////////////////////////////////
	//			Implementation of ZanlungoAgentContext
	////////////////////////////////////////////////////////////////

	AgentContext::AgentContext( VisAgent ** agents, unsigned int agtCount ):BaseAgentContext(agents,agtCount), _showTTI(false), _ttiObject(0), _showForce(false) {
	}

	////////////////////////////////////////////////////////////////

	SceneGraph::ContextResult AgentContext::handleKeyboard( SDL_Event & e ) {
		SceneGraph::ContextResult result = BaseAgentContext::handleKeyboard( e );
		if ( !result.isHandled() ) {
			SDLMod mods = e.key.keysym.mod;
			bool hasCtrl = ( mods & KMOD_CTRL ) > 0;
			bool hasAlt = ( mods & KMOD_ALT ) > 0;
			bool hasShift = ( mods & KMOD_SHIFT ) > 0;
			bool noMods = !(hasCtrl || hasAlt || hasShift );
			if ( e.type == SDL_KEYDOWN ) {
				if ( noMods ) {
					if ( e.key.keysym.sym == SDLK_i ) {
						_showTTI = !_showTTI;
						result.set( true, true );
					} else if ( e.key.keysym.sym == SDLK_f ) {
						_showForce = !_showForce;
						result.set( true, true );
					} else if ( e.key.keysym.sym == SDLK_UP ) {
						if ( ( _showTTI || _showForce ) && _selected ) {
							const Agent * agt = dynamic_cast< Agent * >( _selected->getAgent() );
							int NBRS = (int)agt->_nearAgents.size();
							int OBST = (int)agt->_nearObstacles.size();
							if ( NBRS | OBST ) {
								++_ttiObject;
								if ( _ttiObject > NBRS ) {
									if ( OBST ) {
										_ttiObject = -OBST;
									} else {
										_ttiObject = 0;
									}
								}
								result.set( true, true );
							}
						}
					} else if ( e.key.keysym.sym == SDLK_DOWN ) {
						if ( ( _showTTI || _showForce ) && _selected ) {
							const Agent * agt = dynamic_cast< Agent * >( _selected->getAgent() );
							int NBRS = (int)agt->_nearAgents.size();
							int OBST = (int)agt->_nearObstacles.size();
							if ( NBRS | OBST ) {
								--_ttiObject;
								if ( _ttiObject < -OBST ) {
									if ( NBRS ) {
										_ttiObject = NBRS;
									} else {
										_ttiObject = -1;
									}
								}
								result.set( true, true );
							}
						}
					}
				}
			}
		}
		return result;
	}

	////////////////////////////////////////////////////////////////

	void AgentContext::draw3DGL( bool select ) {
		BaseAgentContext::draw3DGL( select );
		if ( !select && _selected ) {
			glPushAttrib( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_LINE_BIT | GL_POLYGON_BIT );
			glDepthMask( GL_FALSE );
			const Agent * agt = dynamic_cast< Agent * >( _selected->getAgent() );
			drawTTI( agt );
			drawForce( agt );
			drawOrientationFan( agt );
			glPopAttrib();
		}
	}

	////////////////////////////////////////////////////////////////

	std::string AgentContext::agentText( const Agents::BaseAgent * agent ) const {
		const Agent * agt = dynamic_cast< const Agent * >( agent );
		std::string m = BaseAgentContext::agentText( agt );

		std::stringstream ss;
		ss << std::setiosflags(std::ios::fixed) <<  std::setprecision( 2 );

		ss << "\n_________________________";
		ss << "\nTime to (I)nteraction";
		if ( _showTTI && _selected ) {
			float TTI;
			if ( agt->computeTTI( TTI ) ) {
				ss << ": " << TTI << " s ";
			} else {
				ss << ": infinite";
			}
		}
		ss << "\nDraw (F)orces";
		if ( ( _showTTI || _showForce ) && _selected ) {
			ss << "\n     (up/down arrow to select single)";
		}
		return m + ss.str();
	}

	////////////////////////////////////////////////////////////////

	void AgentContext::drawTTI( const Agent * agt ) {
		if ( _showTTI && _selected ) {
			float TTI;
			if ( agt->computeTTI( TTI ) ) {
				glPushMatrix();
				// Draw the future position of THIS agent
				Vector2 futurePos = agt->_pos + agt->_vel * TTI;
				glColor4f( 1.f, 1.f, 1.f, 0.5f );
				glBegin( GL_LINES );
				glVertex3f( futurePos.x(), Y, futurePos.y() );
				glVertex3f( agt->_pos.x(), Y, agt->_pos.y() );
				glEnd();
				glPushMatrix();
				glTranslatef( futurePos.x(), Y, futurePos.y() );
				SceneGraph::Circle::drawCircle( agt->_radius, 1.f, 1.f, 1.f, 0.75f, GL_LINE );
				glPopMatrix();
				
				if ( _ttiObject == 0 ) {
					// draw all the future positions of the agents
					const int NBRS = (int)agt->_nearAgents.size();
					for ( int i = 0; i < NBRS; ++i ) {
						// future position
						const Agent * other = static_cast< const Agent *>( agt->getNeighbor( i ) );
						Vector2 futurePos2 = other->_pos + other->_vel * TTI;
						glColor3f( 1.f, 0.f, 0.f );
						glBegin( GL_LINES );
						glVertex3f( futurePos2.x(), Y, futurePos2.y() );
						glVertex3f( other->_pos.x(), Y, other->_pos.y() );
						glEnd();
						glPushMatrix();
						glTranslatef( futurePos2.x(), Y, futurePos2.y() );
						SceneGraph::Circle::drawCircle( other->_radius, 1.f, 0.f, 0.f, 0.75f, GL_LINE );
						glPopMatrix();
					}
				} else if ( _ttiObject > 0 ) {
					// Draw the future position of _ttiObject
					const Agent * other = static_cast< const Agent *>( agt->getNeighbor( _ttiObject - 1 ) );
					Vector2 futurePos2 = other->_pos + other->_vel * TTI;
					glColor3f( 1.f, 0.f, 0.f );
					glBegin( GL_LINES );
					glVertex3f( futurePos2.x(), Y, futurePos2.y() );
					glVertex3f( other->_pos.x(), Y, other->_pos.y() );
					glEnd();
					glPushMatrix();
					glTranslatef( futurePos2.x(), Y, futurePos2.y() );
					SceneGraph::Circle::drawCircle( other->_radius, 1.f, 0.f, 0.f, 0.75f, GL_LINE );
					glPopMatrix();
				
				} else if ( _ttiObject < 0 ){
					// draw obstacle
				}
				glPopMatrix();
			}
		}
	}

	////////////////////////////////////////////////////////////////

	void AgentContext::drawForce( const Agent * agt ) {
		if ( _showForce && _selected ) {
			float TTI;
			if ( agt->computeTTI( TTI ) ) {	// returns true if there are interactions
				glPushMatrix();
				if ( _ttiObject == 0 ) {
					// draw all the future positions of the agents
					const int NBRS = (int)agt->_nearAgents.size();
					for ( int i = 0; i < NBRS; ++i ) {
						// future position
						const Agent * other = static_cast< const Agent *>( agt->getNeighbor( i ) );
						singleAgentForce( agt, other, TTI );
					}
				} else if ( _ttiObject > 0 ) {
					const Agent * other = static_cast< const Agent *>( agt->getNeighbor( _ttiObject - 1 ) );
					singleAgentForce( agt, other, TTI );
				} else if ( _ttiObject < 0 ){
					// draw obstacle
				}
				glPopMatrix();
			}
		}
	}

	////////////////////////////////////////////////////////////////

	void AgentContext::singleAgentForce( const Agent * agt, const Agent * other, float TTI ) {
		// This is for printing force magnitude and source
		SceneGraph::TextWriter * writer = SceneGraph::TextWriter::Instance();
		// Get screen coordintes of source and force
		double modViewMat[16], projMat[16];
		int viewMat[4];
		// NOTE: I'm doing this for each neighboring force.  That's inefficient
		//	HOWEVER, the expectation is that it is only done for one agent at a time
		//	so, the inefficiency is negligble.
		glGetDoublev( GL_MODELVIEW_MATRIX, &modViewMat[0] );
		glGetDoublev( GL_PROJECTION_MATRIX, &projMat[0] );
		glGetIntegerv( GL_VIEWPORT, &viewMat[0] );

		Vector2 force = agt->agentForce( other, TTI );
		float forceMag = abs( force );
		if ( forceMag > 0.5f ) {
			glColor4f( 1.f, 1.f, 1.f, 1.f );
			Vector2 forceEnd = agt->_pos + force;
			glBegin( GL_LINES );
			glVertex3f( agt->_pos.x(), Y, agt->_pos.y() );
			glVertex3f( forceEnd.x(), Y, forceEnd.y() );
			glEnd();
			// annotate illustration
			std::stringstream ss;
			ss << std::setiosflags(std::ios::fixed) << std::setprecision( 2 );
			ss << other->_id;
			// Label the source agent
			writeAlignedText( ss.str(), other->_pos, SceneGraph::TextWriter::LEFT_BOTTOM, true );
			// Label the fource
			ss << ": " << abs( force ) << " N";
			Vector2 forceDir( norm( force ) * ( 4 * agt->_radius ) + agt->_pos );
			writeText( ss.str(), forceDir, true );
		}
	}

	////////////////////////////////////////////////////////////////

	void AgentContext::drawOrientationFan( const Agent * agt ) {
		// Draws orientation as a fan
		if ( _showOrient ) {
			// Determine the vector rotation for the fan
			float angle = agt->getFOV();
			const int SAMPLES = 10;	// number of spans in the fan
			float dTheta = angle / SAMPLES;
			Vector2 dX( cos( dTheta ), sin( dTheta ) );
			Vector2 dY( -dX.y(), dX.x() );

			// Create the initial point on the fan
			Vector2 dir = agt->_orient;
			{
				float halfAngle = angle * 0.5f;
				float c = cos( halfAngle );
				float s = sin( halfAngle );
				float x = dir.x() * c - dir.y() * s;
				float y = dir.y() * c + dir.x() * s;
				dir.set( x * agt->_neighborDist, y * agt->_neighborDist );
			}

			glEnable( GL_BLEND );
			glColor4f( 0.5f, 0.1f, 0.1f, 0.25f );
			glBegin( GL_TRIANGLE_FAN );
			glVertex3f( agt->_pos.x(), Y, agt->_pos.y() );
			for ( int i = 0; i <= SAMPLES; ++i ) {
				// rotate, add, draw
				Vector2 pt = agt->_pos + dir;
				glVertex3f( pt.x(), Y, pt.y() );
				float x = dir * dX;
				float y = dir * dY;
				dir.set( x, y );
				
			}
			
			glEnd();
		}
	}
}	// namespace Zanlungo
