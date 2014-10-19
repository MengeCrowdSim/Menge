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

#include "ScreenGrab.h"
#include "Logger.h"
#include "png.h"
#ifdef _WIN32		// only supported under windows
#include "windows.h"
#endif
#include <GL/glu.h>
#include <iostream>

namespace Menge {

	bool snapshotPNG(int width, int height, const char* path) {
		static int oldHeight = 0;
		static int oldWidth = 0;
		static GLubyte ** rows = 0x0;
		static GLubyte *image = 0x0;
		if ( oldHeight != height || oldWidth != width ) {
			oldHeight = height;
			oldWidth = width;
			if ( rows ) { 
				delete [] rows;
				delete [] image;
			}
			image = new GLubyte[ width * height * 3 ];
			// Set the pointers
			rows = new GLubyte *[height];
			const int rowSize = width * 3;
			for ( int r = height - 1; r >= 0; --r ) {
				rows[ height - r - 1 ] = image + r * rowSize;
			}
		}
		FILE	*fp;
		static png_structp	png_ptr;
		static png_infop	info_ptr;
	#ifdef _WIN32
		fopen_s( &fp, path, "wb" );
	#else
		fp = fopen( path, "wb" );
	#endif
		if ( fp == 0x0 ) {
			logger << Logger::ERR_MSG << "Unable to write image " << path << "\n";
			return false;
		}
		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		info_ptr = png_create_info_struct(png_ptr);
		png_init_io(png_ptr, fp);

		//png_set_compression_level(png_ptr, PNGWRITER_DEFAULT_COMPRESSION);
		png_set_IHDR(png_ptr, info_ptr, width, height,
			8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
		png_write_info(png_ptr, info_ptr);

		// Acquire the image
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);

		png_write_image(png_ptr, rows );
		png_write_end(png_ptr, info_ptr);
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		return true;
	} 

}	// namespace Menge