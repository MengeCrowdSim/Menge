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

/*!
 @file    VelCompGoal.h
 @brief    Provides the definition of the simple velocity component that computes a preferred
          velocity directly towards the goal.
 */

#ifndef __VEL_COMP_GOAL_H__
#define __VEL_COMP_GOAL_H__

#include "MengeCore/Agents/PrefVelocity.h"
#include "MengeCore/BFSM/VelocityComponents/VelComponent.h"
#include "MengeCore/BFSM/VelocityComponents/VelComponentFactory.h"
#include "MengeCore/CoreConfig.h"

namespace Menge {
namespace BFSM {

/*!
 @brief    A velocity component that returns a preferred velocity whose direction points from the
          current agent position toward the goal point.
 */
class MENGE_API GoalVelComponent : public VelComponent {
 public:
  /*!
   @brief    Default constructor.
   */
  GoalVelComponent();

  /*!
   @brief    Computes and sets the agent's preferred velocity.

   The velocity component directly sets preferred velocity values in the the provided preferred
   velocity instance. See Agents::PrefVelocity for details. Rather than setting the agent's
   preferred velocity value directly, a reference to a preferred velocity instance is passed in to
   make the use more general. This allows the computation of the preferred velocity for the agent,
   without necessarily making changes to it.

   If the agent is already contained in the goal, the preferred velocity will be given zero vectors
   for the three directions (left, right, and preferred). The target point will be `q`.

   @param    agent    The agent for which a preferred velocity is computed.
   @param    goal    The agent's goal (although this may be ignored).
   @param    pVel    The instance of Agents::PrefVelocity to set.
   */
  virtual void setPrefVelocity(const Agents::BaseAgent* agent, const Goal* goal,
                               Agents::PrefVelocity& pVel) const;

  /*!
   @brief    Used by the plugin system to know what artifacts to associate with agents of this type.
   
   Every sub-class of must return a globally unique value if it should be associated with unique
   artifacts.
   */
  virtual std::string getStringId() const { return NAME; }

  /*! The unique identifier used to register this type with run-time components. */
  static const std::string NAME;
};

//////////////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for the GoalVelComponent.
 */
class MENGE_API GoalVCFactory : public VelCompFactory {
 public:
  /*!
   @brief    The name of the velocity component.

   The velocity component's name must be unique among all registered velocity components. Each
   velocity component factory must override this function.

   @returns  A string containing the unique velocity component name.
   */
  virtual const char* name() const { return GoalVelComponent::NAME.c_str(); }

  /*!
   @brief    A description of the velocity component.

   Each velocity component factory must override this function.

   @returns  A string containing the velocity component description.
   */
  virtual const char* description() const {
    return "Provides a preferred velocity which always aims directly toward the goal "
           "(at the agent's preferred speed) unless it will overstep the goal in a "
           "single time step, then it is scaled down.";
  };

 protected:
  /*!
   @brief    Create an instance of this class's velocity component.

   All VelCompFactory sub-classes must override this by creating (on the heap) a new instance of its
   corresponding velocity component type. The various field values of the instance will be set in a
   subsequent call to VelCompFactory::setFromXML(). The caller of this function takes ownership of
   the memory.

   @returns    A pointer to a newly instantiated VelComponent class.
   */
  VelComponent* instance() const { return new GoalVelComponent(); }
};

}  // namespace BFSM
}  // namespace Menge

#endif  // __VEL_COMP_GOAL_H__
