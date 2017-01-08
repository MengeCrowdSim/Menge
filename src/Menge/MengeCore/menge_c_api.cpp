#include "MengeCore/menge_c_api.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Agents/SimulatorInterface.h"
#include "MengeCore/BFSM/FSM.h"
#include "MengeCore/PluginEngine/PluginEngine.h"
#include "MengeCore/Runtime/SimulatorDB.h"

/////////////////////////////////////////////////////////////////////
//					Local Variables
/////////////////////////////////////////////////////////////////////

Menge::Agents::SimulatorInterface * _simulator = 0x0;

/////////////////////////////////////////////////////////////////////
//					API implementation
/////////////////////////////////////////////////////////////////////

extern "C" {
	bool   InitSimulator( const char * behaveFile,
						  const char * sceneFile,
						  const char * model,
						  const char * pluginPath ) {
		const bool VERBOSE = false;
		if ( _simulator != 0x0 ) delete _simulator;
		Menge::SimulatorDB simDB;
		// TODO: Plugin engine is *not* public.  I can't get plugins.
		Menge::PluginEngine engine( &simDB );
		if ( pluginPath != 0x0 ) {
			engine.loadPlugins( pluginPath );
		}
		Menge::SimulatorDBEntry * simDBEntry = simDB.getDBEntry( std::string( model ) );
		if ( simDBEntry == 0x0 ) return false;

		size_t agentCount;
		float timeStep = 0.1f;			// Default to 10Hz
		int subSteps = 0;				// take no sub steps
		float duration = 1e6;			// effectively no simulation duration.
		std::string outFile = "";		// Don't write an scb file.
		std::string scbVersion = "";	// No scb version
		bool verbose = false;
		_simulator = simDBEntry->getSimulator( agentCount, timeStep, subSteps, duration,
											   behaveFile, sceneFile, outFile, scbVersion,
											   verbose );
		return _simulator != 0x0;
	}

	size_t  AgentCount() {
		assert( _simulator != 0x0 );
		return _simulator->getNumAgents();
	}

	void  SetTimeStep( float timeStep ) {
		assert( _simulator != 0x0 );
		_simulator->setTimeStep( timeStep );
	}

	bool  DoStep() {
		assert( _simulator != 0x0 );
		return _simulator->step();
	}

	bool  GetAgentPosition( size_t i, float * x, float * y, float * z ) {
		assert( _simulator != 0x0 );
		Menge::Agents::BaseAgent * agt = _simulator->getAgent( i );
		if ( agt != 0x0 ) {
			*x = agt->_pos._x;
			*y = 0; // get elevation
			*z = agt->_pos._y;
			return true;
		}
		return false;
	}

	MENGE_API bool  GetAgentVelocity( size_t i, float * x, float * y, float * z ) {
		assert( _simulator != 0x0 );
		Menge::Agents::BaseAgent * agt = _simulator->getAgent( i );
		if ( agt != 0x0 ) {
			*x = agt->_vel._x;
			*y = 0; // get elevation
			*z = agt->_vel._y;
			return true;
		}
		return false;
	}
}	// extern"C"