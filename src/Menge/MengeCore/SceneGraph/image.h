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
 *	@file		image.h
 *	@brief		The interface for loading and using images in the scene graph.
 */

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "CoreConfig.h"
#include <string>
#include "graphCommon.h"
#include "ManagedData.h"
#include <map>
#include "SDL/SDL_image.h"

namespace Menge {

	/*!
	 *	@brief		Manged image data (see ManagedData).
	 */
	class MENGE_API ImageData : public ManagedData {
	public:
		/*!
		 *	@brief		Mapping from image resources to its corresponding data.
		 */
		static std::map< std::string, ImageData * > RESOURCES;

		/*!
		 *	@brief		Determines if the ImageData::newGLContext has been registered.
		 */
		static bool IS_REGISTERED;

		/*!
		 *	@brief		The function to call when an OpenGL context has changed.
		 */
		static void newGLContext();

		/*!
		 *	@brief		Initializes the image system.  
		 *
		 *	Must be called before images can be used. Essentially, it calls the 
		 *	SDL Image initialization
		 */
		static void start();

		/*!
		 *	@brief		Called when done using images.
		 */
		static void release();
		
		/*!
		 *	@brief		Constructor.
		 */
		ImageData();

		/*!
		 *	@brief		Destructor.
		 */
		~ImageData();

		/*!
		 *	@brief		Returns the width of the image (in pixels).
		 *
		 *	@returns	The width of the image (in pixels).
		 */
		int getWidth() const { return _img->w; }
		
		/*!
		 *	@brief		Returns the height of the image (in pixels).
		 *
		 *	@returns	The height of the image (in pixels).
		 */
		int getHeight() const { return _img->h; }

		/*!
		 *	@brief		Reports the bits per pixel.
		 *
		 *	@returns	Either 24 or 32, depending on whether the image has an
		 *				alpha channel (32) or not (24).
		 */
		int getBpp() const; 

		/*!
		 *	@brief		Initializes the image for drawing in an OpenGL context.
		 */
		void initGL() const;

		/*!
		 *	@brief		Draws the image onto a quad, centered on the origin of the x-y plane.
		 */
		void drawGL() const;	

		/*!
		 *	@brief		Binds the image to use as a texture in OpenGL.
		 */
		void bind() const;	

		/*!
		 *	@brief		Loads the image from a file (specified by name)
		 *
		 *	@param		fileName	The path to a valid image file.  
		 *	@returns	A pointer to the underlying ImageData.  If there is an
		 *				error, NULL is returned.
		 */
		friend ImageData * readImageData( const std::string & fileName );

		/*!
		 *	@brief		Returns the red channel of the image at the given pixel.
		 *
		 *	@param		x		The x-coordinate of the pixel.
		 *	@param		y		The y-coordinate of the pixel.
		 *  @returns	the red channel of the image at the given pixel.
		 */
		unsigned char getPixelAt( int x, int y ) const;

	private:
		/*!
		 *	@brief		Indicates whether the image uses per-pixel alpha.
		 */
		bool			_useAlpha;

		/*!
		 *	@brief		The OpenGL texture id for the image.
		 */
		mutable GLuint	_texture;

		/*!
		 *	@brief		The SDL surface which contains the image data.
		 */
		SDL_Surface	* _img;
	};


	/*!
	 *	@brief		Wrapper for the managed ImageData.
	 */
	class MENGE_API Image : public ManagedDataWrapper< ImageData > {
	public:
		/*!
		 *	@brief		Construtor.
		 *
		 *	@param		data	An instance of ImageData.
		 */
		Image( ImageData * data );

		/*!
		 *	@brief		Destructor.
		 */
		~Image();

		/*!
		 *	@brief		Binds the image to use as a texture in OpenGL.
		 */
		void bind() const { _data->bind(); }

		/*!
		 *	@brief			Returns the image data.
		 *
		 *	@returns		A const pointer to the underlying image data.
		 */
		const ImageData * data() const { return _data; }

		/*!
		 *	@brief		Returns the width of the image (in pixels).
		 *
		 *	@returns	The width of the image (in pixels).
		 */
		int getWidth() const { return _data->getWidth(); }
		
		/*!
		 *	@brief		Returns the height of the image (in pixels).
		 *
		 *	@returns	The height of the image (in pixels).
		 */
		int getHeight() const { return _data->getHeight(); }

		
	};

	/*!
	 *	@brief		Given a filename, returns a pointer to an Image with that data.
	 *
	 *	@param		fileName		The unique path to the image.
	 *	@returns	A pointer to an Image instance.  If there is a problem loading
	 *				the image, NULL is returned.  The caller is responsible for 
	 *				deleting the pointer.
	 */
	MENGE_API Image * loadImage( const std::string & fileName );
}	// namespace Menge

#endif  // __IMAGE_H__
