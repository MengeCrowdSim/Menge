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
 *	@file		StateEvtTrigger.h
 *	@brief		The definitin of event triggers which key on state properties.
 */

#ifndef __STATE_EVT_TRIGGER_H__
#define __STATE_EVT_TRIGGER_H__

#include "Events/EventTrigger.h"
#include "Events/EventTriggerFactory.h"

namespace Menge {
	
	// forward declaration
	class StateEvtTriggerFactory;

	namespace BFSM {
		class State;
	}	

	//////////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		The base class event triggers which depend on state properties.
	 */
	class MENGE_API StateEvtTrigger : public EventTrigger {
	public:
		/*!
		 *	@brief		Constructor.
		 */
		StateEvtTrigger();

		/*!
		 *	@brief		Allows the trigger to finish initializing itself from its
		 *				parsed state to its running state.
		 */
		virtual void finalize();

		friend class StateEvtTriggerFactory;

	protected:
		/*!
		 *	@brief		The name of the state.
		 */
		std::string 	_stateName;

		/*!
		 *	@brief		The state to operate on.
		 */
		BFSM::State *		_state;
	};

	//////////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		The factory for state event triggers.
	 */
	class MENGE_API StateEvtTriggerFactory : public EventTriggerFactory {
	public:
		/*!
		 *	@brief		Constructor.
		 */
		StateEvtTriggerFactory();

	protected:
		/*!
		 *	@brief		Given a pointer to an EventTrigger instance, sets the appropriate fields
		 *				from the provided XML node.
		 *
		 *	It is assumed that the value of the `type` attribute is this EventTrigger's type.
		 *	(i.e. EventTrigger::thisFactory has already been called and returned true.)
		 *	If sub-classes of EventTriggerFactory introduce *new* EventTrigger parameters, then the
		 *	sub-class should override this method but explicitly call the parent class's
		 *	version.
		 *
		 *	@param		trigger		A pointer to the event trigger whose attributes are to be set.
		 *	@param		node		The XML node containing the event trigger attributes.
		 *	@param		specFldr	The path to the specification file.  If the EventTrigger references
		 *							resources in the file system, it should be defined relative
		 *							to the specification file location.  This is the folder containing
		 *							that path. 
		 *	@returns	A boolean reporting success (true) or failure (false).
		 */
		virtual bool setFromXML( EventTrigger * trigger, TiXmlElement * node, const std::string & specFldr ) const;

		/*!
		 *	@brief		The identifier for the "state" string attribute.
		 */
		size_t	_stateID;

	};

	//////////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		A trigger that fires off when a state's population increases.
	 */
	class MENGE_API StatePopIncreaseTrigger : public StateEvtTrigger {
	public:
		/*!
		 *	@brief		Constructor.
		 */
		StatePopIncreaseTrigger();

		/*!
		 *	@brief		Allows the trigger to finish initializing itself from its
		 *				parsed state to its running state.
		 *
		 *	@throws		EventException if there is a problem finalizing.
		 */
		virtual void finalize();

	protected:

		/*!
		 *	@brief		Evaluates the condition to see if it has been met.
		 *
		 *	This must be implemented by all sub-classes.
		 *
		 *	@returns	True if the condition has been met, false otherwise.
		 */
		virtual bool testCondition();

		/*!
		 *	@brief		The population last time the state was examined.
		 */
		size_t	_lastPop;
	};

	//////////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		The factory for StatePopIncreaseTrigger event triggers.
	 */
	class MENGE_API StatePopIncTriggerFactory : public StateEvtTriggerFactory {
	public:
		/*!
		 *	@brief		The name of the trigger type.
		 *
		 *	The trigger's name must be unique among all registered triggers.  
		 *	Each trigger factory must override this function.
		 *
		 *	@returns	A string containing the unique trigger name.
		 */
		virtual const char * name() const { return "state_pop_increase"; }

		/*!
		 *	@brief		A description of the event trigger.
		 *
		 *	Each trigger factory must override this function.
		 *
		 *	@returns	A string containing the trigger description.
		 */
		virtual const char * description() const {
			return "Event trigger which fires when a state's population increases.";
		};

	protected:
		/*!
		 *	@brief		Create an instance of this class's event trigger implementation.
		 *
		 *	All EventTriggerFactory sub-classes must override this by creating (on the heap)
		 *	a new instance of its corresponding trigger type.  The various field values
		 *	of the instance will be set in a subsequent call to EventTriggerFactory::setFromXML.
		 *	The caller of this function takes ownership of the memory.
		 *
		 *	@returns		A pointer to a newly instantiated EventTrigger class.
		 */
		EventTrigger * instance() const { return new StatePopIncreaseTrigger(); }
	};

}	// namespace Menge
#endif	// __STATE_EVT_TRIGGER_H__