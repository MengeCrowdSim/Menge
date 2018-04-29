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
 @file    Core.h
 @brief    A set of global variables for use by the entire finite state machine.
 */

#ifndef __CORE_H__
#define __CORE_H__

#include "MengeCore/CoreConfig.h"

/*!
 @namespace Menge
 @brief  The core namespace.  All elements of Menge are contained in this namespace.
 */
namespace Menge {

// forward declarations
namespace Agents {
class SpatialQuery;
class SimulatorInterface;
class Elevation;
}  // namespace Agents

namespace BFSM {
class FSM;
}

class EventSystem;

/*!
 @brief    The fsm running for the simulation.
 */
extern MENGE_API BFSM::FSM* ACTIVE_FSM;

/*!
 @brief    The global simulation time.
 */
extern MENGE_API float SIM_TIME;

/*!
 @brief    The simulation time step.
 */
extern MENGE_API float SIM_TIME_STEP;

/*!
 @brief    The spatial query structure for the simulation.
 */
extern MENGE_API Agents::SpatialQuery* SPATIAL_QUERY;

/*!
 @brief    The elevation structure for the simulation.
 */
extern MENGE_API Agents::Elevation* ELEVATION;

/*!
 @brief    The simulator for use with some plugins that need it
 */
extern MENGE_API Agents::SimulatorInterface* SIMULATOR;

/*!
 @brief    The event system.
 */
extern MENGE_API EventSystem* EVENT_SYSTEM;

}  // namespace Menge

#endif  //__CORE_H__
