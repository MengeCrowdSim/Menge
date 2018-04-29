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
 @file    ProjectSpec.h
 @brief    The definition of the parameters required to run a simulation.
 */

#ifndef __PROJECT_SPEC_H__
#define __PROJECT_SPEC_H__

#include <iostream>
#include <string>

#include "MengeCore/CoreConfig.h"
#include "MengeCore/Runtime/Logger.h"

namespace Menge {
// Forward declaration
class SimulatorDB;

/*!
 @brief    The specificaiton of a simulation project.

 Some project parameters have default values (such as duration and random seed). Others require
 explicit definitions (such as scene specification, etc.) The break down is as follows:

 Required parameters:
 - Scene specification
 - Behavior specification
 - Model name

 Optional parameters (with defaults):
 - Interactive flag (false)
 - View specification (None -- no interactive view if interactive flag is false)
 - Output trajectory file name (None -- no output)
 - Trajectory version (2.1)
 - Simulation duration limit (400 s)
 - Simulation time step (use time step in scene specification)
 - Display verbose progress (false)
 - Random seed argument (0)
 */
class MENGE_API ProjectSpec {
 public:
  /*!
   @brief    Constructor
   */
  ProjectSpec();

  /*!
   @brief    Destructor
   */
  ~ProjectSpec();

  /** @name  Parameter access */
  //@{

  /*!
   @brief    Get the scene specification file name

   @returns  The path to the scene specification file.
   */
  std::string getScene() const { return _sceneXML; }

  /*!
   @brief    Set the scene specificaiton file name.

   @param    scene_name   The path to the scene specificaiton XML file. If relative, it is relative
                          to the project file.
   */
  void setScene(const std::string& scene_name) { _sceneXML = scene_name; }

  /*!
   @brief    Get the beahvior specification file name

   @returns  The path to the behavior specification file.
   */
  std::string getBehavior() const { return _behaviorXML; }

  /*!
   @brief    Set the behavior specificaiton file name.

   @param    behavior_name  The path to the behavior specification XML file. If the path is
                            relative, it is relative to the project file.
   */
  void setBehavior(const std::string& behavior_name) { _behaviorXML = behavior_name; }

  /*!
   @brief    Get the model name

   @returns  The path to the string specification file.
   */
  std::string getModel() const { return _modelName; }

  /*!
   @brief    Sets the name of the model to use.

   @param  model_name    The name of the pedestrian model to use.
   */
  void setModel(const std::string& model_name) { _modelName = model_name; }

  /*!
   @brief    Get the name of the trajectory output file.

   @returns  The name of the output file. Empty string if no output file is to be written.
   */
  std::string getOutputName() const { return _scbFileName; }

  /*!
   @brief    Set output file

   @param    fileName  The name of the file to write the trajectories to.
   */
  void setOutputName(const std::string& fileName);

  /*!
   @brief    Get the version string for the trajectory output file.

   @returns  The version of the output file to write.
   */
  std::string getSCBVersion() const { return _scbVersion; }

  /*!
   @brief    Sets the version string for the trajectory output file.
   @param  scb_version    The version string; if the version string is invalid, this will be caught
                          during validation.
   */
  void setSCBVersion(const std::string& scb_version) { _scbVersion = scb_version; }

  /*!
   @brief    Get the verbosity state for the application

   @returns  True if the application should be verbose, false otherwise.
   */
  bool getVerbosity() const { return _verbose; }

  /*!
   @brief    Defines the simulator's verbosity.

   @param    verbose_state   If true, configures the simulator to be verbose.
   */
  void setVerbosity(bool verbose_state) { _verbose = verbose_state; }

  /*!
   @brief    Get the view specification file.

   @returns  The name of the view configuration file (empty string if there is none).
   */
  std::string getView() const { return _viewConfig; }

  /*!
   @brief    Set the the view specification file.

   @param    view_name  The path to the view specification file. If the path is relative, it is
                        relative to the project file.
   */
  void setView(const std::string& view_name) { _viewConfig = view_name; }

  /*!
   @brief    Get the random number seed value

   @returns  The global random number generator seed.
   */
  int getRandomSeed() const { return _seed; }

  /*!
   @brief    Set the random number seed value.

   @param    seed    The seed value for the random number system.
   */
  void setRandomSeed(int seed) { _seed = seed; }

  /*!
   @brief    Get the path at which to dump files.

   @returns  The folder in which screen captures should be written.
   */
  std::string getDumpPath() const { return _imgDumpPath; }

  /*!
   @brief    Sets the path (folder) in which to dump captured image files.

   @param    dump_path   The path to a folder to write captured image files to.
   */
  void setDumpPath(const std::string& dump_path) { _imgDumpPath = dump_path; }

  /*!
   @brief    Get the number of simulation sub steps to take.

   @returns  The number of simulation sub steps.
   */
  size_t getSubSteps() const { return _subSteps; }

  /*!
   @brief    Sets the number of simulation sub steps to take.

   @param    sub_step_count   The number of steps to take between each major time step defined by
                              getTimeStep().
   */
  void setSubSteps(size_t sub_step_count) { _subSteps = sub_step_count; }

  /*!
   @brief    Get the maximum simulation duration

   @returns  The maximum allowable simulation duration.
   */
  float getDuration() const { return _duration; }

  /*!
   @brief    Set the maximum simulation duration.

   @param    duration    The maximum allowablue simulation duration (in simulation seconds).
   */
  void setDuration(float duration) { _duration = duration; }

  /*!
   @brief    Get the override simulation time step

   @returns  The overridden simulation time step. If negative, use the time step in the scene
            specification.
   */
  float getTimeStep() const { return _timeStep; }

  /*!
   @brief    Set the override simulation time step.

   @param    time_step    The override time step. If negative, use the time step in the scene
                          specification.
   */
  void setTimeStep(float time_step) { _timeStep = time_step; }

  //@}

  /*!
   @brief    Loads a project specification from an xml file

   The return value only indicates if there was successful parsing of the xml. It does not imply
   that there was sufficient information in the project to run a simulation. Ultimately, a
   successful project is defined by the union of the project file and the command-line parameters.

   @param    xmlName    The path to the file containing the project specification.
   @returns  True if parsing was successful, false otherwise.
   */
  bool loadFromXML(const std::string& xmlName);

  /*!
   @brief    Reports if the project specification contains enough information to attempt to run a
            simulation.
            
   I.e., it has *values* for all fields but does not know if those values are valid.

   @returns  True if all required fields have values, false otherwise.
   */
  bool fullySpecified() const;

  /*!
   @brief    Print the project specification to an output stream.

   @param    out      An output stream.
   @param    spec    A ProjectSpec
   @returns  A reference to the output stream
   */
  friend MENGE_API Logger& operator<<(Logger& out, const ProjectSpec& spec);

 private:
  /*!
   @brief    The path to the project -- it is the folder containing the project xml.
   
   Defaults to the current working directory.
   */
  std::string _projPath;

  /*!
   @brief    The full path to the scene specification xml file.
   */
  std::string _sceneXML;

  /*!
   @brief    The full path to the behavior specification xml file.
   */
  std::string _behaviorXML;

  /*!
   @brief    The name of the pedestrian model.
   */
  std::string _modelName;

  /*!
   @brief    The name of the output trajectory file to write.
   */
  std::string _scbFileName;

  /*!
   @brief    The scb version to write.
   */
  std::string _scbVersion;

  /*!
   @brief    Determines if the simulator should be verbose (printing status to the console.
   */
  bool _verbose;

  /*!
   @brief    The path to the file that specifies the view configuration.
   */
  std::string _viewConfig;

  /*!
   @brief    The maximum allowable duration of the simulation (in simulation seconds).
   */
  float _duration;

  /*!
   @brief    The size of the simulation time step.
   */
  float _timeStep;

  /*!
   @brief    The seed number for the global random number generator.
   */
  int _seed;

  /*!
   @brief    The path to the output folder for screen grab images
   */
  std::string _imgDumpPath;

  /*!
   @brief    The number of intermediate simulation steps to take.

   If the simulation time step is N Hz, and substeps is set to k, then the effective simulation rate
   is N * ( 1 + k ) Hz. For example, simulate at 10 Hz, with a single substep. Simulation is
   performed at 10 * (1 + 1) = 20 Hz, but the results are only output at 10 Hz.
   */
  size_t _subSteps;
};
}  // namespace Menge
#endif  // __PROJECT_SPEC_H__
