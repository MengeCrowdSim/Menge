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

#include "Watermark.h"

#ifdef _MSC_VER
#include "windows.h"
#endif
#include "GL/gl.h"

#include "Logger.h"
#include "tinyxml.h"
#include "os.h"
#include "image.h"
#include "Utils.h"

namespace Menge {

	namespace Vis {

		////////////////////////////////////////////////////////////////////////////
		//			Implementation of Watermark helper function
		////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		helper function to convert a string to water mark alignment
		 *
		 *	@param		s		the string representing the alignment
		 *	@returns	a WatermarkAlign corresponding to the given string
		 */
		WatermarkAlign getAlignment( std::string s ) {
			WatermarkAlign align = NO_ALIGN;
			if ( s == "centered" ) {
				align = CENTERED;
			} else if ( s == "bottom_left" ) {
				align = BOTTOM_LEFT;
			} else if ( s == "bottom_right" ) {
				align = BOTTOM_RIGHT;
			} else if ( s == "top_left" ) {
				align = TOP_LEFT;
			} else if ( s == "top_right" ) {
				align = TOP_RIGHT;
			}
			return align;
		}

		////////////////////////////////////////////////////////////////////////////
		//			Implementation of Watermark
		////////////////////////////////////////////////////////////////////////////

		Watermark::Watermark(): _fileName(""), _img(0x0), _alignment(BOTTOM_RIGHT), _opacity(0.5f), _scale(0.5f) {
		}

		////////////////////////////////////////////////////////////////////////////

		Watermark::~Watermark() {
			if ( _img ) delete _img;
		}

		////////////////////////////////////////////////////////////////////////////
		
		void Watermark::drawGL( float w, float h ) const {
			float iW = _img->getWidth() * _scale;
			float iH = _img->getHeight() * _scale;

				float min_x, max_x, min_y, max_y;
			float u = 1.f, v = 1.f;
			if ( _alignment == CENTERED ) {
				float midX = w * 0.5f;
				iW *= 0.5f;
				float midY = h * 0.5f;
				iH *= 0.5f;
				min_x = midX - iW;
				max_x = midX + iW;
				min_y = midY - iH;
				max_y = midY + iH;
			} else if ( _alignment == BOTTOM_LEFT ) {
				min_x = min_y = 0.f;
				max_x = iW;
				max_y = iH;
			} else if ( _alignment == BOTTOM_RIGHT ) {
				min_y = 0.f;
				max_y = iH;
				min_x = w - iW;
				max_x = w;
			} else if ( _alignment == TOP_LEFT ) {
				min_x = 0.f;
				max_x = iW;
				min_y = h - iH;
				max_y = h;
			} else if ( _alignment == TOP_RIGHT ) {
				min_x = w - iW;
				max_x = w;
				min_y = h - iH;
				max_y = h;
			}  

			// set up rendering
			glPushAttrib( GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT );
			glDisable( GL_LIGHTING );
			glDisable( GL_DEPTH_TEST );
			glDepthMask( GL_FALSE );
			glMatrixMode( GL_PROJECTION );
			glPushMatrix();
			glLoadIdentity();
			glOrtho( 0.0, w, 0.0, h, -1.0f, 1.0f );
			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();
			glLoadIdentity();

			_img->bind();
			glColor4f( 1.f, 1.f, 1.f, _opacity );
			if ( _opacity < 1.f ) {
				glEnable( GL_BLEND );
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
			}
			glBegin( GL_QUADS );
			glTexCoord2f( 0.f, v );
			glVertex3f( min_x, min_y, 0.f );
			glTexCoord2f( u, v );
			glVertex3f( max_x, min_y, 0.f );
			glTexCoord2f( u, 0.f );
			glVertex3f( max_x, max_y, 0.f );
			glTexCoord2f( 0.f, 0.f );
			glVertex3f( min_x, max_y, 0.f );
			glEnd();

			glPopMatrix();
			glMatrixMode( GL_PROJECTION );
			glPopMatrix();
			glMatrixMode( GL_MODELVIEW );
			glPopAttrib();
		}

		////////////////////////////////////////////////////////////////////////////
		//			Implementation of Watermark
		////////////////////////////////////////////////////////////////////////////

		Watermark *	parseWatermark( TiXmlElement * node, const std::string & viewFldr ) {
			// collect parameters
			//	Filename - make sure it exists
			Watermark * mark = 0x0;
			const char * fNameCStr = node->Attribute( "file_name" );
			Image * img = 0x0;
			if ( fNameCStr ) {
				std::string fName;
				std::string path = os::path::join( 2, viewFldr.c_str(), fNameCStr );
				os::path::absPath( path, fName );
				if ( ! os::path::exists( fName ) ) {
					logger << Logger::WARN_MSG << "Watermark specification on line " << node->Row() << " references a file that doesn't exist: " << fName << ".  No watermark will be created.";
					return mark;
				}
				img = loadImage( fName );
				if ( !img ) {
					logger << Logger::WARN_MSG << "Watermark specification on line " << node->Row() << " references a file that couldn't be loaded as an image: " << fName << ".  No watermark will be created.";
					return mark;
				}
			} else {
				logger << Logger::WARN_MSG << "Watermark specification on line " << node->Row() << " is missing the \"file_name\" attribute.  No watermark will be created.";
				return mark;
			}

			//  alignment
			WatermarkAlign align = BOTTOM_RIGHT;
			const char * alignCStr = node->Attribute( "alignment" );
			if ( alignCStr ) {
				align = getAlignment( std::string( alignCStr ) );
				if ( align == NO_ALIGN ) {
					logger << Logger::WARN_MSG << "Watermark specification on line " << node->Row() << " has invalid value for the \"alignment\" attribute (" << node->Attribute( "alignment" ) << ").  Default value of \"bottom_right\"will be used.";
					align = BOTTOM_RIGHT;
				}
			} else {
				logger << Logger::WARN_MSG << "Watermark specification on line " << node->Row() << " is missing the \"alignment\" attribute.  Default will be used.";
			}
			
			//	opacity
			float opacity = 0.5f;
			const char * opacityStr = node->Attribute( "opacity" );
			if ( opacityStr ) {
				try {
					opacity = toFloat( opacityStr );
					if ( opacity < 0.f ) {
						logger << Logger::WARN_MSG << "Watermark specification on line " << node->Row() << " has negative \"opacity\" value.  Value will be clamped to zero.";
						opacity = 0.f;
					} else if ( opacity > 1.f ) {
						logger << Logger::WARN_MSG << "Watermark specification " << node->Row() << " has \"opacity\" value greater than one.  Value will be clamped to one.";
						opacity = 1.f;
					}
				} catch ( UtilException ) {
					logger << Logger::WARN_MSG << "Watermark specification on line " << node->Row() << " has a badly formatted value for the \"opacity\" attribute.  Default value " << opacity << " will be used.";
				}			
			} else {
				logger << Logger::WARN_MSG << "Watermark specification on line " << node->Row() << " is missing the \"opacity\" attribute.  Default value " << opacity << " will be used.";
			}

			//	scale
			float scale = 0.5f;
			const char * scaleStr = node->Attribute( "scale" );
			if ( scaleStr ) {
				try {
					scale = toFloat( scaleStr );
					if ( scale < 0.f ) {
						logger << Logger::WARN_MSG << "Watermark specification on line " << node->Row() << " has negative \"scale\" value.  Value will be clamped to zero.";
						scale = 0.f;
					}
				} catch ( UtilException ) {
					logger << Logger::WARN_MSG << "Watermark specification on line " << node->Row() << " has a badly formatted value for the \"scale\" attribute.  Default value " << scale << " will be used.";
				}
			} else {
				logger << Logger::WARN_MSG << "Watermark specification on line " << node->Row() << " is missing the \"scale\" attribute.  Default value " << scale << " will be used.";
			}

			mark = new Watermark();
			mark->_fileName = fNameCStr;
			mark->_img = img;
			mark->_alignment = align;
			mark->_opacity = opacity;
			mark->_scale = scale;
			return mark;
		}

	}	// namespace Vis
}	// namespace Menge