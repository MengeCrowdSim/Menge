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
 *  @file       geomQuery.h
 *  @brief      Various mathematical operations and queries on geometry
 */

#ifndef __GEOM_QUERY_H__
#define	__GEOM_QUERY_H__

#include "CoreConfig.h"
#include "Vector2.h"

namespace Menge {

	/*!
	 *	@namespace	Math
	 *	@brief		The namespace for math primitives for simulation and visualization.
	 */
	namespace Math {
		/*!
		 *	@brief		Computes the time to collision between a ray and a circle
		 *
		 *				This is a special-case test.  It assumes the ray originates
		 *				from the origin of the world.
		 *
		 *	@param		dir			A vector in R2 describing the direction (from the origin)
		 *							of the ray.  (Does not need to be normalized)
		 *	@param		center		A vector in R2 describing the position of the circle center.
		 *	@param		radius		A float.  The radius of the circle.
		 *	@returns	The expected "time" to collision ("infinity" if there is no collision).
		 */
		MENGE_API float rayCircleTTC( const Vector2 & dir, const Vector2 & center, float radius );

		/*!
		 *	@brief		Perform spherical linear interpolation between two vectors
		 *
		 *	@param		t			The blend parameter.  T lies in the interval [0, 1]
		 *	@param		p0			The first vector to interpolate (assumes ||p0|| = 1.0.
		 *	@param		p1			The first vector to interpolate (assumes ||p1|| = 1.0.
		 *	@param		sinTheta	The sine of the angle between the two vectors.
		 *	@returns	The interpolated vector.
		 */
		MENGE_API Vector2 slerp( float t, const Vector2 & p0, const Vector2 & p1, float sinTheta );
	}	// namespace Math

}	// namespace Menge
#endif	// __GEOM_QUERY_H__
