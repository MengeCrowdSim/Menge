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

#ifndef __GCF_SIMULATOR_H__
#define __GCF_SIMULATOR_H__

/*!
 @file      GCFSimulator.h
 @brief     Contains the GCF::Simulator class; implements generalized centifugal force pedestrian
            model.
 */

#include "GCFAgent.h"
#include "MengeCore/Agents/SimulatorBase.h"
#include "MengeCore/mengeCommon.h"

/*!
 @namespace		GCF
 @brief			  The namespace for the GCF pedestrian model. 
 
 This is a model based on the model proposed in the Chraibi et al., 2010 paper.
 */
namespace GCF {
// forward declaration
class AgentContext;

/*!
 @brief      Defines the simulator operating on a GCF::Agent.
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
   @param		  tagName		the name of the considered tag
   @returns		By default, the simulator base ONLY uses common parameters. Always returns false.
   */
  virtual bool isExpTarget(const std::string& tagName) { return tagName == "GCF"; }

  /*!
   @brief		Given an Experiment parameter name and value, sets the appropriate simulator parameter.
   @param		paramName		A string containing the parameter name for the experiment.
   @param		value			  A string containing the value for the parameter.
   @returns		Whether or not parameters were successfully set.
   */
  virtual bool setExpParam(const std::string& paramName,
                           const std::string& value) throw(Menge::Agents::XMLParamException);

  /*!
   @brief	  After all agents and all obstacles have been added to the scene does the work to finish
            preparing the simulation to be run.

   This work is performed when the simulator is done being initialized. If a particular new
   pedestrian simulator requires particular finalization work, this function should be sub-classed
   and the parent class's version of the function should be explicitly called before any additional
   work is performed.
   */
  virtual void finalize();

 protected:
  friend class Agent;
  friend class AgentContext;

  /*!
   @brief		The reaction time used to define the driving force
   */
  static float REACTION_TIME;

  /*!
   @brief		The agent force strenth coefficient (nu)
   */
  static float NU_AGENT;

  /*!
   @brief		The maximum EFFECTIVE distance between agents at which any force can be generated.
   */
  static float MAX_AGENT_DIST;

  /*!
   @brief		The maximum force applied by an agent.
   */
  static float MAX_AGENT_FORCE;

  /*!
   @brief		The distance over which agent forces are interpolated
   */
  static float AGENT_INTERP_WIDTH;

 public:
  /*!
   @brief		If true, the agents will be colored based on speed.
   */
  static bool SPEED_COLOR;
};
}  // namespace GCF
#endif  // __GCF_SIMULATOR_H__
