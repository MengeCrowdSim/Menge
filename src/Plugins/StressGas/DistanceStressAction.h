/*!
 *	@file		DistanceStressAction.h
 *	@brief		Definition of actions which triggers accumulation of stress based on distance to
 *				a geometric region.
 */
#ifndef __DISTANCE_STRESS_ACTION_H__
#define __DISTANCE_STRESS_ACTION_H__

#include "BaseStressAction.h"
#include "StressGasConfig.h"
#include "Actions/ActionFactory.h"

using namespace Menge;

namespace StressGAS {
	// forward declaration
	class DistanceStressActionFactory;

	/*!
	 *	@brief		An action that configures an agent to begin accumulating stress.
	 */
	class EXPORT_API DistanceStressAction : public BaseStressAction {
	public:
		/** Default constructor */
		DistanceStressAction() : BaseStressAction(), _outerDist( 0x0 ), _innerDist( 0x0 ) {}

		/*!
		 *	@brief		Returns the stress function for this stress action.
		 *
		 *	Stress actions vary in the stress function they are associated with.  Each sub-class
		 *	must define this method to provide the right type of StressFunction.
		 *
		 *	@param	agent			The agent to compute stress for.
		 *	@param	stressor		The stressor for the StressFunction to use.
		 *	@param	coolDuration	The amount of simulation time required to cool down from
		 *							full stress to no stress  (in simulation seconds).
		 *	@returns	An instance of the appropriate stress function.
		 */
		virtual StressFunction * makeStressFunction( Agents::BaseAgent * agent,
													 AgentStressor * stressor,
													 float coolDuration );

		friend class DistanceStressActionFactory;

	protected:
		/*! @brief	The distance beyond which no stress is accumulated. */
		FloatGenerator * _outerDist;

		/*! @brief  The distance inside which stress level is 100%. */
		FloatGenerator * _innerDist;

	};

	/*!
	 *	@brief		Factory for instantiating DistanceStressAction instances.
	 */
	class EXPORT_API DistanceStressActionFactory : public BaseStressActionFactory {
	public:
		/*! @brief		Default constructor. */
		DistanceStressActionFactory();

		/*!
		 *	@brief		The name of the action.
		 *
		 *	The action's name must be unique among all registered actions.
		 *	Each action factory must override this function.
		 *
		 *	@returns	A string containing the unique action name.
		 */
		virtual const char * name() const { return "distance_stress"; }

		/*!
		 *	@brief		A description of the action.
		 *
		 *	Each action factory must override this function.
		 *
		 *	@returns	A string containing the action description.
		 */
		virtual const char * description() const {
			return "Sets an agent to have a stress level based on proximity to a region.  The " \
				"region is defined as well as two distances.  Out side the \"outer\" distance " \
				"the stress level is zero.  Inside the \"inner\" stress level, the stress " \
				"level is 100%.  Between, the stress level is increased according to the " \
				"specified function: linear or quadratic.";
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
		BFSM::Action * instance() const { return new DistanceStressAction(); }

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
		virtual bool setFromXML( BFSM::Action * action, TiXmlElement * node,
								 const std::string & behaveFldr ) const;

		/** @brief	Identifier for the outer boundary float attribute. */
		size_t _outerId;

		/** @brief  Identifier for the inner boundary float attribute. */
		size_t _innerId;

		/** @brief	Identifier for the interpolating function string attribute. */
		size_t _funcId;
	};

};
#endif	// __DISTANCE_STRESS_ACTION_H__
