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
 @file    NullViewer.h
 @brief   Specification for a do-nothing viewer.
 
 This is the offline simulator.
 */

#ifndef __NULL_VIEWER_H__
#define __NULL_VIEWER_H__

#include "MengeVis/Viewer/Profiler.h"
#include "MengeVis/VisConfig.h"

namespace MengeVis {

// forward declarations
namespace SceneGraph {
class GLScene;
}

namespace Viewer {

/*!
 @brief   The off-line context for running a simulation.

 The NullViewer is a mock viewer, similar to the GLViewer. It takes a SceneGraph node and repeatedly
 updates the scene. However, there is no visualization. Its purpose is simply to run the scene.

 The POINT of this type of thing is to load up an empty scene with a non-trivial system that has
 some secondary, non-visual side effects and simply exercise it.
 */
class MENGEVIS_API NullViewer {
 public:
  /*!
   @brief   Default constructor.
   */
  NullViewer();

  /*!
   @brief   Destructor.
   */
  ~NullViewer();

  /*!
   @brief   The scene to run.
   */
  void setScene(SceneGraph::GLScene* scene);

  /*!
   @brief   The main loop.
   */
  void run();

  /*!
   @brief   Sets the simulator to use a fixed time step, with the given value.

   @param   stepSize    The size of the fixed step the viewer should advance its GLScene.
   */
  void setFixedStep(float stepSize);

 protected:
  /*!
   @brief   The GLScene to draw.
   */
  SceneGraph::GLScene* _scene;

  /*!
   @brief   The step size for fixed-step simulation.
   */
  float _stepSize;

  /*!
   @brief   Timer for determining computation time.
   */
  LapTimer _fpsTimer;
};
}  // namespace Viewer
}  // namespace MengeVis
#endif  // __NULL_VIEWER_H__
