#include "GCFVisAgent.h"
#include "GCFAgent.h"
#include "GCFSimulator.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeVis/SceneGraph/shapes.h"

namespace GCF {

	using MengeVis::Runtime::VisAgent;
	using Menge::Agents::BaseAgent;

	////////////////////////////////////////////////////////////////
	//			Implementation of GCFVisAgent
	////////////////////////////////////////////////////////////////

	VisAgent * GCFVisAgent::moveToClone() {
		VisAgent * agt = new GCFVisAgent();
		agt->setElement( _agent );
		_agent = 0x0;
		return agt;
	}

	////////////////////////////////////////////////////////////////

	void GCFVisAgent::drawGL( bool select ) {
		float r=0.5f, g=0.5f, b=0.5f;
		Agent * agt = (Agent*)_agent;
		if ( select ) {
			loadSelectName();
		} else {
			if ( _selected ) {
				r = 1.f;
				g = 1.f;
				b = 1.f;
			} else {
				// TODO: this is bad.  I only support four classes (5-infinity all look fifth)
				switch( agt->_class ) {
				case 0:
					r = 0.9f;
					g = 0.1f;
					b = 0.1f;
					break;
				case 1:
					r = 0.25f;
					g = 0.25f;
					b = 0.9f;
					break;
				case 2:
					r = 0.1f;
					g = 0.9f;
					b = 0.1f;
					break;
				case 3:
					r = 0.9f;
					g = 0.5f;
					b = 0.1f;
					break;
				default:
					break;
				}
			}
		}

		if ( Simulator::SPEED_COLOR ) {
			float speed = abs( agt->_vel);
			float frac = speed / agt->_prefSpeed;
			frac = frac > 1.f ? 1.f : frac;
			r = 1.f - frac;
			g = frac;
			b = 0.f;
		}

		const Vector2 & pos = agt->_ellipse.getCenter();
		float x = pos.x();
		float y = pos.y();
		glPushMatrix();
		glTranslatef( x, 0.0, y );
		const Vector2 & orient = agt->_ellipse.getOrientation();
		float angle = atan2( orient.y(), orient.x() ) * 180.f / 3.141597f;
		glRotatef( -angle, 0.f, 1.f, 0.f );
		float major = agt->_ellipse.getMajor();
		float minor = agt->_ellipse.getMinor();

		glScalef( major, 1.f, minor );
		MengeVis::SceneGraph::Cylinder::drawCylinder( 1.f, 1.72f, r, g, b, 1.f );

		glPopMatrix();
	}

	////////////////////////////////////////////////////////////////

	bool GCFVisAgent::doValidateAgent( const BaseAgent * agent ) {
		const Agent * agt = dynamic_cast<const Agent *>(agent);
		return agt != 0x0;
	}

}	// namespace GCF
