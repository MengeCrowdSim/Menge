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
 @file       ORCASimulator.h
 @brief      Contains the ORCA::Simulator class.
 */

#ifndef __ORCA_SIMULATOR_H__
#define __ORCA_SIMULATOR_H__

#include "MengeCore/Agents/SimulatorBase.h"
#include "MengeCore/Orca/ORCAAgent.h"
#include "MengeCore/mengeCommon.h"

/*!
 @namespace  ORCA
 @brief    The namespace for the ORCA local collision avoidance model.
 */
namespace ORCA {
/*!
@brief      Defines the simulator operating on ORCA::Agent.
*/
class Simulator : public Menge::Agents::SimulatorBase<Agent> {
 public:
  /*!
   @brief      Constructs a simulator instance.
   */
  Simulator() : Menge::Agents::SimulatorBase<Agent>() {}

 private:
  friend class Agent;
};
}  // namespace ORCA
#endif
