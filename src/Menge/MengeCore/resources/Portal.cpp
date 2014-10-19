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

#include "Portal.h"
#include <cassert>
#include <iostream>

namespace Menge {

	/////////////////////////////////////////////////////////////////////
	//                   Implementation of Portal
	/////////////////////////////////////////////////////////////////////

	Portal::Portal( float x0, float y0, float x1, float y1 ) {
		set( x0, y0, x1, y1 );
	}

	/////////////////////////////////////////////////////////////////////

	void Portal::set( float x0, float y0, float x1, float y1 ) {
		_p0.set( x0, y0 );
		_p1.set( x1, y1 );
		_dir = _p1 - _p0;
		_mag = abs( _dir );
		_dir /= _mag;
	}

	/////////////////////////////////////////////////////////////////////

	void Portal::set( const Vector2 & p0, const Vector2 & p1  ) {
		_p0 = p0;
		_p1 = p1;
		_dir = _p1 - _p0;
		_mag = abs( _dir );
		_dir /= _mag;
	}

	/////////////////////////////////////////////////////////////////////

	Vector2 Portal::nearestPoint( const Vector2 & pos, float radius ) {
		assert( _mag > radius && "Agent's with radius bigger than the portal width can't pass through" );
		// Be smart about this
		//	If the position projects onto the portal, then simply find the closest point to the
		//		"effective" portal
		//  If the position lies to the left of p0
		//		We simply rotate the direction from end point to test position by the appropriate
		//			angle.  We do this as follows:
		//		d = || pos - p0 ||
		//		dHat = || pos - p0 || / d
		//		l = sqrtf( d^2 - radius^2 )
		//		cosTheta = r / d
		//		sinTheta = l / d
		//		gx = R * ( cosTheta * dHat.x + sinTheta * dHat.y ) + p0.x
		//		gy = R * ( cosTheta * dHat.y - sinTheta * dHat.x ) + p0.y
		//	If the position lies to the right of p0, it does the same computation, but with
		//		the opposite rotation

		// Does pos project onto the portal
		Vector2 disp = pos - _p0;
		float dp = disp * _dir;
		float mag = _mag - radius;
		if ( dp < radius || dp > mag ) {
			if ( dp > mag ) {
				disp = pos - _p1;
			}
			float d2 = absSq( disp );
			float R2 = radius * radius;
			if ( R2 > d2 ) {
				// Currently overlapping the the end point
				if ( dp < radius ) {
					return pos + _dir;
				} else {
					return pos - _dir;
				}
			}
			float d = sqrtf( d2 );
			float l = sqrtf( d2 - R2 );
			float cTheta = radius / d;
			float sTheta = l / d;
			float x, y;
			if ( dp < radius ) {
				x = cTheta * disp.x() + sTheta * disp.y();
				y = cTheta * disp.y() - sTheta * disp.x();
			} else {
				x = cTheta * disp.x() - sTheta * disp.y();
				y = cTheta * disp.y() + sTheta * disp.x();			
			}
			Vector2 goal( x, y );
			goal *= radius / d;
			if ( dp < radius ) {
				return goal + _p0;
			} else {
				return goal + _p1;
			}
		} else {
			return _p0 + _dir * dp;
		}
	}
}	// namespace Menge