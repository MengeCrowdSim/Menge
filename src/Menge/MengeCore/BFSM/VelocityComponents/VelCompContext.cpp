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

#include "VelCompContext.h"
#include "BaseAgent.h"
#include "PrefVelocity.h"
#include "VelComponent.h"
#include "shapes.h"
#include "Goals/Goal.h"
#include <sstream>
#include "TextWriter.h"
#include <iomanip>

namespace Menge {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of VelCompContext
		/////////////////////////////////////////////////////////////////////

		std::string VelCompContext::getUIText( const std::string & indent ) const {
			std::stringstream ss;
			ss << indent << "No context defined";
			return ss.str();
		}

		/////////////////////////////////////////////////////////////////////

		void VelCompContext::drawPrefVel( const Agents::PrefVelocity & pVel, const Vector2 & pos, float elevation ) {

			glPushAttrib( GL_LINE_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT | GL_POLYGON_BIT );
			glDepthMask( GL_FALSE );
			glDisable( GL_DEPTH_TEST );
			const float length = pVel.getSpeed();
			glLineWidth( 2.f );
			if ( pVel.hasArea() ) {
				// draw solid angle
				// Display contraction normal
				const Vector2 & left = pVel.getLeft();
				const Vector2 & right = pVel.getRight();
				float theta = acos( left * right );
				
				glColor3f( 0.2f, 1.0f, 0.2f );
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

				float sTheta = atan2( right.y(), right.x() );
				const int SAMPLES = int( theta / (5.f * DEG_TO_RAD ) + 0.5f );
				const float dTheta = theta / SAMPLES;
				glPushMatrix();
				glTranslatef( pos.x(), elevation, pos.y() );
				glScalef( length, length, length );
				glBegin( GL_POLYGON );
				glVertex3f( 0.f, 0.f, 0.f );
				glVertex3f( right.x(), 0.f, right.y() );
				for ( int i = 1; i <= SAMPLES; ++i ) {
					float angle = sTheta + i * dTheta;
					float x = cos( angle );
					float y = sin( angle );
					glVertex3f( x, 0.f, y );
				}
				glEnd();
				
				glPopMatrix();
			}

			// draw preferred direction
			Vector2 dir( pVel.getPreferred() );
			glPushMatrix();
			glTranslatef( pos.x(), elevation, pos.y() );
			float angle = atan2( -dir.y(), dir.x() ) * RAD_TO_DEG;
			glRotatef( angle, 0.f, 1.f, 0.f );
			glColor3f( 1.f, 0.25f, 0.25f );
			glLineWidth( 3.f );
			const float SIZE = 0.1f;
			const float HALF_SIZE = SIZE * 0.5f;
			glBegin( GL_LINE_STRIP );
			glVertex3f( 0.f, 0.f, 0.f );
			glVertex3f( length - SIZE, 0.f, 0.f );
			glVertex3f( length - SIZE, 0.f, -HALF_SIZE );
			glVertex3f( length, 0.f, 0.f );
			glVertex3f( length - SIZE, 0.f, HALF_SIZE );
			glVertex3f( length - SIZE, 0.f, 0.f );
			glEnd();
			glPopMatrix();

			// draw target point
			glPointSize( 5.f );
			glColor3f( 0.3f, 1.f, 1.f );
			glBegin( GL_POINTS );
			glVertex3f( pVel.getTarget()._x, elevation, pVel.getTarget()._y );
			glEnd();
			glPopAttrib();
		}

		/////////////////////////////////////////////////////////////////////
		
		void VelCompContext::drawGoal( const Vector2 & goalPoint, const Agents::BaseAgent * agent ) const {
			drawGoal( Vector3( goalPoint.x(), 0.f, goalPoint.y() ), agent->_radius );
		}

		/////////////////////////////////////////////////////////////////////
		
		void VelCompContext::drawGoal( const Vector3 & goalPoint, float agtRadius ) const {
			glPushAttrib( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			glDepthMask( GL_FALSE );
			glPushMatrix();
			const float R = agtRadius / 5.f;
			glTranslatef( goalPoint.x(), goalPoint.y(), goalPoint.z() );
			SceneGraph::Circle::drawCircle( R * 5.f, 1.f, 0.f, 0.f, 1.f );
			SceneGraph::Circle::drawCircle( R * 4.f, 1.f, 1.f, 1.f, 1.f );
			SceneGraph::Circle::drawCircle( R * 3.f, 1.f, 0.f, 0.f, 1.f );
			SceneGraph::Circle::drawCircle( R * 2.f, 1.f, 1.f, 1.f, 1.f );
			SceneGraph::Circle::drawCircle( R * 1.f, 1.f, 0.f, 0.f, 1.f );
			glPopMatrix();
			glPopAttrib();
		}

	}	// namespace BFSM
}	// namespace Menge