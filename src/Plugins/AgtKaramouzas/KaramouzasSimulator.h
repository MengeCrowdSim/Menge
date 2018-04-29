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

#ifndef __KARAMOUZAS_SIMULATOR_H__
#define __KARAMOUZAS_SIMULATOR_H__

/*!
 @file       KaramouzasSimulator.h
 @brief      Contains the Karamouzas::Simulator class

 Implements Karamouzas's 2009 pedestrian model "A Predictive Collision Avoidance Model for
 Pedestrian Simulation"
 */

#include "KaramouzasAgent.h"
#include "MengeCore/Agents/SimulatorBase.h"
#include "MengeCore/mengeCommon.h"

/*!
 @namespace		Karamouzas
 @brief			Contains the specification of the pedestrian model from the Karamouzas et al., 2009
            paper.
 */
namespace Karamouzas {
/*!
 @brief      Defines the simulator operating on a Karamouzas::Agent.
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
  virtual bool isExpTarget(const std::string& tagName) { return tagName == "Karamouzas"; }

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
   @brief		Agent orientation is a weighted average between preferred direction and actual
            direction; this is that weight.
   */
  static float ORIENT_WEIGHT;

  /*!
   @brief		The half angle that defines the field of view around orientation
   */
  static float COS_FOV_ANGLE;

  /*!
   @brief		The reaction time used to define the driving force
   */
  static float REACTION_TIME;

  /*!
   @brief		The steepness that the wall forces fall off: K > 0
   */
  static float WALL_STEEPNESS;

  /*!
   @brief		The safe distance agents prefer to keep from the walls
   */
  static float WALL_DISTANCE;

  /*!
   @brief		The number of agents considered in the colliding set
   */
  static int COLLIDING_COUNT;

  /*!
   @brief		The critical distance at which agent response force climbs.
   */
  static float D_MIN;

  /*!
   @brief		The distance at which a constant agent response force begins.
   */
  static float D_MID;

  /*!
   @brief		The distance at which there is no longer an agent response force.
   */
  static float D_MAX;

  /*!
   @brief		The level of response force in the constant region.
   */
  static float AGENT_FORCE;
};
}  // namespace Karamouzas

#endif  // __KARAMOUZAS_SIMULATOR_H__
