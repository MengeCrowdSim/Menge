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
 *	@file		PrefVelocity.h
 *	@brief		The definition of a preferred velocity.
 */

#ifndef __PREF_VELOCITY_H__
#define __PREF_VELOCITY_H__

#include "mengeCommon.h"

namespace Menge {

	namespace Agents { 

		/*!
		 *	@brief		The definition of a preferred velocity.
		 *
		 *	The preferred velocity is actually a space of velocities defined
		 *	by two things:
		 *		- a preferred speed
		 *		- a *span* of directions.
		 *	The span of directions represents a contiguous set of directions which
		 *	can be considered topologically equivalent with respect to the agent
		 *	reaching its goal.
		 *
		 *	For example, the shortest path may require an agent to pass down a hall way,
		 *	but whether the agent walks down the left, middle, or side is irrelevant -- all
		 *	three will get the agent to its final goal with negligible difference.
		 */
		class MENGE_API PrefVelocity {
		public:
			/*!
			 *	@brief		Default constructor
			 */
			PrefVelocity();

			/*!
			 *	@brief		Constructor for setting a span of a single direction.
			 *
			 *	@param		dir		The single direction.
			 *	@param		speed	The preferred speed.
			 *	@param		target	The target point from which the preferred direction arises.
			 */
			PrefVelocity( const Vector2 & dir, float speed, const Vector2 & target );

			/*!
			 *	@brief		Constructor for setting a full span.
			 *
			 *	@param		left	The "left"-most extent of the span (see _left).
			 *	@param		right	The "right"-most extent of the span (see _right).
			 *	@param		pref	The preferred direction (see _preferred).
			 *	@param		speed	The preferred speed.
			 *	@param		target	The target point from which the preferred direction arises.
			 */
			PrefVelocity( const Vector2 & left, const Vector2 & right, const Vector2 & pref, float speed, const Vector2 & target );

			/*!
			 *	@brief		Copy constructor.
			 *
			 *	@param		vel		An instance of preferred velocity.
			 */
			PrefVelocity( const PrefVelocity & vel );

			/*!
			 *	@brief		Assignment operator
			 *
			 *	@param		vel		Another instance of PrefVelocity whose values will be copied into this.
			 *	@returns	A reference to this preferred velocity.
			 */
			PrefVelocity & operator=( const PrefVelocity & vel );

			/*!
			 *	@brief		Returns the left extent of the span.
			 *
			 *	@returns	The left extent.
			 */
			inline Vector2 getLeft() const { return _left; }

			/*!
			 *	@brief		Returns the right extent of the span.
			 *
			 *	@returns	The right extent.
			 */
			inline Vector2 getRight() const { return _right; }

			/*!
			 *	@brief		Returns the preferred *direction* of the span.
			 *
			 *	@returns	The preferred direction.
			 */
			inline Vector2 getPreferred() const { return _preferred; }

			/*!
			 *	@brief		Returns the preferred *velocity" of the span.
			 *				This is the preferred direction at the velocity's speed.
			 *
			 *	@returns	A vector pointing in the preferred direction at the
			 *				given speed.
			 */
			inline Vector2 getPreferredVel() const { return _preferred * _speed; }

			/*!
			 *	@brief		Returns the speed of the preferred velocity.
			 *
			 *	@returns	The speed of the preferred velocity.
			 */
			inline float getSpeed() const { return _speed; }

			/*!
			 *	@brief		Sets the speed.
			 *
			 *	@param		speed	The speed.
			 */
			inline void setSpeed( float speed ) { _speed = speed; }

			/*!
			 *	@brief		Sets the preferred velocity to be a single velocity.
			 *
			 *	@param		dir		The single preferred direction.
			 */
			inline void setSingle( const Vector2 & dir ) { _left = _preferred = _right = dir; }

			/*!
			 *	@brief		Gets the target of the preferred velocity.
			 *
			 *	@returns	A singe point in space which corresponds to the preferred direction.
			 */
			inline Vector2 getTarget() const { return _target; }

			/*!
			 *	@brief		Sets the target of the preferred velocity.
			 *
			 *	@param		target		The preferred velocity's target.
			 */
			inline void setTarget( const Vector2 & target ) { _target = target; }

			/*!
			 *	@brief		Sets the preferred velocity span.
			 *
			 *	@param		left		The direction of the left-most extent of the arc.
			 *	@param		right		The direction of the right-most extent of the arc.
			 *	@param		preferred	The single most-preferred direction in the span.
			 */
			void setSpan( const Vector2 & left, const Vector2 & right, const Vector2 & preferred );

			/*!
			 *	@brief		Reports if the arc spans more than a single direction (by reporting 
			 *				whether the arc has area.)
			 *
			 *	@returns	A boolean indicating if the span has area (i.e. spans
			 *				more than a single direction.
			 */
			inline bool hasArea() const { return _left * _right < 1.f; }

		protected:
			/*!
			 *	@brief		The "left" extent of the preferred velocity span
			 *				(i.e. det( right, left ) >= 0).  This direction has
			 *				unit length.
			 */
			Vector2		_left;
			
			/*!
			 *	@brief		The "right" extent of the preferred velocity span
			 *				(i.e. det( right, left ) >= 0).  This direction has
			 *				unit length.
			 */
			Vector2		_right;

			/*!
			 *	@brief		The preferred speed (also the radius of the arc.
			 */
			float		_speed;

			/*!
			 *	@brief		The preferred direction.  All directions in the span
			 *				may be topologically equivalent, but this direction is
			 *				the "best".  This value must lie within the span
			 *				(i.e., det( right, preferred ) > 0 and det( preferred, left) > 0).
			 */
			Vector2		_preferred;

			/*!
			 *	@brief		The immediate goal point -- this corresponds to the preferred
			 *				direction.
			 */
			Vector2		_target;

		};

	}	// namespace Agents
}	// namespace Menge

#endif // __PREF_VELOCITY_H__