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
 *	@file		GCFAgent.h
 *	@brief		The agent specification for the pedestrian model based
 *				on the Chraibi et al., 2010 paper.
 */

#ifndef __GCF_AGENT_H__
#define	__GCF_AGENT_H__

#include "BaseAgent.h"
#include "Ellipse.h"

using namespace Menge;


namespace GCF {
	/*!
	 *	@brief		Agent definition for the generalized centrifugal force pedestrian model.
	 */
	class Agent : public Agents::BaseAgent {
	public:
		/*!
		 *	@brief		Default constructor.
		 */
		Agent();

		/*!
		 *  @brief      Destroys this agent instance.
		 */
		~Agent();

		/*!
		 *	@brief		Initializes the agent
		 *
		 *	Subclasses should call their parent's implementation of initialize.
		 */
		virtual void initialize();

		/*!
		 *	@brief		Method for sub-classes to perform additional update work
		 *			
		 *	This is the last thing called by the update method.  When this is called,
		 *	position, velocity, and orientation will be updated in the base agent
		 *	class.
		 */
		virtual void postUpdate();

		/*!
		 *  @brief      Computes the new velocity of this agent.
		 */
		void computeNewVelocity();

		/*!
		 *	@brief			Provides the various parameters of the pedestrian repulsive 
		 *					force between this agent and the given agent. 
		 *					Helper for visualization.
		 *
		 *	@param		agent		The agent to compute the various repulsion parameters for.
		 *	@param		effDist		The "effective" distance between the closest points on the agent boundaries.
		 *	@param		forceDir	The direction of the force.
		 *	@param		K_ij		The field of view parameter between agents.
		 *	@param		response	The force strength based on effective distance.
		 *	@param		velScale	The scale on the force magnitude based on the relative velocity.
		 *	@param		magnitude	The overall magnitude of the repulsive force.
		 *	@returns	An int indicating the computation success: 
		 *					0 = repulsive force exists.
		 *					1 = agent too far away.
		 *					2 = agent out of field of view
		 */
		int getRepulsionParameters( const Agent * agent, float & effDist, Vector2 & forceDir, float & K_ij, float & response, float & velScale, float & magnitude ) const;

		/*!
		 *	@brief		Computes component of the force magnitude based on effective distance.
		 *	@param		effDist			The effective distance from ellipse center to force source
		 */		
		float computeDistanceResponse( float effDist ) const;

		/*!
		 *	@brief		Updates the ellipse to the current state
		 */
		void updateEllipse();
#if 0

		/*!
		 *	@brief		Compute the force due to another agent
		 *	@param		other			A pointer to a neighboring agent
		 *	@returns	The force imparted by the other agent on this agent
		 */
		Vector2 agentForce( const Agent * other ) const;

		/*!
		 *	@brief		Compute the force due to a nearby obstacle
		 *	@param		obst			A pointer to the obstacle
		 *	@returns	The force imparted by the obstacle on this agent
		 */
		Vector2 obstacleForce( const Agents::Obstacle * obst ) const;
#endif
		/*!
		 *	@brief		Computes the driving force for the agent
		 *	@returns	The vector corresponding to the agent's driving force.
		 */
		Vector2 driveForce() const;

		/*!
		 *	@brief		The ellipse representing the agent's volume
		 */
		Ellipse		_ellipse;

		/*!
		 *	@brief		The minimum value for the radius along the agent's facing direction.
		 */
		float _aMin;			

		/*!
		 *	@brief		The rate of change of the value for the radius along the agent's facing direction.
		 */
		float _aRate;			

		/*!
		 *	@brief		The maximum value for the radius along the agent's perpendicular direction.
		 */
		float _bMax;			

		/*!
		 *	@brief		The rate of change of the value for the radius along the agent's perpendicular direction.
		 */
		float _bGrowth;			
	};
}	// namespace GCF

#endif