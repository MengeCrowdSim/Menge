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
 *	@file	EventTrigger.h
 *	@brief	The definition of the basic event trigger mechanism.
 */

#ifndef __EVENT_TRIGGER_H__
#define __EVENT_TRIGGER_H__

#include "CoreConfig.h"
#include "Element.h"

namespace Menge {
	/*!
	 *	@brief		The base class for event triggers.
	 *
	 *	An event trigger specifies the conditions under which an event is
	 *	triggered (allowing registered event handlers to respond).
	 */
	class MENGE_API EventTrigger : public Element {
	public:
		/*!
		 *	@brief		Constructor.
		 */
		EventTrigger() : Element(), _firePeriod(0.f), _lastFire(-1e6) {}

		/*!
		 *	@brief		Allows the trigger to finish initializing itself from its
		 *				parsed state to its running state.
		 *
		 *	@throws		EventException if there is a problem finalizing.
		 */
		virtual void finalize() {}

		/*!
		 *	@brief		Evaluates the condition to see if it has been met.
		 *
		 *	@returns	True if the condition has been met, false otherwise.
		 */
		bool conditionMet();

		/*!
		 *	@brief		Informs the trigger that the associated event effects
		 *				have been applied (i.e. the event has been fired).
		 */
		void fired();
		
	protected:

		/*!
		 *	@brief		Evaluates the condition to see if it has been met.
		 *
		 *	This must be implemented by all sub-classes.
		 *
		 *	@returns		True if the condition has been met, false otherwise.
		 */
		virtual bool testCondition() = 0;

		/*!
		 *	@brief		The minimum time between two successive firings (in simulation seconds).
		 */
		float	_firePeriod;

		/*!
		 *	@brief		The time of the last firing (global simulation time).
		 */
		float	_lastFire;
	};

}	// namespace Menge
#endif	// __EVENT_TRIGGER_H__