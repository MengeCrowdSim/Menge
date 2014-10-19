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
 *	@file		FSMDescrip.h
 *	@brief		The definition of behavior finite state machines.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <map>
#include <list>

#include "fsmCommon.h"


// Configuration description classes
#include "StateDescrip.h"
#include "resources/ResourceManager.h"
#include "resources/VectorField.h"

#include "Logger.h"
#include "os.h"
#include "tinyxml.h"


namespace Menge {

	// forward declarations
	namespace Agents {
		class SimulatorInterface;
	}

	namespace BFSM {
		// Forward declaration
		class FSM;
		class GoalSet;
		class Task;
		class State;
		class Transition;
		class StateDescrip;
		class VelModifier;

		/*!
		 *	@brief		The full description of the behavioral finite state machine
		 */
		class FSMDescrip {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			FSMDescrip();

			/*!
			 *	@brief		Destructor.
			 */
			~FSMDescrip();

			/*!
			 *	@brief		Instantiates a state based on the description.
			 *
			 *	This is a behavior operation, because the behavior is responsible for:
			 *		- tracking its states by name,
			 *		- confirming no State name conflicts, and
			 *		- connecting states with transitions.
			 *
			 *	@param		sData		The description of the state to create.
			 *	@returns	A pointer to the created state.  If there are two states with the same name
			 *				*in this behavior*, NULL is returned.
			 */
			State * addState( StateDescrip * sData );

			/*!
			 *	@brief		Initializes the configuration from an xml file.
			 *
			 *	@param		xmlName		The path to the xml file containing the behavior
			 *							specification.
			 *	@param		verbose		Dictates whether the parsing is verbose.
			 *	@returns	True if parsing was successfful, false otherwise.
			 */
			bool loadFromXML( const std::string & xmlName, bool verbose );
			
			/*!
			 *	@brief		Creates an FSM instance from the config file.
			 *
			 *	@param		fsmDescrip	The behavior configuration description.
			 *	@param		sim			An instance of the simulator.
			 *	@param		VERBOSE		Dictates whether the construction process is verbose (true)
			 *							or not (false).
			 */
			friend FSM * buildFSM( FSMDescrip & fsmDescrip, Agents::SimulatorInterface * sim, bool VERBOSE );

			/*!
			 *	@brief		Friend operator for printing the behavior configuration to an output stream.
			 *
			 *	@param		out				The logger.
			 *	@param		fsmDescrip		A behavior configuration to write to the stream.
			 *	@returns	The provided output stream.
			 */
			friend Logger & operator<<( Logger & out, const FSMDescrip & fsmDescrip );

		protected:
			
			/*!
			 *	@brief		Adds the transition(s) associated with the given name.
			 *
			 *	@param		name		The name of the state from which the transition
			 *							originates.  Alternatively, a comma-separated list.
			 *	@param		trans		The transition to add.  The FSMDescrip takes ownership
			 *							of the transition.
			 */
			void addTransition( const std::string & name, Transition * trans );

			/*!
			 *	@brief		List of state descriptions.
			 */
			std::list< StateDescrip * >		_states;

			/*!
			 *	@brief		List of transitions.
			 */
			std::map< std::string, std::list< Transition * > >		_transitions;

			/*!
			 *	@brief		Map of state names to states.
			 */
			std::map< std::string, State * >	_stateNameMap;
			
			/*!
			 *	@brief		A map from agent class id to a behavior description instance.
			 */
			std::list< Task * >	_tasks;

			/*!
			 *	@brief		A mapping of goal sets to goals.
			 *				The goal sets are represented by their id.  The goals for each goal
			 *				set are stored in a map, mapping the goal's local id to a descriptor for
			 *				that goal.  This is to facilitate set additions.
			 */
			std::map< size_t, GoalSet * >	_goalSets;

			/*!
			 *	@brief		A list of velocity modifiers to be applied to all states in the simulator
			 */
			std::vector< VelModifier * >	_velModifiers;

			/*!
			 *	@brief		The folder in which the behavior specification file appears
			 */
			std::string		_behaviorFldr;

		};
	}	// namespace BFSM
}	// namespace Menge

#endif	// __CONFIG_H__
