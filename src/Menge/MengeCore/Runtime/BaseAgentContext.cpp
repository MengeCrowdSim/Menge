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

#include "BaseAgentContext.h"

// MengeSim
#include "BaseAgent.h"
#include "Obstacle.h"
// MengeRuntime
#include "VisAgent.h"
// Math
#include "Math/Vector2.h"
// SceneGraph
#include "Select.h"
#include "graphCommon.h"
#include "shapes.h"
// BFSM
#include "FsmContext.h"
// STL
#include <sstream>
#include <iomanip>

namespace Menge {
	
	using namespace Math;

	////////////////////////////////////////////////////////////////////////////
	//			Implementation of BaseAgentContext
	////////////////////////////////////////////////////////////////////////////

	const float BaseAgentContext::Y = 0.01f;

	////////////////////////////////////////////////////////////////////////////

	BaseAgentContext::BaseAgentContext(  VisAgent ** agents, size_t agtCount, BFSM::FsmContext * fsmCtx ): SceneGraph::SelectContext(), _selected(0x0), _state(DEFAULT_ST),
												_showNbrRadius(false), _showNbr(false), _showMaxSpd(false), _showVel(false),
												_showPrefVel(false), _showOrient(false), _visAgents(agents), _agtCount(agtCount), _digitCount(0), _fsmCtx(fsmCtx)
	{
		_digits[ MAX_TYPE_DIGITS ] = '\0';
	}

	////////////////////////////////////////////////////////////////////////////

	BaseAgentContext::~BaseAgentContext() {
		if ( _fsmCtx ) delete _fsmCtx;
	}

	////////////////////////////////////////////////////////////////////////////

	SceneGraph::ContextResult BaseAgentContext::handleKeyboard( SDL_Event & e ) {
		// TODO: Let the fsm context have a chance at this
		SceneGraph::ContextResult result( false, false );
		SDLMod mods = e.key.keysym.mod;
		bool hasCtrl = ( mods & KMOD_CTRL ) > 0;
		bool hasAlt = ( mods & KMOD_ALT ) > 0;
		bool hasShift = ( mods & KMOD_SHIFT ) > 0;
		bool noMods = !(hasCtrl || hasAlt || hasShift );
		if ( _fsmCtx ) {
			result = _fsmCtx->handleKeyboard( e );
		}
		if ( ! result.isHandled() ) {
			if ( e.type == SDL_KEYDOWN ) {
				if ( noMods ) {
					if ( _state == DEFAULT_ST ) {
						if ( e.key.keysym.sym == SDLK_r ) {
							_showNbrRadius = !_showNbrRadius;
							result.set( true, true );
						} else if ( e.key.keysym.sym == SDLK_n ) {
							_showNbr = ! _showNbr;
							result.set( true, true );
						} else if ( e.key.keysym.sym == SDLK_m ) {
							_showMaxSpd = ! _showMaxSpd;
							result.set( true, true );
						} else if ( e.key.keysym.sym == SDLK_v ) {
							_showVel = ! _showVel;
							result.set( true, true );
						} else if ( e.key.keysym.sym == SDLK_p ) {
							_showPrefVel = ! _showPrefVel;
							result.set( true, true );
						} else if ( e.key.keysym.sym == SDLK_RETURN ) {
							beginIDTyping();
							result.set( true, true );
						} else if ( e.key.keysym.sym == SDLK_o ) {
							_showOrient = ! _showOrient;
							result.set( true, true );
						}
					} else if ( _state == TYPE_AGENT_ID_ST ) {
						result.set( true, false );
						if ( ( e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9 ) ||
							( e.key.keysym.sym >= SDLK_KP0 && e.key.keysym.sym <= SDLK_KP9 ) ) {
								int offset;
								if ( e.key.keysym.sym <= SDLK_9 ) offset = e.key.keysym.sym - SDLK_0;
								else offset = e.key.keysym.sym - SDLK_KP0;
								char digit = '0' + offset;
								addIDDigit( digit );
								result.set( true, true );
						} else if ( e.key.keysym.sym == SDLK_RETURN ) {
							finishIDTyping();
							result.set( true, true );
						} else if ( e.key.keysym.sym == SDLK_SPACE ) {
							cancelIDTyping();
							result.set( true, true );
						}
					}
				}
			}
		}
		return result;
	}

	////////////////////////////////////////////////////////////////////////////

	void BaseAgentContext::drawGL( int vWidth, int vHeight ) {
		getOpenGLView();
		glPushAttrib( GL_ENABLE_BIT );
		glDisable( GL_LIGHTING );
		draw3DGL();
		const Agents::BaseAgent * agt = 0x0;
		if ( _fsmCtx ) {
			agt = ( _selected ) ? _selected->getAgent() : 0x0;
			_fsmCtx->draw3DGL( agt );
		}
		drawUIGL( vWidth, vHeight );
		if ( _fsmCtx ) {
			_fsmCtx->drawUIGL( agt, vWidth, vHeight );
		}
		glPopAttrib();
	}

	////////////////////////////////////////////////////////////////////////////

	bool BaseAgentContext::selectGL( const SceneGraph::GLScene * scene, const SceneGraph::GLCamera & camera, int vWidth, int vHeight, int * selectPoint ) {
		bool val = SceneGraph::SelectContext::selectGL( scene, camera, vWidth, vHeight, selectPoint );
		if ( val ) {
			VisAgent * s = dynamic_cast< VisAgent * >( SceneGraph::Selectable::getSelectedObject() );
			val = s != _selected;
			_selected = s;
		} else if ( _selected ) {
			_selected = 0x0;
			val = true;
		}
		return val;
	}

	////////////////////////////////////////////////////////////////////////////

	void BaseAgentContext::drawUIGL( int vWidth, int vHeight, bool select ) {
		if (!select) {
			std::stringstream ss;
			ss << contextName();
			if ( _visAgents != 0x0 ) {
				// Only print the help if the support is possible
				ss << "\n<Enter> to select by id";
			}
		
			if ( _selected ) {
				ss << agentText( _selected->getAgent() );
			}
			writeToScreen( ss.str(), SceneGraph::TextWriter::LEFT_TOP, 15, 10.f, 10.f );

			drawIDTyping();
		}
	}

	////////////////////////////////////////////////////////////////////////////

	void BaseAgentContext::draw3DGL( bool select ) {
		if ( !select && _selected ) {
			glPushAttrib( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_LINE_BIT | GL_POLYGON_BIT );
			glLineWidth( 2.f );
			glDepthMask( GL_FALSE );
			glDisable( GL_DEPTH_TEST );
			const Agents::BaseAgent * agt = _selected->getAgent();
			drawNeighbors( agt );
			drawNbrRadius( agt );
			drawMaxSpeed( agt );
			drawVelocity( agt );
			drawPrefVelocity( agt );
			drawOrientation( agt );
			glPopAttrib();
		}
	}

	////////////////////////////////////////////////////////////////////////////

	void BaseAgentContext::drawNbrRadius( const Agents::BaseAgent * agt ) {
		if ( _showNbrRadius ) {
			glPushAttrib( GL_POLYGON_BIT );
			glEnable( GL_BLEND );
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				
			glPushMatrix();
			glTranslatef( agt->_pos.x(), Y, agt->_pos.y() );
			float r = agt->_neighborDist;
			SceneGraph::Circle::drawCircle( r, 1.f, 0.75f, 0.85f, 0.05f );
			SceneGraph::Circle::drawCircle( r, 1.f, 0.75f, 0.85f, 1.f, GL_LINE );
			
			glPopMatrix();
			glPopAttrib();
		}
	}

	////////////////////////////////////////////////////////////////////////////

	void BaseAgentContext::drawNeighbors( const Agents::BaseAgent * agt ) {
		if ( _showNbr ) {
			glColor4f( 1.f, 1.f, 1.f, 1.f );

			const size_t NBR_COUNT = agt->_nearAgents.size(); 
			for ( size_t i = 0; i < NBR_COUNT; ++i ) {
				std::stringstream ss;
				ss << std::setiosflags(std::ios::fixed) <<  std::setprecision( 2 );
				ss << sqrtf( agt->_nearAgents[i].distanceSquared );
				const Agents::BaseAgent * nbr = agt->_nearAgents[i].agent;
				const Vector2 & p = nbr->_pos;
				writeAlignedText( ss.str(), p, SceneGraph::TextWriter::CENTERED, true );
			}
					// Label the nearby obstacles
			glColor4f( 0.5f, 1.f, 0.5f, 1.f );
			const size_t OBST_COUNT = agt->_nearObstacles.size();
			for ( size_t i = 0; i < OBST_COUNT; ++i ) {
				std::stringstream ss;
				const Agents::Obstacle * obst = agt->_nearObstacles[ i ].obstacle;
				ss << i;
				writeAlignedText( ss.str(), obst->midPt(), SceneGraph::TextWriter::CENTERED, true );
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////

	void BaseAgentContext::drawMaxSpeed( const Agents::BaseAgent * agt ) {
		if ( _showMaxSpd ) {
			glPushAttrib( GL_POLYGON_BIT );
			glEnable( GL_BLEND );
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				
			glPushMatrix();
			glTranslatef( agt->_pos.x(), Y, agt->_pos.y() );
			float r = agt->_maxSpeed;
			SceneGraph::Circle::drawCircle( r, 0.75f, 1.f, 0.85f, 0.05f );
			SceneGraph::Circle::drawCircle( r, 0.75f, 1.f, 0.85f, 1.f, GL_LINE );
			
			glPopMatrix();
			glPopAttrib();
		}
	}

	////////////////////////////////////////////////////////////////////////////

	void BaseAgentContext::drawVelocity( const Agents::BaseAgent * agt ) {
		if ( _showVel ) {
			Vector2 vel = agt->_pos + agt->_vel;
			glDisable( GL_BLEND );
			glColor3f( 0.9f, 0.45f, 0.1f );
			glBegin( GL_LINES );
			glVertex3f( agt->_pos.x(), Y, agt->_pos.y() );
			glVertex3f( vel.x(), Y, vel.y() );
			glEnd();

			writeTextRadially( "v", vel, agt->_vel, true );
		}
	}

	////////////////////////////////////////////////////////////////////////////

	void BaseAgentContext::drawPrefVelocity( const Agents::BaseAgent * agt ) {
		if ( _showPrefVel ) {
			Vector2 vel = agt->_pos + agt->_velPref.getPreferredVel();
			glDisable( GL_BLEND );
			glColor3f( 0.75f, 0.f, 1.f );
			glBegin( GL_LINES );
			glVertex3f( agt->_pos.x(), Y, agt->_pos.y() );
			glVertex3f( vel.x(), Y, vel.y() );
			glEnd();

			writeTextRadially( "v_p", vel, agt->_velPref.getPreferredVel(), true );
		}
	}

	////////////////////////////////////////////////////////////////////////////

	void BaseAgentContext::drawOrientation( const Agents::BaseAgent * agt ) {
		if ( _showOrient ) {
			Vector2 orient = agt->_pos + agt->_orient;
			glDisable( GL_BLEND );
			glColor3f( 0.75f, 0.75f, 0.75f );
			glBegin( GL_LINES );
			glVertex3f( agt->_pos.x(), Y, agt->_pos.y() );
			glVertex3f( orient.x(), Y, orient.y() );
			glEnd();

			writeTextRadially( "orient", orient, agt->_orient, true );
		}
	}
	////////////////////////////////////////////////////////////////////////////

	std::string BaseAgentContext::agentText( const Agents::BaseAgent * agt ) const {
		std::stringstream ss;
		ss << std::setiosflags(std::ios::fixed) <<  std::setprecision( 3 );

		ss << "\nSelected: " << agt->_id;
		ss << "\nPosition: " << agt->_pos;
		ss << "\nNeighbo(r) radius: " << agt->_neighborDist;
		ss << "\n(N)eighbors: " << agt->_nearAgents.size();
		ss << "\n(M)ax Speed: " << agt->_maxSpeed;
		ss << "\n(O)rientation: " << agt->_orient;
		ss << "\n(V)elocity: " << agt->_vel << "(" << abs( agt->_vel ) << ")";
		ss << "\n(P)ref. Velocity: " << agt->_velPref.getPreferredVel() << "(" << agt->_velPref.getSpeed() << ")";
		ss << "\nPriority: " << agt->_priority;
		ss << "\nClass: " << agt->_class;
		/*
			// Unused properties of a BaseAgent
			_radius
			_maxAccel
			_prefSpeed
			_maxNeighbors
			_maxAngVel
		*/
		return ss.str();	
	}

	////////////////////////////////////////////////////////////////////////////

	void BaseAgentContext::beginIDTyping() {
		if ( _visAgents != 0x0 ) {
			_state = TYPE_AGENT_ID_ST;
			_digitCount = 0;
			_digits[0] = '\0';
		}
	}

	////////////////////////////////////////////////////////////////////////////

	void BaseAgentContext::finishIDTyping() {
		_state = DEFAULT_ST;
		size_t id = (size_t)atoi( &_digits[0] );	// I know it must be non-negative
		if ( id < _agtCount ) {
			VisAgent * s = _visAgents[ id ];
			if ( s != _selected ) {
				SceneGraph::Selectable::setSelectedObject( s );
				_selected = s;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////

	void BaseAgentContext::cancelIDTyping() {
		_state = DEFAULT_ST;
	}

	////////////////////////////////////////////////////////////////////////////

	void BaseAgentContext::drawIDTyping() {
		if ( _state == TYPE_AGENT_ID_ST ) {
			std::stringstream ss;
			ss << "Type an id to select: " << (&_digits[0] );
			ss << "\n   (Enter to finish, Space to cancel)";
			writeToScreen( ss.str(), SceneGraph::TextWriter::CENTERED, 15 );
		}
	}

	////////////////////////////////////////////////////////////////////////////

	void BaseAgentContext::addIDDigit( const char digit ) {
		if ( _digitCount < MAX_TYPE_DIGITS ) {
			// Room to add another digit.
			_digits[ _digitCount ] = digit;
			_digits[ _digitCount + 1 ] = '\0';
			++_digitCount;
		}
	}

	////////////////////////////////////////////////////////////////////////////

	void BaseAgentContext::activate() {
		VisAgent * s = dynamic_cast< VisAgent * >( SceneGraph::Selectable::getSelectedObject() );
		if ( s != _selected ) {
			_selected = s;
		}
	}
}	// namespace Menge