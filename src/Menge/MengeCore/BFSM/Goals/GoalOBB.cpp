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

#include "MengeCore/BFSM/Goals/GoalOBB.h"

#include "MengeCore/Agents/PrefVelocity.h"

#if 0
#include "graphCommon.h"
#endif

namespace Menge  {

	namespace BFSM {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of OBBGoal
		/////////////////////////////////////////////////////////////////////
#if 0
		void OBBGoal::drawGLGeometry() const{
			OBBShape * obb = static_cast<OBBShape *>(_geometry);
			Vector2 X = obb->getXBasis();
			Vector2 Y = obb->getYBasis();
			Vector2 size = obb->getSize();
			Vector2 pivot = obb->getPivot();

			Vector2 c( size.x(), 0.f );
			Vector2 c1( c * X, c * Y );
			c.set( size );
			Vector2 c2( c * X, c * Y );
			c.set( 0.f, size.y() );
			Vector2 c3( c * X, c * Y );

			glPushMatrix();
			glTranslatef( pivot.x(), 0.f, pivot.y() );
			glBegin( GL_POLYGON );
				glVertex3f( 0.f, 0.f, 0.f );
				glVertex3f( c1.x(), 0.f, c1.y() );
				glVertex3f( c2.x(), 0.f, c2.y() );
				glVertex3f( c3.x(), 0.f, c3.y() );
				glVertex3f( 0.f, 0.f, 0.f );
			glEnd();
			glPopMatrix();
		}
#endif
		/////////////////////////////////////////////////////////////////////
		//                   Implementation of OBBGoalFactory
		/////////////////////////////////////////////////////////////////////

		bool OBBGoalFactory::setFromXML( Goal * goal, TiXmlElement * node,
										 const std::string & behaveFldr ) const {
			
			OBBGoal * obbGoal = dynamic_cast< OBBGoal * >( goal );
			assert( obbGoal != 0x0 &&
					"Trying to set OBB goal attributes on an incompatible object." );
			if (!GoalFactory::setFromXML(obbGoal, node, behaveFldr)) return false;

			// rely on createOBB to parse errors
			OBBShape * geometry = createOBB(node);
			if (geometry != 0x0) {
				goal->setGeometry(geometry);
				return true;
			}
			return false;
		}
		
		/////////////////////////////////////////////////////////////////////

		
	}	// namespace BFSM
}	// namespace Menge
