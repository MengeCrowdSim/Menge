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
 *	@file		SimulatorState.h
 *	@brief		A snapshot of the simulator state.
 *
 *	TODO: Make this a complete snapshot.
 */

#ifndef __SIMULATOR_STATE_H__
#define	__SIMULATOR_STATE_H__

#include "mengeCommon.h"
#include <string>

namespace Menge {

	namespace Agents {
		/*!
		 *	@brief		A class which caches the state of the simulator.
		 *
		 *	This version of the class is, as yet, incomplete.  Ultimately, it
		 *	will store all critical parameters of the simulator such that the
		 *	simulation can be restarted from this state.
		 *
		 *	This version is currently used to determine agent's starting state.
		 */
		class SimulatorState {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			SimulatorState();

			/*!
			 *	@brief		Sets the state for the given agent.
			 *
			 *	@param		id			The identifier of the agent.
			 *	@param		stateName	The name of the state to which the agent belongs.
			 */
			void setAgentState( size_t id, const std::string & stateName );

			/*!
			 *	@brief		Reports the state name for the given agent.
			 *
			 *	@param		id			The identifier of the agent.
			 *	@returns	The name of this agent's start state.
			 */
			const std::string getAgentState( size_t id ) const;

		protected:
			/*!
			 *	@brief		A mapping from agent id to state name
			 */
			HASH_MAP< size_t, std::string >	_agentStates;
		};

	}	// namespace Agents 
}	// namespace Menge
#endif // __SIMULATOR_STATE_H__