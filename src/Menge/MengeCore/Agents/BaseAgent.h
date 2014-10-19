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

#ifndef __BASE_AGENT_H__
#define __BASE_AGENT_H__

/*!
 *  @file       BaseAgent.h
 *  @brief      Contains the BaseAgent class - the underlying class which
 *				defines the basic functionality for all shared agents.
 */

// UTILS
#include "mengeCommon.h"
#include "XMLSimulatorBase.h"
#include "PrefVelocity.h"
#include "VelocityModifiers/VelModifier.h"
#include "SpatialQueries/ProximityQuery.h"
#include "SpatialQueries/SpatialQueryStructs.h"
// STL
#include <vector>
#include <list>

namespace Menge {

	namespace Agents {

		class Obstacle;

		/*!
		 *	@brief		Exception for BaseAgent problems.
		 */
		class MENGE_API AgentException : public virtual MengeException  {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			AgentException() : MengeException() {}		

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			AgentException( const std::string & s ): MengeException(s) {}
		};

		/*!
		 *	@brief		The fatal agent exception.
		 */
		class MENGE_API AgentFatalException : public AgentException, public MengeFatalException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			AgentFatalException() : MengeException(), AgentException(), MengeFatalException() {}

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			AgentFatalException( const std::string & s ): MengeException(s), AgentException(), MengeFatalException() {}
		};

		/*!
		 *	@brief		Special agent exception - used for non-implemented functionality.
		 */
		class MENGE_API AgentImplementationException : public AgentFatalException  {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			AgentImplementationException() : AgentFatalException() {}		

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			AgentImplementationException( const std::string & s ): AgentFatalException(s) {}
		};

		/*!
		 *  @brief      Defines the basic agent properties and functionality that
		 *				all simulation agents share.
		 */
		class MENGE_API BaseAgent : public ProximityQuery {
		public:

			/*!
			 *	@brief		Default constructor.
			 */
			BaseAgent();

			/*!
			 *	@brief		Initializes the agent
			 */
			void initialize();

			/*!
			 *  @brief      Updates the two-dimensional position and two-dimensional
			 *              velocity of this agent.
			 *
			 *	@param		timeStep		The time step that will be taken.
			 */
			void update( float timeStep );

			/*!
			 *	@brief		Given preferred velocity and neighboring agents and obstacles
			 *				compute a new velocity.  
			 *
			 *	This should be overriden by child classes to give unique behaviors.  
			 *	Each pedestrian model is uniquely defined by how it computes its new 
			 *	velocity and this is the critical class.
			 *
			 *	Trying to instantiate a BaseAgent will cause an exception to be thrown
			 *	when calling this function.
			 */
			void computeNewVelocity();

			/*!
			 *	@brief			Returns a pointer to the neighbor with given index
			 *
			 *	@param			idx		The index of the desired agent.  This index is *not* validated.
			 *	@returns		Pointer to the neighboring agent.
			 */
			const BaseAgent * getNeighbor( int idx ) const { return _nearAgents[ idx ].agent; }

			/*!
			 *	@brief			Returns a pointer to the obstacle with given index
			 *
			 *	@param			idx		The index of the desired obstacle.  This index is *not* validated.
			 *	@returns		Pointer to the nearby obstacle.
			 */
			const Obstacle * getObstacle( int idx ) const { return _nearObstacles[ idx ].obstacle; }

			
			/*!
			 *	@brief			set the agents preferred velocity to the input velocity. 
			 *
			 *	@param			velocity to be applied to the agent.
			 */
			void setPreferredVelocity(PrefVelocity &velocity);

			/*!
			 *	@brief		Add an velocity modifier to the agent
			 *
			 *	@param		v		The modifier to add
			 */
			void addVelModifier( BFSM::VelModifier * v );


			// Properties of a basic agent
			/*!
			 *	@brief		The maximum speed the agent can take.
			 */
			float _maxSpeed;

			/*!
			 *	@brief		The maximum acceleration the agent can experience (interpreted isotropically).
			 */
			float _maxAccel;

			/*!
			 *	@brief		The preferred speed of the agent
			 */
			float _prefSpeed;	

			/*!
			 *	@brief		The current 2D position of the agent
			 */
			Vector2	_pos;	

			/*!
			 *	@brief		The current 2D velocity of the agent
			 */
			Vector2	_vel;	

			/*!
			 *	@brief		The 2D preferred velocity of the agent
			 */
			PrefVelocity	_velPref;	

			/*!
			 *	@brief		The new velocity computed in computeNewVelocity.
			 *
			 *	This exists to allow the agents to be updated
			 *	in parallel while preserving order-of-evaluation independence.
			 */
			Vector2	_velNew;	

			/*!
			 *	@brief		The orientation vector (the direction the agent is facing which is not
			 *				necessarily the same direction as the instantaneous velocity.
			 *
			 *	Not all pedestrian models require orientation in their calculation of a new
			 *	velocity.  However, by introducing the property here, we accomplish two things:
			 *		- All agents which *do* require orientation can update their orientation
			 *		  in a common mechanism, making comparisons them less dependent on differences
			 *		  in orientation computation and focusing on differences in the new velocity
			 *		  computation.
			 *      - Second, it provides orientation information to the output trajectories for
			 *		  later visualization.
			 */
			Vector2	_orient;

			/*!
			 *	@brief		The agent's maximum angular velocity (in radians/sec) -- 
			 *				used for controlling the changes in agent orientation.
			 */
			float	_maxAngVel;

			/*!
			 *	@brief		The number of nearby agents used to plan dynamic respones.
			 */
			size_t _maxNeighbors;

			/*!
			 *	@brief		The maximum distance at which another agent will be 
			 *				considered for a response.
			 */
			float _neighborDist;


			/*!
			 *	@brief		The population class for this agent.  
			 *
			 *	Used to define behavior and visualization properties.
			 */
			size_t _class;
			
			/*!
			 *	@brief		A mask indicating the obstacles with compatible ids which
			 *				this agent can see.  
			 *
			 *	This is a bitwise mask such that if
			 *	the ith bit is 1, obstacles with id 2^i are visible.
			 */
			size_t _obstacleSet;

			/*!
			 *	@brief		The priority of each agent.  
			 *
			 *	The relative priority of agents determines aspects of their interaction behavior.
			 */
			float _priority;

			/*!
			 *	@brief		A globally unique identifier for each agent.
			 */
			size_t _id;

			/*!
			 *	@brief		The agent's radius.  
			 *
			 *	If the agent is represented as a circle, then this is simply 
			 *	the circle's radius.  If the agent is represented as an ellipse, 
			 *	then this is the radius perpendicular to the orientation.
			 *	Other geometries should provide their own interpretation.
			 *
			 *	At a minimum, it is used to determine sideways clearance.
			 */
			float _radius;

			/*!
			 *	@brief  a set of velocity modifiers to be set with the agent. Allows for intermediate velocity changes
			 *
			 */
			std::vector<BFSM::VelModifier *> _velModifiers;

			/*!
			 *	@brief		The nearby agents to which the agent should respond.
			 *
			 *	Each pair consists of distance between the agent positions, squared
			 *	and the pointer to the neigboring agent.
			 */
			std::vector<NearAgent> _nearAgents;

			/*!
			 *	@brief		The nearby obstacles to which the agent should respond.
			 *
			 *	Each pair consists of distance between agent position and wall, squared
			 *	and the pointer to the wall.
			 */
			std::vector<NearObstacle> _nearObstacles;

			/*!
			 *  @brief      Inserts an agent neighbor into the set of neighbors of
			 *              this agent.
			 *
			 *  @param      agent           A pointer to the agent to be inserted.
			 *  @param      distSq         the distance to the indicated agent
			 */
			void insertAgentNeighbor(const BaseAgent* agent, float distSq);

			/*!
			 *  @brief      Inserts a static obstacle neighbor into the set of neighbors
			 *              of this agent.
			 *
			 *  @param      obstacle        a pointer to the obstacle to be inserted
			 *  @param      distSq         the distance to the indicated obstacle
			 */
			void insertObstacleNeighbor(const Obstacle* obstacle, float distSq);

			// TODO: Ultimately, this should go into an intention filter and not the agent itself
			/*!
			 *	@brief		Sets the density sensitivity parameters.
			 *
			 *	@param		stride		The stride factor.  The physical component capturing height
			 *							and the physicl relationship between speed and stride length.
			 *	@param		buffer		The stride buffer.  The psychological buffer required beyond
			 *							that needed for stride length.
			 */
			virtual void setStrideParameters( float stride, float buffer ) {}


			// Methods needed for a spatial query filter to work

			/*!
			 *  @brief     clears the result vectors. Resets the filter
			 */
			virtual void startQuery();

			/*!
			 *  @brief      filters an agent and determines if it needs to be in the near set
			 *
			 *  @param      agent     the agent to consider
			 *  @param      distance  the distance to the agent
			 */
			virtual void filterAgent(const BaseAgent *agent, float distance);

			/*!
			 *  @brief      filters an obstacle and determines if it needs to be in the near set
			 *
			 *  @param      obstacle  the obstacle to consider
			 *  @param      distance  the distance to the obstacle
			 */
			virtual void filterObstacle(const Obstacle *, float distance);

			/*!
			 *  @brief      gets the start point for the query
			 *
			 *   @returns    the query point for this filter
			 */
			virtual Vector2 getQueryPoint(){ return _pos;};

			/*!
			 *  @brief      updates the max agent query range if conditions inside the filter are met
			 *              typically, we don't shrink the query range until the result set is full
			 *
			 *	@returns	The Max query range. Typically this is the initial range unless some 
			 *              special conditions are met
			 */
			virtual float getMaxAgentRange();

			/*!
			 *  @brief      updates the max query obstacle range if conditions inside the filter are met
			 *              typically, we don't shrink the query range until the result set is full
			 *
			 *	@returns	The Max query range. Typically this is the initial range unless some 
			 *              special conditions are met
			 */
			virtual float getMaxObstacleRange() { return _neighborDist * _neighborDist;};
		};
	}	// namespace Agents
}	// namespace Menge
#endif	// __BASE_AGENT_H__
