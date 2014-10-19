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
 *	@file		AgentEventTarget.h
 *	@brief		The definition of the event target for agents. 
 */

#ifndef __AGENT_EVENT_TARGET_H__
#define	__AGENT_EVENT_TARGET_H__

#include "Events/EventTarget.h"
#include "Events/EventTargetFactory.h"
#include <vector>

namespace Menge {
	/*!
	 *	@brief		The base class for all event targets which target agents.
	 *			
	 *	Event targets are fundamentally defined by what the target of the
	 *	effect is.  Each event target specifies a single kind of target.
	 *	This event target produces a set of one or more agents to effect.
	 */
	class MENGE_API AgentEventTarget : public EventTarget {
	public:
		
		/*!
		 *	@brief		Returns an iterator to the beginning of the target's elements.
		 */
		std::vector< Agents::BaseAgent * >::iterator begin() { return _elements.begin(); }

		/*!
		 *	@brief		Returns an iterator to the end of the target's elements.
		 */
		std::vector< Agents::BaseAgent * >::iterator end() { return _elements.end(); }

	protected:
		/*!
		 *	@brief		The agents targeted by the event effect.
		 */
		std::vector< Agents::BaseAgent * >	_elements;
	};

	/////////////////////////////////////////////////////////////////////
	
	/*!
	 *	@brief		Factory for the AgentEventTarget.
	 */
	class MENGE_API AgentEventTargetFactory : public EventTargetFactory {
	};
	
}	// namespace Menge
#endif	// __AGENT_EVENT_TARGET_H__