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
 *	@file		Portal.h
 *	@brief		The definition of a portal - a line which spans a clear area
 *				between obstacles.
 */

#ifndef __PORTAL_H__
#define __PORTAL_H__

#include "mengeCommon.h"

namespace Menge {

	/*!
	 *	@brief		A simple portal definition.
	 *
	 *	A portal is simply a line segment - a region through which agents can pass.
	 */
	class Portal {
	public:
		/*!
		 *	@brief		Default constructor.
		 */
		Portal():_p0(0.f,0.f), _p1(0.f,0.f), _mag(0.f), _dir(1.f,0.f) {}

		/*!
		 *	@brief		Constructor specifying portal geometry.
		 *
		 *	@param		x0		The x-position of the first end point.
		 *	@param		y0		The y-position of the first end point.
		 *	@param		x1		The x-position of the second end point.
		 *	@param		y1		The y-position of the second end point.
		 */
		Portal( float x0, float y0, float x1, float y1 );

		/*!
		 *	@brief		Sets the geometry of the portal
		 *
		 *	@param		x0		The x-position of the first end point.
		 *	@param		y0		The y-position of the first end point.
		 *	@param		x1		The x-position of the second end point.
		 *	@param		y1		The y-position of the second end point.
		 */
		void set( float x0, float y0, float x1, float y1 );

		/*!
		 *	@brief		Sets the geometry of the portal
		 *
		 *	@param		p0		The position of the first end point.
		 *	@param		p1		The position of the second end point.
		 */
		void set( const Vector2 & p0, const Vector2 & p1 );

		/*!
		 *	@brief		Computes the neareset *clearable* point w.r.t. the portal
		 *
		 *	Given the position and radius of an agent, computes the best clearable goal point.
		 *	Clearable means that the straight-line path between the agent's current position
		 *	and that goal point does NOT intersect either end point of the goal.
		 *
		 *	This is important because we assume that the end points attach to obstacles and
		 *	if the end point intersects the path, that means the preferred velocity is 
		 *	leading the agent into a collision with the obstacle.
		 *
		 *	@param		pos		The position of the agent.
		 *	@param		radius		The radius of the agent.
		 *	@returns	The nearest *clearable* point.
		 */
		Vector2 nearestPoint( const Vector2 & pos, float radius );

		/*!
		 *	@brief		The first end point of the portal line segment.
		 */
		Vector2 _p0;
		
		/*!
		 *	@brief		The second end point of the portal line segment.
		 */
		Vector2 _p1;

		/*!
		 *	@brief		The distance between endpoints (i.e., length of the portal).
		 */
		float _mag;

		/*!
		 *	@brief		The direction from the first to the second end point of the portal.
		 */
		Vector2 _dir;
	};
}	// namespace Menge
#endif	// __PORTAL_H__