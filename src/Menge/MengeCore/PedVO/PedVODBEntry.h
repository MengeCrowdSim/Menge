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
 @file    PedVODBEntry.h
 @brief    The simulator database entry for the PedVO pedestrian model.
 */

#ifndef __PEDVO_DB_ENTRY_H__
#define __PEDVO_DB_ENTRY_H__

#include "MengeCore/PedVO/PedVO.h"
#include "MengeCore/Runtime/SimulatorDBEntry.h"

namespace PedVO {
/*!
 @brief    The simulator database entry for the PedVO simulator.
 */
class DBEntry : public Menge::SimulatorDBEntry {
 public:
  /*!
   @brief  Gives a brief description of the simulator.

   @returns  A brief description of the simulator and pedestrian model.
   */
  virtual ::std::string briefDescription() const;

  /*!
   @brief  Gives a long description of the simulator.

   @returns  A long description of the simulator and pedestrian model.
   */
  virtual ::std::string longDescription() const;

  /*!
   @brief  Gives a label to apply to the interactive viewer.

   @returns  The name for display on the interactive viewer.
   */
  virtual ::std::string viewerName() const;

  /*!
   @brief    Gives a unique name to be used as a command-line parameter.

   This name MUST satisfy two constraints:
   - It must contain no spaces.
   - It must be unique from that used by all other simulators.

   @returns  A single string (with no spaces) that can be used as a command line parameter to
            uniquely identify this model.
   */
  virtual ::std::string commandLineName() const { return Agent::NAME; }

  /*!
   @brief    Returns a pointer to this model's Simulator instance.

   This must be overridden by a derived class

   @returns  A newly instantiated simulator instance of a type appropriate for this database entry.
   */
  virtual Menge::Agents::SimulatorInterface* getNewSimulator();

  /*!
   @brief    Provides an AgentInitializer appropriate to this simulator class.

   @returns  A pointer to an agent initializer. The caller is responsible for freeing up the memory.
   */
  virtual Menge::Agents::AgentInitializer* getAgentInitalizer() const;
};
}  // namespace PedVO

#endif  // __PEDVO_DB_ENTRY_H__
