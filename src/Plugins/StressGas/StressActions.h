/*!
 *	@file		StressActions.h
 *	@brief		Definition of actions used to apply stress to agents.
 */
#ifndef __STRESS_GAS_ACTION_H__
#define __STRESS_GAS_ACTION_H__

#include "StressGasConfig.h"
#include "Actions/Action.h"
#include "Actions/ActionFactory.h"
#include "FSMEnumeration.h"
#include <map>

//forward declaration
class TiXmlElement;

using namespace Menge;

namespace StressGAS {
	// forward declaration
	class RegisterStressAgentActionFactory;
	
	/*!
	 *	@brief		An action sets an agent for accumulating stress
	 */
	class EXPORT_API RegisterStressAgentAction : public BFSM::Action {
	public:
		/*!
		 *	@brief		Constructor
		 */
		RegisterStressAgentAction();

		/*!
		 *	@brief		Virtual destructor.
		 */
		~RegisterStressAgentAction();

		/*! 
		 *	@brief		Upon entering the state, this is called -- it is the main work
		 *				of the action.
		 *
		 *	@param		agent		The agent to act on.
		 */
		void onEnter( Agents::BaseAgent * agent );

		/*! 
		 *	@brief		Returns the task for managing stress accumulation.
		 *
		 *	@returns   The StressTask instance
		 */
		virtual BFSM::Task * getTask();

		friend class RegisterStressAgentActionFactory;

	protected:

		/*!
		 *	@brief		The work to do upon state exit.
		 *
		 *	@param		agent		The agent to act on.
		 */
		void leaveAction( Agents::BaseAgent * agent );

	protected:
		
	};

	/*!
	 *	@brief		Factory for instantiating RegisterStressAgentAction instances.
	 */
	class EXPORT_API RegisterStressAgentActionFactory : public BFSM::ActionFactory {
	public:
		/*!
		 *	@brief		The name of the action.
		 *
		 *	The action's name must be unique among all registered actions.
		 *	Each action factory must override this function.
		 *
		 *	@returns	A string containing the unique action name.
		 */
		virtual const char * name() const { return "stress_agent"; }

		/*!
		 *	@brief		A description of the action.
		 *
		 *	Each action factory must override this function.
		 *
		 *	@returns	A string containing the action description.
		 */
		virtual const char * description() const {
			return "Registers an agent to the stress update system.";
		};

	protected:
		/*!
		 *	@brief		Create an instance of this class's action.
		 *
		 *	All ActionFactory sub-classes must override this by creating (on the heap)
		 *	a new instance of its corresponding action type.  The various field values
		 *	of the instance will be set in a subsequent call to ActionFactory::setFromXML.
		 *	The caller of this function takes ownership of the memory.
		 *
		 *	@returns		A pointer to a newly instantiated Action class.
		 */
		BFSM::Action * instance() const { return new RegisterStressAgentAction(); }	
		
		/*!
		 *	@brief		Given a pointer to an Action instance, sets the appropriate fields
		 *				from the provided XML node.
		 *
		 *	It is assumed that the value of the `type` attribute is this Action's type.
		 *	(i.e. ActionFactory::thisFactory has already been called and returned true.)
		 *	If sub-classes of ActionFactory introduce *new* Action parameters, then the
		 *	sub-class should override this method but explicitly call the parent class's
		 *	version.
		 *
		 *	@param		action		A pointer to the action whose attributes are to be set.
		 *	@param		node		The XML node containing the action attributes.
		 *	@param		behaveFldr	The path to the behavior file.  If the action references
		 *							resources in the file system, it should be defined relative
		 *							to the behavior file location.  This is the folder containing
		 *							that path. 
		 *	@returns	A boolean reporting success (true) or failure (false).
		 */
		virtual bool setFromXML( BFSM::Action * action, TiXmlElement * node, const std::string & behaveFldr ) const;
	};

};
#endif	// __STRESS_GAS_ACTION_H__
