#include "MengeVis/Runtime/GoalRenderer/CircleGoalRenderer.h"

#include "MengeCore/BFSM/Goals/GoalCircle.h"
#include "MengeCore/Math/Geometry2D.h"
#include "MengeVis/Runtime/VisElementException.h"
#include "MengeVis/SceneGraph/graphCommon.h"
#include "MengeVis/SceneGraph/shapes.h"

namespace MengeVis {
	namespace Runtime {
		namespace GoalVis {

			using Menge::BFSM::CircleGoal;
			using Menge::Math::CircleShape;
			using Menge::Math::Vector2;

			/////////////////////////////////////////////////////////////////////
			//					Implementation of CircleGoalRenderer
			/////////////////////////////////////////////////////////////////////

			std::string CircleGoalRenderer::getElementName() const {
				return CircleGoal::NAME;
			}

			/////////////////////////////////////////////////////////////////////

			void CircleGoalRenderer::doDrawGeometry() const {
				CircleGoal * goal = dynamic_cast<CircleGoal *>( _goal );
				if ( goal == 0x0 ) {
					throw VisElementException( "Attempting to draw goal of type " +
											   _goal->getStringId() + " with circle goal renderer." );
				}
				const CircleShape * shape = static_cast<const CircleShape *>( goal->getGeometry() );
				glPushMatrix();
				const Vector2 & pos = shape->getCenter();
				float radius = shape->getRadius();
				glTranslatef( pos.x(), 0.f, pos.y() );
				glScalef( radius, radius, radius );
				SceneGraph::Circle::drawUnit();
				glPopMatrix();
			}
		}	// namespace GoalVis
	}	// namespace Runtime
}	// namespace MengeVis