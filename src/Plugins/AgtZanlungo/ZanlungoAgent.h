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
 *  @file       ZanlungoAgent.h
 *  @brief      Contains the Zanlungo::Agent class
 *				Implements Zanlungo's 2011 pedestrian model
 *				"Social Force Model with Explicit Collision Prediction"
 */

#ifndef __ZANLUNGO_AGENT_H__
#define	__ZANLUNGO_AGENT_H__

#include "BaseAgent.h"

using namespace Menge;

namespace Zanlungo {
	/*!
	 *	@brief		Agent definition for the Zanlungo pedestrian model.
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
		 *	@brief		Computes the time to interaction
		 *	@param		tti		The time to interaction.  Call by reference means
		 *						that this value is set to the time to interaction
		 *	@returns	A boolean reporting if there is interaction.  True if there
		 *				is interaction, false otherwise.  
		 */
		bool computeTTI( float & tti ) const;

		/*!
		 *	@brief		Compute the force due to another agent
		 *	@param		other			A pointer to a neighboring agent
		 *	@param		T_i				The time to interaction
		 *	@returns	The force imparted by the other agent on this agent
		 */
		Vector2 agentForce( const Agent * other, float T_i ) const;

		/*!
		 *  @brief      Computes the new velocity of this agent.
		 */
		void computeNewVelocity();

		/*!
		 *	@brief		Reports the field of view for the agent
		 */
		inline float getFOV() const { return 3.14159265f /* * 0.5*/; }// 90 degrees 

		/*!
		 *	@brief			Computes the velocities based on right of way
		 *	@param			otherVel		The other agent's current velocity.  
		 *									The right-of-way-relative other velocity 
		 *									will be set in this variable.
		 *	@param			otherPrefVel	The other agent's preferred velocity.
		 *	@param			otherPriority	The other agent's priority value
		 *	@param			vel				This agent's right-of-way-relative velocity
		 *									will be placed in this variable.
		 *	@returns		The signed right of way value. (Negative implies other has
		 *					right of way, positive implies this agent has right of way.)
		 */
		float rightOfWayVel( Vector2 & otherVel, const Vector2 & otherPrefVel, float otherPriority, Vector2 & vel ) const;

		/*!
		 *	@brief		The mass of the agent
		 */
		float		_mass;

	};
}	// namespace Zanlungo

#endif	 // __ZANLUNGO_AGENT_H__