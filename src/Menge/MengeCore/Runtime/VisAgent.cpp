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

#include "VisAgent.h"
#include "BaseAgent.h"
#include "shapes.h"

namespace Menge {

	/////////////////////////////////////////////////////////////////////////////
	//                     Implementation of VisAgent
	/////////////////////////////////////////////////////////////////////////////

	VisAgent::VisAgent( Agents::BaseAgent * agent ):SceneGraph::GLNode(), SceneGraph::Selectable(), _agent(agent) {
		const Vector2 & pos = _agent->_pos;
		_pos.set( pos.x(), 0.f, pos.y() );
	}

	/////////////////////////////////////////////////////////////////////////////

	void VisAgent::drawGL( bool select ) {
		float r=0.5f, g=0.5f, b=0.5f;
		if ( select ) {
			loadSelectName();
		} else {
			getColor( r, g, b );
		}
		float radius = _agent->_radius;
		glPushMatrix();
		glTranslatef( _pos.x(), _pos.y(), _pos.z() );
		SceneGraph::Cylinder::drawCylinder( radius, 1.72f, r, g, b, 1.f );
		
		glPopMatrix();
	}

	///////////////////////////////////////////////////////////////////////////////

	void VisAgent::getColor( float & r, float & g, float & b ) {
		if ( _selected ) {
			r = 1.f;
			g = 1.f;
			b = 1.f;
		} else {
			// TODO: this is bad.  I only support six classes 
			//		Ultimately, replace this with a class that determines colors based
			//		on arbitrary rules
			size_t colorClass = _agent->_class % 6;
			switch( colorClass ) {
				case 0:
					r = 0.9f;	// red
					g = 0.1f;
					b = 0.1f;
					break;
				case 1:
					r = 0.25f;	// blue
					g = 0.25f;
					b = 0.9f;
					break;
				case 2:
					r = 0.1f;	// green
					g = 0.9f;
					b = 0.1f;
					break;
				case 3:
					r = 0.9f;	// orange-yellow
					g = 0.75f;
					b = 0.1f;
					break;
				case 4:
					r = 0.25f;	// cyan
					g = 0.9f;
					b = 0.9f;	
					break;
				case 5:
					r = 0.9f;	// magenta
					g = 0.1f;
					b = 0.9f;
				default:
					break;
			}
		}
	}
}	// namespace Menge