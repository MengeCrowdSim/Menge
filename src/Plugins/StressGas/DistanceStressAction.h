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
#ifndef __DISTANCE_STRESS_ACTION_H__
#define __DISTANCE_STRESS_ACTION_H__

#include "BaseStressAction.h"
#include "DistanceStressFunction.h"
#include "StressGasConfig.h"

#include "MengeCore/BFSM/Actions/ActionFactory.h"

// forward declaration
namespace Menge {
namespace Math {
class Geometry2D;
class FloatGenerator;
}  // namespace Math
}  // namespace Menge

namespace StressGAS {
// forward declaration
class DistanceStressActionFactory;

/*!
 @brief		An action that configures an agent to become stressed based on proximity to a region.

 When the action is applied to an agent, the agent's stress level is related to its distance to a
 defined region. Within a specified distance to the region, the stress is at 100%. As the agent
 moves away from the stress region, the stress level is lower until at a maximum range, the region
 imparts no stress on the agent.

 How the imparted stress varies with respect to the distance depends on the stress function defined
 in the DistanceStressFuction @see DistanceStressFuction. But more particularly, when the agent has
 achieved a certain amount of stress and then moves *away* from the stress region, his stress level
 does not directly fall. The rate at which the stress level falls is a function of the cool down
 rate of the basic stress Action.

 For example, if the agent moves close to the stress region, the peakstress level may reach 90%. And
 then, if the agent moves quickly away, to a region that would ordinarily only invoke 20% stress,
 the agent's actual stress level would be:

 `max( 0.2, 0.9 - (\delta t / cool down duration) )`

 XML Example:

 ```xml
 <Action type="distance_stress"
 exit_behavior={"reset"|"continue"|"pause"|"cool down"}
 min_distance_dist="c|n|u" ...
 max_distance_dist="c|n|u" ...
 fall_off={"linear"|"quadratic"}
 shape={"point"|"circle"|"AABB"|"OBB"}
 [shape parameters]
 />
 ```
 
 The DistanceStressAction introduces several new parameters:
 - `min_distance` - the distance from the region at which the induced stress level is 100%.
 - `max_distance` - the ditance from the region at which no stress is induced.
 - `shape` - the type of region that induces stress. Depending on the type, additional parameters
             will be required to defined the shape's size and position.
 */
class EXPORT_API DistanceStressAction : public BaseStressAction {
 public:
  /** Default constructor */
  DistanceStressAction() : BaseStressAction(), _outerDist(0x0), _innerDist(0x0), _geometry(0x0) {}

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

  friend class DistanceStressActionFactory;

 protected:
  /*! @brief	The distance beyond which no stress is accumulated. */
  Menge::Math::FloatGenerator* _outerDist;

  /*! @brief  The distance inside which stress level is 100%. */
  Menge::Math::FloatGenerator* _innerDist;

  /*! @brief  The function interpolator to use. */
  DistanceStressFunction::StressInterpEnum _func;

  /*! @brief  The region with respect to which stress is defined. */
  Menge::Math::Geometry2D* _geometry;
};

/*!
 @brief		Factory for instantiating DistanceStressAction instances.
 */
class EXPORT_API DistanceStressActionFactory : public BaseStressActionFactory {
 public:
  /*! @brief		Default constructor. */
  DistanceStressActionFactory();

  /*!
   @brief		The name of the action.

   The action's name must be unique among all registered actions.
   Each action factory must override this function.

   @returns	A string containing the unique action name.
   */
  virtual const char* name() const { return "distance_stress"; }

  /*!
   @brief		A description of the action.

   Each action factory must override this function.

   @returns	A string containing the action description.
   */
  virtual const char* description() const {
    return "Sets an agent to have a stress level based on proximity to a region.  The "
           "region is defined as well as two distances.  Out side the \"outer\" distance "
           "the stress level is zero.  Inside the \"inner\" stress level, the stress "
           "level is 100%.  Between, the stress level is increased according to the "
           "specified function: linear or quadratic.";
  };

 protected:
  /*!
   @brief		Create an instance of this class's action.

   All ActionFactory sub-classes must override this by creating (on the heap) a new instance of its
   corresponding action type. The various field values of the instance will be set in a subsequent
   call to ActionFactory::setFromXML(). The caller of this function takes ownership of the memory.

   @returns		A pointer to a newly instantiated Action class.
   */
  Menge::BFSM::Action* instance() const { return new DistanceStressAction(); }

  /*!
   @brief		Given a pointer to an Action instance, sets the appropriate fields from the provided
            XML node.

   It is assumed that the value of the `type` attribute is this Action's type (i.e.
   ActionFactory::thisFactory() has already been called and returned true). If sub-classes of
   ActionFactory introduce *new* Action parameters, then the sub-class should override this method
   but explicitly call the parent class's version.

   @param		action		  A pointer to the action whose attributes are to be set.
   @param		node		    The XML node containing the action attributes.
   @param		behaveFldr	The path to the behavior file. If the action references resources in the
                        file system, it should be defined relative to the behavior file location.
                        This is the folder containing that path.
   @returns	True if successful.
   */
  virtual bool setFromXML(Menge::BFSM::Action* action, TiXmlElement* node,
                          const std::string& behaveFldr) const;

  /** @brief	Identifier for the outer boundary float attribute. */
  size_t _outerId;

  /** @brief  Identifier for the inner boundary float attribute. */
  size_t _innerId;

  /** @brief	Identifier for the interpolating function string attribute. */
  size_t _funcId;
};

}  // namespace StressGAS
#endif  // __DISTANCE_STRESS_ACTION_H__
