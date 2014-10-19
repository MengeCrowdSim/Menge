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

/*!
 *  @file       ORCATypeAgentContext.h
 *  @brief      A basic context for interacting with and displaying
 *				ORCA-type agent parameters.
 *
 *	ORCA-type agents are those that solve w.r.t. a set of linear constraints
 *	this context gives some basic functionality for displaying those constraints
 */

#ifndef __ORCA_TYPE_AGENT_CONTEXT_H__
#define __ORCA_TYPE_AGENT_CONTEXT_H__

#include "BaseAgentContext.h"
#include "shapes.h"
#include "VisAgent.h"
#include <sstream>
#include <iomanip>

/*!
 *	@brief		Context class for displaying various aspects of the
 *				ORCA-type agent computation.
 */
template < class Agent >
class ORCATypeAgentContext : public Menge::BaseAgentContext {
public:
	/*!
	 *	@brief		Construtor
	 *
	 *	@param		agents		An array of pointers to VisAgent instances.
	 *	@param		agtCount	The number of agents contained in the array.
	 */
	ORCATypeAgentContext( Menge::VisAgent ** agents, unsigned int agtCount );
	
	/*!
	 *	@brief		Returns the name of the context for display.
	 *
	 *	@returns		The name of this context.
	 */
	virtual std::string contextName() const { return "UNDEFINED ORCA TYPE"; }

	/*!
	 *	@brief		Give the context the opportunity to respond to a keyboard
	 *				event.
	 *
	 *	@param		e		The SDL event with the keyboard event data.
	 *	@returns	A ContextResult instance reporting if the event was handled and
	 *				if redrawing is necessary.
	 */
	virtual Menge::SceneGraph::ContextResult handleKeyboard( SDL_Event & e );

	/*!
	 *	@brief		Allow the context to update any time-dependent state it might have to
	 *				the given global time.
	 */
	virtual void update(); 

protected:
	
	/*!
	 *	@brief		Draw context elements into the 3D world.
	 *
	 *	@param		select		Defines if the drawing is being done for selection
	 *							purposes (true) or visualization (false).
	 */
	virtual void draw3DGL( bool select=false );

	/*!
	 *	@brief		Helper function for drawing a halfplane
	 *	@param		line		The line object which defines the half plane
	 *	@param		pos			The relative position to draw the plane
	 *							Typically the position of the agent on which the half-plane
	 *							is applied.
	 *	@param		r			The red component of the half plane color
	 *	@param		g			The green component of the half plane color
	 *	@param		b			The blue component of the half plane color
	 *	@param		Y			Value, on the y-axis (in world coordinates), of the 
	 *							plane on which to draw the half plane
	 */
	void drawHalfPlane( const Menge::Math::Line & line, const Vector2 & pos, float r, float g, float b, float Y ) const;

	/*!
	 *	@brief		Draws the given ORCA line for the given agent
	 *
	 *	@param		agent		A pointer to the agent to whom this line applies
	 *	@param		line		The actual line
	 *	@param		isAgent		A boolean reporting if the orca line comes from an agent.
	 *							true --> agent, false --> obstacle
	 */
	void drawORCALine( const Agent * agent, const Menge::Math::Line & line, bool isAgent ) const;

	/*!
	 *	@brief		Draw the optimized velocity for the current set of orca lines
	 *
	 *	@param		agent		A pointer to the agent for which the velocity is drawn
	 *							computeNewVelocity will be called on the agent
	 */
	void drawOptVelocity( Agent * agent ) const;

	/*!
	 *	@brief		Creates a formatted string to be printed in the context
	 *				for a particular agent
	 *
	 *	@param		agent		A pointer to the agent for which the information is 
	 *							displayed.
	 *	@returns	A formatted string for display in the context's 2D gui.
	 */
	virtual std::string agentText( const Menge::Agents::BaseAgent * agent ) const;

	/*!
	 *	@brief		Determines if the ORCA lines are drawn
	 */
	bool		_showOrcaLines;

	/*!
	 *	@brief		Function for drawing the ORCA lines acting on agt
	 *
	 *	@param		agt			A pointer to the agent whose ORCA lines will be drawn.
	 */
	void drawORCALines( const Agent * agt ) const;

	/*!
	 *	@brief		Determines if the ORCA line construction is visualized
	 */
	bool		_visualizeORCA;

	/*!
	 *	@brief		The agent to visualize
	 */
	size_t		_visNbrID;

	/*!
	 *	@brief		The function that draws the visualization of the orca construction
	 *
	 *	@param		agt			A pointer to the agent for whom the *computation* of
	 *							a single ORCA line is illustrated.
	 */
	void visORCA( const Agent * agt ) const;
};

////////////////////////////////////////////////////////////////

template< class Agent >
ORCATypeAgentContext< Agent >::ORCATypeAgentContext( Menge::VisAgent ** agents, unsigned int agtCount ): Menge::BaseAgentContext(agents,agtCount), _showOrcaLines(false),_visualizeORCA(false),_visNbrID(0) {
}

////////////////////////////////////////////////////////////////

template< class Agent >
 void ORCATypeAgentContext< Agent >::update() {
	if ( this->_selected && _visNbrID ) {
		const Agent * agt = dynamic_cast< const Agent * >( this->_selected->getAgent() );
		if ( _visNbrID > 0 ) {
			size_t NBR_COUNT = agt->_nearAgents.size();
			if ( _visNbrID > NBR_COUNT ) {
				_visNbrID = NBR_COUNT;
			}
		}
	}
}

////////////////////////////////////////////////////////////////

template< class Agent >
Menge::SceneGraph::ContextResult ORCATypeAgentContext< Agent >::handleKeyboard( SDL_Event & e ) {
	Menge::SceneGraph::ContextResult result = BaseAgentContext::handleKeyboard( e );
	if ( !result.isHandled() ) {
		SDLMod mods = e.key.keysym.mod;
		bool hasCtrl = ( mods & KMOD_CTRL ) > 0;
		bool hasAlt = ( mods & KMOD_ALT ) > 0;
		bool hasShift = ( mods & KMOD_SHIFT ) > 0;
		bool noMods = !(hasCtrl || hasAlt || hasShift );
		if ( e.type == SDL_KEYDOWN ) {
			if ( noMods ) {
				if ( e.key.keysym.sym == SDLK_c ) {
					_showOrcaLines = !_showOrcaLines;
					result.set( true, true );
				} else if ( e.key.keysym.sym == SDLK_z ) {
					_visualizeORCA = !_visualizeORCA;
					_visNbrID = 0;
					result.set( true, true );
				} else if ( e.key.keysym.sym == SDLK_UP ) {
					if ( _visualizeORCA && this->_selected ) {
						const Agent * agt = dynamic_cast< const Agent * >( this->_selected->getAgent() );
						++_visNbrID;
						if ( _visNbrID >= agt->_nearAgents.size() ) _visNbrID = 0;
						result.set( true, true );
					}
				} else if ( e.key.keysym.sym == SDLK_DOWN ) {
					if ( _visualizeORCA && this->_selected ) {
						const Agent * agt = dynamic_cast< const Agent * >( this->_selected->getAgent() );
						if ( _visNbrID == 0 ) _visNbrID = agt->_nearAgents.size() - 1;
						else --_visNbrID;
						result.set( true, true );
					}
				}
			}
		}
	}
	return result;
}

////////////////////////////////////////////////////////////////

template< class Agent >
void ORCATypeAgentContext< Agent >::draw3DGL( bool select ) {
	Menge::BaseAgentContext::draw3DGL( select );
	if ( !select && this->_selected ) {
		glPushAttrib( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_LINE_BIT | GL_POLYGON_BIT );
		glDepthMask( GL_FALSE );
		glDisable( GL_DEPTH_TEST );
		const Agent * agt = dynamic_cast< const Agent * >( this->_selected->getAgent() );
		drawORCALines( agt );
		visORCA( agt );
		glPopAttrib();
	}
}

////////////////////////////////////////////////////////////////

template< class Agent >
std::string ORCATypeAgentContext< Agent >::agentText( const Menge::Agents::BaseAgent * agt ) const {
	const Agent * agent = dynamic_cast< const Agent * >( agt );
	std::string m = Menge::BaseAgentContext::agentText( agent );
	std::stringstream ss;
	ss << std::setiosflags(std::ios::fixed) <<  std::setprecision( 2 );
	
	ss << "\n_________________________";
	ss << "\nDraw OR(C)A lines";
	if ( _showOrcaLines ) {
		const size_t LINE_COUNT = agent->_orcaLines.size();
		const size_t AGT_COUNT = agent->_nearAgents.size();
		const size_t OBST_COUNT = LINE_COUNT - AGT_COUNT;
		ss << "\n    " << OBST_COUNT << " obstacle lines";
		ss << "\n    " << AGT_COUNT << " agent lines";
	}
	ss << "\nVisuali(z)e ORCA";
	if ( _visualizeORCA ) {
		if ( agent->_nearAgents.size() == 0 ) {
			ss << "\n    No nearby agents.";
		} else {
			size_t id = (agent->_nearAgents[_visNbrID].agent)->_id;
			ss << "\n    Showing agent: " << id << " (up/down arrow to change)";
		}
	}
	return m + ss.str();
}

////////////////////////////////////////////////////////////////

template< class Agent >
void ORCATypeAgentContext< Agent >::drawHalfPlane( const Menge::Math::Line & line, const Vector2 & pos, float r, float g, float b, float Y ) const {
	const float DIST = 35.f;
	Vector2 norm( -line._direction.y(), line._direction.x() );
	Vector2 p0 = line._point + line._direction * DIST + pos;
	Vector2 p1 = p0 - norm * DIST;
	Vector2 p2 = p1 - line._direction * ( 2 * DIST );
	Vector2 p3 = p2 + norm * DIST;

	glColor4f( r, g, b, 0.1f );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glEnable( GL_BLEND );

	glBegin( GL_QUADS );
	glVertex3f( p0.x(), Y, p0.y() );
	glVertex3f( p1.x(), Y, p1.y() );
	glVertex3f( p2.x(), Y, p2.y() );
	glVertex3f( p3.x(), Y, p3.y() );
	glEnd();
	glDisable( GL_BLEND );

	glBegin( GL_LINES );
	glVertex3f( p0.x(), Y, p0.y() );
	glVertex3f( p3.x(), Y, p3.y() );
	glEnd();
}

////////////////////////////////////////////////////////////////

template< class Agent >
void ORCATypeAgentContext< Agent >::drawORCALines( const Agent * agent ) const {
	if ( _showOrcaLines && this->_selected ) {
		Agent * agt = const_cast< Agent * >( agent );
		agt->computeORCALines();
		const size_t LINE_COUNT = agt->_orcaLines.size();
		const size_t FIRST_AGENT = LINE_COUNT - agt->_nearAgents.size();
		const float DIST = 35.f;

		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glEnable( GL_BLEND );
		// Obstacle color
		glColor4f( 0.75f, 0.75f, 0.75f, 0.1f );
		glBegin( GL_QUADS );
		for ( size_t i = 0; i < LINE_COUNT; ++i ) {
			// Agent color
			if ( i == FIRST_AGENT ) {
				glColor4f( 1.f, 0.f, 0.f, 0.1f );
			}
			const Menge::Math::Line & line = agt->_orcaLines[ i ];
			// Find the nearest point on the orca line to the agent -- use that as the center
			Vector2 norm( -line._direction.y(), line._direction.x() );
			float t = line._direction * ( -line._point );
			Vector2 nearPt = line._point + t * line._direction;
			Vector2 p0 = nearPt + line._direction * DIST + agt->_pos;
			Vector2 p1 = p0 - norm * DIST;
			Vector2 p2 = p1 - line._direction * ( 2 * DIST );
			Vector2 p3 = p2 + norm * DIST;

			glVertex3f( p0.x(), this->Y, p0.y() );
			glVertex3f( p1.x(), this->Y, p1.y() );
			glVertex3f( p2.x(), this->Y, p2.y() );
			glVertex3f( p3.x(), this->Y, p3.y() );
		}
		glEnd();
		glDisable( GL_BLEND );

		glColor4f( 0.75f, 0.75f, 0.75f, 0.1f );
		glBegin( GL_LINES );
		for ( size_t i = 0; i < LINE_COUNT; ++i ) {
			if ( i == FIRST_AGENT ) {
				glColor4f( 1.f, 0.f, 0.f, 0.1f );
			}
			const Menge::Math::Line & line = agt->_orcaLines[ i ];
			float t = line._direction * ( -line._point );
			Vector2 nearPt = line._point + t * line._direction;
			Vector2 p0 = nearPt + line._direction * DIST + agt->_pos;
			Vector2 p1 = nearPt - line._direction * DIST + agt->_pos;
			glVertex3f( p0.x(), this->Y, p0.y() );
			glVertex3f( p1.x(), this->Y, p1.y() );
		}
		glEnd();
		// Label the orca lines from agents
		glColor4f( 1.f, 0.f, 0.f, 1.f );
		for ( size_t i = FIRST_AGENT; i < LINE_COUNT; ++i ) {
			std::stringstream ss;
			const Menge::Agents::BaseAgent * nbr = agent->_nearAgents[ i - FIRST_AGENT ].agent;
			ss << nbr->_id;
			Vector2 d = agent->_orcaLines[ i ].nearestPt( Vector2(0.f,0.f) );
			Vector2 p = d + agent->_pos;
			this->writeTextRadially( ss.str(), p, d, true );
			this->writeAlignedText( ss.str(), nbr->_pos, Menge::SceneGraph::TextWriter::CENTERED, true );
		}
	}
}

////////////////////////////////////////////////////////////////

template< class Agent >
void ORCATypeAgentContext< Agent >::visORCA( const Agent * agt ) const {
	if ( _visualizeORCA && this->_selected ) {
		if ( agt->_nearAgents.size() > 0 ) {
			Vector2 velPref = agt->_velPref.getPreferredVel();
			const float RAD_TO_DEG = 180.f * 3.1415927f;
			glColor3f( 0.1f, 1.f, 0.1f );
			Agent * agent = const_cast< Agent * >( agt );
			agent->computeORCALines();
			const Agent * nbr = static_cast< const Agent * >( agent->_nearAgents[ _visNbrID ].agent );
			float R = agent->_radius + nbr->_radius;
			Vector2 disp = nbr->_pos - agent->_pos;
			float dist = abs( disp );
			Vector2 dir = disp / dist;
			Vector2 perp( -dir.y(), dir.x() );

			// Compute the tangent portions of the minkowski sum
			float cosPhi = R / dist;
			float sinPhi = sqrtf( 1 - cosPhi * cosPhi );
			float cx = cosPhi * -dir.x();
			float sx = sinPhi * -dir.x();
			float cy = cosPhi * -dir.y();
			float sy = sinPhi * -dir.y();

			Vector2 r0 = disp + R * Vector2( cx - sy, sx + cy );
			Vector2 l0 = disp + R * Vector2( cx + sy, -sx + cy );
			// modify the positions of r0 and l0 so that they project onto the center
			//
			float l = dist / ( r0 * dir );
			r0 *= l;
			l0 *= l;
			r0 += agent->_pos;
			l0 += agent->_pos;

			// What's the closest circle?
			const float TAU = agent->_timeHorizon;
			float minVel = dist / TAU;
			float Rmin = R / TAU;
			Vector2 center( agent->_pos + dir * minVel );
			// First, draw leading circle
			glPushMatrix();
			glTranslatef( center.x(), this->Y, center.y() );
			Menge::SceneGraph::Circle::drawCircle( Rmin, 0.1f, 1.f, 0.1f, 0.75f, GL_LINE );
			glPopMatrix();

			Vector2 r1 = center + Rmin * Vector2( cx - sy, sx + cy );
			Vector2 l1 = center + Rmin * Vector2( cx + sy, -sx + cy );

			glBegin( GL_LINES );
			glVertex3f( r0.x(), this->Y, r0.y() );
			glVertex3f( r1.x(), this->Y, r1.y() );
			glVertex3f( l0.x(), this->Y, l0.y() );
			glVertex3f( l1.x(), this->Y, l1.y() );
			glEnd();

			// Use right of way to compute velocities
			float row = agent->_priority - nbr->_priority;
			Vector2 agtVel = agent->_vel;
			Vector2 nbrVel = nbr->_vel;
			Vector2 nbrVelPref = nbr->_velPref.getPreferredVel();
			if ( row > 0.f ) {
				// agent's advantage
				row = row > 1.f ? 1.f : row;
				if ( dir * velPref > dir * agent->_vel ) {
					agtVel = velPref * row + ( 1.f - row ) * agent->_vel;
				}
			} else if ( row < 0.f ) {
				// nbr's advantage
				row = row < -1.f ? 1.f : -row;
				if ( dir * nbrVelPref < dir * nbr->_vel ) {
					nbrVel = nbrVelPref * row + ( 1.f - row ) * nbr->_vel;
				}
			}
			
			// Other guy's velocity
			glColor3f( 0.1f, 0.1f, 0.8f );
			glBegin( GL_LINES );
			glVertex3f( nbr->_pos.x(), this->Y, nbr->_pos.y() );
			glVertex3f( nbr->_pos.x() + nbrVel.x(), this->Y, nbr->_pos.y() + nbrVel.y() );
			glEnd();
			this->writeTextRadially( "v_j", nbr->_pos + nbrVel, nbrVel, true);

			// My velocity
			glColor3f( 0.1f, 0.8f, 0.1f );
			glBegin( GL_LINES );
			glVertex3f( agent->_pos.x(), this->Y, agent->_pos.y() );
			glVertex3f( agent->_pos.x() + agtVel.x(), this->Y, agent->_pos.y() + agtVel.y() );
			glEnd();
			this->writeTextRadially( "v_i", agent->_pos + agtVel, agtVel, true );

			// Relative velocity
			glColor3f( 0.1f, 0.8f, 0.8f );
			glBegin( GL_LINES );
			Vector2 rel = agtVel - nbrVel;
			glVertex3f( agent->_pos.x(), this->Y, agent->_pos.y() );
			glVertex3f( agent->_pos.x() + rel.x(), this->Y, agent->_pos.y() + rel.y() );
			glEnd();
			this->writeTextRadially( "v_ij", agent->_pos + rel, rel, true );


			// Draw the ORCA line
			// Determine which line it is
			size_t NBR_COUNT = agent->_nearAgents.size();
			size_t FIRST_NBR = agent->_orcaLines.size() - NBR_COUNT;
			drawORCALine( agent, agent->_orcaLines[ FIRST_NBR + _visNbrID ], true );

			// optimized velocity in transformed space
			drawOptVelocity( agent );
		}
	}
}

////////////////////////////////////////////////////////////////

template< class Agent >
void ORCATypeAgentContext< Agent >::drawORCALine( const Agent * agent, const Menge::Math::Line & line, bool isAgent ) const {
	if ( isAgent ) {
		drawHalfPlane( line, agent->_pos, 1.f, 0.f, 0.f, this->Y );
	} else {
		drawHalfPlane( line, agent->_pos, 0.75f, 0.75f, 0.75f, this->Y );
	}
}

////////////////////////////////////////////////////////////////

template< class Agent >
void ORCATypeAgentContext< Agent >::drawOptVelocity( Agent * agent ) const {
	// Draw the optimized velocity (transformed and untransformed
	agent->computeNewVelocity();
	// NORMAL space
	glPushAttrib( GL_POINT_BIT );
	glPointSize( 3.f );
	glColor3f( 0.2f, 0.2f, 1.f );
	glBegin( GL_POINTS );
	glVertex3f( agent->_pos.x() + agent->_velNew.x(), this->Y, agent->_pos.y() + agent->_velNew.y() );
	glEnd();
	this->writeTextRadially( "  v_new  ", agent->_pos + agent->_velNew, agent->_velNew, true );
}


#endif // __ORCA_TYPE_AGENT_CONTEXT_H__