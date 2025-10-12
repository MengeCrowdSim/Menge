/*
 OSM Simulation plugin

 Sergio Lozano Mateos
 UNED

*/

/*!
 @file       OSMAgent.h
 @brief      Contains the OSM::Agent class.

 Implements OSM "Natural discretization of pedestrian movement in continuous space"
 */

#ifndef __OSM_AGENT_H__
#define __OSM_AGENT_H__

#include "MengeCore/Agents/BaseAgent.h"

namespace OSM {
/*!
 @brief		The agent definition for the Karamouzas 2009 agent.
 */
class Agent : public Menge::Agents::BaseAgent {
 public:
  /*!
   @brief		A variant of the copy constructor.
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
   @brief		Used by the plugin system to know what artifacts to associate with agents of this type.
   
   Every sub-class of must return a globally unique value if it should be associated with unique
   artifacts.
   */
  virtual std::string getStringId() const { return NAME; }

  /*! @brief	The name identifier for this agent type. */
  static const std::string NAME;

  /*!
   @brief		The personal space (in meters) of the agent
   */
  float _perSpace;

  /*!
   @brief		The anticipation time (in seconds) of the agent
   */
  float _anticipation;
};
}  // namespace OSM
#endif  // __OSM_AGENT_H__
