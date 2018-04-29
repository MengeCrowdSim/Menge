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

#ifndef __NAV_MESH_VC_CONTEXT_H__
#define __NAV_MESH_VC_CONTEXT_H__

#include "MengeVis/Runtime/VCContext/VelCompContext.h"

// forward declarations
namespace Menge {
namespace Agents {
class BaseAgent;
}
namespace BFSM {
class NavMeshVelComponent;
class Goal;
}  // namespace BFSM
}  // namespace Menge

namespace MengeVis {
namespace Runtime {
namespace VCContext {

/*!
 @brief   The context for the NavMeshVelComponent.
 */
class MENGEVIS_API NavMeshVCContext : public VelCompContext {
 public:
  /*!
   @brief   Default constructor.
   */
  NavMeshVCContext();

  /*!
   @brief   Sets the agent for this context.

   This method works in conjunction with the VisElementDatabase. When this visualization element is
   triggered, the database will supply the triggering element.

   @param   vc    The velocity component to interact with.
   */
  virtual void setElement(const Menge::BFSM::VelComponent* vc);

  /*!
   @brief   The value used to store this element in the visual element database.

   This string value should correspond to the getStringId method of the corresponding simulation
   element.
   */
  virtual std::string getElementName() const;

  /*!
   @brief   Provides a string to be printed in the display as a UI element detailing velocity
            component information.

   @param   indent    An optional string representing indentation to be applied to the text. It is
                      prefixed at the start of each line.
   @returns The string for printing on the UI layer.
   */
  virtual std::string getUIText(const std::string& indent = "") const;

  /*!
   @brief   Give the context the opportunity to respond to a keyboard event.

   @param   e   The SDL event with the keyboard event data.
   @returns A ContextResult instance reporting if the event was handled and if redrawing is
            necessary.
   */
  virtual SceneGraph::ContextResult handleKeyboard(SDL_Event& e);

  /*!
   @brief   Draw context elements into the 3D world.

   This should never be called in select mode.

   @param   agt     The particular agent for which the FSM is being visualized.
   @param   goal    The agent's goal (although this may be ignored).
   */
  virtual void draw3DGL(const Menge::Agents::BaseAgent* agt, const Menge::BFSM::Goal* goal);

 protected:
  /*!
   @brief   Draws the navigation mesh to the 3D context based on current settings.
   */
  void drawNavMesh() const;

  /*!
   @brief   Draws the path for the selected agent.

   @param   agt     The particular agent for which the FSM is being visualized.
   @param   goal    The agent's goal (although this may be ignored).
   */
  void drawPath(const Menge::Agents::BaseAgent* agt, const Menge::BFSM::Goal* goal) const;

  /*!
   @brief   Draws the ids on the polygon mesh centers.
   */
  void drawNodeIDs() const;

  /*!
   @brief   The underlying finite state machine velocity component.
   */
  const Menge::BFSM::NavMeshVelComponent* _vc;

  /*!
   @brief   Visualization flag - controls if polygon centers are drawn.
   */
  bool _drawCenters;

  /*!
   @brief   Visualizaton flag - controls if polygon ids are drawn.
   */
  bool _drawNodeIDs;
};
}  // namespace VCContext
}  // namespace Runtime
}  // namespace MengeVis

#endif  // __NAV_MESH_VC_CONTEXT_H__
