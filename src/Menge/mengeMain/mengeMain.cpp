/*

License

Menge
Copyright © and trademark ™ 2012-14 University of North Carolina at Chapel Hill.
All rights reserved.

Permission to use, copy, modify, and distribute this software and its documentation
for educational, research, and non-profit purposes, without fee, and without a
written agreement is hereby granted, provided that the above copyright notice,
this paragraph, and the following four paragraphs appear in all copies.

This software program and documentation are copyrighted by the University of North
Carolina at Chapel Hill. The software program and documentation are supplied "as is,"
without any accompanying services from the University of North Carolina at Chapel
Hill or the authors. The University of North Carolina at Chapel Hill and the
authors do not warrant that the operation of the program will be uninterrupted
or error-free. The end-user understands that the program was developed for research
purposes and is advised not to rely exclusively on the program for any reason.

IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE AUTHORS
BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
DOCUMENTATION, EVEN IF THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE
AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY
DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY STATUTORY WARRANTY
OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND
THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS HAVE NO OBLIGATIONS
TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

Any questions or comments should be sent to the authors {menge,geom}@cs.unc.edu

*/

#include "MengeCore/Agents/SimulatorInterface.h"
#include "MengeCore/Math/RandGenerator.h"
#include "MengeCore/PluginEngine/CorePluginEngine.h"
#include "MengeCore/ProjectSpec.h"
#include "MengeCore/Runtime/Logger.h"
#include "MengeCore/Runtime/SimulatorDB.h"
#include "MengeCore/Runtime/os.h"

#include "MengeVis/PluginEngine/VisPluginEngine.h"
#include "MengeVis/Runtime/AgentContext/BaseAgentContext.h"
#include "MengeVis/Runtime/EventInjectContext.h"
#include "MengeVis/Runtime/MengeContext.h"
#include "MengeVis/Runtime/SimSystem.h"
#include "MengeVis/SceneGraph/ContextSwitcher.h"
#include "MengeVis/SceneGraph/GLScene.h"
#include "MengeVis/SceneGraph/TextWriter.h"
#include "MengeVis/Viewer/GLViewer.h"
#include "MengeVis/Viewer/ViewConfig.h"

#include "thirdParty/tclap/CmdLine.h"

#include <stdio.h>
#include <algorithm>
#include <exception>
#include <iostream>
#include <string>

#if _MSC_VER >= 1900
FILE _iob[] = {*stdin, *stdout, *stderr};
extern "C" FILE* __cdecl __iob_func(void) { return _iob; }
#endif  // _MSC_VER >= 1900

using namespace Menge;
using Menge::ProjectSpec;
using Menge::PluginEngine::CorePluginEngine;
using MengeVis::PluginEngine::VisPluginEngine;

// Time step (in seconds)
float TIME_STEP = 0.2f;
// The number of uniform simulation steps to take between logical time steps
size_t SUB_STEPS = 0;
// Maximum duration of simulation (in seconds)
float SIM_DURATION = 800.f;
// Controls whether the simulation is verbose or not
bool VERBOSE = false;
// The location of the executable - for basic executable resources
std::string ROOT;

SimulatorDB simDB;

/*!
 *  @brief    Utility function for defining the plugin directory.

 *  @param    The path to the plugins.
 */
std::string getPluginPath() {
#ifdef _WIN32
#ifdef NDEBUG
  return os::path::join(2, ROOT.c_str(), "plugins");
#else   // NDEBUG
  return os::path::join(3, ROOT.c_str(), "plugins", "debug");
#endif  // NDEBUG
#else   // _WIN32
  return os::path::join(2, ROOT.c_str(), "plugins");
#endif  // _WIN32
}

/*!
 *  @brief    Parse the command line arguments

 *  @param    argc    the number of command-line arguments.
 *  @param    argv    The command-line parameters.
 *  @param    simDB    A pointer to the current simulator database.
 *  @returns  True if a simulation should be attempted (i.e. the
 *            project specification and command-line parameters require it
 *            and false if not.
 */
bool parseCommandParameters(int argc, char* argv[], ProjectSpec* spec, const SimulatorDB& simDB) {
  bool valid = true;
  // Command line argument fields
  bool verbose = false;
  try {
    TCLAP::CmdLine cmd("Crowd simulation with behavior.  ", ' ', "0.9.2");
    // arguments: flag, name, description, required, default value type description
    TCLAP::ValueArg<std::string> projArg("p", "project", "The name of the project file", false, "",
                                         "string", cmd);
    TCLAP::ValueArg<std::string> sceneArg("s", "scene", "Scene configuration file", false, "",
                                          "string", cmd);
    TCLAP::ValueArg<std::string> behaveArg("b", "behavior", "Scene behavior file", false, "",
                                           "string", cmd);
    TCLAP::ValueArg<std::string> viewCfgArg("", "view",
                                            "A view config file to specify the "
                                            "view.",
                                            false, "", "string", cmd);
    TCLAP::ValueArg<std::string> outputArg("o", "output",
                                           "Name of output scb file (Only writes"
                                           " output if file provided)",
                                           false, "", "string", cmd);
    TCLAP::ValueArg<std::string> versionArg("", "scbVersion",
                                            "Version of scb file to write "
                                            "(1.0, 2.0, 2.1, 2.2, 2.3, or 2.4 -- 2.1 is the "
                                            "default",
                                            false, "", "string", cmd);
    TCLAP::ValueArg<float> durationArg("d", "duration",
                                       "Maximum duration of simulation (if "
                                       "final state is not achieved.)  Defaults to 400 seconds.",
                                       false, -1.f, "float", cmd);
    TCLAP::ValueArg<float> timeStepArg("t", "timeStep",
                                       "Override the time step in the scene "
                                       "specification with this one",
                                       false, -1.f, "float", cmd);
    TCLAP::SwitchArg silentArg("", "verbose",
                               "Make the simulator print loading and simulating "
                               "progress",
                               cmd, false);
    TCLAP::ValueArg<int> randomSeedArg("r", "random",
                                       "Specify the global, default random seed. "
                                       "If not defined, or zero is given, the default seed will "
                                       "be extracted from the system clock every time a default "
                                       "seed is requested.  Otherwise the constant value will "
                                       "be provided.",
                                       false, -1, "int", cmd);
    TCLAP::ValueArg<int> subSampleArg("", "subSteps",
                                      "Specify the number of sub steps to take."
                                      " If the simulation time step is 10 Hz with 1 substep, it"
                                      " actually runs at 20 Hz, but output is only updated at"
                                      " 10 Hz.",
                                      false, -1, "int", cmd);

    std::string modelDoc = "The pedestrian model to use.  Should be one of: ";
    modelDoc += simDB.paramList();
    TCLAP::ValueArg<std::string> modelArg("m", "model", modelDoc.c_str(), false, "", "string", cmd);
    TCLAP::SwitchArg listModelsArg("l", "listModels",
                                   "Lists the models supported. If this is "
                                   "specified, no simulation is run.",
                                   cmd, false);
    TCLAP::SwitchArg listModelsFullArg("L", "listModelsDetails",
                                       "Lists the models supported and"
                                       " provides more details. If this is specified, no "
                                       "simulation is run.",
                                       cmd, false);
    TCLAP::ValueArg<std::string> dumpPathArg("u", "dumpPath",
                                             "The path to a folder in which "
                                             "screen grabs should be dumped.  Defaults to "
                                             "current directory.  (Will create the directory "
                                             "if it doesn't already exist.)",
                                             false, "", "string", cmd);

    cmd.parse(argc, argv);

    if (listModelsFullArg.getValue()) {
      std::cout << "\n" << simDB.longDescriptions() << "\n";
      return false;
    }
    if (listModelsArg.getValue()) {
      std::cout << "\n" << simDB.briefDescriptions() << "\n";
      return false;
    }

    // Read the project file
    std::string projName = projArg.getValue();
    if (projName != "") {
      if (!spec->loadFromXML(projName)) {
        return false;
      }
    }

    // Required arguments
    std::string temp = sceneArg.getValue();
    if (temp != "") {
      std::string tmp = os::path::join(2, ".", temp.c_str());
      os::path::absPath(tmp, temp);
      spec->setScene(temp);
    }

    temp = behaveArg.getValue();
    if (temp != "") {
      std::string tmp = os::path::join(2, ".", temp.c_str());
      os::path::absPath(tmp, temp);
      spec->setBehavior(temp);
    }

    temp = modelArg.getValue();
    if (temp != "") spec->setModel(temp);

    // Optional arguments
    verbose = silentArg.getValue();
    spec->setVerbosity(verbose);

    temp = outputArg.getValue();
    if (temp != "") spec->setOutputName(temp);

    temp = versionArg.getValue();
    if (temp != "") spec->setSCBVersion(temp);

    float f = timeStepArg.getValue();
    if (f > 0.f) spec->setTimeStep(f);

    f = durationArg.getValue();
    if (f > 0.f) spec->setDuration(f);

    int seed = randomSeedArg.getValue();
    if (seed > -1) spec->setRandomSeed(seed);

    temp = viewCfgArg.getValue();
    if (temp != "") spec->setView(temp);

    int sub_steps = subSampleArg.getValue();
    if (sub_steps > -1) spec->setSubSteps(static_cast<size_t>(sub_steps));

    temp = dumpPathArg.getValue();
    if (temp != "") {
      std::string tmp = os::path::join(2, ".", temp.c_str());
      os::path::absPath(tmp, temp);
      spec->setDumpPath(temp);
    }

  } catch (TCLAP::ArgException& e) {
    std::cerr << "Error parsing command-line arguments: " << e.error() << " for arg " << e.argId()
              << std::endl;
  }

  if (verbose) {
    logger << Logger::INFO_MSG << (*spec) << "\n";
  }
  return valid;
}

/*!
 *  @brief    Initialize and start the simulation.

 *  @param    dbEntry      The SimulatorDBEntry that describes the simulator
 *                to be instantiated.
 *  @param    behaveFile    The path to a valid behavior specification file.
 *  @param    sceneFile    The path to a valid scene specification file.
 *  @param    outFile      The path to the output file to write.  If it is the
 *                empty string, no output file will be
 *written.
 *  @param    scbVersion    The string indicating the version of scb file to
 *write.
 *  @param    visualize    Determines if the simulation should be visualized.
 *                If true, an OpenGL visualizer is spawned, if
 *false
 *                the simulation runs offline.
 *  @param    viewCfgFile    If visualizing, a path to an optional view
 *configuration
 *                specification.  If none is provided, defaults are
 *used.
 *  @param    dumpPath    The path to write screen grabs.  Only used in
 *windows.
 *  @returns  0 for a successful run, non-zero otherwise.
 */
int simMain(SimulatorDBEntry* dbEntry, const std::string& behaveFile, const std::string& sceneFile,
            const std::string& outFile, const std::string& scbVersion, bool visualize,
            const std::string& viewCfgFile, const std::string& dumpPath) {
  size_t agentCount;
  if (outFile != "") {
    logger << Logger::INFO_MSG << "Attempting to write scb file: " << outFile << "\n";
  }

  using Menge::Agents::SimulatorInterface;
  using MengeVis::Runtime::BaseAgentContext;
  using MengeVis::Runtime::EventInjectionContext;
  using MengeVis::Runtime::SimSystem;
  using MengeVis::SceneGraph::Context;
  using MengeVis::SceneGraph::ContextSwitcher;
  using MengeVis::SceneGraph::GLScene;
  using MengeVis::SceneGraph::TextWriter;
  using MengeVis::Viewer::GLViewer;
  using MengeVis::Viewer::ViewConfig;

  SimulatorInterface* sim =
      dbEntry->getSimulator(agentCount, TIME_STEP, SUB_STEPS, SIM_DURATION, behaveFile, sceneFile,
                            outFile, scbVersion, VERBOSE);

  if (sim == 0x0) {
    return 1;
  }

  std::cout << "Starting...\n";

  if (visualize) {
    logger.line();
    logger << Logger::INFO_MSG << "Initializing visualization...";
    VisPluginEngine visPlugins;
    visPlugins.loadPlugins(getPluginPath());

    TextWriter::setDefaultFont(os::path::join(2, ROOT.c_str(), "arial.ttf"));

    ViewConfig viewCfg;
    if (VERBOSE) {
      logger << Logger::INFO_MSG << "Using visualizer!";
    }
    if (viewCfgFile == "") {
      if (VERBOSE) {
        logger << Logger::INFO_MSG << "\tUsing default visualization settings.";
      }
    } else {
      // TODO: Error handling
      if (viewCfg.readXML(viewCfgFile)) {
        if (VERBOSE) {
          logger << Logger::INFO_MSG << "\tUsing visualization from: " << viewCfgFile << "\n";
          logger << Logger::INFO_MSG << viewCfg << "\n";
        }
      } else {
        logger << Logger::ERR_MSG << "Unable to read the specified view configuration ("
               << viewCfgFile << "). Terminating.";
        return 1;
      }
    }
    GLViewer view(viewCfg);

    view.setDumpPath(dumpPath);

#ifdef NDEBUG
    std::string viewTitle = "Pedestrian Simulation - " + dbEntry->viewerName();
#else
    std::string viewTitle = "(DEBUG) Pedestrian Simulation - " + dbEntry->viewerName();
#endif
    if (!view.initViewer(viewTitle)) {
      std::cerr << "Unable to initialize the viewer\n\n";
      visualize = false;
    } else {
      GLScene* scene = new GLScene();
      SimSystem* system = new SimSystem(sim);
      system->populateScene(scene);
      scene->addSystem(system);
      view.setScene(scene);

      view.setFixedStep(TIME_STEP);
      view.setBGColor(0.1f, 0.1f, 0.1f);
      MengeVis::Runtime::MengeContext* ctx = new MengeVis::Runtime::MengeContext(sim);
      scene->setContext(new EventInjectionContext(ctx));
      view.newGLContext();
      logger.line();

      view.run();
    }
  } else {
    bool running = true;
    while (running) {
      running = sim->step();
    }
  }

  std::cout << "...Finished\n";
  std::cout << "Simulation time: " << dbEntry->simDuration() << "\n";
  logger << Logger::INFO_MSG << "Simulation time: " << dbEntry->simDuration() << "\n";

  return 0;
}

int main(int argc, char* argv[]) {
  logger.setFile("log.html");
  logger << Logger::INFO_MSG << "initialized logger";

  std::string exePath(argv[0]);
  std::string absExePath;
  os::path::absPath(exePath, absExePath);
  std::string tail;
  os::path::split(absExePath, ROOT, tail);
  CorePluginEngine plugins(&simDB);
  logger.line();
  std::string pluginPath = getPluginPath();
  logger << Logger::INFO_MSG << "Plugin path: " << pluginPath;
  plugins.loadPlugins(pluginPath);
  if (simDB.modelCount() == 0) {
    logger << Logger::INFO_MSG << "There were no pedestrian models in the plugins folder\n";
    return 1;
  }

  ProjectSpec projSpec;
  if (!parseCommandParameters(argc, argv, &projSpec, simDB)) {
    return 0;
  }

  if (!projSpec.fullySpecified()) {
    return 1;
  }

  VERBOSE = projSpec.getVerbosity();
  TIME_STEP = projSpec.getTimeStep();
  SUB_STEPS = projSpec.getSubSteps();
  SIM_DURATION = projSpec.getDuration();
  std::string dumpPath = projSpec.getDumpPath();
  Menge::Math::setDefaultGeneratorSeed(projSpec.getRandomSeed());
  std::string outFile = projSpec.getOutputName();

  std::string viewCfgFile = projSpec.getView();
  bool useVis = viewCfgFile != "";
  std::string model(projSpec.getModel());

  SimulatorDBEntry* simDBEntry = simDB.getDBEntry(model);
  if (simDBEntry == 0x0) {
    std::cerr << "!!!  The specified model is not recognized: " << model << "\n";
    logger.close();
    return 1;
  }

  int result =
      simMain(simDBEntry, projSpec.getBehavior(), projSpec.getScene(), projSpec.getOutputName(),
              projSpec.getSCBVersion(), useVis, viewCfgFile, dumpPath);

  if (result) {
    std::cerr << "Simulation terminated through error.  See error log for details.\n";
  }
  logger.close();
  return result;
}
