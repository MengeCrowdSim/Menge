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

#include "HelbingAgentContext.h"
#include "HelbingAgent.h"
#include "VisAgent.h"
#include <iomanip>
#include <sstream>

namespace Helbing {

	////////////////////////////////////////////////////////////////
	//			Implementation of HelbingAgentContext
	////////////////////////////////////////////////////////////////

	AgentContext::AgentContext( VisAgent ** agents, unsigned int agtCount ): BaseAgentContext(agents,agtCount), _showForce(false),_forceObject(0)
	{
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
					if ( e.key.keysym.sym == SDLK_f ) {
						_showForce = !_showForce;
						result.set( true, true );
					} else if ( e.key.keysym.sym == SDLK_UP ) {
						if ( _showForce && _selected ) {
							const Agent * agt = dynamic_cast< const Agent * >( _selected->getAgent() );
							assert( agt != 0x0 && "Helbing context trying to work with a non helbing agent" );
							int NBRS = (int)agt->_nearAgents.size();
							int OBST = (int)agt->_nearObstacles.size();
							if ( NBRS | OBST ) {
								++_forceObject;
								if ( _forceObject > NBRS ) {
									if ( OBST ) {
										_forceObject = -OBST;
									} else {
										_forceObject = 0;
									}
								}
								result.set( true, true );
							}
						}
					} else if ( e.key.keysym.sym == SDLK_DOWN ) {
						if ( _showForce && _selected ) {
							const Agent * agt = dynamic_cast< const Agent * >( _selected->getAgent() );
							assert( agt != 0x0 && "Helbing context trying to work with a non helbing agent" );
							int NBRS = (int)agt->_nearAgents.size();
							int OBST = (int)agt->_nearObstacles.size();
							if ( NBRS | OBST ) {
								--_forceObject;
								if ( _forceObject < -OBST ) {
									if ( NBRS ) {
										_forceObject = NBRS;
									} else {
										_forceObject = -1;
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

	void AgentContext::update() {
		BaseAgentContext::update();
		if ( _selected && _forceObject ) {
			const Agent * agt = dynamic_cast< const Agent * >( _selected->getAgent() );
			assert( agt != 0x0 && "Helbing context trying to work with a non helbing agent" );							
			if ( _forceObject > 0 ) {
				int NBR_COUNT = (int)agt->_nearAgents.size();
				if ( _forceObject > NBR_COUNT ) {
					_forceObject = NBR_COUNT;
				}
			} else {	// _forceObject < 0
				int OBST_COUNT = (int)agt->_nearObstacles.size();
				if ( -_forceObject > OBST_COUNT ) {
					_forceObject = -OBST_COUNT;
				}
			}
		}
	}

	////////////////////////////////////////////////////////////////

	void AgentContext::draw3DGL( bool select ) {
		BaseAgentContext::draw3DGL( select );
		if ( !select && _selected ) {
			glPushAttrib( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_LINE_BIT | GL_POLYGON_BIT );
			glDepthMask( GL_FALSE );
			const Agent * agt = dynamic_cast< const Agent * >( _selected->getAgent() );
			assert( agt != 0x0 && "Helbing context trying to work with a non helbing agent" );
			drawForce( agt );
			glPopAttrib();
		}
	}

	////////////////////////////////////////////////////////////////

	std::string AgentContext::agentText( const Agents::BaseAgent * agent ) const {
		const Agent * agt = dynamic_cast< const Agent * >( agent );
		std::string m = BaseAgentContext::agentText( agt );

		std::stringstream ss;
		ss << std::setiosflags(std::ios::fixed) <<  std::setprecision( 2 );
		ss << "\nMass";
		if ( _selected ) {
			const Agent * agt = dynamic_cast< const Agent * >( _selected->getAgent() );
			assert( agt != 0x0 && "Helbing context trying to work with a non helbing agent" );
			ss << " " << agt->_mass << " kg";
		}
		ss << "\n_________________________";
		ss << "\nDraw (F)orces";
		if ( _showForce && _selected ) {
			ss << "\n    (up/down arrow to change)";
			if ( _forceObject == 0 ) {
				ss << "\n     All forces";
			} else if ( _forceObject > 0 ) {
				const Agent * agt = dynamic_cast< const Agent * >( _selected->getAgent() );
				assert( agt != 0x0 && "Helbing context trying to work with a non helbing agent" );
				const Agent * other = static_cast< const Agent *>( agt->getNeighbor( _forceObject - 1 ) );
				float force = abs( agt->agentForce( other ) );
				ss << "\n     Agent " << other->_id << ": " << force << " N";
			} else if ( _forceObject < 0 ) {
				const Agent * agt = dynamic_cast< const Agent * >( _selected->getAgent() );
				assert( agt != 0x0 && "Helbing context trying to work with a non helbing agent" );
				const Agents::Obstacle * obst = agt->getObstacle( -_forceObject - 1 );
				float force = abs( agt->obstacleForce( obst ) );
				ss << "\n     Obstacle " << obst->_id << ": " << force << " N";
			}
		}
		return m + ss.str();
	}

	////////////////////////////////////////////////////////////////

	void AgentContext::drawForce( const Agent * agt ) {
		if ( _showForce && _selected ) {
			if ( agt->_nearAgents.size() > 0 ) {
				glPushMatrix();
				// Draw driving force
				glColor4f( 0.1f, 1.f, 0.1f, 1.f );
				Vector2 driveForce( agt->drivingForce() );
				drawForce( agt, driveForce, "D" );
				// Draw repulsive forces
				if ( _forceObject == 0 ) {
					// draw forces for all agents
					const int NBRS = (int)agt->_nearAgents.size();
					for ( int i = 0; i < NBRS; ++i ) {
						const Agent * other = static_cast< const Agent *>( agt->getNeighbor( i ) );
						singleAgentForce( agt, other );
					}
					// draw forces for all obstacles
					const int OBSTS = (int)agt->_nearObstacles.size();
					for ( int i = 0; i < OBSTS; ++i ) {
						const Agents::Obstacle * obst = agt->getObstacle( i );
						singleObstacleForce( agt, obst );
					}
				} else if ( _forceObject > 0 ) {
					// single agent
					const Agent * other = static_cast< const Agent *>( agt->getNeighbor( _forceObject - 1 ) );
					singleAgentForce( agt, other, 0.f );
				} else {	
					// draw obstacle
					const Agents::Obstacle * obst = agt->getObstacle( -_forceObject - 1 );
					singleObstacleForce( agt, obst, 0.f );
				}
				glPopMatrix();
			}
		}
	}

	////////////////////////////////////////////////////////////////

	void AgentContext::singleAgentForce( const Agent * agt, const Agent * other, float thresh ) {
		Vector2 force = agt->agentForce( other );
		float forceMag = abs( force );
		if ( forceMag > thresh  ) {
			std::stringstream ss;
			ss << std::setiosflags(std::ios::fixed) <<  std::setprecision( 2 );
			ss << other->_id;
			glColor4f( 0.65f, 0.65f, 1.f, 1.f );
			drawForce( agt, force, ss.str() );	
			// Label the source agent
			writeAlignedText( ss.str(), other->_pos, SceneGraph::TextWriter::CENTERED, true );
		}
	}

	////////////////////////////////////////////////////////////////

	void AgentContext::singleObstacleForce( const Agent * agt, const Agents::Obstacle * obst, float thresh ) {
		Vector2 force = agt->obstacleForce( obst );
		float forceMag = abs( force );
		if ( forceMag > thresh ) {
			// Draw the force line
			std::stringstream ss;
			ss << std::setiosflags(std::ios::fixed) <<  std::setprecision( 2 );
			ss << obst->_id;
			glColor4f( 1.f, 0.65f, 0.65f, 1.f );
			drawForce( agt, force, ss.str() );	

			// Highlight the obstacle
			glPushAttrib( GL_LINE_BIT );
			glLineWidth( 3.f );
			glBegin( GL_LINES );
			glVertex3f( obst->getP0().x(), Y, obst->getP0().y() );
			glVertex3f( obst->getP1().x(), Y, obst->getP1().y() );
			glEnd();
			glPopAttrib();

			// Label the source agent
			Vector2 midPoint = obst->midPt();
			writeText( ss.str(), midPoint, true );
		}
	}

	////////////////////////////////////////////////////////////////

	void AgentContext::drawForce( const Agent * agt, const Vector2 & force, const std::string & label ) {
		// This is for printing force magnitude and source
		const float FORCE_RADIUS = 4 * agt->_radius;
		Vector2 forceEnd = norm( force ) * FORCE_RADIUS + agt->_pos;
		glBegin( GL_LINES );
		glVertex3f( agt->_pos.x(), Y, agt->_pos.y() );
		glVertex3f( forceEnd.x(), Y, forceEnd.y() );
		glEnd();
		// annotate illustration
		std::stringstream ss;
		ss << std::setiosflags(std::ios::fixed) <<  std::setprecision( 2 );
		// Label the source
		if ( label.size() > 0 ) {
			ss << label << ": ";
		}
		ss << abs( force ) << " N";
		writeTextRadially( ss.str(), forceEnd, force, true );
	}
}	// namespace Helbing
