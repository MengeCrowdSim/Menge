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

#include "image.h"
#include <iostream>
#include "GLContextManager.h"

namespace Menge {

	///////////////////////////////////////////////////////////////////////////
	//				IMPLEMENTATION FOR ImageData
	///////////////////////////////////////////////////////////////////////////

	std::map< std::string, ImageData * > ImageData::RESOURCES;
	bool ImageData::IS_REGISTERED = false;

	///////////////////////////////////////////////////////////////////////////

	void ImageData::start() {
		int flags = IMG_INIT_JPG | IMG_INIT_PNG;
		int initted = IMG_Init( flags );
		if( ( initted & flags ) == 0x0) {
			logger << Logger::ERR_MSG << "Failed to initialize png and jpg support!  No images available!\n";
			// TODO: Put some flags in here that will silently suppress images
		} else if ( ( initted & IMG_INIT_JPG ) == 0 ) {
			logger << Logger::ERR_MSG << "Failed to initialize jpg support!\n";
		} else if ( ( initted & IMG_INIT_PNG ) == 0 ) {
			logger << Logger::ERR_MSG << "Failed to initialize png support!\n";
		}
	}

	///////////////////////////////////////////////////////////////////////////

	void ImageData::release() {
		IMG_Quit();
	}

	///////////////////////////////////////////////////////////////////////////

	ImageData::ImageData():ManagedData(),_useAlpha( false ), _texture( 0 ), _img( 0 ) {
		if ( ! IS_REGISTERED ) {
			GLContextManager::addCallback( &newGLContext );
			IS_REGISTERED = true;
		}
	}

	///////////////////////////////////////////////////////////////////////////

	ImageData * readImageData( const std::string & fileName ) {
		ImageData * data = new ImageData();

		data->_img = IMG_Load( fileName.c_str() );
		if ( data->_img != 0x0 ) {
			data->_useAlpha = data->_img->format->BytesPerPixel > 3;
			ImageData::RESOURCES[ fileName ] = data;
		} else {
			delete data;
			data = 0x0;
		}
		return data;
	}

	///////////////////////////////////////////////////////////////////////////

	int ImageData::getBpp() const {
		return _useAlpha ? 32 : 24;
	}

	///////////////////////////////////////////////////////////////////////////

	ImageData::~ImageData() {
		if ( _texture != 0 ) {
			glDeleteTextures( 1, &_texture );
			_texture = 0;
		}
		if ( _img ) {
			SDL_FreeSurface( _img );
			_img = 0x0;
		}
		removeResource< ImageData >( this, ImageData::RESOURCES );
	}

	///////////////////////////////////////////////////////////////////////////

	void ImageData::initGL() const {
		unsigned int w = getWidth();
		unsigned int h = getHeight();
		
		glGenTextures( 1, &_texture );
		glBindTexture( GL_TEXTURE_2D, _texture );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		if ( _useAlpha ) {
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
				GL_BGRA, GL_UNSIGNED_BYTE, _img->pixels );
		} else {
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0,
				GL_BGR, GL_UNSIGNED_BYTE, _img->pixels );
		}

	}

	///////////////////////////////////////////////////////////////////////////

	void ImageData::bind() const {
		glBindTexture( GL_TEXTURE_2D, _texture );
		glEnable( GL_TEXTURE_2D );
		if ( _useAlpha ) {
			glEnable( GL_BLEND );
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	}

	///////////////////////////////////////////////////////////////////////////

	void ImageData::drawGL() const {
		glPushAttrib( GL_ENABLE_BIT | GL_CURRENT_BIT );
		bind();
		glDisable( GL_LIGHTING );
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
		glBegin( GL_QUADS );
			glTexCoord2f( 0.0f, 1.0f );
			glVertex3f( 0.0f, 0.0f, 0.0f );
			glTexCoord2f( 1.0, 1.0f );
			glVertex3f( (GLfloat)getWidth(), 0.0f, 0.0f );
			glTexCoord2f( 1.0, 0.0f );
			glVertex3f( (GLfloat)getWidth(), (GLfloat)getHeight(), 0.0 );
			glTexCoord2f( 0.0, 0.0f );
			glVertex3f( 0.0f, (GLfloat)getHeight(), 0.0 );
		glEnd();
		glPopAttrib();
	}

	///////////////////////////////////////////////////////////////////////////

	void ImageData::newGLContext() { 
		// I don't call glDeleteTexture because at this point, I assume
		//	that I've already lost it by changing contexts and merely need
		//	to recreate the gl texture.
		std::map< std::string, ImageData * >::iterator itr = RESOURCES.begin();
		for ( ; itr != RESOURCES.end(); ++itr ) {
			itr->second->initGL();
		}

	}

	///////////////////////////////////////////////////////////////////////////

	unsigned char ImageData::getPixelAt( int x, int y ) const {

		int bpp = _img->format->BytesPerPixel;
		unsigned char * p = static_cast< unsigned char * >(_img->pixels) + ( y * _img->pitch + x * bpp );

		int out = 0;
		for ( int i = 0; i < bpp; ++i ) {
			out += p[ i ];
		}
		out = out / bpp;
		return static_cast< unsigned char >( out );

	}

	///////////////////////////////////////////////////////////////////////////
	//				IMPLEMENTATION FOR Image
	///////////////////////////////////////////////////////////////////////////

	Image::Image( ImageData *data ): ManagedDataWrapper< ImageData >(data) {
	}

	///////////////////////////////////////////////////////////////////////////

	Image::~Image() {
		freeData();
	}

	///////////////////////////////////////////////////////////////////////////

	Image * loadImage( const std::string & fileName ) {
		return loadManagedData< Image, ImageData>( fileName, &readImageData );
	}
}	// namespace Menge