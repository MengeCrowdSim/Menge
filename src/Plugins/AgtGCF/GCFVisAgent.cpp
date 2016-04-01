#include "GCFVisAgent.h"
#include "GCFAgent.h"
#include "shapes.h"


namespace GCF {

	////////////////////////////////////////////////////////////////
	//			Implementation of GCFVisAgent
	////////////////////////////////////////////////////////////////

	GCFVisAgent::GCFVisAgent(Agents::BaseAgent * agent ) : VisAgent(agent) {
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
		Vector2 pos = agt->_pos;
		float x = pos.x();
		float y = pos.y();
		glPushMatrix();
		glTranslatef( x, 0.0, y );
		float angle = atan2( _agent->_orient.y(), _agent->_orient.x() ) * 180.f / 3.141597f;
		glRotatef( -angle, 0.f, 1.f, 0.f );
		float major = agt->_ellipse.getMajor();
		float minor = agt->_ellipse.getMinor();

		glScalef( major, 1.f, minor );
		Menge::SceneGraph::Cylinder::drawCylinder( 1.f, 1.72f, r, g, b, 1.f );

		glPopMatrix();
	}

}	// namespace GCF