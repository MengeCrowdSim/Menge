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
 @file       PedVOSimulator.h
 @brief      Contains the ORCA::Simulator class.
 */

#ifndef __PEDVO_SIMULATOR_H__
#define __PEDVO_SIMULATOR_H__

#include "MengeCore/Agents/SimulatorBase.h"
#include "MengeCore/PedVO/PedVOAgent.h"
#include "MengeCore/mengeCommon.h"

/*!
 @namespace    PedVO
 @brief      Contains the specification of the pedestrian model based on Pedestrian Velocity
            Obstacles.
 */
namespace PedVO {
/*!
 @brief      Defines the simulator operating on ORCA::Agent.
 */
class Simulator : public Menge::Agents::SimulatorBase<Agent> {
 public:
  /*!
   @brief      Constructor.
   */
  Simulator() : Menge::Agents::SimulatorBase<Agent>() {}

 protected:
  friend class Agent;

  /*!
   @brief    The cosine of the threshold angle beyond which the constraints should be tilted.
   
   If the dot product between the preferred *direction* and the constraint's normal is greater than
   or equal then this threshhold, the obstacle is tilted.

   This is also used to perform the minimum rotation.
   */
  static float COS_OBST_TURN;

  /*!
   @brief    The sine of the threshold angle beyond which the constraints should be tilted.
   
   Used in conjunction with COS_OBST_TURN to perform the minmum tilt.
   */
  static float SIN_OBST_TURN;
};
}  // namespace PedVO
#endif
