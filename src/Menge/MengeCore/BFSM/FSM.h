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
 *	@file		FSM.h
 *	@brief		The definition of the behavior finite state machine.
 */

#ifndef __FSM_H__
#define __FSM_H__

// Finite-state machine used to compute preferred velocity
//	according to varying conditions

#include "fsmCommon.h"
#include "FSMDescrip.h"
#include "MengeException.h"

#include <vector>
#include <cassert>
#include <map>
#ifndef _MSC_VER
#include <string.h>
#endif


namespace Menge {
	// forward declaration
	namespace Agents {
		class SimulatorInterface;
		class BaseAgent;
	}

	namespace BFSM {

		// Forward declaration
		class FsmContext;
		class State;
		class Transition;
		class Goal;
		class GoalSet;
		class Task;
		class FSMDescrip;

		/////////////////////////////////////////////////////////////////////
		
		/*!
		 *	@brief		Generic base class for FSM exceptions.
		 */
		class FSMException : public virtual MengeException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			FSMException() : MengeException() {}		

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			FSMException( const std::string & s ): MengeException(s) {}
		};

		/////////////////////////////////////////////////////////////////////
		
		/*!
		 *	@brief		Exception thrown when the FSM has an error which cannot be
		 *				recovered from.
		 */
		class FSMFatalException : public FSMException, public MengeFatalException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			FSMFatalException() : MengeException(), FSMException(), MengeFatalException() {}

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			FSMFatalException( const std::string & s ): MengeException(s), FSMException(), MengeFatalException() {}
		};

		/////////////////////////////////////////////////////////////////////
		
		/*!
		 *	@brief		Templated class for the behavior finite state machine.
		 */
		class FSM {
		public:
			/*!
			 *	@brief		Constructor.
			 *
			 *	@param		sim			The simulator to operate on.
			 */
			FSM( Agents::SimulatorInterface * sim );

			/*!
			 *	@brief		Destructor
			 */
			~FSM();

			/*!
			 *	@brief		Collects all tasks from the FSM elements.
			 */
			void collectTasks();

			/*!
			 *	@brief		Adds a task to the set.
			 *
			 *	Elements can blindly add tasks to the finite state machine (including adding
			 *	a null pointer.  If the pointer is null, no work is done.  If the pointer is
			 *	a duplicate of a previous task, it will not be repeatedly added.
			 *
			 *	The FSM takes possession of the task and is responsible for cleaning the memory.
			 *
			 *	@param		task		A pointer to a task to add.
			 */
			void addTask( Task * task );

			/*!
			 *	@brief		Initializes the memory required for the number of agents
			 *				included in the FSm.
			 *
			 *	@param		count		The number of agents.
			 */
			void setAgentCount( size_t count );

			/*!
			 *	@brief		Advances the FSM based on the current state for the given
			 *				agent.
			 *	
			 *	@param		agent		The agent to advance the FSM for.
			 */
			void advance( Agents::BaseAgent * agent );

			/*!
			 *	@brief		Computes the preferred velocity for the given agent
			 *				based on the FSM's record of which state the agent is in.
			 */
			void computePrefVelocity( Agents::BaseAgent * agent );

			/*!
			 *	@brief		Get the pointer for the node with the given identifier.
			 *
			 *	@param		id		The state identifier.  Id is not validated.
			 *	@returns	A pointer to the state with the given identifier.
			 */
			State * getNode( size_t id ) { return _nodes[id]; }

			/*!
			 *	@brief		Returns the state with the given name.
			 *
			 *	@param		name		The name of the desired state.
			 *	@returns	A pointer to the desired state (if it is found), 
			 *				otherwise, returns null.
			 */
			State * getNode( const std::string & name );

			/*!
			 *	@brief		Reports the number of states in the FSM.
			 *
			 *	@returns	The number of states in the fsm.
			 */
			size_t getNodeCount() const { return _nodes.size(); }

			/*!
			 *	@brief		Adds a state to the BFSM.
			 *
			 *	@param		node		The state to add.
			 *	@returns	A unique identifier for the newly added state.
			 */
			size_t addNode( State * node );

			/*!
			 *	@brief		Adds the given transition to the FSM.
			 *
			 *	The transition already contains a pointer to the destination state
			 *	(see Transition).
			 *
			 *	@param		fromNode		The global identifier from the origin state.
			 *	@param		t				The transition to add.
			 *	@returns	True if the transition is successfully added, false otherwise.
			 */
			bool addTransition( size_t fromNode, Transition * t );

			/*!
			 *	@brief		Adds a goal to the indicated goal set.
			 *
			 *	@param		goalSet		The id of the goal set which receives the goal.
			 *	@param		goalID		The id of the goal in the goal set.
			 *	@param		goal		The goal to add.
			 *	@returns	A boolean reporting success (true) or failure (false)
			 */
			bool addGoal( size_t goalSet, size_t goalID, Goal * goal );

			/*!
			 *	@brief		Retrieves the given goal from the given goal set.
			 *
			 *	@param		goalSet		The identifier of the goal set.
			 *	@param		goalID		The identifier of the goal.
			 *	@returns	A pointer to the corresponding goal.  If no such goal exists
			 *				NULL is returned.
			 */
			Goal * getGoal( size_t goalSet, size_t goalID );

			/*!
			 *	@brief		Retrives the given goal set.
			 *
			 *	@param		goalSetID		The identifier of the goal set.
			 *	@returns	A pointer to the corresponding goal set.  If no such goal set
			 *				exists, NULL is returned.
			 */
			GoalSet * getGoalSet( size_t goalSetID );

			/*!
			 *	@brief		Update the fsm state by one time step
			 *
			 *	@returns	A boolean reporting if all agents are in a final state (true)
			 *				or not (false).
			 */
			bool doStep();

			/*!
			 *	@brief		Sets the current state for the given agent.
			 *
			 *	@param		agt			The agent whose BFSM state gets set.
			 *	@param		currNode	The unique identifier of the desired state 
			 *							(returned by FSM::addNode).
			 */
			void setCurrentState( Agents::BaseAgent * agt, size_t currNode );

			/*!
			 *	@brief		Gets a pointer to the state the agent is currently in.
			 *
			 *	@param		agt			The agent.
			 *	@returns	A pointer to the agent's current state.
			 */
			State * getCurrentState( const Agents::BaseAgent * agt ) const;

			/*!
			 *	@brief		Reports the state the given agent is currently in.
			 *
			 *	@param		agent		A pointer to the agent whose state id is returned.
			 *	@returns	The id of the state the given agent is in.
			 */
			size_t getAgentStateID( const Agents::BaseAgent * agent ) const;
			
			/*!
			 *	@brief		Reports the state the given agent is currently in.
			 *
			 *	@param		agentID		The unique id of the agent whose state id is returned.
			 *	@returns	The id of the state the given agent is in.
			 */
			size_t getAgentStateID( size_t agentID ) const;

			/*!
			 *	@brief		Reports if all agents are in final states (i.e. the simulation is done.)
			 *
			 *	@returns	True if all agents are in a final state, false otherwise.
			 */
			bool allFinal() const;

			/*!
			 *	@brief		Retrieve the simulator
			 */
			inline const Agents::SimulatorInterface * getSimulator() const { return _sim; }

			/*!
			 *	@brief		Performs the work in the FSM's tasks.
			 */
			void doTasks();

			/*!
			 *	@brief		Returns the number of registered tasks.
			 */
			size_t getTaskCount() const { return _tasks.size(); }
			
			/*!
			 *	@brief		Finalize the FSM
			 */
			void finalize();

			/*!
			 *	@brief		Returns the BFSM Context for this FSM
			 *
			 *	A new FsmContext will be constructed upon each call to
			 *	this function.  It is the responsibility of the caller to
			 *	make sure it gets deleted.
			 *
			 *	@returns	A pointer to a valid fsm context. 
			 */
			FsmContext * getContext();

			/*!
			 *	@brief		Returns the goal sets defined in the simulator.
			 *
			 *	@returns	A reference to the goal set map.
			 */
			std::map< size_t, GoalSet * > & getGoalSets() { return _goalSets; }

			friend FSM * buildFSM( FSMDescrip & fsmDescrip, Agents::SimulatorInterface * sim, bool VERBOSE );

			/*!
			 *	@brief		Add an velocity modifier to the FSM
			 *
			 *	@param		v		The modifier to add
			 */
			void addVelModifier( VelModifier * v ) { _velModifiers.push_back( v ); }

		protected:
			/*!
			 *	@brief		The simulator on which the FSM acts.
			 */
			Agents::SimulatorInterface	* _sim;

			/*!
			 *	@brief		Number of agents attached to the state machine
			 *
			 *	This should be the same as the number of agents in the simulator.
			 */
			size_t _agtCount;

			/*!
			 *	@brief		The active state for each agent in the system.
			 */
			State ** _currNode;

			/*!
			 *	@brief		The states in the BFSM.
			 */
			std::vector< State * >	_nodes;

			/*!
			 *	@brief		The set of tasks to perform at each time step
			 */
			std::vector< Task * > _tasks;

			/*!
			 *	@brief		Mapping from goal set identifier to GoalSet.
			 */
			std::map< size_t, GoalSet * >	_goalSets;

			/*!
			 *	@brief		A list of velocity modifiers to be applied to all states in the simulator
			 */
			std::vector< VelModifier * >	_velModifiers;

		};

		/////////////////////////////////////////////////////////////////////
		
		/*!
		 *	@brief		Templated function which builds the behavior fsm from the
		 *				behavior configuration given.
		 *
		 *	@param		fsmDescrip		The Behavior configuration.
		 *	@param		sim				The simulator which uses the BFSM.
		 *	@param		VERBOSE			The optional argument which causes the construction
		 *								process to report actions as it goes.
		 *	@returns	A pointer to the resultant FSM.  If there is an error in
		 *				construction, NULL is returned.
		 */
		 FSM * buildFSM( FSMDescrip & fsmDescrip, Agents::SimulatorInterface * sim, bool VERBOSE=false );

	}	// namespace BFSM
} // namespace Menge
#endif	// __FSM_H__
