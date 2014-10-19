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
 *	@file		EventEffect.h
 *	@brief		The definition of the event effect -- defines the response
 *				to a triggered response.
 */

#ifndef __EVENT_EFFECT_H__
#define __EVENT_EFFECT_H__

#include "CoreConfig.h"
#include "Element.h"

namespace Menge {
	// forward declaration
	class EventTarget;
	class EventEffectFactory;

	/*!
	 *	@brief		The definition of a response to an event. 
	 *
	 *	Event effects determine how the simulation changes due to a triggered
	 *	event.  The EventEffect is responsible for changing the simulation of the
	 *	event target (and optionally caching the original state).  Essentially,
	 *	the event effect is the operator, and the corresponding EventTarget is 
	 *	the operand.
	 */
	class MENGE_API EventEffect : public Element {
	public:
		/*!
		 *	@brief		Constructor.
		 */
		EventEffect(): Element() {}

		/*!
		 *	@brief		Reports if the given target is compatible with this effect.
		 *
		 *	Each effect can only work on certain types of targets.  This function 
		 *	reports if the given target works with this effect.
		 *
		 *	@param		target		The target instance to test.
		 *	@returns	True if the target is a valid argument to EventEffect::apply,
		 *				false, otherwise.
		 */
		virtual bool isCompatible( EventTarget * target ) = 0;

		/*!
		 *	@brief		Applies the effect to the simulation target.
		 *
		 *	Not all targets work with all effects.  The target passed here
		 *	must have previously passed the EventEffect::isCompatible test to
		 *	work.
		 *
		 *	@param		target		The target to apply the event to.
		 */
		virtual void apply( EventTarget * target ) = 0;

		/*!
		 *	@brief		Allows the event effect to finish initializing itself from its
		 *				parsed state to its running state.
		 *
		 *	@throws		EventException if there is a problem finalizing.
		 */
		virtual void finalize() {}

		friend class EventEffectFactory;

	};

}	// namespace Menge

#endif	// __EVENT_EFFECT_H__