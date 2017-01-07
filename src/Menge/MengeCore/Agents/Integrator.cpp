#include "MengeCore/Agents/Integrator.h"

#include "MengeCore/Agents/SCBWriter.h"
#include "MengeCore/Agents/SimulatorInterface.h"
#include "MengeCore/BFSM/FSM.h"

namespace Menge {
	namespace Agents {

		////////////////////////////////////////////////////////////////////////////
		//			Implementation of Integrator
		////////////////////////////////////////////////////////////////////////////

		Integrator::Integrator( SimulatorInterface * sim, BFSM::FSM * fsm ) :
			_sim( sim ), _fsm( fsm ), _scbWriter( 0x0 ), _isRunning( true ), _maxDuration( 100.f )
		{
		}

		////////////////////////////////////////////////////////////////////////////

		Integrator::~Integrator() {
			if ( _sim ) delete _sim;
			if ( _fsm ) delete _fsm;
			if ( _scbWriter ) delete _scbWriter;
		}

		////////////////////////////////////////////////////////////////////////////

		bool Integrator::setOutput( const std::string & outFileName,
									const std::string & scbVersion ) {
			try {
				_scbWriter = new SCBWriter( outFileName, scbVersion, _sim );
				return true;
			} catch ( SCBFileException ) {
				logger << Logger::WARN_MSG << "Error preparing output trajectory file: ";
				logger << outFileName << ".";
				return false;
			}
		}

		////////////////////////////////////////////////////////////////////////////

		void Integrator::step( float timeStep ) {
			const int agtCount = static_cast<int>( _sim->getNumAgents() );
			if ( _isRunning ) {
				if ( _scbWriter ) _scbWriter->writeFrame( _fsm );
				if ( _sim->getGlobalTime() >= _maxDuration ) {
					_isRunning = false;
				} else {
					for ( size_t i = 0; i <= _sim->getSubSteps(); ++i ) {
						try {
							// TODO: doStep for FSM is a *bad* name; it should be "evaluate".
							_isRunning = !_fsm->doStep();
						} catch ( BFSM::FSMFatalException & e ) {
							logger << Logger::ERR_MSG << "Error in updating the finite state ";
							logger << "machine -- stopping!\n";
							logger << "\t" << e.what() << "\n";
							throw IntegratorFatalException();
						}

						_sim->doStep();
						try {
							_fsm->doTasks();
						} catch ( BFSM::FSMFatalException &e ) {
							logger << Logger::ERR_MSG << e.what() << "\n";
							throw IntegratorFatalException();
						}
					}
				}
			} 
			if ( !_isRunning ) {
				// if the simulation ended due to simulation reaching final state; throw exception.
				throw IntegratorException();
			}
		}

		////////////////////////////////////////////////////////////////////////////

		bool Integrator::isFinished() const {
			return !_isRunning || _fsm->allFinal();
		}

		////////////////////////////////////////////////////////////////////////////


	}	// namespace Agents
}	// namespace Menge
