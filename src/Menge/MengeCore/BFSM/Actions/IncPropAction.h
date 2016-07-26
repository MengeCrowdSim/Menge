

/*!
 *	@file		IncPropAction.h
 *	@brief		Defines a BFSM action that causes agents radius to increase
 */

#ifndef __INCPROP_ACTION_H__
#define __INCPROP_ACTION_H__

#include "CoreConfig.h"
#include "Actions/Action.h"
#include "Actions/ActionFactory.h"
#include "fsmCommon.h"

namespace Menge {

	// forward declaration

	namespace Agents {
		class BaseAgent;
	}

	namespace BFSM {
		// forward declaration
		class IncPropActFactory;

		/*!
		 *	@brief		Causes the agents to grow bigger radius.
		 *
		 */
		class MENGE_API IncPropAction : public Action {
		public:
			/*!
			 *	@brief		Constructor
			 *
			 */
			IncPropAction();

			/*!
			 *	@brief		Virtual destructor.
			 */
			virtual ~IncPropAction();

			/*!
			 *	@brief		Upon entering the state, this is called -- it is the main work
			 *				of the action.
			 *
			 *	@param		agent		The agent to act on.
			 */
			virtual void onEnter( Agents::BaseAgent * agent );

			friend class IncPropActFactory;
		protected:

			/*!
			 *	@brief		The work to do upon state exit.
			 *
			 *	@param		agent		The agent to act on.
			 */
			virtual void leaveAction( Agents::BaseAgent * agent ) {}

			//variables here
			float _increment;
		};

		/*!
		 *	@brief		Factory for the IncPropAction.
		 */
		class MENGE_API IncPropActFactory : public ActionFactory {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			IncPropActFactory();

			/*!
			 *	@brief		The name of the action.
			 *
			 *	The action's name must be unique among all registered actions.
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the unique action name.
			 */
			virtual const char * name() const { return "increment_property"; }

			/*!
			 *	@brief		A description of the action.
			 *
			 *	Each action factory must override this function.
			 *
			 *	@returns	A string containing the action description.
			 */
			virtual const char * description() const {
				return "increments a property of an agent by a given step size";
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
			Action * instance() const;

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
			virtual bool setFromXML( Action * action, TiXmlElement * node, const std::string & behaveFldr ) const;
			size_t	_IncrementID;
			/*!
			 *	@brief		The identifier for the "exit_reset" boolean attribute.
			 */

		};

	}	//  namespace BFSM

}	//
#endif // __TELEPORT_ACTION_H__
