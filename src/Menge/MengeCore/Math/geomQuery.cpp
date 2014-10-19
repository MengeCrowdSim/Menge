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

#include "geomQuery.h"
#include "consts.h"

namespace Menge {

	////////////////////////////////////////////////////////////////

	const float INFTY = 1e6f;
	const float PI = (float)(asin( 1.0f ) * 2.f);
	const float TWOPI = PI * 2.f;
	const float HALFPI = PI * 0.5f;
	const float DEG_TO_RAD = PI / 180.0f;
	const float RAD_TO_DEG = 180.0f / PI;
	const float EPS = 0.00001f;

	namespace Math {

		////////////////////////////////////////////////////////////////

		// determines the time to collision of a ray from the origin with a circle (center, radius)
		//	Returns an "infinity" style number if no collision
		float rayCircleTTC( const Vector2 & dir, const Vector2 & center, float radius ) {
			float a = absSq( dir );
			float b = -2 * ( dir * center );
			float c = absSq( center ) - ( radius * radius );
			float discr = b * b - 4 * a * c;
			if ( discr < 0.f ) {
				return INFTY;
			}
			const float sqrtDiscr = sqrtf( discr );
			float t0 = ( -b - sqrtDiscr ) / ( 2.f * a );
			float t1 = ( -b + sqrtDiscr ) / ( 2.f * a );
			// If the points of collision have different signs, it means I'm already colliding
			if (( t0 < 0.f && t1 > 0.f ) ||
				( t1 < 0.f && t0 > 0.f ) ) return 0.f;
			if ( t0 < t1 && t0 > 0.f ) return t0;
			else if ( t1 > 0.f ) return t1;	
			else return INFTY;
		}

		////////////////////////////////////////////////////////////////

		// Computes the spherical linear interpolation between two vectors
		//	the result is (conceptually) (1-t)*p0 + t*p1 
		//	sinTheta is the sine of the angle between p1 and p1
		Vector2 slerp( float t, const Vector2 & p0, const Vector2 & p1, float sinTheta ) {
			float theta = asin( sinTheta );
			float t0 = sin( (1 - t ) * theta ) / sinTheta;
			float t1 = sin( t * theta ) / sinTheta;
			return p0 * t0 + p1 * t1;
		}
	}	// namespace Math
}	// namespace Menge