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
 @file       ZanlungoAgentContext.h
 @brief      A basic context for interacting with and displaying Zanlungo agent parameters
 */

#ifndef __ZANLUNGO_AGENT_CONTEXT_H__
#define __ZANLUNGO_AGENT_CONTEXT_H__

#include "MengeCore/Agents/Obstacle.h"
#include "MengeVis/Runtime/AgentContext/BaseAgentContext.h"
#include "ZanlungoAgent.h"

namespace Zanlungo {
// forward declarations
class Agent;

/*!
 @brief		The context for displaying the computational aspects of the Zanlungo model (see
          Agents::Zanlungo::Agent).
 */
class AgentContext : public MengeVis::Runtime::BaseAgentContext {
 public:
  /*!
   @brief		Constructor.
   */
  AgentContext();

  /*!
   @brief		Sets the agent for this context.

   This method works in conjunction with the VisElementDatabase. When this visualization element is
   triggered, the database will supply the triggering element.

   @param		agent		The agent to interact with.
   */
  virtual void setElement(MengeVis::Runtime::VisAgent* agent);

  /*!
   @brief		Returns the name of the context for display.

   @returns		The name of this context.
   */
  virtual std::string contextName() const { return "Zanlungo 2010"; }

  /*!
   @brief		The value used to store this element in the visual element database.

   This string value should correspond to the getStringId method of the corresponding simulation
   element.
   */
  virtual std::string getElementName() const { return Agent::NAME; }

  /*!
   @brief		Give the context the opportunity to respond to a keyboard event.

   @param		e		The SDL event with the keyboard event data.
   @returns	A ContextResult instance reporting if the event was handled and if redrawing is
            necessary.
   */
  virtual MengeVis::SceneGraph::ContextResult handleKeyboard(SDL_Event& e);

 protected:
  /*!
   @brief		Draw context elements into the 3D world.

   @param		select		Defines if the drawing is being done for selection purposes (true) or
                      visualization (false).
   */
  virtual void draw3DGL(bool select = false);

  /*!
   @brief		Creates a formatted string to be printed in the context for a particular agent.

   @param		agent		The agent whose data is to be displayed.
   @returns	A formatted string for display in the context's 2D gui.
   */
  virtual std::string agentText(const Menge::Agents::BaseAgent* agent) const;

  /*!
   @brief		Determines whether the time to interaction is displayed
   */
  bool _showTTI;

  /*!
   @brief		The id of the time to interaction structure shown.

   If zero, all agents, if positive, a single agent, if negative, an obstacle.
   */
  int _ttiObject;

  /*!
   @brief		Function for illustrating time to interaction computation.

   @param		agt			The agent whose time to interaction is displayed.
   */
  void drawTTI(const Agent* agt);

  /*!
   @brief		Determines if the force vectors are drawn
   */
  bool _showForce;

  /*!
   @brief		Function for drawing the force vectors acting on agt

   @param		agt			The agent whose forces are displayed.
   */
  void drawForce(const Agent* agt);

  /*!
   @brief		Draw the force vector from srcAgent acting on agt

   @param		agt			The agent on which the force is acting
   @param		other		The agent imparting the force
   @param		TTI			Time to interaction
   */
  void singleAgentForce(const Agent* agt, const Agent* other, float TTI);

  /*!
   @brief		Draws the orientation of the agent

   @param		agt			The agent whose orientation displayed.
   */
  void drawOrientationFan(const Agent* agt);
};
}  // namespace Zanlungo

#endif  // __ZANLUNGO_AGENT_CONTEXT_H__
