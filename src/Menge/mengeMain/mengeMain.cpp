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

// STL
#include <iostream>
#include <algorithm>
#include <string>
#include <exception>

// UTILS
#include "ProjectSpec.h"
// Command-line parser
#include "tclap/CmdLine.h"
// Visualization
#include "GLScene.h"
#include "GLViewer.h"
#include "ViewConfig.h"
#include "NullViewer.h"
#include "ContextSwitcher.h"
// Menge Runtime
#include "SimSystem.h"
#include "PluginEngine.h"
#include "SimulatorDB.h"
#include "os.h"
#include "BaseAgentContext.h"
#include "Logger.h"
// SceneGraph
#include "TextWriter.h"
// Menge Math
#include "RandGenerator.h"

using namespace Menge;

// Time step (gets set by the scene.xml file
float TIME_STEP = 0.2f;
// The number of uniform simulation steps to take between logical time steps
size_t SUB_STEPS = 0;

// Maximum duration of simulation (in seconds)
//		Can be set on command line.
float SIM_DURATION = 800.f;

// Controls whether the simulation is verbose or not
bool VERBOSE = false;

// The location of the executable - for basic executable resources
std::string ROOT;

SimulatorDB simDB;

/*!
 *	@brief		Initialize and start the simulation.
 *
 *	@param		dbEntry			The SimulatorDBEntry that describes the simulator 
 *								to be instantiated.
 *	@param		behaveFile		The path to a valid behavior specification file.
 *	@param		sceneFile		The path to a valid scene specification file.
 *	@param		outFile			The path to the output file to write.  If it is the
 *								empty string, no output file will be written.
 *	@param		scbVersion		The string indicating the version of scb file to write.
 *	@param		visualize		Determines if the simulation should be visualized.
 *								If true, an OpenGL visualizer is spawned, if false
 *								the simulation runs offline.
 *	@param		viewCfgFile		If visualizing, a path to an optional view configuration
 *								specification.  If none is provided, defaults are used.
 *	@param		dumpPath		The path to write screen grabs.  Only used in windows.
 *	@returns	0 for a successful run, non-zero otherwise.
 */
int simMain( SimulatorDBEntry * dbEntry, const std::string & behaveFile, const std::string & sceneFile, const std::string & outFile, const std::string & scbVersion, bool visualize, const std::string & viewCfgFile, const std::string & dumpPath ) {
	size_t agentCount;
	if ( outFile != "" ) logger << Logger::INFO_MSG << "Attempting to write scb file: " << outFile << "\n";
	SimSystem * system = dbEntry->getSimulatorSystem( agentCount, TIME_STEP, SUB_STEPS, SIM_DURATION, behaveFile, sceneFile, outFile, scbVersion, visualize, VERBOSE );

	if ( system == 0x0 ) {
		return 1;
	}

	SceneGraph::GLScene * scene = new SceneGraph::GLScene();
	scene->addSystem( system );
	
	if ( visualize ) {
		Vis::ViewConfig viewCfg;
		if ( VERBOSE ) {
			logger << Logger::INFO_MSG << "Using visualizer!";
		}
		if ( viewCfgFile == "" ) {
			if ( VERBOSE ) {
				logger << Logger::INFO_MSG << "\tUsing default visualization settings.";
			}
		} else {
			// TODO: Error handling
			if ( viewCfg.readXML( viewCfgFile ) ) {
				if ( VERBOSE ) {
					logger << Logger::INFO_MSG << "\tUsing visualization from: " << viewCfgFile << "\n";
					logger << Logger::INFO_MSG << viewCfg << "\n";
				}
			} else {
				logger << Logger::ERR_MSG << "Unable to read the specified view configuration (" << viewCfgFile << "). Terminating.";
				return 1;
			}
		}
		Vis::GLViewer view( viewCfg );

		view.setDumpPath( dumpPath );

#ifdef NDEBUG
		std::string viewTitle = "Pedestrian Simulation - " + dbEntry->viewerName();
#else
		std::string viewTitle = "(DEBUG) Pedestrian Simulation - " + dbEntry->viewerName();
#endif
		if ( !view.initViewer( viewTitle ) ) {
			std::cerr << "Unable to initialize the viewer\n\n";
			visualize = false;
		} else {
			view.setScene( scene );
			view.setFixedStep( TIME_STEP );
			view.setBGColor( 0.1f, 0.1f, 0.1f );
			dbEntry->populateScene( system, scene );
			SceneGraph::ContextSwitcher * switcher = new SceneGraph::ContextSwitcher();
			SceneGraph::Context * ctx = dbEntry->getAgentContext( system );
			switcher->addContext( ctx, SDLK_a );
			scene->setContext( switcher );
			view.newGLContext();
			logger.line();
		
			view.run();
			logger << Logger::INFO_MSG << "Simulation time: " << dbEntry->simDuration() << "\n";
		}
	} else  {
		logger << Logger::INFO_MSG << "NO VISUALIZATION!\n";
		Vis::NullViewer view;	// need the call back
		view.setScene( scene );
		view.setFixedStep( TIME_STEP );
		logger.line();
		
		view.run();
		std::cout << "Simulation time: " << dbEntry->simDuration() << "\n";
		logger << Logger::INFO_MSG << "Simulation time: " << dbEntry->simDuration() << "\n";
	}

	return 0;
}

int main(int argc, char* argv[]) {
	logger.setFile( "log.html" );
	logger << Logger::INFO_MSG << "initialized logger";

	std::string exePath( argv[0] );
	std::string absExePath;
	os::path::absPath( exePath, absExePath );
	std::string tail;
	os::path::split( absExePath, ROOT, tail );
	PluginEngine plugins( &simDB );
#ifdef _WIN32 
	#ifdef NDEBUG
	std::string pluginPath = os::path::join( 2, ROOT.c_str(), "plugins" );
	#else	// NDEBUG
	std::string pluginPath = os::path::join( 3, ROOT.c_str(), "plugins", "debug" );
	#endif	// NDEBUG
#else	// _WIN32
	std::string pluginPath = os::path::join( 2, ROOT.c_str(), "plugins" );
#endif	// _WIN32
	logger.line();
	logger << Logger::INFO_MSG << "Plugin path: " << pluginPath;
	plugins.loadPlugins( pluginPath );
	if ( simDB.modelCount() == 0 ) {
		logger << Logger::INFO_MSG << "There were no pedestrian models in the plugins folder\n";
		return 1;
	}

	SceneGraph::TextWriter::setDefaultFont( os::path::join( 2, ROOT.c_str(), "arial.ttf" ) );
	ProjectSpec projSpec;

	if (! projSpec.parseCommandParameters( argc, argv, &simDB ) ) {
		return 0;
	}	

	if ( !projSpec.fullySpecified() ) {
		return 1;
	}

	VERBOSE = projSpec.getVerbosity();
	TIME_STEP = projSpec.getTimeStep();
	SUB_STEPS = projSpec.getSubSteps();
	SIM_DURATION = projSpec.getDuration();
	std::string dumpPath = projSpec.getDumpPath();
	setDefaultGeneratorSeed( projSpec.getRandomSeed() );
	std::string outFile = projSpec.getOutputName();

	std::string viewCfgFile = projSpec.getView();
	bool useVis = viewCfgFile != "";
	std::string model( projSpec.getModel() );

	SimulatorDBEntry * simDBEntry = simDB.getDBEntry( model );
	if ( simDBEntry == 0x0 ) {
		std::cerr << "!!!  The specified model is not recognized: " << model << "\n";
		logger.close();
		return 1;
	}

	int result = simMain( simDBEntry, projSpec.getBehavior(), projSpec.getScene(), projSpec.getOutputName(), projSpec.getSCBVersion(), useVis, viewCfgFile, dumpPath );

	if ( result ) {
		std::cerr << "Simulation terminated through error.  See error log for details.\n";
	}
	logger.close();
	return result;
}

