/*
 Menge Crowd Simulation Framework

 Copyright and trademark 2012-17 University of North Carolina at Chapel Hill

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0
 or
    LICENSE.txt in the root of the Menge repository.

 Any questions or comments should be sent to the authors menge@cs.unc.edu

 <http://gamma.cs.unc.edu/Menge/>
*/

/*!
 @file       ORCAAgent.h
 @brief      Contains the PedVOAgent class.
 */

#ifndef __ORCA_AGENT_H__
#define __ORCA_AGENT_H__

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Math/Line.h"

#include <vector>

namespace ORCA {
/*!
 @brief      Defines an agent in the simulation.
 */
class MENGE_API Agent : public Menge::Agents::BaseAgent {
 public:
  /*!
   @brief    Constructor
   */
  Agent();

  /*!
   @brief      Destroys this agent instance.
   */
  ~Agent();

  /*!
   @brief      Computes the new velocity of this agent.
   */
  void computeNewVelocity();

  /*!
   @brief    Used by the plugin system to know what artifacts to associate with agents of this type.
   
   Every sub-class of must return a globally unique value if it should be associated with unique
   artifacts.
   */
  virtual std::string getStringId() const;

  /*!
   @brief      Based on the neighbors, computes the ORCA lines

   @returns    The total number of obstacle lines
   */
  size_t computeORCALines();

  /*!
   @brief    The set of ORCA constraints.
   */
  std::vector<Menge::Math::Line> _orcaLines;

  /*!
   @brief    The time horizon for inter-agent interactions.
   */
  float _timeHorizon;

  /*!
   @brief    The time horizon for agent-obstacle interactions.
   */
  float _timeHorizonObst;

  // DEFAULT VALUES FOR THE AGENT PARAMTERS
  /*!
   @brief    The default time horizon for inter-agent interactions.
   */
  static const float TAU;

  /*!
   @brief    The default time horizon for agent-obstacle interactions.
   */
  static const float TAU_OBST;

  /*! @brief  The name identifier for this agent type. */
  static const std::string NAME;

  friend class Simulator;

 protected:
  /*!
   @brief    Constructs an ORCA line for the given obstacle under the assumption that the agent is on
            its right side.

   If appropriate, it adds the obstacle to the set of orca lines.

   @param    obstNbrID    The index of the near-by obstacle to test.
   @param    invTau      1 / _timeHorizonObst - the inverse of the time horizon for obstacles.
   @param    flip        The agent is on the left side of this obstacle.
   */
  void obstacleLine(size_t obstNbrID, const float invTau, bool flip);
};

/*!
 @brief      Solves a one-dimensional linear program on a specified line subject to linear
             constraints defined by lines and a circular constraint.

 @param      lines         Lines defining the linear constraints.
 @param      lineNo        The specified line constraint.
 @param      radius        The radius of the circular constraint.
 @param      optVelocity   The optimization velocity.
 @param      directionOpt  True if the direction should be optimized.
 @param      result        A reference to the result of the linear program.
 @returns    True if successful.
 */
bool linearProgram1(const std::vector<Menge::Math::Line>& lines, size_t lineNo, float radius,
                    const Menge::Math::Vector2& optVelocity, bool directionOpt,
                    Menge::Math::Vector2& result);

/*!
 @brief     Solves a two-dimensional linear program subject to linear constraints defined by lines
            and a circular constraint.

 @param     lines         Lines defining the linear constraints.
 @param     radius        The radius of the circular constraint.
 @param     optVelocity   The optimization velocity.
 @param     directionOpt  True if the direction should be optimized.
 @param     result        A reference to the result of the linear program.
 @returns   The number of the line it fails on, and the number of lines if successful.
 */
size_t linearProgram2(const std::vector<Menge::Math::Line>& lines, float radius,
                      const Menge::Math::Vector2& optVelocity, bool directionOpt,
                      Menge::Math::Vector2& result);

/*!
 @brief     Solves a two-dimensional linear program subject to linear constraints defined by lines
            and a circular constraint.

 @param     lines         Lines defining the linear constraints.
 @param     numObstLines  Count of obstacle lines.
 @param     beginLine     The line on which the 2-d linear program failed.
 @param     radius        The radius of the circular constraint.
 @param     result        A reference to the result of the linear program.
 */
void linearProgram3(const std::vector<Menge::Math::Line>& lines, size_t numObstLines,
                    size_t beginLine, float radius, Menge::Math::Vector2& result);
}  // namespace ORCA

#endif
