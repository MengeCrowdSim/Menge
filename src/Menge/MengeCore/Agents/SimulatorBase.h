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

#ifndef __SIMULATOR_BASE_H__
#define __SIMULATOR_BASE_H__

/*!
 *  @file       SimulatorBase.h
 *  @brief      Contains the SimulatorBase class - the common, generic simulator to
 *				work with different types of agents.  It is templated on the Agent type.
 */

// UTILS
#include "mengeCommon.h"
#include "Utils.h"
// Ped Models
#include "SimulatorInterface.h"
#include "AgentInitializer.h"
#include "SpatialQueries/SpatialQuery.h"

// STL
#include <vector>

#if HAVE_OPENMP || _OPENMP
#include <omp.h>
#endif

namespace Menge { 

	namespace Agents {

		/*!
		 *  @brief      Defines the basic simulator.  It is responsible for tracking agents and
		 *				obstacles as well as initializing such from files.
		 */
		template < class Agent >
		class SimulatorBase : public SimulatorInterface {
		public:
			/*!
			 *  @brief      Constructs a simulator instance.
			 */
			SimulatorBase();

			/*!
			 *  @brief      Destorys a simulator instance.
			 */
			~SimulatorBase();

			/*!
			 *  @brief      Lets the simulator perform a simulation step and updates the
			 *              two-dimensional _p and two-dimensional velocity of
			 *              each agent.
			 */
			void doStep();

			/*!
			 *	@brief		Initalize spatial query structure.
			 */
			virtual bool initSpatialQuery();
			
			/*!
			 *	@brief	After all agents and all obstacles have been added to the scene
			 *			does the work to finish preparing the simulation to be run.
			 *
			 *	This work is performed when the simulator is done being initialized.
			 *	If a particular new pedestrian simulator requires particular finalization
			 *	work, this function should be sub-classed and the parent class's
			 *	version of the function should be explicitly called before any additional
			 *	work is performed.
			 */
			virtual void finalize();

			/*!
			 *  @brief      Accessor for agents.
			 *
			 *  @param      agentNo         The number of the agent who is to be retrieved.
			 *								This is *not* the same as the agent identifier.
			 *								It is merely the local index of the agent in the
			 *								simulator's local store.
			 *  @returns    A pointer to the agent.
			 */
			virtual BaseAgent * getAgent( size_t agentNo ) { return &_agents[ agentNo ]; }

			/*!
			 *  @brief      Const accessor for agents.
			 *
			 *  @param      agentNo         The number of the agent who is to be retrieved.
			 *								This is *not* the same as the agent identifier.
			 *								It is merely the local index of the agent in the
			 *								simulator's local store.
			 *  @returns    A pointer to the agent.
			 */
			virtual const BaseAgent * getAgent( size_t agentNo ) const { return &_agents[ agentNo ]; }
			
			/*!
			 *	@brief		Add an agent with specified position to the simulator whose properties
			 *				are defined by the given agent initializer.
			 *
			 *	It uses the agent initializer to define the values of the remaining agent parameters
			 *
			 *	@param		pos			The 2d vector representing the agent's position
			 *	@param		agentInit	The AgentInitializer necessary to parse AgentSet properties
			 *	@returns	A pointer to the agent (if initialization was succesful) or NULL if failed.
			 */
			virtual BaseAgent * addAgent( const Vector2 & pos, AgentInitializer * agentInit );

			/*!
			 *  @brief      Returns the count of agents in the simulation.
			 *
			 *  @returns    The count of agents in the simulation.
			 */
			virtual size_t getNumAgents() const { return _agents.size(); }

			/*!
			 *	@brief			Reports if there are non-common Experiment parameters that
			 *					this simulator requires in the XML file.
			 *
			 *	@returns		By default, the simulator base ONLY uses common parameters.
			 *					Always returns false.
			 */
			virtual bool hasExpTarget() { return false; }

			/*!
			 *	@brief			Reports if the given Experiment attribute tag name belongs to this
			 *					simulator.
			 *
			 *	@param			tagName			The name of the candidate experiment XML tag.
			 *	@returns		By default, the simulator base ONLY uses common parameters.
			 *					Always returns false.
			 */
			virtual bool isExpTarget( const std::string & tagName ) { return false; }

			/*!
			 *	@brief			Given an Experiment parameter name and value, sets the appropriate
			 *					simulator parameter.
			 *
			 *	// TODO: Define the conditions of success/failure.
			 *
			 *	@param			paramName		A string containing the parameter name for the experiment.
			 *	@param			value			A string containing the value for the parameter.
			 *	@returns		True if the parameter was successfully set, false otherwise.
			 */
			virtual bool setExpParam( const std::string & paramName, const std::string & value ) throw( XMLParamException );

		protected:

			/*!
			 *	@brief		Computes the neighbors for the given agent.
			 *
			 *	@param		agent		The agent whose neighbors are to be computed.
			 */
			void computeNeighbors( Agent * agent );

			/*!
			 *	@brief		The collection of agents in the simulation
			 */
			std::vector< Agent > _agents;
		};

		////////////////////////////////////////////////////////////////
		//					Implementation of SimulatorBase
		////////////////////////////////////////////////////////////////
		
		template < class Agent >
		SimulatorBase<Agent>::SimulatorBase(): SimulatorInterface(), _agents() {
		}

		////////////////////////////////////////////////////////////////

		template < class Agent >
		SimulatorBase<Agent>::~SimulatorBase() {
			_agents.clear();
		}

		////////////////////////////////////////////////////////////////

		template < class Agent >
		void SimulatorBase<Agent>::doStep() {
			assert( _spatialQuery != 0x0 && "Can't run without a spatial query instance defined" );

			_spatialQuery->updateAgents();
			int AGT_COUNT = static_cast< int >( _agents.size() );
			#pragma omp parallel for
			for (int i = 0; i < AGT_COUNT; ++i) {
				computeNeighbors( &(_agents[i]) );
				_agents[i].computeNewVelocity();
			}

			#pragma omp parallel for
			for (int i = 0; i < AGT_COUNT; ++i) {
			  _agents[i].update( TIME_STEP );
			}
			
			_globalTime += TIME_STEP;

		}

		////////////////////////////////////////////////////////////////

		template < class Agent >
		bool SimulatorBase<Agent>::initSpatialQuery() {
			assert( _spatialQuery != 0x0 && "Can't run without a spatial query instance defined" );

			const size_t AGT_COUNT = _agents.size();
			std::vector< BaseAgent * > agtPointers( AGT_COUNT );
			for ( size_t a = 0; a < AGT_COUNT; ++a ) {
				agtPointers[ a ] = &_agents[a];
			}
			_spatialQuery->setAgents( agtPointers );
			
			_spatialQuery->processObstacles();

			return true;
		}
			
		////////////////////////////////////////////////////////////////

		template < class Agent >
		void SimulatorBase<Agent>::finalize() {
			SimulatorInterface::finalize();

			// initialize agents
			for ( size_t i = 0; i < _agents.size(); ++i ) {
				_agents[ i ].initialize();
			}
		}

		////////////////////////////////////////////////////////////////

		template < class Agent >
		BaseAgent * SimulatorBase<Agent>::addAgent( const Vector2 & pos, AgentInitializer * agentInit ) {
			Agent agent;

			agent._pos = pos;
			agent._id = _agents.size();
			if ( ! agentInit->setProperties( &agent ) ) {
				logger << Logger::ERR_MSG << "Error initializing agent " << agent._id << "\n";
				return 0x0;
			}
			_agents.push_back(agent);

			return &_agents[ _agents.size() - 1 ];
		}

		////////////////////////////////////////////////////////////////

		template < class Agent >
		bool SimulatorBase<Agent>::setExpParam( const std::string & paramName, const std::string & value ) throw( XMLParamException ) {
			
			if ( paramName == "time_step" ) {
				try {
					LOGICAL_TIME_STEP = toFloat( value );
				} catch ( UtilException ) {
					throw XMLParamException( std::string( "Common parameters \"time_step\" value couldn't be converted to a float.  Found the value: " ) + value );
				}
			} else {
				return false;
			}

			return true;
		}

		////////////////////////////////////////////////////////////////

		template< class Agent >
		void SimulatorBase<Agent>::computeNeighbors( Agent * agent ) {
			// obstacles
			agent->startQuery();
			_spatialQuery->obstacleQuery(agent);

			// agents
			if ( agent->_maxNeighbors > 0 ) {
				_spatialQuery->agentQuery(agent);
			}
		}
	}	// namespace Agents
}	// namespace Menge
#endif	// __SIMULATOR_BASE_H__