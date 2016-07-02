/*

License

Menge
Copyright � and trademark � 2012-14 University of North Carolina at Chapel Hill. 
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

#include "PrefVelocity.h"
#include <cassert>

namespace Menge {

	namespace Agents {
		
		////////////////////////////////////////////////////////////////
		//					Implementation of PrefVelocity
		////////////////////////////////////////////////////////////////

		PrefVelocity::PrefVelocity(): _left(1.f,0.f), _right(1.f,0.f), _speed(1.f), _preferred(1.f,0.f), _target(0.f,0.f) {}

		////////////////////////////////////////////////////////////////

		PrefVelocity::PrefVelocity( const Vector2 & dir, float speed, const Vector2 & target ):_left(dir), _right(dir), _speed(speed), _preferred(dir), _target(target) {
		}

		////////////////////////////////////////////////////////////////

		PrefVelocity::PrefVelocity( const Vector2 & left, const Vector2 & right, const Vector2 & pref, float speed, const Vector2 & target ): _left(left), _right(right), _speed(speed), _preferred(pref), _target(target) {
		}

		////////////////////////////////////////////////////////////////

		PrefVelocity::PrefVelocity( const PrefVelocity & vel ): _left(vel._left), _right(vel._right), _speed(vel._speed), _preferred(vel._preferred), _target(vel._target) {
		}

		////////////////////////////////////////////////////////////////

		PrefVelocity & PrefVelocity::operator=( const PrefVelocity & vel ) {
			// NOTE:
			//	This doesn't include the typical if ( this != &vel ) test.
			//	The reason for this is, it is considered highly unlikely that the user will
			//	ever do a = a (even indirectly), and, as such, it is deemed better to simply
			//	perform the copying cost instead of incurring the test for EVERY 
			//	other event.
			_left = vel._left;
			_right = vel._right;
			_speed = vel._speed;
			_preferred = vel._preferred;
			_target = vel._target;
			return (*this);
		}

		////////////////////////////////////////////////////////////////

		void PrefVelocity::setSpan( const Vector2 & left, const Vector2 & right, const Vector2 & preferred ) { 
			_left = left;
			_right = right;
			_preferred = preferred;
	#if _DEBUG
			const float SPAN_EPS = -1.e-4f;
			assert( det( right, left ) >= SPAN_EPS );
			assert( det( right, preferred )>= SPAN_EPS );
			assert( det( preferred, left ) >= SPAN_EPS );
	#endif
		}
				
	}	// namespace Agents
}	// namespace Menge
