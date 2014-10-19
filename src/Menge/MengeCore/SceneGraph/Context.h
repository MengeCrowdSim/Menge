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
 *	@file	Context.h
 *	@brief	Defines a context for handling user interaction (mouse and keyboard input).
 */

#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "CoreConfig.h"
//#include "SDL/SDL.h"
#include "GLCamera.h"
#include "TextWriter.h"

// forward declaration
union SDL_Event;

/*!
 *	@brief		Forward declaration of the SDL event type.
 */
typedef SDL_Event SDL_Event;

namespace Menge {

	/*!
	 * @namespace SceneGraph
	 * @brief	The SceneGraph (SceneGraph) name space, containing all elements to use in a SceneGraph.
	 */
	namespace SceneGraph {
		class Context;
		class GLScene;

		// TODO: decouple this from SDL events
		/*!
		 *	@brief		Defines the result of the context's consideration of user input.
		 */
		class MENGE_API ContextResult {
		public:
			/*!
			 *	@brief		Constructor
			 *
			 *	@param		handled			Set to true if the event has been handled and does
			 *								not need to be considered by any other handler.
			 *	@param		needsRedraw		Set to true if the processing of the event requires a redraw.
			 */
			ContextResult( bool handled, bool needsRedraw ): _handled(handled), _redraw(needsRedraw){}

			/*!
			 *	@brief		Simple assignment operator overloaded.
			 *
			 *	@param		res		Const reference to the ContextResult instance to copy from.
			 *	@returns	A reference to this context result.
			 */
			ContextResult & operator=( const ContextResult & res ) { _handled=res._handled; _redraw=res._redraw; return *this; }

			/*!
			 *	@brief		Sets the "handled" state to the given boolean state.
			 *
			 *	@param		state		True means the result should consider the event handled, otherwise false.
			 */
			inline void setHandled( bool state ) { _handled = state; }

			/*!
			 *	@brief		Reports if the result considers the event handled.
			 *
			 *	@returns	True if the result considers the event handled, false otherwise.
			 */
			inline bool isHandled() const { return _handled; }
			
			/*!
			 *	@brief		Sets the "needs redraw" state to the given boolean state.
			 *
			 *	@param		state		True means the result should believe the event requires a redraw, otherwise false.
			 */
			inline void setNeedsRedraw( bool state ) { _redraw = state; }
			
			/*!
			 *	@brief		Reports if the result believes the event handling requires a redraw.
			 *
			 *	@returns	True if the result requires a redraw, false otherwise.
			 */
			inline bool needsRedraw() const { return _redraw; }

			/*!
			 *	@brief		Sets the handled and needs redraw state simultaneously.
			 *
			 *	@param		handled		True if the result should consider the event handled, false otherwise.
			 *	@param		redraw		True means the result should believe the event requires a redraw, otherwise false.
			 */
			inline void set( bool handled, bool redraw ) { _handled = handled; _redraw = redraw; }

			/*!
			 *	@brief		Combines the provided context result with this one; flags are combined with a boolean OR.
			 *
			 *	@param		res		The context results to combine with this.
			 */
			inline void combine( const ContextResult & res ) { _handled = _handled || res._handled; _redraw = _redraw || res._redraw; }
			
			friend class Context;
		protected:
			/*!
			 *	@brief		Reports if the event has been handled (and no one else needs to worry about it).
			 */
			bool _handled;

			/*!
			 *	@brief		Reports if the event requires a redraw on the scene.
			 */
			bool _redraw;
		};

		/*!
		 *	@brief		The base context class for defining a how events are handled.
		 *				It also is responsible drawing UI elements in both screen space
		 *				and in viewer world space.  While the main viewer handles 
		 *				basic view manipulation, all other interactions should be handled
		 *				with a context.
		 */
		class MENGE_API Context {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			Context(){}

			/*!
			 *	@brief		Virtual destructor.
			 */
			virtual ~Context(){}

			/*!
			 *	@brief		The draw function for the context.
			 *
			 *	@param		vWidth		The width of the viewport (in pixels).
			 *	@param		vHeight		The height of the viewport (in pixels).
			 */
			virtual void drawGL( int vWidth, int vHeight ){}
			
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
			virtual bool selectGL( const GLScene * scene, const GLCamera & camera, int vWidth, int vHeight, int * selectPoint ) { return false; }

			/*!
			 *	@brief		Give the context the opportunity to respond to a mouse
			 *				event.
			 *
			 *	@param		e		The SDL event with the mouse event data.
			 *	@returns	A ContextResult instance reporting if the event was handled and
			 *				if redrawing is necessary.
			 */
			virtual ContextResult handleMouse( SDL_Event & e ) { return ContextResult( false, false ); }
			
			/*!
			 *	@brief		Give the context the opportunity to respond to a keyboard
			 *				event.
			 *
			 *	@param		e		The SDL event with the keyboard event data.
			 *	@returns	A ContextResult instance reporting if the event was handled and
			 *				if redrawing is necessary.
			 */
			virtual ContextResult handleKeyboard( SDL_Event & e ) { return ContextResult( false, false ); }

			/*!
			 *	@brief		Allow the context to update any time-dependent state it might have to
			 *				the given global time.
			 */
			virtual void update() {} 

			/*!
			 *	@brief		Callback for when the OpenGL context is changed.
			 */
			virtual void newGLContext() {}

			/*!
			 *	@brief		Called when the context is activated.
			 */
			virtual void activate() {}

			/*!
			 *	@brief		Called when the context is deactivated.
			 */
			virtual void deactivate() {}

		protected:
			/*!
			 *	@brief		UI setup - handles the configuration of the opengl context 
			 *				for orthographic rendering.
			 *
			 *	@param		vWidth		The width of the view (in pixels).
			 *	@param		vHeight		The height of the view (in pixels).
			 */
			void uiSetup( int vWidth, int vHeight ); 

			/*!
			 *	@brief		Cleans up the OpenGL state after having drawn the UI elements of
			 *				the context.
			 */
			void uiShutdown();

			/*!
			 *	@brief		Draw UI elements into the context.
			 *
			 *	@param		vWidth		The width of the viewport (in pixels).
			 *	@param		vHeight		The height of the viewport (in pixels).
			 *	@param		select		Defines if the drawing is being done for selection
			 *							purposes (true) or visualization (false).
			 */
			virtual void drawUIGL( int vWidth, int vHeight, bool select=false ) {};

			/*!
			 *	@brief		Draw context elements into the 3D world.
			 *
			 *	@param		select		Defines if the drawing is being done for selection
			 *							purposes (true) or visualization (false).
			 */
			virtual void draw3DGL( bool select=false ) {};

			/*!
			 *	@brief		Updates the various view matrices: modelview,
			 *				projection, and view.
			 */
			void getOpenGLView();

			/*!
			 *	@brief		Writes the given text at the given location
			 *	@param		txt			The text to write to the screen.
			 *	@param		pos			The position, in world coordinates, to place the text
			 *	@param		currColor	Indicates if the current opengl color should be used (true) or the
			 *							text writer's default color should be used (false).
			 */
			void writeText( const std::string & txt, const Vector2 & pos, bool currColor=false ) const;

			/*!
			 *	@brief		Writes the given text at the given 3D location
			 *	@param		txt			The text to write to the screen.
			 *	@param		pos			The position, in world coordinates, to place the text
			 *	@param		currColor	Indicates if the current opengl color should be used (true) or the
			 *							text writer's default color should be used (false).
			 */
			void writeText( const std::string & txt, const Vector3 & pos, bool currColor=false ) const;

			/*!
			 *	@brief		Writes the given text at the given location, but anchored
			 *				based on the radial direction given.  
			 *	@param		txt			The text to write to the screen.
			 *	@param		pos			The position, in world coordinates, to place the text
			 *	@param		dir			A vector representing the direction of the radius at whose
			 *							end the text should be placed.  Direction defined in
			 *							world coordinates
			 *	@param		currColor	Indicates if the current opengl color should be used (true) or the
			 *							text writer's default color should be used (false).
			 */
			void writeTextRadially( const std::string & txt, const Vector2 & pos, const Vector2 & dir, bool currColor=false ) const;

			/*!
			 *	@brief		Writes the given text aligned to the given location
			 *	@param		txt			The text to write to the screen.
			 *	@param		pos			The position, in world coordinates, to place the text
			 *	@param		align		TextWriter alignment specification.
			 *	@param		currColor	Indicates if the current opengl color should be used (true) or the
			 *							text writer's default color should be used (false).
			 */
			void writeAlignedText( const std::string & txt, const Vector2 & pos, TextWriter::Alignment align, bool currColor=false ) const;

			/*!
			 *	@brief		Writes the given text to the screen based on the given alignment.
			 *
			 *	The text will be written to one of 9 locations:
			 *		- Top left corner, top center, top right corner
			 *		- left center, screen center, right center
			 *		- bottom left corner, bottom center, bottom right corner
			 *
			 *	@param		txt			The text to write to the screen.
			 *	@param		align		TextWriter alignment specification.
			 *	@param		fontSize	The size of the text to write.
			 *	@param		hPad		The number of pixels to pad the text horizontally.
			 *	@param		vPad		The number of pixels to pad the text vertically.
			 *	@param		currColor	Indicates if the current opengl color should be used (true) or the
			 *							text writer's default color should be used (false).
			 *	@param		trans		Determines if the text is printed transparently (true)
			 *							or not (false).
			 */
			void writeToScreen( const std::string & txt, TextWriter::Alignment align, int fontSize, float hPad=0.f, float vPad=0.f, bool currColor=false, bool trans=true ) const;

			/*!
			 *	@brief		The current model view matrix
			 */
			double _modViewMat[16];
			
			/*!
			 *	@brief		The current projection matrix
			 */
			double _projMat[16];
			
			/*!
			 *	@brief		The current viewport parameters
			 */
			int _viewMat[4];	
		};

		/*!
		 *	@brief		A context that performs object selection by directly clicking on selectable entities
		 *				in the OpenGL context.
		 */
		class MENGE_API SelectContext : public Context {
		public:
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
		};

	}	// namespace SceneGraph
}	// namespace Menge
#endif // __CONTEXT_H__
