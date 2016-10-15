/*!
 *	@file		BaseStressAction.h
 *	@brief		Definition of the basic Stress action -- all actions which accumualte stress come 
 *				through this base class.
 */
#ifndef __BASE_STRESS_ACTION_H__
#define __BASE_STRESS_ACTION_H__

#include "StressFunction.h"
#include "StressGasConfig.h"

#include "Actions/Action.h"
#include "Actions/ActionFactory.h"
#include "Math/RandGenerator.h"

//forward declaration
class TiXmlElement;

using namespace Menge;

namespace StressGAS {
	// forward declaration
	class BaseStressActionFactory;

	/*!
	 *	@enum	ExitBehavior
	 *
	 *	@brief	The various actions the action can take when the agent leaves the BFSM state.
	 */
	enum ExitBehavior {
		RESET,		///< Traditional action; original values reset and stress stops.
		COOL_DOWN,	///< Accumulation stops, cool down starts.  Stressor over at end of cool down.
		PAUSE,		///< Stress levels maintained, no new accumulation.
		CONTINUE	///< Stress continues even after exit
	};
	
	/*!
	 *	@brief		The base class for all stress actions.  
	 *
	 *	It provides the core functionality for defining the stress vector (the offset to agent 
	 *	parameters which represent a change to 100% stressed.) Handles the initial registration 
	 *	with the stress manager.
	 */
	class EXPORT_API BaseStressAction : public BFSM::Action {
	public:
		/*! The default time to cool down from 100% stress. */
		const static float DEFAULT_COOL_DURATION;

		/*!
		 *	@brief		Constructor
		 */
		BaseStressAction() : Action(), _exitBehavior( RESET ), _deltaNeighborDist( 0x0 ),
							_deltaMaxNeighbors( 0x0 ), _deltaRadius(0x0), _deltaTimeHorizon(0x0),
							_deltaPrefSpeed(0x0), _coolDuration(0x0) {}

		/*!
		 *	@brief		Virtual destructor.
		 */
		~BaseStressAction();

		/*!
		 *	@brief		Returns the stress function for this stress action.
		 *
		 *	Stress actions vary in the stress function they are associated with.  Each sub-class
		 *	must define this method to provide the right type of StressFunction.
		 *
		 *	@param	stressor	The stressor for the StressFunction to use.
		 *	@returns	An instance of the appropriate stress function.
		 */
		virtual StressFunction * makeStressFunction( Agents::BaseAgent * agent, 
													 AgentStressor * stressor, 
													 float coolDuration ) = 0;

		/*!
		 *	@brief		Upon entering the state, this is called -- it is the main work of the 
		 *				action.
		 *
		 *	@param		agent		The agent to act on.
		 */
		void onEnter(Agents::BaseAgent * agent);

		/*!
		 *	@brief		Returns the task for managing stress accumulation.
		 *
		 *	@returns   The StressTask instance
		 */
		virtual BFSM::Task * getTask();

		friend class BaseStressActionFactory;

	protected:
		/*!
		 *	@brief		The work to do upon state exit.
		 *
		 *	@param		agent		The agent to act on.
		 */
		void leaveAction(Agents::BaseAgent * agent);

		/*! @brief	The exit behavior the action takes. */
		ExitBehavior _exitBehavior;

		/*! @brief	The value for the change in neighbor distance. */
		FloatGenerator * _deltaNeighborDist;

		/*! @brief	The value for the change in maximum neighbors. */
		FloatGenerator * _deltaMaxNeighbors;

		/*! @brief	The value for the change in radius. */
		FloatGenerator * _deltaRadius;

		/*! @brief	The value for the change in time horizon. */
		FloatGenerator * _deltaTimeHorizon;

		/*! @brief	The value for the change in preference speed. */
		FloatGenerator * _deltaPrefSpeed;

		/*! @brief  The value for the cool down duration. */
		FloatGenerator * _coolDuration;

	protected:

	};

	/*!
	 *	@brief		Factory for instantiating BaseStressAction instances.
	 */
	class EXPORT_API BaseStressActionFactory : public BFSM::ActionFactory {
	public:
		
		/*!
		 @brief	Default constructor.
		 */
		BaseStressActionFactory();

	protected:
		/*!
		 *	@brief		Given a pointer to an Action instance, sets the appropriate fields from the
		 *				provided XML node.
		 *
		 *	It is assumed that the value of the `type` attribute is this Action's type. (i.e. 
		 *	ActionFactory::thisFactory has already been called and returned true.) If sub-classes 
		 *	of ActionFactory introduce *new* Action parameters, then the sub-class should override 
		 *	this method but explicitly call the parent class's version.
		 *
		 *	@param		action		A pointer to the action whose attributes are to be set.
		 *	@param		node		The XML node containing the action attributes.
		 *	@param		behaveFldr	The path to the behavior file.  If the action references
		 *							resources in the file system, it should be defined relative to 
		 *							the behavior file location.  This is the folder containing that
		 *							path.
		 *	@returns	A boolean reporting success (true) or failure (false).
		 */
		virtual bool setFromXML(BFSM::Action * action, TiXmlElement * node, 
								const std::string & behaveFldr) const;

		/** @brief	Identifier for the neighbor distance change float attribute. */
		size_t _neighborDistId;

		/** @brief	Identifier for the maximum neighbor change int attribute. */
		size_t _maxNeighborsId;

		/** @brief	Identifier for the radius change float attribute. */
		size_t _radiusId;

		/** @brief	Identifier for the preference speed change float attribute. */
		size_t _prefSpeedId;

		/** @brief	Identifier for the time horizon change float attribute; only affects   
		 *			ORCA-related agents. */
		size_t _timeHorizonId;

		/** @brief  Identifier for the exit behavior of the stress action */
		size_t _exitBehaviorId;

		/** @brief  Identifier for the cool down duration of the stress action */
		size_t _coolDurationId;
	};
}	 // namespace StressGAS

#endif // __BASE_STRESS_ACTION_H__
