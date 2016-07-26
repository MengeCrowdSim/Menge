/*

License

Menge
Copyright � and trademark � 2012-14 University of North Carolina at Chapel Hill. 
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
 *	@file	SimulatorDBEntry.h
 *	@brief	Definition of an entry into the simulator database.
 *
 *	This provides the mechanism by which the executable can learn about
 *	new pedestrian models/simulators without any significant code 
 *	contortions.
 */

#ifndef __SIMULATOR_DB_ENTRY_H__
#define	__SIMULATOR_DB_ENTRY_H__

#include <string>
#include <iostream>
#include "CoreConfig.h"

namespace Menge {

	class SimSystem;
	class BaseAgentContext;
	namespace Agents {
		class AgentInitializer;
		class SimulatorInterface;
	}
	namespace SceneGraph {
		class GLScene;
	}
	namespace BFSM {
		class FSMDescrip;
		class FSM;
	}

	/*!
	 *	@brief		An entry in the simulator database.
	 *
	 *	Every pedestrian model must define and register a SimulatorDBEntry.
	 *	The entry provides brief and long descriptions of the pedestrian 
	 *	model to display in response to command-line queries.  Furthermore,
	 *	they are responsible for instantiating simulators, behavior FSM,
	 *	and SimSystem (although, this is done in the base class and <i>not</i>
	 *	the derived classes.
	 *
	 *	A derived class should do the following:
	 *		- Inherit from the SimulatorDBEntry
	 *		- Implement the following functions:
	 *			- SimulatorDBEntry::briefDescription
	 *			- SimulatorDBEntry::longDescription
	 *			- SimulatorDBEntry::viewName
	 *			- SimulatorDBEntry::commandLineName
	 *			- SimulatorDBEntry::getNewSimulator
	 *		- Optionally implement the following functionsl
	 *			- SimulatorDBEntry::contextFromSystem if the pedestrian model provides
	 *			a unique sub-class of Agents::BaseAgentContext.
	 *			- SimulatorDBEntry::getAgentInitalizer if the pedestrian model has
	 *			custom per-agent attributes to parse (in addition to the common parameters).
	 */
	class MENGE_API SimulatorDBEntry {
	public:
		/*!
		 *	@brief		Gives a brief description of the simulator.
		 *
		 *	@returns	A brief description of the simulator and pedestrian
		 *				model.
		 */
		virtual ::std::string briefDescription() const = 0;

		/*!
		 *	@brief		Gives a long description of the simulator.
		 *
		 *	@returns	A long description of the simulator and pedestrian
		 *				model.
		 */
		virtual ::std::string longDescription() const = 0;

		/*!
		 *	@brief		Gives a label to apply to the interactive viewer.
		 *
		 *	@returns	The name for display on the interactive viewer.
		 *	@returns	A newly instantiated simulator instance of a type appropriate
		 *				for this database entry.
		 */
		virtual ::std::string viewerName() const = 0;

		/*!
		 *	@brief		Gives a unique name to be used as a command-line parameter.
		 *
		 *	This name MUST satisfy two constraints:
		 *		- It must contain no spaces.
		 *		- It must be unique from that used by all other simulators.
		 *
		 *	@returns	A single string (with no spaces) that can be used as
		 *				a command line parameter to uniquely identify this model.
		 */
		virtual ::std::string commandLineName() const = 0;

		/*!
		 *	@brief		Returns a pointer to this model's Simulator instance.
		 *
		 *	This must be overridden by a derived class
		 *
		 *	@returns	A newly instantiated simulator instance of a type appropriate
		 *				for this database entry.
		 */
		virtual Agents::SimulatorInterface * getNewSimulator() = 0;

		/*!
		 *	@brief		Returns a simulator system that can be attached to a
		 *				SceneGraph::GLScene and advanced by a Viewer.
		 *
		 *	@param		agentCount		The number of the agents in the system.
		 *	@param		simTimeStep		The simulator's time step (for updating the sim system).
		 *	@param		subSteps		The number of computation sub-steps to take.
		 *	@param		simDuration		The maximum duration to allow the simulation to run.
		 *	@param		behaveFile		The full path to the xml <i>behavior</i> specification.
		 *	@param		sceneFile		The full path to the xml <i>scene</i> specification.
		 *	@param		outFile			The full path to the output file to write the agent 
		 *								trajectories.  If the empty string, no output file will
		 *								be written.
		 *	@param		scbVersion		The scb version to write.
		 *	@param		visualize		Determines if this simulator is to be visualized.
		 *	@param		VERBOSE			Determines if the initialization process prints status
		 *								and information to the console.  True ouputs, false does not.
		 *	@returns	A pointer to the resultant System for running the simulation.
		 *				If there is an error, NULL is returned.
		 */
		SimSystem * getSimulatorSystem( size_t & agentCount,
										 float & simTimeStep,
										 size_t subSteps,
										 float simDuration,
										 const std::string & behaveFile, 
										 const std::string & sceneFile, 
										 const std::string & outFile, 
										 const std::string & scbVersion, 
										 bool visualize, 
										 bool VERBOSE );

		/*!
		 *	@brief		Populates the given GLScene with visualization entities tracked 
		 *				in the system.
		 *
		 *	@param		system		The system which tracks the agents.  This should be
		 *							the same system which was returned by a call to 
		 *							SimulatorDBEntry::getSimulatorSystem.
		 *	@param		scene		The scene to populate with visual elements.
		 */
		void populateScene( SimSystem * system, SceneGraph::GLScene * scene );

		/*!
		 *	@brief		Returns a pointer to an agent context appropriate to
		 *				the corresponding simulator.
		 *
		 *	@param		system		The system which tracks the agents.  This should be
		 *							the same system which was returned by a call to 
		 *							SimulatorDBEntry::getSimulatorSystem.
		 *	@returns	A pointer to the appropriate agent context.  
		 */
		BaseAgentContext * getAgentContext( SimSystem * system );

		/*!
		 *	@brief		Reports the current run-time of an instantiated simulation.
		 *
		 *	This is only meaningful if called *after* getSimulatorSystem.
		 *
		 *	@returns	The current run-time of the instantiated simulation.  If no
		 *				simulation has been instantiated, -1 is returned.
		 */
		float simDuration() const;

	protected:
		/*!
		 *	@brief		Returns a pointer to an agent context appropriate to
		 *				the corresponding simulator.
		 *
		 *	If the provided system is not, in fact, a pointer to a SimSystem for the
		 *	appropriate simulator type, this function will report failure.  Furthermore,
		 *	the default implementation is to return a BaseAgentContext.  If the
		 *	simulator comes with a novel context, this function should be overridden
		 *	in the derived SimulatorDBEntry.
		 *
		 *	@param		simSystem	The system which tracks the agents.  This should be
		 *							the same system which was returned by a call to 
		 *							SimulatorDBEntry::getSimulatorSystem.
		 *	@returns	A pointer to the appropriate agent context.  If the system is of
		 *				the wrong type (or if there is any other problem), NULL is returned.
		 */
		virtual BaseAgentContext * contextFromSystem( SimSystem * simSystem );

		/*!
		 *	@brief		Provides an AgentInitializer appropriate to this simulator class.
		 *
		 *	Each derived database entry must provide the appropriate AgentInitializer
		 *	for its simulator type, in order to fully process all of the corresponding
		 *	XML data to initialize the agent population.
		 *
		 *	@returns	A pointer to an agent initializer.  The caller is responsible for
		 *				freeing up the memory.
		 */
		virtual Agents::AgentInitializer * getAgentInitalizer() const;

		/*!
		 *	@brief		Creates the simulator.
		 *
		 *	@param		sceneFileName		The full path to the simulation scene specification.
		 *	@param		VERBOSE				Determines if the initialization outputs status
		 *									and information to the console.  True outputs, 
		 *									false remains silent.
		 *	@returns	A pointer to the instantiated simulator.
		 *				If there is an error, NULL is returned.
		 */
		Agents::SimulatorInterface * initSimulator( const std::string & sceneFileName, bool VERBOSE );

		/*!
		 *	@brief		Creates the finite state machine and finalizes simulator and fsm
		 *
		 *	@param		behaveFile		string containing the full path to the behavior file
		 *	@param		sim				pointer to the simulator interface to be used in conjunction with the FSM
		 *	@param		VERBOSE			boolean flag for verbose output
		 *	@returns	A pointer to the instantiated finite state machine for the simulator.
		 *				If there is an error, NULL is returned.
		 */
		BFSM::FSM * initFSM( const std::string & behaveFile, Agents::SimulatorInterface * sim, bool VERBOSE );

		/*! 
		 *	@brief		Finalizes the finite state machine and simulator in preparation for
		 *				execution.
		 *
		 *	@param		sim		A pointer to the simulator.
		 *	@param		fsm		The finite state machine that goes with the simulator.
		 *	@returns	A boolean reporting if finalization was successful (true) or not (false).
		 */
		bool finalize( Agents::SimulatorInterface * sim, BFSM::FSM * fsm );

		/*! 
		 *	@brief		Creates an instance of a SimSystem to populate.
		 *
		 *	This is the mechanism by which pedestrian plug-ins can override the behavior
		 *	of the SimSystem based on models, by sub-classing the SimSystem and providing
		 *	an alternative implementation.
		 *
		 *	@param		visualize		True if the SimSystem will be connected to
		 *								a visualizer (such as a Vis::GLViewer).
		 *	@param		duration		The maximum duration (in simulation time)
		 *								the system will run.
		 *	@returns	A pointer to a new SimSystem.
		 */
		virtual SimSystem * createSimSystem(  bool visualize, float duration );

		/*!
		 *	@brief		A pointer to the simulator.  The database entry is not responsible for
		 *				deleting it unless there is an error in initialization.
		 */


		/*!
		 *	@brief		A pointer to the behavior finite state machine.  The database entry is *not*
		 *				responsible for deleting it unless there is an error in initialization.
		 */
		
		public:
                        static BFSM::FSM * _fsm;
			static Agents::SimulatorInterface * _sim;
	};
}	// namespace Menge

#endif	 // __SIMULATOR_DB_ENTRY_H__
