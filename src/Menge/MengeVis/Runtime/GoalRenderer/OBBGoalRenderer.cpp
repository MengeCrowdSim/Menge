#include "MengeVis/Runtime/GoalRenderer/OBBGoalRenderer.h"

#include "MengeCore/BFSM/Goals/GoalOBB.h"
#include "MengeCore/Math/Geometry2D.h"
#include "MengeVis/Runtime/VisElementException.h"
#include "MengeVis/SceneGraph/graphCommon.h"

namespace MengeVis {
	namespace Runtime {
		namespace GoalVis {

			using Menge::BFSM::OBBGoal;
			using Menge::Math::OBBShape;
			using Menge::Math::Vector2;

			/////////////////////////////////////////////////////////////////////
			//					Implementation of OBBGoalRenderer
			/////////////////////////////////////////////////////////////////////

			std::string OBBGoalRenderer::getElementName() const {
				return OBBGoal::NAME;
			}

			/////////////////////////////////////////////////////////////////////

			void OBBGoalRenderer::doDrawGeometry() const {
				OBBGoal * goal = dynamic_cast<OBBGoal *>( _goal );
				if ( goal == 0x0 ) {
					throw VisElementException( "Attempting to draw goal of type " +
											   _goal->getStringId() + " with OBB goal renderer." );
				}
				const OBBShape * obb = static_cast<const OBBShape *>( goal->getGeometry() );
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
		}	// namespace GoalVis
	}	// namespace Runtime
}	// namespace MengeVis