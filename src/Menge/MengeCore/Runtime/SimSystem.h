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
 *	@file		SimSystem.h
 *	@brief		The system which runs the simulation, coordinating
 *				the FSM and simulator.
 */

#ifndef __SIM_SYSTEM_H__
#define	__SIM_SYSTEM_H__

// Scene graph
#include "System.h"
#include "MengeException.h"

namespace Menge {

	// forward declarations
	class VisAgent;

	namespace Agents {
		class SimulatorInterface;
		class SCBWriter;
	}

	namespace SceneGraph {
		class GLScene;
	}

	namespace BFSM {
		class FSM;
	}

	// Exceptions for the system
	/*!
	 *	@brief		Generic exception for the SimSystem.
	 */
	class MENGE_API SimSystemException : public virtual Menge::MengeException {
	public:
		/*!
		 *	@brief		Default constructor.
		 */
		SimSystemException() : Menge::MengeException() {}		

		/*!
		 *	@brief		Constructor with message.
		 *
		 *	@param		s		The exception-specific message.
		 */
		SimSystemException( const std::string & s ): Menge::MengeException(s) {}
	};

	/*!
	 *	@brief		The fatal SimSystem exception.
	 */
	class MENGE_API SimSystemFatalException : public SimSystemException, public Menge::MengeFatalException {
	public:
		/*!
		 *	@brief		Default constructor.
		 */
		SimSystemFatalException() : Menge::MengeException(), SimSystemException(), Menge::MengeFatalException() {}

		/*!
		 *	@brief		Constructor with message.
		 *
		 *	@param		s		The exception-specific message.
		 */
		SimSystemFatalException( const std::string & s ): Menge::MengeException(s), SimSystemException(), Menge::MengeFatalException() {}
	};


	/*!
	 *	@brief		Templated class for performing simulation.
	 *
	 *	The SimSystem is the main mechanism which performs the simulation loop.
	 *	It is responsible for updating the simulation step after step, with
	 *	appropriate calls to the BFSM and the simulator.
	 */
	class MENGE_API SimSystem : public SceneGraph::System {
	public:

		/*!
		 *	@brief		Constructor
		 *
		 *	@param		visualize		True if the SimSystem will be connected to
		 *								a visualizer (such as a Vis::GLViewer).
		 */
		SimSystem( bool visualize );

		/*!
		 *	@brief		Constructor with duration limit
		 *
		 *	@param		visualize		True if the SimSystem will be connected to
		 *								a visualizer (such as a Vis::GLViewer).
		 *	@param		duration		The maximum duration (in simulation time)
		 *								the system will run.
		 */
		SimSystem( bool visualize, float duration );

		/*!
		 *	@brief		Destructor.
		 */
		~SimSystem();

		/*!
		 *	@brief		Sets the maximum length allowed for the simulation to run.
		 *
		 *	@param		duration		The maximum duration of the simulation
		 *								(in simulation time).  After this time has
		 *								elapsed, the system no longer updates.
		 */
		inline void setMaxDuration( float duration ) {_maxDuration = duration; }

		/*!
		 *	@brief		Update the simulation (and possibly visual elements) to the
		 *				given global time.
		 *
		 *	@param		time		The global time of the system.
		 *	@returns	True if the system has changed such that it requires a redraw.
		 */
		virtual bool updateScene( float time );

		/*!
		 *	@brief		Reports if the SimSystem is finished updating.
		 *
		 *	@returns	True if the system will no longer update.  False if still
		 *				willing to evalute.
		 */
		bool isFinished() const;

		/*!
		 *	@brief		Set the simulator and corresponding FSM for the simulator.
		 *
		 *	This assumes that no output file (scb file) will be written.  
		 *	Once the simulator and finite state machine have been given to the SimSystem,
		 *	the SimSystem is repsonsible for managing the memory (i.e. freeing up the memory.)
		 *
		 *	Throws a SimSystemStateException if the simulator/fsm can't be set.  
		 *	It remains the responsibility of the caller to delete the sim and fsm in this case.
		 *
		 *	@param	sim		A pointer to the simulator.
		 *	@param	fsm		A pointer to the behavior finite state machine.
		 *	@throws	SimSystemFatalException		If there is an error in assigning the simulator.
		 */
		void setSimulator( Agents::SimulatorInterface * sim, BFSM::FSM * fsm );

		// Sets the simulator/fsm for the visualizer - prepares an scb file for writing (with the provided version)
		/*!
		 *	@brief		Set the simulator and corresponding FSM for the simulator.
		 *
		 *	This is used if an output file (scb file) *is* to be written.
		 *	Once the simulator and finite state machine have been given to the SimSystem,
		 *	the SimSystem is repsonsible for managing the memory (i.e. freeing up the memory.)
		 *
		 *	Throws a SimSystemStateException if the simulator/fsm can't be set.  
		 *	It remains the responsibility of the caller to delete the sim and fsm in this case.
		 *
		 *	@param	sim				A pointer to the simulator.
		 *	@param	fsm				A pointer to the behavior finite state machine.
		 *	@param	outFileName		The name of the scb file to write.
		 *	@param	scbVersion		The version of scb file to write.
		 *	@throws	SimSystemFatalException		If there is an error in assigning the simulator, or in 
		 *										initializing the output file.
		 */
		void setSimulator( Agents::SimulatorInterface * sim, BFSM::FSM * fsm, const std::string & outFileName, const std::string & scbVersion );

		/*!
		 *	@brief		Add visual representations of the simulation obstcles to the GLScene.
		 *
		 *	@param		scene		The scene which receives nodes for drawing obstacles.
		 */
		void addObstacleToScene( SceneGraph::GLScene * scene );

		/*!
		 *	@brief		Add visual representations of the simulation agents to the GLScene.
		 *
		 *	@param		scene		The scene which receives nodes for drawing agents.
		 */
		virtual void addAgentsToScene( SceneGraph::GLScene * scene );

		/*!
		 *	@brief		Add visual representations of obstacles and agents to the GLScene.
		 *
		 *	@param		scene		The scene which receives nodes for drawing agents.
		 */
		void populateScene( SceneGraph::GLScene * scene );

		
		/*!
		 *	@brief		Update the position of the *visual* agents from the simulation data.
		 *
		 *	@param		agtCount		The number of agents in the system.
		 */
		virtual void updateAgentPosition( int agtCount );

		/*!
		 *	@brief		Returns a pointer to the visualization agents
		 *
		 *	@returns	Returns the pointer to the pointers.
		 */
		inline VisAgent ** getVisAgents() { return _visAgents; }

		/*!
		 *	@brief		Reports the number of agents.
		 *
		 *	@returns	The number of VisAgents updated by the system.
		 */
		size_t getAgentCount() const;
                const Menge::Agents::SimulatorInterface* getSimulator() const
                {
                    return _sim;
                }

	protected:

		/*!
		 *	@brief		Determines if the system is actually for driving
		 *				a visual scene.
		 */
		bool	_forVis;

		/*!
		 *	@brief		Simulator to run and (possibly) visualize.
		 */
		Agents::SimulatorInterface * _sim;

		/*!
		 *	@brief		The visualization agents the system is responsible
		 *				for updating.
		 */
		VisAgent **	_visAgents;

		/*!
		 *	@brief		The behavior finite state machine for the simulator.
		 */
		BFSM::FSM * _fsm;

		/*!
		 *	@brief		The optional scb writer (if an output file has been
		 *				successfully specified.
		 */
		Agents::SCBWriter * _scbWriter;

		/*!
		 *	@brief		The global time of last system update.
		 */
		float	_lastUpdate;

		/*!
		 *	@brief		Indicates if the simulation is running.
		 */
		bool	_isRunning;

		/*!
		 *	@brief		Maximum length of simulation time to compute (in simulation time).
		 */
		float	_maxDuration;
	};
}	// namespace Menge
#endif	// __VIS_RVO_SIM_H__
