/*
 Menge Crowd Simulation Framework

 Copyright and trademark 2012-17 University of North Carolina at Chapel Hill

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0
 or
    LICENSE.txt in the root of the Menge repository.

 Any questions or comments should be sent to the authors menge@cs.unc.edu

 <http://gamma.cs.unc.edu/Menge/>
*/
#ifndef __TIMED_STRESS_ACTION_H__
#define __TIMED_STRESS_ACTION_H__

#include "BaseStressAction.h"
#include "StressGasConfig.h"

#include "MengeCore/BFSM/Actions/ActionFactory.h"

// forward declarations
namespace Menge {
namespace Agents {
class BaseAgent;
}
namespace Math {
class FloatGenerator;
}
}  // namespace Menge

namespace StressGAS {
// forward declaration
class TimedStressActionFactory;

/*!
 @brief		An action that configures an agent to begin accumulating stress.
 */
class EXPORT_API TimedStressAction : public BaseStressAction {
 public:
  /** Default constructor */
  TimedStressAction() : BaseStressAction(), _duration(0x0) {}

  /*!
   @brief		Returns the stress function for this stress action.

   Stress actions vary in the stress function they are associated with. Each sub-class must define
   this method to provide the right type of StressFunction.

   @param	agent			    The agent to compute stress for.
   @param	stressor		  The stressor for the StressFunction to use.
   @param	coolDuration	The amount of simulation time required to cool down from full stress to no
                        stress (in simulation seconds).
   @returns	An instance of the appropriate stress function.
  */
  virtual StressFunction* makeStressFunction(Menge::Agents::BaseAgent* agent,
                                             AgentStressor* stressor, float coolDuration);

  friend class TimedStressActionFactory;

 protected:
  /*! @brief	The value for the duration of time to reach full stress. */
  Menge::Math::FloatGenerator* _duration;
};

/*!
 @brief		Factory for instantiating TimedStressAction instances.
 */
class EXPORT_API TimedStressActionFactory : public BaseStressActionFactory {
 public:
  /*!
   @brief		Default constructor.
   */
  TimedStressActionFactory();

  /*!
   @brief		The name of the action.

   The action's name must be unique among all registered actions.
   Each action factory must override this function.

   @returns	A string containing the unique action name.
   */
  virtual const char* name() const { return "timed_stress"; }

  /*!
   @brief		A description of the action.

   Each action factory must override this function.

   @returns	A string containing the action description.
   */
  virtual const char* description() const {
    return "Sets an agent to begin accumulating stress with the passage of time. "
           "Configured by setting the amount of time it will take the agent to reach "
           "100% stress (\"duration\").";
  };

 protected:
  /*!
   @brief		Create an instance of this class's action.

   All ActionFactory sub-classes must override this by creating (on the heap) a new instance of its
   corresponding action type. The various field values of the instance will be set in a subsequent
   call to ActionFactory::setFromXML(). The caller of this function takes ownership of the memory.

   @returns		A pointer to a newly instantiated Action class.
   */
  Menge::BFSM::Action* instance() const { return new TimedStressAction(); }

  /*!
   @brief		Given a pointer to an Action instance, sets the appropriate fields from the provided
            XML node.

   It is assumed that the value of the `type` attribute is this Action's type (i.e.
   ActionFactory::thisFactory() has already been called and returned true). If sub-classes of
   ActionFactory introduce *new* Action parameters, then the sub-class should override this method
   but explicitly call the parent class's version.

   @param		action		  A pointer to the action whose attributes are to be set.
   @param		node		    The XML node containing the action attributes.
   @param		behaveFldr	The path to the behavior file.  If the action references resources in the
                        file system, it should be defined relative to the behavior file location.
                        This is the folder containing that path.
   @returns	True if successful.
   */
  virtual bool setFromXML(Menge::BFSM::Action* action, TiXmlElement* node,
                          const std::string& behaveFldr) const;

  /** @brief	Identifier for the stress duration float attribute. */
  size_t _durationId;
};
}  // namespace StressGAS
#endif  // __TIMED_STRESS_ACTION_H__
