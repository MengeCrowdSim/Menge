/*!
 *	@file		Integrator.h
 *	@brief		The base class for advancing time in the simulation (the integrator).
 */

#ifndef __INTEGRATOR_H__
#define __INTEGRATOR_H__

#include "MengeCore/mengeCommon.h"

namespace Menge {
	// Forward declarations
	namespace BFSM {
		class FSM;
	}
	namespace Agents {
		// Forward declarations
		class SCBWriter;
		class SimulatorInterface;

		/////////////////////////////////////////////////////////////////////

		/*!
		*	@brief		Generic base class for Integrator exceptions.
		*/
		class IntegratorException : public virtual MengeException {
		public:
			/*!
			*	@brief		Default constructor.
			*/
			IntegratorException() : MengeException() {}

			/*!
			*	@brief		Constructor with message.
			*
			*	@param		s		The exception-specific message.
			*/
			IntegratorException( const std::string & s ) : MengeException( s ) {}
		};

		/////////////////////////////////////////////////////////////////////

		/*!
		*	@brief		Exception thrown when the FSM has an error which cannot be
		*				recovered from.
		*/
		class IntegratorFatalException : public IntegratorException, public MengeFatalException {
		public:
			/*!
			*	@brief		Default constructor.
			*/
			IntegratorFatalException() : MengeException(), IntegratorException(),
				MengeFatalException() {}

			/*!
			*	@brief		Constructor with message.
			*
			*	@param		s		The exception-specific message.
			*/
			IntegratorFatalException( const std::string & s ) : MengeException( s ),
				IntegratorException(), MengeFatalException() {}
		};

		/////////////////////////////////////////////////////////////////////
		/*!
		 *	A simple class for advancing the state of the simulation.
		 */
		class MENGE_API Integrator {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	The Integrator takes ownership of the various instances it is instantiated with.
			 *
			 *	@param		sim		The simulator to advance.
			 *	@param		fsm		The BFSM associated with the simulator.
			 *	@param		writer	The (optional) SCBWriter to use to output at each time step.
			 */
			Integrator( SimulatorInterface * sim, BFSM::FSM * fsm );
			
			/*!
			 *	@brief		Desetructor.
			 */
			~Integrator();

			/*!
			 *	@brief		Sets the trajectory output state.
			 *
			 *	@param		outFileName				The path to the file to write trajectories to.
			 *	@param		scbVersion				The version of scb file to write.
			 *	@returns	True if the SCB writer has been successfully configured.
			 */
			bool setOutput( const std::string & outFileName, const std::string & scbVersion );

			/*!
			 *	@brief		Advances the simulation the given amount of time.
			 *
			 *	@param		time		The global time of the system.
			 */
			void step( float timeStep );

			/*!
			 *	@brief		Reports if the Integrator is finished updating.
			 *
			 *	@returns	True if the Integrator will no longer update.
			 */
			bool isFinished() const;

			/*!
			 *	@brief		Sets the maximum length allowed for the simulation to run.
			 *
			 *	@param		duration		The maximum duration of the simulation
			 *								(in simulation time).  After this time has
			 *								elapsed, the system no longer updates.
			 */
			inline void setMaxDuration( float duration ) { _maxDuration = duration; }

		protected:
			/*!
			 *	@brief		Simulator to run and (possibly) visualize.
			 */
			SimulatorInterface * _sim;

			/*!
			 *	@brief		The behavior finite state machine for the simulator.
			 */
			BFSM::FSM * _fsm;

			/*!
			 *	@brief		The optional scb writer (if an output file has been
			 *				successfully specified.
			 */
			SCBWriter * _scbWriter;

			/*!
			 *	@brief		Indicates if the simulation is running.
			 */
			bool	_isRunning;

			/*!
			 *	@brief		Maximum length of simulation time to compute (in simulation time).
			 */
			float	_maxDuration;
		};
	}	// namespace Agents
}	// namespace Menge

#endif	// __INTEGRATOR_H__
