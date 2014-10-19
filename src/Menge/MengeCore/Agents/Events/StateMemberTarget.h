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
 *	@file		StateMemberTarget.h
 *	@brief		Defines an event effect target based on membership in state(s).
 */

#ifndef __STATE_MEMBER_TARGET_H__
#define __STATE_MEMBER_TARGET_H__

#include "AgentEventTarget.h"

namespace Menge {
	// forward declaration
	class NamedStateMemberTargetFactory;

	namespace BFSM {
		class State;
	}	


	//////////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		Defines the target of an event effect based on membership
	 *				relationship to a single *named* state.
	 */
	class MENGE_API NamedStateMemberTarget : public AgentEventTarget {
	public:
		/*!
		 *	@brief		Default constructor.
		 */
		NamedStateMemberTarget() : AgentEventTarget() {}

		/*!
		 *	@brief		Allows the event target to finish initializing itself from its
		 *				parsed state to its running state.
		 *
		 *	@throws		EventException if there is a problem finalizing.
		 */	 
		virtual void finalize();

		/*!
		 *	@brief		Gives the target a chance to update its knowledge of the 
		 *				target entities.
		 *
		 *	If a sub-class needs to do particular computation to evaluate the target,
		 *	it should be implemented here.
		 */
		virtual void update();

		friend class NamedStateMemberTargetFactory;

	protected:
		/*!
		 *	@brief		The name of the state to operate on.
		 */
		std::string	_stateName;

		/*!
		 *	@brief		The state to operate on.
		 */
		BFSM::State * _state;

		/*!
		 *	@brief		Deterimines if membership in the specified state (true)
		 *				or non-membership (false) makes an agent a target.
		 */
		bool	_inState;
	};

	/////////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		The factory to generate NamedStateMemberTarget instances.
	 */
	class MENGE_API NamedStateMemberTargetFactory : public AgentEventTargetFactory {
	public:
		/*!
		 *	@brief		Constructor.
		 */
		NamedStateMemberTargetFactory();
		
		/*!
		 *	@brief		The name of the target.
		 *
		 *	The target's name must be unique among all registered targets.
		 *	Each target factory must override this function.
		 *
		 *	@returns	A string containing the unique target name.
		 */
		virtual const char * name() const { return "named_state_member"; }

		/*!
		 *	@brief		A description of the target.
		 *
		 *	Each target factory must override this function.
		 *
		 *	@returns	A string containing the target description.
		 */
		virtual const char * description() const {
			return "Defines an agent as a target based on its membership relationship to a single state.";
		};

	protected:
		/*!
		 *	@brief		Create an instance of this class's target.
		 *
		 *	All EventTargetFactory sub-classes must override this by creating (on the heap)
		 *	a new instance of its corresponding event target type.  The various field values
		 *	of the instance will be set in a subsequent call to EventTargetFactory::setFromXML.
		 *	The caller of this function takes ownership of the memory.
		 *
		 *	@returns		A pointer to a newly instantiated EventTarget class.
		 */
		EventTarget * instance() const { return new NamedStateMemberTarget(); }	

		/*!
		 *	@brief		Given a pointer to an EventTarget instance, sets the appropriate fields
		 *				from the provided XML node.
		 *
		 *	It is assumed that the value of the `type` attribute is this EventTarget's type.
		 *	(i.e. EventTargetFactory::thisFactory has already been called and returned true.)
		 *	If sub-classes of EventTargetFactory introduce *new* EventTarget parameters, then the
		 *	sub-class should override this method but explicitly call the parent class's
		 *	version.
		 *
		 *	@param		target			A pointer to the target whose attributes are to be set.
		 *	@param		node			The XML node containing the target attributes.
		 *	@param		behaveFldr		The path to the behavior file.  If the target references
		 *								resources in the file system, it should be defined relative
		 *								to the behavior file location.  This is the folder containing
		 *								that path. 
		 *	@returns	A boolean reporting success (true) or failure (false).
		 */
		virtual bool setFromXML( EventTarget * target, TiXmlElement * node, const std::string & behaveFldr ) const;

		/*!
		 *	@brief		The identifier for the "state" string attribute.
		 */
		size_t	_stateID;

		/*!
		 *	@brief		The identifier for the "is_member" bool attribute.
		 */
		size_t _isMemberID;		
	};

	// TODO: Future state-based targets
	//	Set of named states
	//	membership based on state properties
	//		final/not final
	//		transitions to state X
	//		etc.

}	// namespace Menge
#endif	// __STATE_MEMBER_TARGET_H__