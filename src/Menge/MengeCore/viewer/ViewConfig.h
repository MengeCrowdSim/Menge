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
 *	@file		ViewConfig.h
 *	@brief		Specification for parsing the view configuration file.
 */

#ifndef __VIEW_CONFIG_H__
#define __VIEW_CONFIG_H__

#include <string>
#include <vector>
#include "CoreConfig.h"
#include "GLCamera.h"
#include "GLLight.h"
#include "Logger.h"

namespace Menge {

	namespace Vis {

		// forward declarations
		class Watermark;

		/*!
		 *	@brief		A set of parameters specifying a single camera.
		 */
		class CameraParam {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			CameraParam() {
				_posX = _posY = _tgtX = _tgtY = _tgtZ = 0.0f;
				_posZ = 10.f;
				_farPlane = 200.f;
				_nearPlane = 0.01f;
				_orthoScale = 1.f;
				_fov = 0.f;
				_projType = SceneGraph::GLCamera::ORTHO;
			}

			/*!
			 *	@brief		Camera x-position in world space.
			 */
			float _posX;
			
			/*!
			 *	@brief		Camera y-position in world space.
			 */
			float _posY;
			
			/*!
			 *	@brief		Camera z-position in world space.
			 */
			float _posZ;
			
			/*!
			 *	@brief		Camera's target x-position in world space.
			 */
			float _tgtX;
			
			/*!
			 *	@brief		Camera's target y-position in world space.
			 */
			float _tgtY;
			
			/*!
			 *	@brief		Camera's target z-position in world space.
			 */
			float _tgtZ;
			
			/*!
			 *	@brief		Distance to camera's far plane.
			 */
			float _farPlane;
			
			/*!
			 *	@brief		Distance to camera's near plane.
			 */
			float _nearPlane;
			
			/*!
			 *	@brief		The "scale" factor applid to the camera in orthographic view.
			 */
			float _orthoScale;

			/*!
			 *	@brief		The camera's horizontal field of view (in degrees).
			 */
			float _fov;

			/*!
			 *	@brief		The camera's projection type (perspective or orthographic).
			 *
			 *	@see		SceneGraph::GLCamera
			 */
			SceneGraph::GLCamera::CamEnum	_projType;
		};

		////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		A set of parameters specifying a single light
		 */
		class LightParam {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			LightParam() {
				_r = _g = _b = 1.f;
				_x = _y = _z = 1.f;
				_w = 0.f;	// 0 --> directional, 1 --> point
			}

			/*!
			 *	@brief		The red channel of the light's diffuse color.
			 */
			float	_r;

			/*!
			 *	@brief		The green channel of the light's diffuse color.
			 */
			float	_g;

			/*!
			 *	@brief		The blue channel of the light's diffuse color.
			 */
			float	_b;

			/*!
			 *	@brief		The alpha channel of the lights' diffuse color.
			 */
			float	_a;

			/*!
			 *	@brief		The x-value of the light position
			 */
			float	_x;

			/*!
			 *	@brief		The y-value of the light position
			 */
			float	_y;

			/*!
			 *	@brief		The z-value of the light position
			 */
			float	_z;

			/*!
			 *	@brief		The w-value of the light position. Determines if the light
			 *				Is a point or directional light.
			 */
			float	_w;

			/*!
			 *	@brief		The space in which the light lives.
			 */
			SceneGraph::GLLight::LightSpace _space;
		};

		////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The specification of an OpenGL GLViewer for a scene
		 *
		 *	@see	GLViewer
		 */
		class MENGE_API ViewConfig {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			ViewConfig();

			/*!
			 *	@brief		Destructor.
			 */
			~ViewConfig();

			/*!
			 *	@brief		Parses the XML configuration file.
			 *
			 *	@param		fileName		The name of the view configuration file to parse.
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			bool readXML( const std::string & fileName );

			/*!
			 *	@brief		Sets the view configuration to a set of default values.
			 */
			void setDefaults();

			/*!
			 *	@brief		Set the camera properties based on the configuration
			 *
			 *	@param		camera		The camera to set
			 *	@param		i			The index of the camera
			 */
			void setCamera( SceneGraph::GLCamera & camera, size_t i=0 ) const;

			/*!
			 *	@brief		Sets the vector of cameras based on the camera specifications
			 *
			 *	@param		cameras		A vector to populate with cameras.  Any pre-existing cameras
			 *							will be deleted.
			 */
			void setCameras( std::vector< SceneGraph::GLCamera > & cameras ) const;

			/*!
			 *	@brief		Set the light properties based on the configuration
			 *
			 *	@param		light		The light to set
			 *	@param		i			The index of the light specification to apply
			 */
			void setLight( SceneGraph::GLLight & light, size_t i=0 ) const;

			/*!
			 *	@brief		Sets the vector of lights based on the light specifications
			 *
			 *	@param		lights		A vector to populate with lights.  Any pre-existing lights
			 *							will be deleted.
			 */
			void setLights( std::vector< SceneGraph::GLLight > & lights ) const;

			/*!
			 *	@brief		The folder the view configuration file is located in.
			 */
			std::string	_viewFldr;

			/*!
			 *	@brief		Width of viewport (in pixels).
			 */
			int _width;
			
			/*!
			 *	@brief		Height of viewport (in pixels).
			 */
			int _height;

			/*!
			 *	@brief		The name of the background image to use.
			 */
			std::string	_bgImg;

			/*!
			 *	@brief		The optional watermark.
			 */
			Watermark	* _waterMark;

			/*!
			 *	@brief		Font name.
			 */
			std::string	_fontName;

			/*!
			 *	@brief		Default font color.
			 */
			float	_fontColor[4];
			
			/*!
			 *	@brief		The set of cameras for the configuration
			 */
			std::vector< CameraParam >	_camSpecs;
			
			/*!
			 *	@brief		The set of cameras for the configuration
			 */
			std::vector< LightParam >	_lightSpecs;
		};
	}	// namespace Vis

	/*!
	 *	@brief		Streaming output operator to display configuration specification.
	 *
	 *	@param		out		The output stream to which to write the view configuration.
	 *	@param		cfg		The configuration to convert to a string.
	 *	@returns	The output stream.
	 */
	MENGE_API Logger & operator<< ( Logger & out, const Vis::ViewConfig & cfg ); 
}	// namespace Menge

#endif // __VIEW_CONFIG_H__
