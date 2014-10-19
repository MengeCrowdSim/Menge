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
 *	@file		ContextSwitcher.h
 *	@brief		The definition of a context which allows the ability to select between 
 *				multiple contexts.
 */

#ifndef __CONTEXT_SWITCHER_H__
#define __CONTEXT_SWITCHER_H__

#include "Context.h"
#include <map>

namespace Menge {
	namespace SceneGraph {
		/*!
		 *	@brief		A mapping from a SDL key value and a context
		 */
		typedef std::map< SDLKey, Context * > KeyContextMap;

		/*!
		 *	@brief		Special context for switching between multiple contexts.
		 *				Each context tied to a keyboard shortcut.
		 */
		class MENGE_API ContextSwitcher : public Context {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			ContextSwitcher();

			/*!
			 *	@brief		Destructor.
			 */
			virtual ~ContextSwitcher();

			/*!
			 *	@brief		Add a context and it's "hotkey"
			 *
			 *	@param		context		The context to add.
			 *	@param		key			The key which activates the context.
			 */
			void addContext( Context * context, SDLKey key );

			/*!
			 *	@brief		Switch to the indicated context.
			 *
			 *	@param		context		The newly active context.
			 *	@returns	A boolean reporting if a change is actually made (true)
			 *				or not (false).
			 */
			bool switchContexts( Context * context );

			/*!
			 *	@brief		Allow the context to update its time-dependent state to
			 *				the given global time.
			 */
			virtual void update();

			/*!
			 *	@brief		The draw function for the context.
			 *
			 *	@param		vWidth		The width of the viewport (in pixels).
			 *	@param		vHeight		The height of the viewport (in pixels).
			 */
			virtual void drawGL( int vWidth, int vHeight );
			
			/*!
			 *	@brief		Performs selection based on a click on screen space.
			 *				Uses the OpenGL selection mechanism.
			 *
			 *	@param		scene			The scene to select in.
			 *	@param		camera			The camera.
			 *	@param		vWidth			The width of the viewport.
			 *	@param		vHeight			The height of the viewport.
			 *	@param		selectPoint		The point (in screen space) at which object selection
			 *								should take place.
			 *	@returns	A boolean indicating whether a redraw needs to take place.
			 */
			virtual bool selectGL( const GLScene * scene, const GLCamera & camera, int vWidth, int vHeight, int * selectPoint );

			/*!
			 *	@brief		Callback for when the OpenGL context is changed.
			 */
			virtual void newGLContext();
		
			/*!
			 *	@brief		Give the context the opportunity to respond to a mouse
			 *				event.
			 *
			 *	@param		e		The SDL event with the mouse event data.
			 *	@returns	A ContextResult instance reporting if the event was handled and
			 *				if redrawing is necessary.
			 */
			virtual ContextResult handleMouse( SDL_Event & e );
			
			/*!
			 *	@brief		Give the context the opportunity to respond to a keyboard
			 *				event.
			 *
			 *	@param		e		The SDL event with the keyboard event data.
			 *	@returns	A ContextResult instance reporting if the event was handled and
			 *				if redrawing is necessary.
			 */
			virtual ContextResult handleKeyboard( SDL_Event & e );

		protected:

			/*!
			 *	@brief		The currently active context
			 */
			Context *	_activeContext;

			/*! 
			 *	@brief		A mapping for key to context.
			 */
			KeyContextMap	_contexts;
		};
	}	// namespace SceneGraph
}	// namespace Menge

#endif	// __CONTEXT_SWITCHER_H__