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

#include "ProjectSpec.h"
#include "tinyxml.h"
#include "SimulatorDB.h"
#include "tclap/CmdLine.h"
#include "os.h"

////////////////////////////////////////////////////////////////
//			Implementation of ProjectSpec
////////////////////////////////////////////////////////////////

ProjectSpec::ProjectSpec() : _projPath("."),
							 _sceneXML(""),
							 _behaviorXML(""),
							 _modelName(""),
							 _scbFileName(""),
							 _scbVersion("2.1"),
							 _verbose(false),
							 _viewConfig(""),
							 _duration(400.f),
							 _timeStep(-1.f),
							 _seed(0),
							 _imgDumpPath(".")
							 {
}

////////////////////////////////////////////////////////////////

ProjectSpec::~ProjectSpec() {
}

////////////////////////////////////////////////////////////////

bool ProjectSpec::parseCommandParameters( int argc, char* argv[], const SimulatorDB * simDB ) {
	bool valid = true;
	// Command line argument fields
	try {
		TCLAP::CmdLine cmd("Crowd simulation with behavior.  ", ' ', "0.9");
		// arguments: flag, name, description, required, default value type description
		TCLAP::ValueArg< std::string > projArg( "p", "project", "The name of the project file", false, "", "string", cmd );
		TCLAP::ValueArg< std::string > sceneArg( "s", "scene", "Scene configuration file", false, "", "string", cmd );
		TCLAP::ValueArg< std::string > behaveArg( "b", "behavior", "Scene behavior file", false, "", "string", cmd );
		TCLAP::ValueArg< std::string > viewCfgArg( "", "view", "A view config file to specify the view - if this argument is specified, do not specify the -i/-interactive argument.", false, "", "string", cmd );
		TCLAP::ValueArg< std::string > outputArg( "o", "output", "Name of output file (Only writes output if file provided)", false, "", "string", cmd );
		TCLAP::ValueArg< std::string > versionArg( "", "scbVersion", "Version of scb file to write (1.0, 2.0, 2.1, 2.2, 2.3, or 2.4 -- 2.1 is the default", false, "2.1", "string", cmd );
		TCLAP::ValueArg< float > durationArg( "d", "duration", "Maximum duration of simulation (if final state is not achieved.)  Defaults to 400 seconds.", false, -1.f, "float", cmd );
		TCLAP::ValueArg< float > timeStepArg( "t", "timeStep", "Override the time step in the scene specification with this one", false, -1.f, "float", cmd );
		TCLAP::SwitchArg silentArg( "", "verbose", "Make the simulator print loading and simulating progress", cmd, false );
		TCLAP::ValueArg< int > randomSeedArg( "r", "random", "Specify the global, default random seed.  If not defined, or zero is given, the default seed will be extracted from the system clock every time a default seed is requested.  Otherwise the constant value will be provided.", false, 0, "int", cmd );
		TCLAP::ValueArg< int > subSampleArg( "", "subSteps", "Specify the number of sub steps to take.  If the simulation time step is 10 Hz with 1 substep, it actually runs at 20 Hz, but output is only updated at 10 Hz.", false, 0, "int", cmd );

		std::string modelDoc = "The pedestrian model to use.  Should be one of: ";
		modelDoc += simDB->paramList();
		TCLAP::ValueArg< std::string > modelArg( "m", "model", modelDoc.c_str(), false, "", "string", cmd );
		TCLAP::SwitchArg listModelsArg( "l", "listModels", "Lists the models supported. If this is specified, no simulation is run.", cmd, false );
		TCLAP::SwitchArg listModelsFullArg( "L", "listModelsDetails", "Lists the models supported and provides more details. If this is specified, no simulation is run.", cmd, false );
		TCLAP::ValueArg< std::string > dumpPathArg( "u", "dumpPath", "The path to a folder in which screen grabs should be dumped.  Defaults to current directory.  (Will create the directory if it doesn't already exist.)", false, "", "string", cmd );
        
		cmd.parse( argc, argv );

		if ( listModelsFullArg.getValue() ) {
			std::cout << "\n" << simDB->longDescriptions() << "\n";
			return false;
		}
		if ( listModelsArg.getValue() ) {
			std::cout << "\n" << simDB->briefDescriptions() << "\n";
			return false;
		}

		// Read the project file
		std::string projName = projArg.getValue();
		if ( projName != "" ) {
			if ( ! loadFromXML( projName ) ) {
				return false;
			}
		}

		// Required arguments
		std::string temp = sceneArg.getValue();
		if ( temp != "" ) {
			std::string tmp = os::path::join( 2, ".", temp.c_str() );
			os::path::absPath( tmp, _sceneXML );
		}
		if ( _sceneXML == "" ) {
			std::cerr << "!!!  To run a simulation, a scene specification must be provided (-s or --scene)\n";
			std::cerr << "     or defined in the project file.\n";
			valid = false;
		}
		
		temp = behaveArg.getValue();
		if ( temp != "" ) {
			std::string tmp = os::path::join( 2, ".", temp.c_str() );
			os::path::absPath( tmp, _behaviorXML );
		}
		if ( _behaviorXML == "" ) {
			std::cerr << "!!!  To run a simulation, a behavior specification must be provided (-b or --behavior)\n";
			std::cerr << "     or defined in the project file.\n";
			valid = false;
		}
		
		temp = modelArg.getValue();
		if ( temp != "" ) _modelName = temp;
		if ( _modelName == "" ) {
			std::cerr << "!!!  To run a simulation, a named pedestrian model must be specified (-m or --model)\n";
			std::cerr << "     or defined in the project file.\n";
			valid = false;
		}

		// Optional arguments
		_verbose = silentArg.getValue();
		
		temp = outputArg.getValue();
		if ( temp != "" ) setOutputName( temp );

		temp = versionArg.getValue();
		if ( temp != "" ) _scbVersion = temp;

		float f = timeStepArg.getValue();
		if ( f > 0.f ) _timeStep = f;
		
		f = durationArg.getValue();
		if ( f > 0.f ) _duration = f;

		int seed = randomSeedArg.getValue();
		if ( seed > -1 ) _seed = seed;

		temp = viewCfgArg.getValue();
		if ( temp != "" ) _viewConfig = temp;

		_subSteps = (size_t)subSampleArg.getValue();


		temp = dumpPathArg.getValue();
		if ( temp != "" ) {
			std::string tmp = os::path::join( 2, ".", temp.c_str() );
			os::path::absPath( tmp, _imgDumpPath );
		}

	} catch ( TCLAP::ArgException &e ) {
		std::cerr << "Error parsing command-line arguments: " << e.error() << " for arg " << e.argId() << std::endl;
	}

	if ( _verbose ) {
		logger << Logger::INFO_MSG << (*this) << "\n";
	}
	return valid;
}

////////////////////////////////////////////////////////////////

bool ProjectSpec::fullySpecified() const {
	bool valid = true;
	if ( _behaviorXML == "" ) {
		std::cerr << "!!!  To run a simulation, a behavior specification must be provided (-b or --behavior)\n";
		std::cerr << "     or defined in the project file.\n";
		valid = false;
	}
	
	if ( _sceneXML == "" ) {
		std::cerr << "!!!  To run a simulation, a scene specification must be provided (-s or --scene)\n";
		std::cerr << "     or defined in the project file.\n";
		valid = false;
	}

	
	if ( _modelName == "" ) {
		std::cerr << "!!!  To run a simulation, a named pedestrian model must be specified (-m or --model)\n";
		std::cerr << "     or defined in the project file.\n";
		valid = false;
	}

	return valid;
}

////////////////////////////////////////////////////////////////

void ProjectSpec::setOutputName( const std::string & fileName ) {	
	os::path::absPath( fileName, _scbFileName );
	if ( _scbFileName != "" ) {
		size_t nameLen = _scbFileName.size();
		if ( _scbFileName[ nameLen - 1 ] != 'b' ||
			_scbFileName[ nameLen - 2 ] != 'c' ||
			_scbFileName[ nameLen - 3 ] != 's' ||
			_scbFileName[ nameLen - 4 ] != '.' ) {
				_scbFileName += ".scb";
		}
	}
}

////////////////////////////////////////////////////////////////

bool ProjectSpec::loadFromXML( const std::string & xmlName ) {
	// TODO: Extract the project path from xmlName. path, file = os.path.split( os.path.absPath( xmlName ) )
	TiXmlDocument xml( xmlName );
	bool valid = xml.LoadFile();

	if ( !valid ) {	// load xml file
		std::cerr << "Could not load project specification xml " << xmlName << ".\n";
		return false;
	}

	TiXmlElement* rootNode = xml.RootElement();	
	if( ! rootNode ) {
		std::cerr << "Root element does not exist\n.";
		return false;
	}

	if( rootNode->ValueStr () != "Project" ) {
		std::cerr << "Root element value is not 'Project'.\n" ;
		return false;
	}

	std::string absPath;
	os::path::absPath( xmlName, absPath );
	std::string junk;
	os::path::split( absPath, _projPath, junk );
	logger.line();
	logger << Logger::INFO_MSG << "Project root: " << _projPath << "\n";

	// Project parameters
	{
		const char * name = rootNode->Attribute( "scene" );
		if ( name != 0x0 ) {
			std::string tmp = os::path::join( 2, _projPath.c_str(), name );
			os::path::absPath( tmp, _sceneXML );
		}
	}

	{
		const char * name = rootNode->Attribute( "behavior" );
		if ( name != 0x0 ) {
			std::string tmp = os::path::join( 2, _projPath.c_str(), name );
			os::path::absPath( tmp, _behaviorXML );
		}
	}

	{
		const char * name = rootNode->Attribute( "model" );
		if ( name != 0x0 ) {
			_modelName = std::string( name );
		}
	}

	{
		const char * name = rootNode->Attribute( "output" );
		if ( name != 0x0 ) {
			setOutputName( os::path::join( 2, _projPath.c_str(), name ) );
		}
	}

	{
		const char * name = rootNode->Attribute( "scbVersion" );
		if ( name != 0x0 ) {
			// TODO: Validate this version
			_scbVersion = std::string( name );
		}
	}

	{
		const char * name = rootNode->Attribute( "dumpPath" );
		if ( name != 0x0 ) {
			std::string tmp = os::path::join( 2, _projPath.c_str(), name );
			os::path::absPath( tmp, _imgDumpPath );
		}
	}

	{
		const char * name = rootNode->Attribute( "view" );
		if ( name != 0x0 ) {
			std::string tmp = os::path::join( 2, _projPath.c_str(), name );
			os::path::absPath( tmp, _viewConfig );
		}
	}

	double d;
	int i;

	if ( rootNode->Attribute( "duration", &d ) ) {
		_duration = (float)d;
	}

	if ( rootNode->Attribute( "timeStep", &d ) ) {
		_timeStep = (float)d;
	}

	if ( rootNode->Attribute( "random", &i ) ) {
		_seed = i;
	}

	if ( rootNode->Attribute( "subSteps", &i ) ) {
		_subSteps = (size_t)i;
	}

	return true;
}

////////////////////////////////////////////////////////////////

Logger & operator<<( Logger & out, const ProjectSpec & spec ) {
	out << "<?xml version=\"1.0\"?>\n\n<Project\n";
	out << "\tscene=\"" << spec._sceneXML << "\"\n";
	out << "\tbehavior=\"" << spec._behaviorXML << "\"\n";
	out << "\tmodel=\"" << spec._modelName << "\"\n";
	out << "\toutput=\"" << spec._scbFileName << "\"\n";
	out << "\tscbVersion=\"" << spec._scbVersion << "\"\n";
	out << "\tview=\"" << spec._viewConfig << "\"\n";
	out << "\tduration=\"" << spec._duration << "\"\n";
	out << "\ttimeStep=\"" << spec._timeStep << "\"\n";
	out << "\trandom=\"" << spec._seed << "\"\n";
	out << "\tdumpPath=\"" << spec._imgDumpPath << "\"\n";
	out << "\tsubSteps=\"" << spec._subSteps << "\"\n";
	out << "/>";
	return out;
}