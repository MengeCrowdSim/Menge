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
 @file       DummySimulator.h
 @brief      Contains the Dummy::Simulator class.
 */

#ifndef __DUMMY_SIMULATOR_H__
#define __DUMMY_SIMULATOR_H__

#include "DummyAgent.h"
#include "MengeCore/Agents/SimulatorBase.h"
#include "MengeCore/Math/RandGenerator.h"
#include "MengeCore/mengeCommon.h"

/*!
 @namespace		Dummy
 @brief			  The name space for the Dummy pedestrian model.

 The dummy pedestrian model is an incredibly simple example model. Agents compute a new velocity by
 randomly perturbing their preferred velocity. It is an example of the minimum implementation
 necessary to create a pedestrian model plug-in.
 */
namespace Dummy {
/*!
 @brief		The simulator for the Dummy pedestrian model.
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

   @param		  tagName		The name of the considered tag.
   @returns		By default, the simulator base ONLY uses common parameters. Always returns false.
   */
  virtual bool isExpTarget(const std::string& tagName) { return tagName == "Dummy"; }

  /*!
   @brief		Given an Experiment parameter name and value, sets the appropriate simulator parameter.
   @param		paramName		A string containing the parameter name for the experiment.
   @param		value			  A string containing the value for the parameter.
   @returns		Whether or not parameters were successfully set.
   */
  virtual bool setExpParam(const std::string& paramName,
                           const std::string& value) throw(Menge::Agents::XMLParamException);

 protected:
  friend class Agent;

  /*!
   @brief		The standard deviation of speed.
   */
  static Menge::Math::NormalFloatGenerator _speedDeviation;

  /*!
   @brief		The uniform distribution of direction.
   */
  static Menge::Math::UniformFloatGenerator _angleDeviation;
};
}  // namespace Dummy

#endif  // __DUMMY_SIMULATOR_H__
