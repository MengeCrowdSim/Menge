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
 *	@file		Action.h
 *	@brief		The definition of actions that are taken as agents enter states.
 *
 *	Actions are associated with behavior states.  When an agent enters the corresponding
 *	behavior state, the action is taken on the agent.  It is typically used to modify
 *	agent parameters, consistent with what the state represents.  By default the agent
 *	should restore the agents parameter values when the agent leaves the FSM state.
 *	However, a flag can deactivate this behavior.
 */

#ifndef __ACTION_H__
#define __ACTION_H__

#include "CoreConfig.h"
#include "fsmCommon.h"
#include "FSMEnumeration.h"
#include "Element.h"

// forward declaration
class TiXmlElement;

namespace Menge {

	/*!
	 * @namespace BFSM
	 * @brief	The namespace contains the Behavior Finite State Machine (BFSM) definition
	 */
	namespace BFSM {

		// forward declaration
		class ActionFactory;

		/*!
		 *	@brief		The abstract definition of an action.
		 *
		 *	An action is explicitly executed upon entering a state and has the
		 *	option of undoing its effect upon exiting the state.
		 */
		class MENGE_API Action : public Element {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			Action(): Element(), _undoOnExit(true){}

		protected:
			/*!
			 *	@brief		Virtual destructor.
			 */
			virtual ~Action(){}

		public:
			/*! 
			 *	@brief		Upon entering the state, this is called -- it is the main work
			 *				of the action.
			 *
			 *	This is a purely virtual function.  Any instantiable Action sub-class
			 *	must *explicitly* account for this function.
			 *
			 *	@param		agent		The agent to act on.
			 */
			virtual void onEnter( Agents::BaseAgent * agent ) = 0;

			/*!
			 *	@brief		The work to do upon state exit.
			 *
			 *	@param		agent		The agent to act on.
			 */
			void onLeave( Agents::BaseAgent * agent );

			friend class ActionFactory;

		protected:
			/*! 
			 *	@brief		The actual work to do upon exiting the state.
			 *
			 *	This is a purely virtual function.  Any Action sub-class must
			 *	*explicitly* account for this function.  It will only be called if the
			 *	action is set to undo its work upon exit.
			 *
			 *	@param		agent		The agent to act on.
			 */
			virtual void leaveAction( Agents::BaseAgent * agent ) = 0;

			/*!
			 *	@brief		Determines if the action undoes itself on exiting the state.
			 */
			bool _undoOnExit;
		};

		/*!
		 *	@brief		Parses a TinyXML element containing an action specification
		 *
		 *	@param		node			The TinyXML element
		 *	@param		behaveFldr		The folder in which the behavior is defined -- all resources
		 *								are defined relative to this folder.
		 *	@returns	A pointer to the new action .
		 */
		Action * parseAction( TiXmlElement * node, const std::string & behaveFldr );
	}	// namespace BFSM
}	// namespace Menge
#endif	// __ACTION_H__
