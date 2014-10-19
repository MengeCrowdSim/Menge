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
 *	@file		EventTarget.h
 *	@brief		The definition of the base event target class.  Defines what
 *				an event effect operates on.
 */

#ifndef __EVENT_TARGET_H__
#define	__EVENT_TARGET_H__

#include "CoreConfig.h"
#include "Element.h"
#include "Core.h"


namespace Menge {
	// forward declaration
	namespace Agents {
		class BaseAgent;
	}

	/*!
	 *	@brief		The base class for all event targets.
	 *			
	 *	An event target defines the operand of an event effect. 
	 *	i.e., if the event effect defines how things change, the target
	 *	defines which elements undergo the effect.
	 */
	class MENGE_API EventTarget : public Element {
	public:
		/*!
		 *	@brief		Constructor.
		 */
		EventTarget() : Element(), _lastUpdate(-1.f) {}

		/*!
		 *	@brief		Allows the event target to finish initializing itself from its
		 *				parsed state to its running state.
		 *
		 *	@throws		EventException if there is a problem finalizing.
		 */	 
		virtual void finalize() {}

		/*!
		 *	@brief		Gives the target a chance to update its knowledge of the 
		 *				target entities.
		 *
		 *	If a sub-class needs to do particular computation to evaluate the target,
		 *	it should be implemented here.
		 */
		virtual void update() { _lastUpdate = SIM_TIME; }

	protected:
		/*!
		 *	@brief		The global time that the target was last updated.
		 */
		float	_lastUpdate;
	};

}	// namespace Menge
#endif	// __EVENT_TARGET_H__