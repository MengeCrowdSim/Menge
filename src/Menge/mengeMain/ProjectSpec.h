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

/*!
 *	@file		ProjectSpec.h
 *	@brief		The definition of the parameters required to run a simulation.
 */

#ifndef __PROJECT_SPEC_H__
#define	__PROJECT_SPEC_H__

#include <string>
#include <iostream>
#include "Logger.h"

using namespace Menge;

// Forward declaration
namespace Menge {
	class SimulatorDB; 
}

/*!
 *	@brief		The specificaiton of a simulation project.
 *
 *	Some project parameters have default values (such as duration and random seed).
 *	Others require explicit definitions (such as scene specification, etc.) 
 *	The break down is as follows:
 *
 *	Required parameters:
 *		- Scene specification
 *		- Behavior specification
 *		- Model name
 *
 *	Optional parameters (with defaults):
 *		- Interactive flag (false)
 *		- View specification (None -- no interactive view if interactive flag is false)
 *		- Output trajectory file name (None -- no output)
 *		- Trajectory version (2.1)
 *		- Simulation duration limit (400 s)
 *		- Simulation time step (use time step in scene specification)
 *		- Display verbose progress (false)
 *		- Random seed argument (0)
 */
class ProjectSpec {
public:
	/*!
	 *	@brief		Constructor
	 */
	ProjectSpec();


	/*!
	 *	@brief		Destructor
	 */
	~ProjectSpec();

	/*!
	 *	@brief		Get the scene specification file name
	 *
	 *	@returns	The path to the scene specification file.
	 */
	std::string getScene() const { return _sceneXML; }

	/*!
	 *	@brief		Get the beahvior specification file name
	 *
	 *	@returns	The path to the behavior specification file.
	 */
	std::string getBehavior() const { return _behaviorXML; }

	/*!
	 *	@brief		Get the model name
	 *
	 *	@returns	The path to the string specification file.
	 */
	std::string getModel() const { return _modelName; }

	/*!
	 *	@brief		Get the name of the trajectory output file.
	 *
	 *	@returns	The name of the output file.  Empty string if no
	 *				output file is to be written.
	 */
	std::string getOutputName() const { return _scbFileName; }

	/*!
	 *	@brief		Get the version string for the trajectory output file.
	 *
	 *	@returns	The version of the output file to write.
	 */
	std::string getSCBVersion() const { return _scbVersion; }

	/*!
	 *	@brief		Get the verbosity state for the application
	 *
	 *	@returns	True if the application should be verbose, false otherwise.
	 */
	bool getVerbosity() const { return _verbose; }

	/*!
	 *	@brief		Get the view specification file.
	 *
	 *	@returns	The name of the view configuration file (empty string if
	 *				there is none).
	 */
	std::string getView() const { return _viewConfig; }
	
	/*!
	 *	@brief		Get the random number seed value
	 *
	 *	@returns	The global random number generator seed.
	 */
	int getRandomSeed() const { return _seed; }

	/*!
	 *	@brief		Get the path at which to dump files.
	 *
	 *	@returns	The folder in which screen captures should be written.
	 */
	std::string getDumpPath() const { return _imgDumpPath; }

	/*!
	 *	@brief		Get the number of simulation sub steps to take.
	 *
	 *	@returns	The number of simulation sub steps.
	 */
	size_t getSubSteps() const { return _subSteps; }

	/*!
	 *	@brief		Get the maximum simulation duration
	 *
	 *	@returns	The maximum allowable simulation duration.
	 */
	float getDuration() const { return _duration; }

	/*!
	 *	@brief		Get the override simulation time step
	 *
	 *	@returns	The overridden simulation time step.  If negative,
	 *				use the time step in the scene specification.
	 */
	float getTimeStep() const { return _timeStep; }

	/*!
	 *	@brief		Parse the command line arguments
	 *
	 *	@param		argc		the number of command-line arguments.
	 *	@param		argv		The command-line parameters.
	 *	@param		simDB		A pointer to the current simulator database.
	 *	@returns	True if a simulation should be attempted (i.e. the 
	 *				project specification and command-line parameters require it
	 *				and false if not.
	 */
	bool parseCommandParameters( int argc, char* argv[], const SimulatorDB * simDB );

	/*!
	 *	@brief		Reports if the project specification contains enough information
	 *				to attempt to run a simulation.  I.e., it has *values* for all
	 *				fields but does not know if those values are valid.
	 *
	 *	@returns	True if all required fields have values, false otherwise.
	 */
	bool fullySpecified() const;

	/*!
	 *	@brief		Print the project specification to an output stream.
	 *
	 *	@param		out		An output stream.
	 *	@param		spec		A ProjectSpec
	 *	@returns	A reference to the output stream
	 */
	friend Logger & operator<<( Logger & out, const ProjectSpec & spec );

protected:
	/*!
	 *	@brief		Set output file
	 *
	 *	@param		fileName	The name of the file to write the trajectories to.
	 */
	void setOutputName( const std::string & fileName );

	/*!
	 *	@brief		Loads a project specification from an xml file
	 *
	 *	The return value only indicates if there was successful parsing of the xml.
	 *	It does not imply that there was sufficient information in the project to
	 *	run a simulation.  Ultimately, a successful project is defined by the union
	 *	of the project file and the command-line parameters.
	 *
	 *	@param		xmlName		The path to the file containing the project specification.
	 *	@returns	True if parsing was successful, false otherwise.
	 */
	bool loadFromXML( const std::string & xmlName );

	/*!
	 *	@brief		The path to the project -- it is the folder containing the
	 *				project xml.  Defaults to the current working directory.
	 */
	std::string		_projPath;

	/*!
	 *	@brief		The full path to the scene specification xml file.
	 */
	std::string		_sceneXML;

	/*!
	 *	@brief		The full path to the behavior specification xml file.
	 */
	std::string		_behaviorXML;

	/*!
	 *	@brief		The name of the pedestrian model.
	 */
	std::string		_modelName;

	/*!
	 *	@brief		The name of the output trajectory file to write.
	 */
	std::string		_scbFileName;

	/*!
	 *	@brief		The scb version to write.
	 */
	std::string		_scbVersion;

	/*!
	 *	@brief		Determines if the simulator should be verbose (printing status
	 *				to the console.
	 */
	bool			_verbose;

	/*!
	 *	@brief		The path to the file that specifies the view configuration.
	 */
	std::string		_viewConfig;

	/*!
	 *	@brief		The maximum allowable duration of the simulation (in simulation seconds).
	 */
	float			_duration;

	/*!
	 *	@brief		The size of the simulation time step.
	 */
	float			_timeStep;

	/*!
	 *	@brief		The seed number for the global random number generator.
	 */
	int				_seed;

	/*!
	 *	@brief		The path to the output folder for screen grab images
	 */
	std::string		_imgDumpPath;

	/*!
	 *	@brief		The number of intermediate simulation steps to take.
	 *
	 *				If the simulation time step is N Hz, and substeps is set to
	 *				k, then the effective simulation rate is N * ( 1 + k ) Hz.
	 *				For example, simulate at 10 Hz, with a single substep. 
	 *				Simulation is performed at 10 * (1 + 1) = 20 Hz, but the
	 *				results are only output at 10 Hz.
	 */
	size_t			_subSteps;

};

#endif	// __PROJECT_SPEC_H__
