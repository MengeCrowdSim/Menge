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
 *	@file		AgentEventEffect.h
 *	@brief		The definition of the agent event effect -- defines the agent-base
 *				effect.
 */

#ifndef __AGENT_EVENT_EFFECT_H__
#define __AGENT_EVENT_EFFECT_H__

#include "EventEffect.h"
#include "EventEffectFactory.h"


namespace Menge {
	// forward declaration
	class AgentEventTarget;

	namespace Agents {
		class BaseAgent;
	}
	
	/*!
	 *	@brief		The event effect class that operates on sets of agents.
	 */
	class MENGE_API AgentEventEffect : public EventEffect {
	public:

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
		virtual bool isCompatible( EventTarget * target );

		/*!
		 *	@brief		Applies the effect to the simulation target.
		 *
		 *	Not all targets work with all effects.  The target passed here
		 *	must have previously passed the EventEffect::isCompatible test to
		 *	work.
		 *
		 *	@param		target		The target to apply the event to.
		 */
		virtual void apply( EventTarget * target );
		
	protected:

		/*!
		 *	@brief		The actual work of the effect.
		 *
		 *	Sub-classes should implement this.  It is the action to be taken for each
		 *	agent.
		 *
		 *	@param		agent		The agent to operate on.
		 */
		virtual void agentEffect( Agents::BaseAgent * agent ) = 0;
	};
	
}	// namespace Menge

#endif	// __AGENT_EVENT_EFFECT_H__