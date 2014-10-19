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
 *	@file		SimulatorInterface.h
 *	@brief		The definition of the interface of the simulator required
 *				by the finite state machine.
 */
#ifndef __SIMULATOR_INTERFACE_H__
#define __SIMULATOR_INTERFACE_H__

#include "CoreConfig.h"
#include "XMLSimulatorBase.h"
#include "Math/Vector2.h"
using namespace Menge::Math;
#include <vector>
#include "Core.h"

namespace Menge {

	namespace Agents {
		// forward declaration
		class BaseAgent;
		class SpatialQuery;
		class Obstacle;
		class Elevation;

		/*!
		 *	@brief		The basic simulator interface required by the fsm.
		 */
		class MENGE_API SimulatorInterface: public XMLSimulatorBase {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			SimulatorInterface();

			/*!
			 *	@brief		Destructor
			 */
			virtual ~SimulatorInterface();

			/*!
			 *  @brief      Returns the count of agents in the simulation.
			 *
			 *  @returns    The count of agents in the simulation.
			 */
			virtual size_t getNumAgents() const = 0;

			/*!
			 *  @brief      Accessor for agents.
			 *
			 *  @param      agentNo         The number of the agent who is to be retrieved.
			 *								This is *not* the same as the agent identifier.
			 *								It is merely the local index of the agent in the
			 *								simulator's local store.
			 *  @returns    A pointer to the agent.
			 */
			virtual BaseAgent * getAgent( size_t agentNo ) = 0;

			/*!
			 *  @brief      Const accessor for agents.
			 *
			 *  @param      agentNo         The number of the agent who is to be retrieved.
			 *								This is *not* the same as the agent identifier.
			 *								It is merely the local index of the agent in the
			 *								simulator's local store.
			 *  @returns    A pointer to the agent.
			 */
			virtual const BaseAgent * getAgent( size_t agentNo ) const = 0;		

			/*!
			 *  @brief      Lets the simulator perform a simulation step and updates the
			 *              two-dimensional _p and two-dimensional velocity of
			 *              each agent.
			 */
			virtual void doStep() = 0;

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
			 *  @brief      Returns the global time of the simulation.
			 *
			 *  @returns    The present global time of the simulation (zero initially).
			 */
			inline float getGlobalTime() const { return _globalTime; }
			
			/*!
			 *  @brief      Sets the time step of the simulation.
			 *
			 *  @param      timeStep        The time step of the simulation.
			 *                              Must be positive.
			 */
			inline void setTimeStep(float timeStep) { LOGICAL_TIME_STEP = timeStep; updateEffTimeStep(); }
			
			/*!
			 *  @brief      Sets the number of intermediate computation sub steps to take.
			 *
			 *	For the given sim time step, this number of sub steps will be taken.
			 *	This decreases the effective time step, but the simulation state to the outside
			 *	world is only reported at the simulation's *official* time step rate.
			 *
			 *  @param      subSteps			The number of sub steps to take.  
			 */
			inline void setSubSteps(size_t subSteps) { SUB_STEPS = subSteps; updateEffTimeStep(); }
			
			/*!
			 *  @brief      Returns the logical time step of the simulation.
			 *
			 *  @returns    The present time step of the simulation.
			 */
			inline float getTimeStep() const { return LOGICAL_TIME_STEP; }

			/*!
			 *	@brief			Returns the elevation of the given agent.
			 *
			 *	@param			agent		The agent.
			 *	@returns		The elevation for the given agent.
			 */
			float getElevation( const BaseAgent * agent ) const;

			/*!
			 *	@brief			Returns the elevation of the x-z position.
			 *
			 *	@param			point		The x-z point.
			 *	@returns		The elevation at the given point.
			 */
			float getElevation( const Vector2 & point ) const;

			/*!
			 *	@brief		Set the elevation instance of the simulator
			 *
			 *	@param		elevation		The elevation object.
			 */
			void setElevationInstance( Elevation * elevation );

			/*!
			 *	@brief		Set the elevation instance of the simulator
			 *
			 *	@returns	The elevation instance.
			 */
			Elevation * getElevationInstance() { return _elevation; }

			/*!
			 *	@brief		Reports if the elevation has been set.
			 *	
			 *	@returns	True if the elevation has been set, false otherwise.
			 */
			bool hasElevation() const { return _elevation != 0x0; }

			/*!
			 *	@brief		Sets the spatial query instance of the simulator.
			 *
			 *	@param		spatialQuery		The spatial query object.
			 */
			void setSpatialQuery( SpatialQuery * spatialQuery );

			/*!
			 *	@brief		get the spatial query instance of the simulator.
			 *
			 *	@returns    a pointer to The spatial query object.
			 */
			SpatialQuery *  getSpatialQuery() { return _spatialQuery;};

			/*!
			 *	@brief		Reports if the spatial query has been set.
			 *
			 *	@returns	True if the elevation has been set, false otherwise.
			 */
			bool hasSpatialQuery() const { return _spatialQuery != 0x0; }


			/*!
			 *  @brief      Performs a visibility query between the two specified
			 *              points with respect to the obstacles.
			 *
			 *	The obstacles are one-sided.  So, the ordering of point1 and point2 matter.
			 *	Looking from the inside out is not equivalent to looking from the outside in.
			 *
			 *  @param      point1          The first point of the query.
			 *  @param      point2          The second point of the query.
			 *  @param      radius          The minimal distance between the line
			 *                              connecting the two points and the obstacles
			 *                              in order for the points to be mutually
			 *                              visible (optional). Must be non-negative.
			 *  @returns    A boolean specifying whether the two points are mutually
			 *              visible. Returns true when the obstacles have not been
			 *              processed.
			 */
			bool queryVisibility(const Vector2& point1, const Vector2& point2, float radius = 0.0f) const;

			/*!
			 *	@brief		Reports the number of simulation substeps to take.
			 *
			 *	@returns	The number of substeps to take.
			 */
			inline size_t getSubSteps() const { return SUB_STEPS; }

		protected:

			/*!
			 *	@brief		Updates the effective time step -- how large an actual simulation time
			 *				step is due to computation sub-steps.
			 *
			 */
			void updateEffTimeStep() { SIM_TIME_STEP = TIME_STEP = LOGICAL_TIME_STEP / ( 1.f + SUB_STEPS ); }

			/*!
			 *	@brief		The logical simulation time step.  The simulation's state is communicated
			 *				to the outside world at this time step.  In practice, sub-steps can decrease
			 *				the effective time step.
			 */
			static float LOGICAL_TIME_STEP;

			/*!
			 *	@brief		The effective simulation time step - takes into account time step and
			 *				computation sub-steps.
			 */
			static float TIME_STEP;

			/*!
			 *	@brief		The number of intermediate steps taken between subsequent simulation
			 *				time steps.
			 */
			static size_t SUB_STEPS;

			/*!
			 *	@brief		The total accumulated simulation time.
			 */
			float _globalTime;	

			/*!
			 *	@brief		Data structure for reporting the elevation data of agents.
			 *				This allows the simulation to be more than 2D (and more than
			 *				topologically planar.)
			 */
			Elevation	*	_elevation;
			
			/*!
			 *	@brief		The data structure used to perform spatial queries.
			 */
			SpatialQuery	* _spatialQuery;
		};
	}	// namespace Agents 
}	// namespace Menge
#endif	// __SIMULATOR_INTERFACE_H__