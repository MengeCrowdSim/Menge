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
 *  @file       PedVOAgentContext.h
 *  @brief      A basic context for interacting with and displaying
 *				PedVO agent parameters
 */

#ifndef __PEDVO_AGENT_CONTEXT_H__
#define	__PEDVO_AGENT_CONTEXT_H__

#include "../Orca/ORCATypeAgentContext.h"
#include "PedVOAgent.h"

namespace PedVO {
	/*!
	 *	@brief		Declaration of ORCA-type agent context for PedVO agents.
	 */
	typedef ORCATypeAgentContext< Agent > PedVOAgentContext;
}

////////////////////////////////////////////////////////////////
//			Implementation of PedVOAgentContext
////////////////////////////////////////////////////////////////

// Specialization
template <>
std::string ORCATypeAgentContext< PedVO::Agent >::contextName() const { return "PedVO"; }

////////////////////////////////////////////////////////////////

/*!
 *	@brief		Draw the optimized velocity for the current set of orca lines
 *
 *	@param		agent		A pointer to the agent for which the velocity is drawn
 *							computeNewVelocity will be called on the agent
 */
template<>
inline void ORCATypeAgentContext< PedVO::Agent >::drawOptVelocity( PedVO::Agent * agent ) const {
	// Draw the optimized velocity (transformed and untransformed
	agent->computeNewVelocity();
	glPushAttrib( GL_POINT_BIT );
	glPointSize( 3.f );
	// NORMAL space
	glColor3f( 0.2f, 0.2f, 1.f );
	glBegin( GL_POINTS );
	glVertex3f( agent->_pos.x() + agent->_velNew.x(), Y, agent->_pos.y() + agent->_velNew.y() );
	glEnd();
	writeTextRadially( "  v_new  ", agent->_pos + agent->_velNew, agent->_velNew, true );
	
	Vector2 prefDir( agent->_velPref.getPreferred() );
	Vector2 n( -prefDir.y(), prefDir.x() );
	float vx = agent->_velNew * prefDir;
	float vy = agent->_velNew * n;
	vy /= agent->_turningBias;
	glColor3f( 1.f, 0.1f, 1.f );
	glBegin( GL_POINTS );
	glVertex3f( agent->_pos.x() + vx, Y, agent->_pos.y() + vy );
	glEnd();
	Vector2 vn( vx, vy );
	writeTextRadially( "  v_new^x", agent->_pos + vn, vn, true );
	glPopAttrib();
}

////////////////////////////////////////////////////////////////

/*!
 *	@brief		Draws the given ORCA line for the given agent
 *
 *	@param		agent		A pointer to the agent to whom this line applies
 *	@param		line		The actual line
 *	@param		isAgent		A boolean reporting if the orca line comes from an agent.
 *							true --> agent, false --> obstacle
 */
template<>
void ORCATypeAgentContext< PedVO::Agent >::drawORCALine( const PedVO::Agent * agent, const Menge::Math::Line & line, bool isAgent ) const {
	if ( agent->_turningBias != 1.f ) {
		// Transform line into new line
		Menge::Math::Line lEuclid;	// the line transformed, fully, back into Euclidian space
		lEuclid._point = line._point;
		lEuclid._direction = line._direction;
		Menge::Math::Line lAffine;	// the line (with scale) rotated back into Euclidian
		float prefSpeed = agent->_velPref.getSpeed();
		// Transformation is dependent on prefSpeed being non-zero
		bool rotated = false;
		if ( prefSpeed > Menge::EPS ) {
			Vector2 prefDir( agent->_velPref.getPreferred() );
			Vector2 n( prefDir.y(), prefDir.x() );
			// rotate and scale all of the lines
			float turnInv = 1.f / agent->_turningBias;

			// scale
			Vector2 p( line._point.x(), line._point.y() * agent->_turningBias );
			Vector2 d ( line._direction.x(), line._direction.y() * agent->_turningBias );
			// rotate
			float px = p * prefDir;
			float py = p * n;
			float dx = d * prefDir;
			float dy = d * n ;
			// set
			lEuclid._point.set( px, py );
			lEuclid._direction.set( dx, dy );

			px = line._point * prefDir;
			py = line._point * n;
			dx = line._direction * prefDir;
			dy = line._direction * n ;
			lAffine._point.set( px, py );
			lAffine._direction.set( dx, dy );
			rotated = true;
		}
		if ( rotated ) {
			if ( isAgent ) {
				drawHalfPlane( lAffine, agent->_pos, 1.f, 0.5f, 0.f, Y );
				if ( ! _showOrcaLines ) drawHalfPlane( lEuclid, agent->_pos, 1.f, 0.f, 0.f, Y );
			} else {
				drawHalfPlane( lAffine, agent->_pos, 0.5f, 0.5f, 0.5f, Y );
				if ( ! _showOrcaLines ) drawHalfPlane( lEuclid, agent->_pos, 0.75f, 0.75f, 0.75f, Y );
			}
		} else {
			if ( isAgent ) {
				drawHalfPlane( lEuclid, agent->_pos, 1.f, 0.f, 0.f, Y );
			} else {
				drawHalfPlane( lEuclid, agent->_pos, 0.75f, 0.75f, 0.75f, Y );
			}
		}
		
	} else {
		if ( isAgent ) {
			drawHalfPlane( line, agent->_pos, 1.f, 0.f, 0.f, Y );
		} else {
			drawHalfPlane( line, agent->_pos, 0.75f, 0.75f, 0.75f, Y );
		}
	}
}

#endif	// __PEDVO_AGENT_CONTEXT_H__
