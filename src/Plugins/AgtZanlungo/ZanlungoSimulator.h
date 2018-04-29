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

#ifndef __ZANLUNGO_SIMULATOR_H__
#define __ZANLUNGO_SIMULATOR_H__

/*!
 @file       ZanlungoSimulator.h
 @brief      Contains the Zanlungo::Simulator class

 Implements Zanlungo's 2011 pedestrian model "Social Force Model with Explicit Collision
 Prediction".
 */

#include "MengeCore/Agents/SimulatorBase.h"
#include "MengeCore/mengeCommon.h"
#include "ZanlungoAgent.h"

/*!
 @namespace		Zanlungo
 @brief			Contains the specification of the pedestrian model based on the Zanlungo et al., 2011
            paper.
 */
namespace Zanlungo {
/*!
 @brief      Defines the simulator operating on a Zanlungo::Agent.
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
  virtual bool isExpTarget(const std::string& tagName) { return tagName == "Zanlungo"; }

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
   @brief		The magnitude of the inter-agent repulsion forces; in the paper, this is the parameter
            A in the agent repulsion force
   */
  static float AGENT_SCALE;

  /*!
   @brief		The magnitude of the agent-obstacle repulsion forces.

   This variable is not in the paper (dealing with obstacles is not discussed). However, this is the
   equivalent of AGENT_SCALE but for obstacles.
   */
  static float OBST_SCALE;

  /*!
   @brief		The reaction time used to define the driving force
   */
  static float REACTION_TIME;

  /*!
   @brief		The the fall-off distance of repulsive forces;  in the paper, this is the parameter B in
            the agent repulsion force.
   */
  static float FORCE_DISTANCE;
};
}  // namespace Zanlungo
#endif  // __ZANLUNGO_SIMULATOR_H__
