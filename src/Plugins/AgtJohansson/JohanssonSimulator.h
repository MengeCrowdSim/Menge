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

#ifndef __JOHANSSON_SIMULATOR_H__
#define __JOHANSSON_SIMULATOR_H__

/*!
 @file       JohanssonSimulator.h
 @brief      Contains the Johansson::Simulator class; implements Johansson's 2007 pedestrian model.
 */

#include "JohanssonAgent.h"
#include "MengeCore/Agents/SimulatorBase.h"
#include "MengeCore/mengeCommon.h"

/*!
 @namespace		Johansson
 @brief			Contains the specification of the pedestrian model from the Johansson et al., 2007
            paper.
 */
namespace Johansson {
/*!
 @brief      Defines the simulator operating on a Johansson::Agent.
 */
class Simulator : public Menge::Agents::SimulatorBase<Agent> {
 public:
  /*!
   @brief      Constructor.
   */
  Simulator() : Menge::Agents::SimulatorBase<Agent>() {}

  /*!
   @brief			Reports if there are non-common Experiment parameters that this simulator requires in
              the XML file.
   @returns		By default, the simulator base ONLY uses common parameters. Always returns false.
   */
  virtual bool hasExpTarget() { return true; }

  /*!
   @brief			Reports if the given Experiment attribute tag name belongs to this simulator.
   @param		tagName		the name of the considered tag
   @returns		By default, the simulator base ONLY uses common parameters. Always returns false.
   */
  virtual bool isExpTarget(const std::string& tagName) { return tagName == "Johansson"; }

  /*!
   @brief			Given an Experiment parameter name and value, sets the appropriate simulator
              parameter.
   @param			paramName		A string containing the parameter name for the experiment.
   @param			value			  A string containing the value for the parameter.
   @returns		True if the parameters were successfully set.
   */
  virtual bool setExpParam(const std::string& paramName,
                           const std::string& value) throw(Menge::Agents::XMLParamException);

 protected:
  friend class Agent;
  /*!
   @brief		The magnitude of the inter-agent repulsion forces
   */
  static float AGENT_SCALE;

  /*!
   @brief		The magnitude of the agent-obstacle repulsion forces
   */
  static float OBST_SCALE;

  /*!
   @brief		The reaction time used to define the driving force
   */
  static float REACTION_TIME;

  /*!
   @brief		The the fall-off distance of repulsive forces
   */
  static float FORCE_DISTANCE;

  /*!
   @brief		The time of a pedestrian step
   */
  static float STRIDE_TIME;
};
}  // namespace Johansson

#endif  // __JOHANSSON_SIMULATOR_H__
