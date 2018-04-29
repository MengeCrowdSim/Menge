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
 @file    VelModifierContext.h
 @brief    The definition of a basic UI context for finite state machine *velocity modifiers*.
 */

#if 0

#ifndef __VEL_MOD_CONTEXT_H__
#define __VEL_MOD_CONTEXT_H__

#include <vector>
#include "Context.h"

namespace Menge {

namespace Agents {
  class BaseAgent;
  class PrefVelocity;
}

namespace BFSM {

// Forward declarations
class VelModifier;

/*!
  @brief    Base context for finite state machine velocity modifiers.

  This differs from the standard scene graph context by being dependent on an input agent.  
  */
class MENGE_API VelModContext : public SceneGraph::Context {
public:
  /*!
    @brief    Constructor.
    */
  VelModContext(){}

  /*!
    @brief    This supplants the destructor.

    In order to preserve potential problems in windows when dlls do not share the same c-runtime
    library, the destructor is held to be private. To garbage collect an Velmodifier, the destroy
    method should be called (which in turn, will call the destructor from its own memory space,
    averting run-time crashes).

    Once this has been called, the VelModifier no longer exists. Calling methods or accessing
    members will produce indetermine behavior (most likely errors).
    */
  void destroy() { delete this; }

protected:
  /*!
   @brief   Destructor.
   */
  virtual ~VelModContext(){}

public:
  /*!
   @brief   Provides a string to be printed in the display as a UI element detailing velocity
            modifier information.

   @param   indent    An optional string representing indentation to be applied to the text. It is
                      prefixed at the start of each line.
   @returns  The string for printing on the UI layer.
   */
  virtual std::string getUIText( const std::string & indent="" ) const;

  /*!
   @brief   Draw context elements into the 3D world.

   This should never be called in select mode.

   @param   agt   The particular agent for which the FSM is being visualized.
   */
  virtual void draw3DGL( const Agents::BaseAgent * agt){}
};

}  // namespace BFSM
}  // namespace Menge
#endif  // __VEL_MOD_CONTEXT_H__
#endif  // 0