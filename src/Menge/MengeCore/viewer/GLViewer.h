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
 *	@file		GLViewer.h
 *	@brief		Specification for an OpenGL based 3D viewer.
 *
 *				Can draw a SceneGraph GLScene, perform view
 *				manipulation and overlay text on the scene.
 */

#ifndef __GLVIEWER_H__
#define __GLVIEWER_H__

#include "SDL/SDL.h"
#include <string>
#include <vector>

#include "CoreConfig.h"
#include "GLCamera.h"
#include "GLGroundPlane.h"
#include "GLScene.h"
#include "Profiler.h"
#include "ScreenGrab.h"

namespace Menge {

	// Forward declarations
	class Image;

	/*! 
	 *	@namespace	Vis
	 *	@brief		The namespace containing visualization functionality for the
	 *				pedestrian simulation.
	 */
	namespace Vis {

		// Forward declarations
		class ViewConfig;
		class Watermark;

		/*!
		 *	@brief		Class for defining an OpenGL Viewer.
		 */
		class MENGE_API GLViewer {
		public:
			/*!
			 *	@brief		Default constructor based on preferred viewer size.
			 *
			 *	@param		width		The width of the view window, in pixels.
			 *	@param		height		The height of the view window, in pixels.
			 */
			GLViewer( int width, int height );

			/*!
			 *	@brief		Constructor based on configuration in a ViewConfig instance.
			 *
			 *	@param		cfg		A fully specified view configuration.
			 */
			GLViewer( ViewConfig & cfg );

			/*!
			 *	@brief		Destructor.
			 */
			~GLViewer(void);

			/*!
			 *	@brief		Runs the main loop -- including advancing the scene
			 *				in time.  Exiting this function means the main event
			 *				loop is no longer running.
			 */
			void run();

			/*!
			 *	@brief		Sets the optional background image to the viewer.
			 *
			 *	@param		imgName		A valid path to a valid image to load.
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			bool setBGImage( const std::string & imgName );

			/*!
			 *	@brief		Reports if the viewer has a background image.
			 *
			 *	@returns	True if there is a background image, false otherwise.
			 */
			bool hasBGImage() const { return _bgImg != 0x0; }

			/*!
			 *	@brief		Sets the watermark to the viewer.
			 *
			 *	@param		imgName		A valid path to a valid image to load.
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			bool setWatermark( const std::string & imgName );

			/*!
			 *	@brief		Reports if the viewer has a watermark.
			 *
			 *	@returns	True if there is a watermark, false otherwise.
			 */
			bool hasWatermark() const { return _waterMark != 0x0; }

			/*!
			 *	@brief		Clears the background image from the viewer.
			 */
			void clearBGImage();

			/*!
			 *	@brief		Initializes the viewer with the given title.
			 *
			 *	@param		title		The name displayed at the top of the window.
			 *	@returns	True if initialization was successful, false otherwise.
			 */
			bool initViewer( const std::string & title );

			/*!
			 *	@brief		Resizes the window to the indicated size.
			 *
			 *	@param		w		The new width of the viewer window.
			 *	@param		h		The new height of the viewer window.
			 */
			void resizeGL( int w, int h );

			/*!
			 *	@brief		Initializes the opengl mechanism.
			 */
			void initGL();

			/*!
			 *	@brief		Sets the GLScene to be drawn.
			 *				A viewer can only draw a single scene; all drawn 
			 *				elements belong either to the GLScene or a GLContext.
			 *
			 *	@param		scene	The GLScene to be rendered and temporally advanced.
			 */
			void setScene( SceneGraph::GLScene * scene );

			/*!
			 *	@brief		Causes the viewer to recognize that a new OpenGL context has been
			 *				created (such as window resizes).
			 */
			void newGLContext();

			/*!
			 *	@brief		Main drawing function -- draws scene and context as available.
			 */
			void drawGL();

			/*!
			 *	@brief		Processes a mouse event.
			 *
			 *	@param		e		An SDL_Event corresponding with an appropriate SDL mouse event (button up/down, move).
			 *	@returns	A boolean indicating if a scene redraw is necessary (true)
			 *				or not (false).
			 */
			bool handleMouse( SDL_Event & e );
			
			/*!
			 *	@brief		Processes a keyboard event.
			 *
			 *	@param		e		An SDL_Event corresponding with an appropriate SDL keyboard event (key up/down).
			 *	@returns	A boolean indicating if a scene redraw is necessary (true)
			 *				or not (false).
			 */
			bool handleKeyboard( SDL_Event & e );

			/*!
			 *	@brief		Sets whether the world axis is drawn or not.
			 *
			 *	@param		state	A boolean controlling whether the world axis is drawn (true)
			 *						or not (false).
			 */
			void setDrawWorldAxis( bool state ) { _drawWorldAxis = state; }

			/*!
			 *	@brief		Reports the state of whether the world axis is drawn or not.
			 *
			 *	@returns	A boolean indicating if the world axis is drawn (true) or not (false).
			 */
			bool getDrawWorldAxis() const { return _drawWorldAxis; }

			/*!
			 *	@brief		Draws a red frame around the viewer, indicating that images
			 *				are being saved.
			 */
			void drawRecordingFrame() const;

			/*!
			 *	@brief		Sets the horizontal field of view for the ith camera.
			 *
			 *	@param		i		The index of the camera - index value is NOT checked.
			 *	@param		fov		The horizontal field of view for the indicated camera.
			 *						Value is not validated.
			 */
			void setCameraFOV( int i, float fov ) { _cameras[i].setFOV( fov ); }

			/*!
			 *	@brief		Sets the distance to the camera's far clipping plane.
			 *
			 *	@param		i		The index of the camera - index value is NOT checked.
			 *	@param		dist	The distance, from the camera, to its far clipping plane.
			 *						Value is not validated.
			 */
			void setCameraFarPlane( int i, float dist ) { _cameras[i].setFarPlane( dist ); }

			/*!
			 *	@brief		Change the scene time by the given offset.
			 *	
			 *	@param		delta	The amount of time change.  Can be negative, but only 
			 *						meaningful if the GLScene can support negative time changes.
			 */
			void offsetTime( float delta );
			/*!
			 *	@brief		Sets the current time.
			 *
			 *	@param		t		The current time for the viewer and its GLScene.
			 */
			void setTime( float t ) { _viewTime = t; };

			/*!
			 *	@brief		Sets the view's background color.
			 *				This must be called AFTER the view has been initialized.
			 *
			 *	@param		r		The red component of the background color (must be in the range [0, 1]).
			 *	@param		g		The green component of the background color (must be in the range [0, 1]).
			 *	@param		b		The blue component of the background color (must be in the range [0, 1]).
			 */
			void setBGColor( float r, float g, float b );

			/*!
			 *	@brief		Gets a pointer to the ith camera.
			 *
			 *	@param		i		The index of the desired camera.  The index value
			 *						is not checked.
			 *	@returns	A pointer to the ith camera.
			 */
			SceneGraph::GLCamera * getCamera( int i=0 ) { return &_cameras[i]; }

			/*!
			 *	@brief		Sets the simulator to use a fixed time step, with the given value.
			 *
			 *	@param		stepSize		The size of the fixed step the viewer should advance
			 *								its GLScene.
			 */
			void setFixedStep( float stepSize );
			
			/*!
			 *	@brief		Sets the path for when outputting screen capture images.
			 *
			 *	@param		path		The path to which output images should be written.
			 */
			void setDumpPath( const std::string & path );

		protected:
			/*!
			 *	@brief		The various SDL flags which determine the view.
			 */
			static Uint32 FLAGS;

			////////////////////////////////////////////////////////////////
			// members

			/*!
			 *	@brief		The width of the viewer window (in pixels).
			 */
			int	_width;
			
			/*!
			 *	@brief		The height of the viewer window (in pixels).
			 */
			int _height;

			/*!
			 *	@brief		The GLScene to draw.
			 */
			SceneGraph::GLScene *	_scene;

			/*!
			 *	@brief		A set of cameras from which to draw the scene.
			 */
			std::vector< SceneGraph::GLCamera > _cameras;

			/*!
			 *	@brief		The index of camera currently being used to draw the scene.
			 */
			size_t		_currCam;

			/*!
			 *	@brief		The x-position of the mouse when a
			 *				mouse button was depressed (in screen space).
			 */
			int _downX;			
			
			/*!
			 *	@brief		The y-position of the mouse when a
			 *				mouse button was depressed (in screen space).
			 */
			int _downY;			

			// HUD/UI components

			/*!
			 *	@brief		Determines if the GLScene needs to update.
			 */
			bool	_update;

			/*!
			 *	@brief		Determines whether a world-aligned axis is drawn
			 */
			bool	_drawWorldAxis;

			/*!
			 *	@brief		Determines whether the frame rate is displayed in the viewer.
			 */
			bool	_showFPS;

			/*!
			 *	@brief		A timer to determine the displayed frame rate value.
			 */
			SampleTimer	_fpsDisplayTimer;			

			/*!
			 *	@brief		Controls whether the viewer advances the GLScene (true) or not (false).
			 */
			bool	_pause;

			/*!
			 *	@brief		Determines if the viewer should still operate -- as long as it is true, it will
			 *				continue its main loop (@see GLViewer::run).  
			 */
			bool _running;

			/*!
			 *	@brief		The current time at which the viewer is running.  Modified by calls to GLViewer::setTime and
			 *				GLViewer::offsetTime.
			 */
			float _viewTime;

			/*!
			 *	@brief		The viewer's background color.
			 */
			Vector3	_bgColor;

			/*!
			 *	@brief		The step size for fixed-step simulation.
			 */
			float	_stepSize;

			/*! 
			 *	@brief		A pointer to the optional background image.
			 */
			Image	*	_bgImg;

			/*!
			 *	@brief		An optional watermark.
			 */
			Watermark	* _waterMark;

			/*!
			 *	@brief		Determines if screen capture images will be output at each time advance.
			 */
			bool	_dumpImages;

			/*!
			 *	@brief		Reports if the screen capture output path is valid.
			 */
			bool	_validDumpPath;

			/*!
			 *	@brief		The path to the folder in which to write screen capture images.
			 */
			std::string _dumpPath;

			/*!
			 *	@brief		The number of images which have been output.  Used for 
			 *				labeling the image file names into a sequential order.
			 */
			size_t _imgCount;

			/*!
			 *	@brief		The set of lights to use in rendering.
			 */
			std::vector< SceneGraph::GLLight > _lights;

			/*!
			 *	@brief		Initizlies the OpenGL lighting based on the set of lights.
			 */
			void initLighting();

			////////////////////////////////////////////////////////////////
			// methods

			/*!
			 *	@brief		Initializes the SDL mechanism for event handling and context creation.
			 */
			bool initSDL( const std::string & title );

			/*!
			 *	@brief		Draws a simple, three-color world axis at the origin of world space.
			 */
			void drawWorldAxis() const;
		};
	}	// namespace Vis
}	// namespace Menge
#endif  // __GLVIEWER_H__
