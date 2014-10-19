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


#ifdef _MSC_VER
#include "windows.h"
#endif

// Viewer
#include "GLViewer.h"
#include "ViewConfig.h"
#include "ScreenGrab.h"
#include "Profiler.h"
#include "Watermark.h"

// menge runtime
#include "os.h"

// Scene graph
#include "Select.h"
#include "Context.h"
#include "TextWriter.h"
#include "XformMatrix.h"
#include "GLContextManager.h"
#include "image.h"
#include "shapes.h"

// standard library
#include <iostream>
#include <sstream>
#include <iomanip>

namespace Menge {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	std::string useMessage("\nViewer controls:\n" \
						   "  Quit the program ............ Ctrl + q\n"\
						   "  Camera controls:\n" \
						   "    Ctrl + Left Mouse ......... ROTATION\n" \
						   "    Ctrl + Shift + Left Mouse.. MOVE\n" \
						   "    Shift + Left Mouse ........ ZOOM\n" \
						   "    'o' ....................... Orthographic view\n" \
						   "    'p' ....................... Perspective view\n" \
						   "    'x' ....................... Look along x-axis\n" \
						   "    Shift + 'x'................ Look along negative x-axis\n"\
						   "    'y' ....................... Look along y-axis\n" \
						   "    Shift + 'y'................ Look along negative y-axis\n"\
						   "    'z' ....................... Look along z-axis\n" \
						   "    Shift + 'z'................ Look along negative z-axis\n"\
						   "    Ctrl + 'c'................. Display camera parameters to the console\n"\
						   "    Ctrl + '1'-'9'............. Switch cameras (if a camera exists)\n"\
						   "  Playback:\n" \
						   "    spacebar .................. Stop/start playback\n" \
						   "    RIGHT arrow ............... Advance one frame (when stopped)\n" \
						   "  Output images ............... Ctrl + o\n"\
						   );

	size_t FULL_FRAME = 0;
	size_t SCENE_UPDATE = 0;
	size_t FULL_DRAW = 0;
	size_t BUFFER_SWAP = 0;

#endif	// DOXYGEN_SHOULD_SKIP_THIS

	namespace Vis {

		///////////////////////////////////////////////////////////////////////////
		//				IMPLEMENTATION FOR GLViewer
		///////////////////////////////////////////////////////////////////////////

		Uint32 GLViewer::FLAGS = SDL_OPENGL | SDL_DOUBLEBUF | SDL_RESIZABLE;

		///////////////////////////////////////////////////////////////////////////

		GLViewer::GLViewer( int width, int height ): _width( width ), _height( height ), _scene( 0 ), _downX( 0 ), 
													 _downY(0), _update(false), _drawWorldAxis(false), 
													 _showFPS(false), _fpsDisplayTimer(10), _pause(true), _running(true),
													 _viewTime(0.f),
													 _bgColor(0.74f, 0.69f, 0.64f), _stepSize(1 / 120.f), _bgImg(0x0),
													 _dumpImages(false), _validDumpPath(true), _dumpPath( ".\\" ), _imgCount(0)
		{
			ImageData::start();
			FULL_FRAME = addTimer( "Full frame            " );
			SCENE_UPDATE = addTimer( "  scene update        " );
			FULL_DRAW = addTimer( "  scene draw          " );
			BUFFER_SWAP = addTimer( "  buffer swap         " );
		}

		///////////////////////////////////////////////////////////////////////////

		GLViewer::GLViewer( ViewConfig & cfg ): _width(cfg._width), _height(cfg._height), _scene(0x0), _downX(0), 
													 _downY(0), _update(false), _drawWorldAxis(false), 
													 _showFPS(false), _fpsDisplayTimer(10), _pause(true), _running(true),
													 _viewTime(0.f),
													 _bgColor(0.74f, 0.69f, 0.64f), _stepSize(1 / 120.f), _bgImg(0x0),
													 _dumpImages(false), _validDumpPath(true), _dumpPath( ".\\" ), _imgCount(0)
		{
			ImageData::start();
			FULL_FRAME = addTimer( "Full frame            " );
			SCENE_UPDATE = addTimer( "  scene update        " );
			FULL_DRAW = addTimer( "  scene draw          " );
			BUFFER_SWAP = addTimer( "  buffer swap         " );

			// initialize from config
			cfg.setCameras( _cameras );
			cfg.setLights( _lights );
			// TODO: Set this in the view config file
			SceneGraph::TextWriter::Instance()->setDefaultColor( cfg._fontColor[0], cfg._fontColor[1], cfg._fontColor[2], cfg._fontColor[3] );
			SceneGraph::TextWriter::Instance()->setFont( cfg._fontName );
			_currCam = 0;
			
			if ( cfg._bgImg != "" ) {
				if ( ! setBGImage( cfg._bgImg ) ) {
					logger << Logger::ERR_MSG << "Unable to load background image: " << cfg._bgImg << "\n";
				}
			}
			_waterMark = cfg._waterMark;
			cfg._waterMark = 0x0;
		}

		///////////////////////////////////////////////////////////////////////////

		GLViewer::~GLViewer() {
			if ( _scene != 0x0 ) {
				//delete _scene;	TODO: Figure out why navigation mesh causes problems here
				_scene = 0x0;
			}
			
			if ( _bgImg != 0x0 ) {
				delete _bgImg;
			}
			SDL_Quit();
		}

		///////////////////////////////////////////////////////////////////////////

		bool GLViewer::initViewer( const std::string & title ) {
			if ( ! initSDL( title ) ) return false;
			initGL();
			SceneGraph::initShapes();
			resizeGL( _width, _height );

			return true;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLViewer::run() {
			bool redraw = true;
			float time = 0.f;
			_fpsDisplayTimer.start();

			while ( _running ) {
				SDL_Event e;
				while ( SDL_PollEvent( &e ) ) {
					if( e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
						redraw = handleKeyboard( e ) || redraw;
					} else if( e.type == SDL_QUIT ) {
						_running = false;
						redraw = false;
						break;
					} else if( e.type == SDL_VIDEOEXPOSE ) {
						redraw = true;
					} else if( e.type == SDL_MOUSEMOTION || 
							   e.type == SDL_MOUSEBUTTONDOWN || 
							   e.type == SDL_MOUSEBUTTONUP ) {
						redraw = handleMouse( e )  || redraw;
					} else if ( e.type == SDL_VIDEORESIZE ) {
						resizeGL( e.resize.w, e.resize.h );
						redraw = true;
					}
				}
				if ( !_pause ) startTimer( FULL_FRAME );
				if ( redraw || _update || !_pause ) {
					
					// draw stuff
					if ( _scene && ( !_pause || _update ) ) {
						startTimer( SCENE_UPDATE );
						try {
							redraw = _scene->updateScene( _viewTime );
						} catch ( SceneGraph::SystemStopException ) {
							// When a system sends an exception that things are over
							// pause everything
							std::cout << "System stopped!\n";
							_pause = true;	
						}
						if (!_update ) {
							 offsetTime( _stepSize );
						}
						stopTimer( SCENE_UPDATE );	
					}
					if ( redraw ) {
						startTimer( FULL_DRAW );
						drawGL();			
						stopTimer( FULL_DRAW );

						startTimer( BUFFER_SWAP );
						SDL_GL_SwapBuffers();
						stopTimer( BUFFER_SWAP );
						redraw = false;
					}
					if ( !_pause ) lapTimer( FULL_FRAME );			
				}

				if ( ( !_pause || _update ) && _dumpImages ) {
					std::stringstream fullPath;				
					fullPath << _dumpPath << "img";
					fullPath << std::setfill( '0' ) << std::setw( 6 ) << ++_imgCount << ".png";
					snapshotPNG( _width, _height, fullPath.str().c_str() );
				}
				_update = false;
			}
			printAverages();

			_scene->finish();
		}

		///////////////////////////////////////////////////////////////////////////

		bool GLViewer::setBGImage( const std::string & imgName ) {
			_bgImg = loadImage( imgName );
			return _bgImg != 0x0;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLViewer::clearBGImage() {
			if ( _bgImg != 0x0 ) {
				delete _bgImg;
				_bgImg = 0x0;
			}
		}

		///////////////////////////////////////////////////////////////////////////

		bool GLViewer::initSDL( const std::string & title ) {
			if( SDL_Init( SDL_INIT_VIDEO ) != 0 ) {
				logger << Logger::ERR_MSG << "Unable to initialize SDL: " << SDL_GetError();
				SDL_Delay( 3000 );
				return false;
			}

			SDL_WM_SetCaption( title.c_str(), 0x0 );
	#if _MSC_VER <= 1400
			SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
	#endif
			SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
			SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32 );
			SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
			SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
			SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );
			
			if ( !SDL_SetVideoMode( _width, _height, 32, FLAGS ) ) {
				logger << Logger::ERR_MSG << "Unable to set video mode: " << SDL_GetError();
				SDL_Delay( 3000 );
				SDL_Quit();
				return false;
			}
			SDL_EnableKeyRepeat( 100, 33 ); // start repeat after tenth of a second and signal at 30 Hz

			return true;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLViewer::resizeGL( int w, int h ) {
			_width = w;
			_height = h;
			if ( _height == 0 ) {
				_height = 1;
			}
			SDL_SetVideoMode( _width, _height, 32, FLAGS );

			// Inform all dependent entities that the context has changed
			newGLContext();
		}

		///////////////////////////////////////////////////////////////////////////

		void GLViewer::setScene( SceneGraph::GLScene * scene ) {
			if ( _scene && scene != _scene ) {
				delete _scene;
			}
			_scene = scene;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLViewer::newGLContext() {
			SceneGraph::TextWriter::Instance()->resize( _width, _height );
			SceneGraph::TextWriter::Instance()->newGLContext();
			initGL();
			GLContextManager::newGLContext();
			if ( _scene ) {
				_scene->newGLContext();
			}	
			// Re-initialize the cameras
			for ( size_t i = 0; i < _cameras.size(); ++i ) {
				_cameras[i].setViewport( _width, _height );
			}
		}

		///////////////////////////////////////////////////////////////////////////

		void GLViewer::initGL() {
			glEnable( GL_NORMALIZE );
			glShadeModel( GL_SMOOTH );
			glClearColor( _bgColor.x(), _bgColor.y(), _bgColor.z(), 0.f );
			glClearDepth( 1.f );
			glEnable(GL_DEPTH_TEST);
			if ( _lights.size() > 0 ) {
				initLighting();
			} else {
				glDisable( GL_LIGHTING );
			}

			glEnable( GL_COLOR_MATERIAL );
		}

		///////////////////////////////////////////////////////////////////////////

		void GLViewer::drawGL() {
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			if ( _bgImg ) {
				glPushAttrib( GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT );
				glDisable( GL_LIGHTING );
				glDepthMask( GL_FALSE );
				glMatrixMode( GL_PROJECTION );
				glPushMatrix();
				glLoadIdentity();
				glOrtho( 0.0, _width, 0.0, _height, -1.0f, 1.0f );
				glMatrixMode( GL_MODELVIEW );
				glPushMatrix();
				glLoadIdentity();

				_bgImg->bind();
				glColor3f( 1.f, 1.f, 1.f );
				glBegin( GL_QUADS );
				glTexCoord2f( 0.f, 1.f );
				glVertex3f( 0.f, 0.f, 0.f );
				glTexCoord2f( 1.f, 1.f );
				glVertex3f( (float)_width, 0.f, 0.f );
				glTexCoord2f( 1.f, 0.f );
				glVertex3f( (float)_width, (float)_height, 0.f );
				glTexCoord2f( 0.f, 0.f );
				glVertex3f( 0.f, (float)_height, 0.f );
				glEnd();

				glPopMatrix();
				glMatrixMode( GL_PROJECTION );
				glPopMatrix();
				glMatrixMode( GL_MODELVIEW );
				glPopAttrib();
			}
			if ( _scene ) {		
				_scene->drawGL( _cameras[ _currCam ], _lights, _width, _height );
			}
			// various view decorations
			// world axis
			if ( _drawWorldAxis ) drawWorldAxis();

			const int STRING_LEN = 100;
			char msg[STRING_LEN];
			// UI messsages
			float time = _fpsDisplayTimer.lap();
			if ( _showFPS ) {		
		#ifdef TIME_CROWD
				sprintf_s( msg, STRING_LEN, "FPS: %.1f    UPDATE TIME: %.1f microsec    SIM TIME: %.2f s", 1.f / time, 1000.f * averageTime(SCENE_UPDATE), _viewTime );
		#else
				sprintf_s( msg, STRING_LEN, "FPS: %.1f    SIM TIME: %.2f s", 1.f / time, _viewTime );
		#endif
				SceneGraph::TextWriter * writer = SceneGraph::TextWriter::Instance();
				writer->printAlignText( std::string(msg), SceneGraph::TextWriter::CENTER_BOTTOM, 15, 5.f, 5.f );
			}
			if ( _waterMark ) {
				_waterMark->drawGL( (float)_width, (float)_height );
			}
			if ( _dumpImages ) {
				drawRecordingFrame();
			}
		}

		///////////////////////////////////////////////////////////////////////////

		void GLViewer::drawRecordingFrame() const {

			GLfloat width = (GLfloat)_width;
			GLfloat height = (GLfloat)_height;
			// set up rendering
			glPushAttrib( GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT );
			glDisable( GL_LIGHTING );
			glDisable( GL_DEPTH_TEST );
			glDepthMask( GL_FALSE );
			glMatrixMode( GL_PROJECTION );
			glPushMatrix();
			glLoadIdentity();
			glOrtho( 0.0, _width, 0.0, _height, -1.0f, 1.0f );
			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();
			glLoadIdentity();

			glColor4f( 1.f, 0.f, 0.f, 1.f );

			glBegin( GL_LINE_STRIP );
			glVertex3f( 1.f, 1.f, 0.f );
			glVertex3f( width, 1.f, 0.f );
			glVertex3f( width, height, 0.f );
			glVertex3f( 1.f, height, 0.f );
			glVertex3f( 1.f, 1.f, 0.f );
			glEnd();

			glPopMatrix();
			glMatrixMode( GL_PROJECTION );
			glPopMatrix();
			glMatrixMode( GL_MODELVIEW );
			glPopAttrib();
		}

		///////////////////////////////////////////////////////////////////////////

		bool GLViewer::handleMouse( SDL_Event & e ) {
			bool redraw = false;
			SceneGraph::ContextResult result( false, false );
			if ( _scene ) {
				result = _scene->handleMouse( e );
				redraw = result.needsRedraw();
			}
			if ( ! result.isHandled() ) {
				SDLMod mods = SDL_GetModState();
				int x = e.button.x;
				int y = e.button.y;

				bool hasCtrl = ( mods & KMOD_CTRL ) > 0;
				bool hasAlt = ( mods & KMOD_ALT ) > 0;
				bool hasShift = ( mods & KMOD_SHIFT ) > 0;

				bool rotate = hasCtrl && !(hasAlt || hasShift );
				bool pan = !hasAlt && hasCtrl && hasShift;
				bool zoom = hasShift && !(hasAlt || hasCtrl );

				if ( e.type == SDL_MOUSEMOTION ) {
					if ( e.button.button == SDL_BUTTON_LEFT ) {
						if ( rotate ) {	// orbit around the camera
							float deltaX = ( x - _downX ) * 0.0075f; // TODO: get rid of this magic number
							float deltaY = ( y - _downY ) * 0.0075f;	// TODO: get rid of this magic number
							_cameras[ _currCam ].orbitHorizontalAxis( deltaY );
							_cameras[ _currCam ].orbitVerticalAxis( -deltaX );
						} else if ( pan ) {
							/// This scale should be the size of a pixel at the target depth
							float deltaX = ( x - _downX ) * 0.0025f;
							_cameras[ _currCam ].truck( -deltaX );
							float deltaY = ( y - _downY ) * 0.0025f;
							_cameras[ _currCam ].crane( deltaY );		
						} else if ( zoom ) {
							const float scale = 1.f / 5.0f;
							float deltaY = ( y - _downY ) * scale;
							_cameras[ _currCam ].zoom( -deltaY );
						}
						_downX = x;
						_downY = y;
						redraw = true;
					}
				} else if ( e.type == SDL_MOUSEBUTTONDOWN ) {
					if ( e.button.button == SDL_BUTTON_LEFT ) {
						_downX = e.button.x;
						_downY = e.button.y;
						if ( !( hasCtrl || hasAlt || hasShift ) && _scene != 0x0 ) {
							// TODO: This won't work when there are widgets
							//		 Would this be better off embedded into the scene?
							//		 After all, a manipulator merely has a special context...
							//		 So, should a context have some kind of drawable function?
							//		
							int selectPoint[2] = { _downX, _downY };
							redraw = _scene->selectGL( _cameras[ _currCam ], _width, _height, &selectPoint[0] );	
						}
					} else if ( e.button.button == SDL_BUTTON_RIGHT ) {
					} else if ( e.button.button == SDL_BUTTON_WHEELUP ) {
						float amount = 0.5;
						if ( hasCtrl ) amount *= 2;
						if ( hasAlt ) amount *= 2;
						if ( hasShift ) amount *= 2;
						_cameras[ _currCam ].zoom( amount );
						redraw = true;
					} else if ( e.button.button == SDL_BUTTON_WHEELDOWN ) {
						float amount = -0.5;
						if ( hasCtrl ) amount *= 2;
						if ( hasAlt ) amount *= 2;
						if ( hasShift ) amount *= 2;
						_cameras[ _currCam ].zoom( amount );
						redraw = true;
					}
				} else if ( e.type == SDL_MOUSEBUTTONUP ) {
					
				} else {
					logger << Logger::ERR_MSG << "handleMouse called with invalid event type: " << (int)e.type << "\n";
				}
			}

			return redraw;
		}

		///////////////////////////////////////////////////////////////////////////

		bool GLViewer::handleKeyboard( SDL_Event & e ) {
			bool redraw = false;
			SceneGraph::ContextResult result( false, false );
			if ( _scene ) {
				result = _scene->handleKeyboard( e );
				redraw = result.needsRedraw();
			}

			if ( ! result.isHandled() ) {
				SDLMod mods = SDL_GetModState();
				bool hasCtrl = ( mods & KMOD_CTRL ) > 0;
				bool hasAlt = ( mods & KMOD_ALT ) > 0;
				bool hasShift = ( mods & KMOD_SHIFT ) > 0;
				bool noMods = !(hasCtrl || hasAlt || hasShift );
				if ( e.key.type == SDL_KEYDOWN ) {
					if ( noMods ) {
						if ( e.key.keysym.sym == SDLK_f ) {
							_showFPS = !_showFPS;
							redraw = true;
						} else if ( e.key.keysym.sym == SDLK_h ) {
							std::cout << useMessage;
						} else if ( e.key.keysym.sym == SDLK_a ) {
							_drawWorldAxis = !_drawWorldAxis;
							redraw = true;
						} else if ( e.key.keysym.sym == SDLK_SPACE ) {
							_pause = !_pause;
							redraw = true;
						} else if ( e.key.keysym.sym == SDLK_RIGHT ) {
							if ( _pause ) {
								offsetTime( _stepSize );
								redraw = _update = true;
							}
						} else if ( e.key.keysym.sym == SDLK_x ) {
							_cameras[ _currCam ].viewXAxis();
							redraw = true;
						} else if ( e.key.keysym.sym == SDLK_y ) {
							_cameras[ _currCam ].viewYAxis();
							redraw = true;
						} else if ( e.key.keysym.sym == SDLK_z ) {
							_cameras[ _currCam ].viewZAxis();
							redraw = true;
						} else if ( e.key.keysym.sym >= SDLK_1 && e.key.keysym.sym <= SDLK_9 ) {
							size_t camIndex = (size_t)e.key.keysym.sym - (size_t)SDLK_1;
							assert( camIndex <= 8 && "Invalid camera index!\n" );
							if ( camIndex < _cameras.size() && _currCam != camIndex ) {
								_currCam = camIndex;
								_cameras[ _currCam ].setActive();
								redraw = true;
							}
						} else if ( e.key.keysym.sym == SDLK_o ) {
							_cameras[ _currCam ].setOrtho();
							redraw = true;
						} else if ( e.key.keysym.sym == SDLK_p ) {
							_cameras[ _currCam ].setPersp();
							redraw = true;
						}
					} else if ( hasCtrl && !hasShift && !hasAlt) {
						if ( e.key.keysym.sym == SDLK_q ) {
							_running = false;
						} else if ( e.key.keysym.sym == SDLK_c ) {
							std::cout << "View:\n";
							std::cout << "\t( " << _width << ", " << _height << " )\n";
							_cameras[ _currCam ].outputState();
						} 
						else if ( e.key.keysym.sym == SDLK_o ) {
							_dumpImages = !_dumpImages && _validDumpPath;
							redraw = true;
						}
					} else if ( !hasCtrl && hasShift && !hasAlt ) {
						if ( e.key.keysym.sym == SDLK_x ) {
							_cameras[ _currCam ].viewXAxis( false );
							redraw = true;
						} else if ( e.key.keysym.sym == SDLK_y ) {
							_cameras[ _currCam ].viewYAxis( false );
							redraw = true;
						} else if ( e.key.keysym.sym == SDLK_z ) {
							_cameras[ _currCam ].viewZAxis( false );
							redraw = true;
						}
					}
				}
			}
			
			return redraw;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLViewer::drawWorldAxis() const { 
			// NOTE: This doesn't GUARANTEE that it's being drawn in the correct space
			//		It assumes that the modelview matrix is the camera matrix
			glPushAttrib( GL_LINE_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT );
			glDisable( GL_LIGHTING );
			glDisable( GL_DEPTH_TEST );
			glLineWidth( 2.0f );
			const float AXIS_SCALE = 4.f;
			glBegin( GL_LINES );
			glColor3f( 1.f, 0.f, 0.f );
			glVertex3f( 0.f, 0.f, 0.f );
			glVertex3f( AXIS_SCALE, 0.f, 0.f );
			glColor3f( 0.f, 1.f, 0.f );
			glVertex3f( 0.f, 0.f, 0.f );
			glVertex3f( 0.f, AXIS_SCALE, 0.f );
			glColor3f( 0.f, 0.f, 1.f );
			glVertex3f( 0.f, 0.f, 0.f );
			glVertex3f( 0.f, 0.f, AXIS_SCALE );
			glEnd();
			glPopAttrib();
		}

		///////////////////////////////////////////////////////////////////////////

		void GLViewer::offsetTime( float delta ) {
			setTime( _viewTime + delta );
		}

		///////////////////////////////////////////////////////////////////////////

		void GLViewer::setBGColor( float r, float g, float b ) {
			_bgColor.set( r, g, b );
			glClearColor( r, g, b, 1.f );
		}

		///////////////////////////////////////////////////////////////////////////

		void GLViewer::setFixedStep( float stepSize ) {
			_stepSize = stepSize;
		}

		///////////////////////////////////////////////////////////////////////////

		void GLViewer::initLighting() {
			glEnable( GL_LIGHTING );
			for ( size_t i = 0; i < _lights.size(); ++i ) {
				_lights[i].initGL( (int) i, SceneGraph::GLLight::CAMERA );
			}
		}

		///////////////////////////////////////////////////////////////////////////

		void GLViewer::setDumpPath( const std::string & path ) {
			std::string fullpath;
			os::path::absPath( path, fullpath );
			if ( ! os::path::exists( fullpath ) ) {
				if ( ! os::makedirs( fullpath ) ) {
					logger << Logger::WARN_MSG << "Unable to make path for dumping: " << fullpath << "\n";
					_validDumpPath = false;
					return;
				}
			}
			logger << Logger::INFO_MSG << "Dumping png to: " << fullpath << "\n";
			_dumpPath = path;
			size_t pos = path.rfind( os::path::pathSep() );
			if ( pos != path.size() - 1 ) {
				_dumpPath += os::path::pathSep();
			}
		}

		///////////////////////////////////////////////////////////////////////////

	}	// namespace Vis
}	// namespace Menge