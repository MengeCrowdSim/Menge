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
 *  @file       ORCAAgent.h
 *  @brief      Contains the PedVOAgent class.
 */

#ifndef __ORCA_AGENT_H__
#define __ORCA_AGENT_H__

#include "BaseAgent.h"
#include "Math/Line.h"

namespace ORCA {
	/*!
	 *  @brief      Defines an agent in the simulation.
	 */
	class Agent : public Menge::Agents::BaseAgent 	{
	public:
		/*!
		 *	@brief		Constructor
		 */
		Agent();

		/*!
		 *  @brief      Destroys this agent instance.
		 */
		~Agent();

		/*!
		 *  @brief      Computes the new velocity of this agent.
		 */
		void computeNewVelocity();

		/*!
		 *	@brief			Based on the neighbors, computes the ORCA lines
		 *
		 *	@returns		The total number of obstacle lines
		 */
		size_t computeORCALines();
	
		/*!
		 *	@brief		The set of ORCA constraints.
		 */
		std::vector<Menge::Math::Line> _orcaLines;

		/*!
		 *	@brief		The time horizon for inter-agent interactions.
		 */
		float _timeHorizon;

		/*!
		 *	@brief		The time horizon for agent-obstacle interactions.
		 */
		float _timeHorizonObst;

		// DEFAULT VALUES FOR THE AGENT PARAMTERS
		/*!
		 *	@brief		The default time horizon for inter-agent interactions.
		 */
		static const float TAU;

		/*!
		 *	@brief		The default time horizon for agent-obstacle interactions.
		 */
		static const float TAU_OBST;

		friend class Simulator;

  protected:
		/*!
		 *	@brief		Constructs an ORCA line for the given obstacle
		 *				under the assumption that the agent is on its right side.
		 *				If appropriate, it adds the obstacle to the set of orca lines.
		 *
		 *	@param		obstNbrID		The index of the near-by obstacle to test.
		 *	@param		invTau			1 / _timeHorizonObst - the inverse of the 
		 *								time horizon for obstacles.
		 *	@param		flip			The agent is on the left side of this obstacle.
		 */
		void obstacleLine( size_t obstNbrID, const float invTau, bool flip );
  };

  /*!
   *  @brief      Solves a one-dimensional linear program on a specified line
   *              subject to linear constraints defined by lines and a circular
   *              constraint.
   *
   *  @param      lines         Lines defining the linear constraints.
   *  @param      lineNo        The specified line constraint.
   *  @param      radius        The radius of the circular constraint.
   *  @param      optVelocity   The optimization velocity.
   *  @param      directionOpt  True if the direction should be optimized.
   *  @param      result        A reference to the result of the linear program.
   *  @returns    True if successful.
   */
  bool linearProgram1(const std::vector<Menge::Math::Line>& lines, size_t lineNo,
					  float radius, const Vector2& optVelocity,
					  bool directionOpt, Vector2& result);

  /*!
   *  @brief      Solves a two-dimensional linear program subject to linear
   *              constraints defined by lines and a circular constraint.
   *
   *  @param      lines         Lines defining the linear constraints.
   *  @param      radius        The radius of the circular constraint.
   *  @param      optVelocity   The optimization velocity.
   *  @param      directionOpt  True if the direction should be optimized.
   *  @param      result        A reference to the result of the linear program.
   *  @returns    The number of the line it fails on, and the number of lines if successful.
   */  
  size_t linearProgram2(const std::vector<Menge::Math::Line>& lines, float radius,
					  const Vector2& optVelocity, bool directionOpt,
					  Vector2& result);

  /*!
   *  @brief      Solves a two-dimensional linear program subject to linear
   *              constraints defined by lines and a circular constraint.
   *
   *  @param      lines         Lines defining the linear constraints.
   *  @param      numObstLines  Count of obstacle lines.
   *  @param      beginLine     The line on which the 2-d linear program failed.
   *  @param      radius        The radius of the circular constraint.
   *  @param      result        A reference to the result of the linear program.
   */
  void linearProgram3(const std::vector<Menge::Math::Line>& lines, size_t numObstLines, size_t beginLine, 
					  float radius, Vector2& result);
}		// namespace ORCA

#endif
