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

#include "CoreConfig.h"

#include "GLNode.h"
#include "System.h"
#include "Context.h"
#include "GLLight.h"

#include <list>


namespace Menge {

	namespace SceneGraph {

		class GLCamera;

		/*!
		 *	@brief		The class which contains the entire drawable scene.
		 */
		class MENGE_API GLScene {
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
			void drawGL( const GLCamera & camera, const GLLightVector & lights, int vWidth, int vHeight );

			/*!
			 *	@brief		Draws the selectable elements of the scene in preparation for selection.
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
			bool selectGL( const GLCamera & camera, int vWidth, int vHeight, int * selectPoint );

			/*!
			 *	@brief		Updates the state of the scene graph to the global time
			 *
			 *	@param		time		The time for which the scene should be drawn.
			 *	@returns	A boolean reporting if a redraw is necessary (true) or not (false).
			 */
			bool updateScene( float time );

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
			bool addNode( GLNode * node, GLDagNode * parent = 0x0 );

			/*!
			 *	@brief		Adds a System to the scene.
			 *
			 *	@param		system		The system to add to the scene.
			 *	@returns	True if the system is successfully added, false otherwise.
			 *				In this implementation, it is *always* successful.
			 */
			bool addSystem( System * system );

			/*!
			 *	@brief		Sets the context the scene will use to handle UI events.
			 *
			 *	If the scene already has a context assigned, that context will be
			 *	deactivated and *deleted*.
			 *
			 *	@param		context		The scene's context.
			 */
			void setContext( Context * context );

			/*!
			 *	@brief		Allows the scene to respond to a mouse event.
			 *
			 *	@param		e		An instance of a mouse event.
			 *	@returns	An instance of a ContextResult, reporting whether the event has
			 *				been handled and whether a redraw is necessary.
			 */
			ContextResult handleMouse( SDL_Event & e );
			
			/*!
			 *	@brief		Allows the scene to respond to a keyboard event.
			 *
			 *	@param		e		An instance of a keyboard event.
			 *	@returns	An instance of a ContextResult, reporting whether the event has
			 *				been handled and whether a redraw is necessary.
			 */
			ContextResult handleKeyboard( SDL_Event & e );

			friend class SelectContext;
		private:
			/*!
			 *	@brief		List of animation systems
			 */
			SystemList	_systems;

			/*!
			 *	@brief		List of nodes which form the graph.  Specifically, the nodes
			 *				that are direct children of the scene.
			 */
			GLNodeList	_nodes;
			
			/*!
			 *	@brief		UI context for handling events.
			 */
			Context *	_uiContext;
		};

	}	// namespace SceneGraph
}	// namespace Menge
#endif  //__GLSCENE_H__