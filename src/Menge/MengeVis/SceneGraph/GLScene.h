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
 *	@file		GLScene.h
 *	@brief		The definition of the scene -- the root of the directed,
 *				acyclic scene graph.
 *
 *	It contains transforms, instances, lights, etc.  It does *not* contain
 *	a camera.  That is part of the viewer.
 */

//	TODO: determine if the camera should be part of the scene or viewer.

#ifndef __GLSCENE_H__
#define __GLSCENE_H__

#include "MengeVis/VisConfig.h"

#include "MengeVis/SceneGraph/Context.h"
#include "MengeVis/SceneGraph/GLLight.h"
#include "MengeVis/SceneGraph/GLNode.h"
#include "MengeVis/SceneGraph/System.h"

#include <list>

namespace MengeVis {

namespace SceneGraph {

class GLCamera;

/*!
 *	@brief		The class which contains the entire drawable scene.
 */
class MENGEVIS_API GLScene {
 public:
  /*!
   *	@brief		Default constructor.
   */
  GLScene();

  /*!
   *	@brief		Destructor.
   */
  ~GLScene();

  /*!
   *	@brief		Informs the scene that it is done being used.
   *
   *	This gives the scene the chance to free up memory and stop whatever
   *	processes it may have been using (e.g., flushing caches, etc.)
   *	Typically just called by the viewer at the conclusion of visualization.
   *	More particularly, it gives the scene's systems to clean up and conclude
   *	their processes.
   */
  void finish();

  /*!
   *	@brief		Allows the scene to recreate any unique OpenGL objects
   *				for all elements of the scene due to acquisition of a new
   *				OpenGL context.
   */
  void newGLContext();

  /*!
   *	@brief		Causes the scene to draw into an open gl context
   *
   *	@param		camera		The camera from which to derive the draw the scene.
   *	@param		lights		The lights to illuminate the 3D elements.
   *	@param		vWidth		The viewport width, in pixels, used in text overlay.
   *	@param		vHeight		The viewport height, in pixels, used in text overlay.
   */
  void drawGL(const GLCamera& camera, const GLLightVector& lights, int vWidth, int vHeight);

  /*!
   *	@brief		Draws the selectable elements of the scene in preparation for
   *				selection.
   *
   *	This uses the OpenGL selection context to perform active selection of elements
   *	in the scene.
   *
   *	@param		camera		The camera from which to derive the draw the scene.
   *	@param		vWidth		The viewport width, in pixels, used in text overlay.
   *	@param		vHeight		The viewport height, in pixels, used in text overlay.
   *	@param		selectPoint	The point in screen space at which selection should occur.
   *	@returns	True if the selection changed, false otherwise.
   */
  bool selectGL(const GLCamera& camera, int vWidth, int vHeight, int* selectPoint);

  /*!
   *	@brief		Updates the state of the scene graph to the global time
   *
   *	@param		time		The time for which the scene should be drawn.
   *	@returns	A boolean reporting if a redraw is necessary (true) or not (false).
   */
  bool updateScene(float time);

  /*!
   *	@brief		Add a node to the scene (with an optional parent)
   *
   *	A node is added to the scene.  If no parent is specified, the node is a child
   *	of the scene directly.  Otherwise, it is a child of the parent.
   *	NOTE: No test is performed to confirm that the parent already exists in the
   *	scene.  It is the responsibility of the caller to confirm that this is the case.
   *
   *	@param		node		The node to add to the scene.
   *	@param		parent		The optional parent node.
   *	@returns	Indication if the addition was successful (true) or not (false).
   *				Currently, this function always returns true.
   */
  bool addNode(GLNode* node, GLDagNode* parent = 0x0);

  /*!
   *	@brief		Adds a System to the scene.
   *
   *	@param		system		The system to add to the scene.
   *	@returns	True if the system is successfully added, false otherwise.
   *				In this implementation, it is *always* successful.
   */
  bool addSystem(System* system);

  /*!
   *	@brief		Sets the context the scene will use to handle UI events.
   *
   *	If the scene already has a context assigned, that context will be
   *	deactivated and *deleted*.
   *
   *	@param		context		The scene's context.
   */
  void setContext(Context* context);

  /*!
   *	@brief		Allows the scene to respond to a mouse event.
   *
   *	@param		e		An instance of a mouse event.
   *	@returns	An instance of a ContextResult, reporting whether the event has
   *				been handled and whether a redraw is necessary.
   */
  ContextResult handleMouse(SDL_Event& e);

  /*!
   *	@brief		Allows the scene to respond to a keyboard event.
   *
   *	@param		e		An instance of a keyboard event.
   *	@returns	An instance of a ContextResult, reporting whether the event has
   *				been handled and whether a redraw is necessary.
   */
  ContextResult handleKeyboard(SDL_Event& e);

  friend class SelectContext;

 private:
  /*!
   *	@brief		List of animation systems
   */
  SystemList _systems;

  /*!
   *	@brief		List of nodes which form the graph.  Specifically, the nodes
   *				that are direct children of the scene.
   */
  GLNodeList _nodes;

  /*!
   *	@brief		UI context for handling events.
   */
  Context* _uiContext;
};

}  // namespace SceneGraph
}  // namespace MengeVis
#endif  //__GLSCENE_H__
