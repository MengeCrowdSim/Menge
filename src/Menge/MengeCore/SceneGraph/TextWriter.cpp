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

#include "TextWriter.h"
#include "graphCommon.h"

namespace Menge {

	namespace SceneGraph {

		///////////////////////////////////////////////////////////////////////////
		//				IMPLEMENTATION FOR TextWriter
		///////////////////////////////////////////////////////////////////////////

		TextWriter * TextWriter::_instance = 0x0;
		std::string TextWriter::DEFAULT_FONT = std::string( "arial.ttf" );
		const int TextWriter::DEFAULT_SIZE = 20;

		///////////////////////////////////////////////////////////////////////////

		void TextWriter::setDefaultFont( const std::string & fontName ) {
			DEFAULT_FONT = fontName;
		}

		///////////////////////////////////////////////////////////////////////////

		TextWriter * TextWriter::Instance() {
			if ( _instance == 0x0 ) {
				_instance = new TextWriter();
			}
			return _instance;
		}

		///////////////////////////////////////////////////////////////////////////
		
		TextWriter::TextWriter(): _width(640), _height(480), _fontName(""), _textID(0), _textQuadID(0) {
			TTF_Init();
			setFont( DEFAULT_FONT );
			assert( _fonts.size() > 0 && "Unable to load font" );
			_defColor[0] = _defColor[1] = _defColor[2] = 0.f;
			_defColor[3] = 0.75f;
		}

		///////////////////////////////////////////////////////////////////////////

		TextWriter::~TextWriter() {
			closeFonts();
			glDeleteLists( _textQuadID, 1 );
			glDeleteTextures( 1, &_textID );
			TTF_Quit();
		}

		///////////////////////////////////////////////////////////////////////////
		
		void TextWriter::newGLContext() {
			makeTextDList();
			makeGLTexture();
		}

		///////////////////////////////////////////////////////////////////////////

		void TextWriter::printAlignText( const std::string & text, Alignment alignment, int fontSize, float hpad, float vpad, bool currColor, bool transparent ) {
			float w, h;
			textSize( text, fontSize, w, h );
			
			float left = hpad, bottom = vpad;	// defaults to LEFT, BOTTOM alignment
			if ( alignment & VCENTER ) {
				bottom = ( _height - h ) / 2.0f;
			} else if ( alignment & TOP ) {
				bottom = ( _height - h - vpad);
			}
			if ( alignment & HCENTER ) {
				left = ( _width - w ) / 2.0f;
			} else if ( alignment & RIGHT ) {
				left = ( _width - w - hpad);
			}
			printText( text, left, bottom, fontSize, currColor, transparent );
		}

		///////////////////////////////////////////////////////////////////////////

		void TextWriter::printText( const std::string & text, Alignment alignment, float anchorX, float anchorY, int fontSize, bool currColor, bool transparent ) {
			float w, h;
			textSize( text, fontSize, w, h );
			
			float left = anchorX, bottom = anchorY;	// defaults to LEFT, BOTTOM alignment
			if ( alignment & VCENTER ) {
				bottom = anchorY - h / 2.0f;
			} else if ( alignment & TOP ) {
				bottom = anchorY - h;
			}
			if ( alignment & HCENTER ) {
				left = anchorX - w / 2.0f;
			} else if ( alignment & RIGHT ) {
				left = anchorX - w;
			}
		
			printText( text, left, bottom, fontSize, currColor, transparent );
		}

		///////////////////////////////////////////////////////////////////////////

		void TextWriter::printAnchorText( const std::string & text, float anchorX, float anchorY, float xWeight, float yWeight, int fontSize, bool currColor, bool transparent ) {
			float w, h;
			textSize( text, fontSize, w, h );

			float left = anchorX - xWeight * w;
			float bottom = anchorY - yWeight * h;
		
			printText( text, left, bottom, fontSize, currColor, transparent );
		}

		///////////////////////////////////////////////////////////////////////////

		void TextWriter::printText( const std::string & text, float left, float bottom, int fontSize, bool currColor, bool transparent ) {
			TTF_Font * font = getFont( fontSize );
			if ( font == 0x0 ) return;
			int lineCount = 1;
			int nlIndex = (int)text.find_first_of( "\n" );
			while ( nlIndex > -1 ) {
				lineCount += 1;
				nlIndex = (int)text.find_first_of( "\n", nlIndex + 1 );
			}

			// set up orthogonal view
			glMatrixMode( GL_PROJECTION );
			glPushMatrix();
			glLoadIdentity();
			glOrtho( 0.0, _width, 0.0, _height, -1.0f, 1.0f );
			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();
			glLoadIdentity();
			
			glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
			glDisable( GL_DEPTH_TEST );

			glBindTexture( GL_TEXTURE_2D, _textID );
			if ( transparent ) {
				glEnable( GL_BLEND );
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;
			}
			glDisable( GL_LIGHTING );
			glEnable( GL_TEXTURE_2D );

			if ( !currColor ) {
				glColor4fv( &_defColor[0] );
			}
			// for each line
			//	Render the line
			//  put the surface into the texture
			//	render the quad
			int startIndex = 0;
			nlIndex = (int)text.find_first_of( "\n" );
			while ( nlIndex > -1 ) {
				--lineCount;
				int length = nlIndex - startIndex;
				if ( length ) {
					std::string testStr = text.substr( startIndex, length );
					printLine( font, testStr.c_str(), left, bottom, lineCount );	
					startIndex = nlIndex + 1;
					nlIndex = (int)text.find_first_of( "\n", startIndex );
				}
			}
			int length = nlIndex - startIndex;
			if ( length ) {
				std::string testStr = text.substr( startIndex, length );
				--lineCount;
				printLine( font, testStr.c_str(), left, bottom, lineCount );
			}

			glPopAttrib();
			glPopMatrix();
			//		draw a character
			glMatrixMode( GL_PROJECTION );
			glPopMatrix();
			glMatrixMode( GL_MODELVIEW );
		}
		
		///////////////////////////////////////////////////////////////////////////

		void TextWriter::resize( int w, int h ) {
			_width = w;
			_height = h;
		}

		///////////////////////////////////////////////////////////////////////////

		void TextWriter::textSize( const std::string & text, int fontSize, float & textWidth, float & textHeight ) {
			TTF_Font * font = getFont( fontSize );
			if ( font == 0x0 ) {
				textWidth = 0.f;
				textHeight = 0.f;
				return;
			}
			int w, h;
			textHeight = 0.f;
			textWidth = 0.f;
			int startIndex = 0;
			int nlIndex = (int)text.find_first_of( "\n" );
			while ( nlIndex > -1 ) {
				int length = nlIndex - startIndex;
				std::string testStr = text.substr( startIndex, length );
				TTF_SizeText( font, testStr.c_str(), &w, &h );
				textHeight += (float)h;
				if ( (float)w > textWidth ) textWidth = (float)w;
				startIndex = nlIndex + 1;
				nlIndex = (int)text.find_first_of( "\n", startIndex );
			}
			int length = (int)text.length() - startIndex;
			std::string testStr = text.substr( startIndex, length );
			TTF_SizeText( font, testStr.c_str(), &w, &h );
			textHeight += (float)h;
			if ( (float)w > textWidth ) textWidth = (float)w;
		}

		///////////////////////////////////////////////////////////////////////////

		void TextWriter::printLine( TTF_Font * font, const char * text, float left, float bottom, int lineNum ) {
			static const SDL_Color foregroundColor = { 255, 255, 255 };
			SDL_Surface * surf = TTF_RenderText_Blended( font, text, foregroundColor );
			sdlSurfaceToGLTex( surf );
			glPushMatrix();
			/// scale and translate the quad
			// translate so that the left-bottom corner is at (left, bottom + height * lineCount )
			// I'm assuming that each line, for a given font size, is the same height
			glTranslatef( left, bottom + surf->h * lineNum, 0.f );
			glScalef( (float)surf->w, (float)surf->h, 1.f );
			glCallList( _textQuadID );
			glPopMatrix();
			SDL_FreeSurface(surf);
		}

		///////////////////////////////////////////////////////////////////////////

		void TextWriter::closeFonts() {
			FontMapItr itr = _fonts.begin();
			for ( ; itr != _fonts.end(); ++itr ) {
				TTF_CloseFont( itr->second );
			}
			_fonts.clear();
		}

		///////////////////////////////////////////////////////////////////////////

		bool TextWriter::setFont( const std::string &fontName ) {
			if ( _fontName != fontName ) {
				// Only do the work if it's really a change
				TTF_Font * newFont = TTF_OpenFont( fontName.c_str(), DEFAULT_SIZE );
				if ( newFont ) {
					closeFonts();
					_fonts.clear();
					_fonts[ DEFAULT_SIZE ] = newFont;
					_fontName = fontName;
					return true;
				} 
			}
			return false;
		}

		///////////////////////////////////////////////////////////////////////////

		void TextWriter::setDefaultColor( float r, float g, float b, float alpha ) {
			_defColor[ 0 ] = r;
			_defColor[ 1 ] = g;
			_defColor[ 2 ] = b;
			_defColor[ 3 ] = alpha;
		}
		///////////////////////////////////////////////////////////////////////////

		TTF_Font * TextWriter::getFont( int size ) {
			if ( size <= 0 ) return 0x0;
			if ( _fonts.find( size ) == _fonts.end() ) {
				TTF_Font * newFont = TTF_OpenFont( _fontName.c_str(), size );
				if ( newFont ) {
					_fonts[ size ] = newFont;
				}
			}
			return _fonts[ size ];
		}

		///////////////////////////////////////////////////////////////////////////

		void TextWriter::makeTextDList() {
			_textQuadID = glGenLists( 1 );
			glNewList( _textQuadID, GL_COMPILE );
			glBegin( GL_QUADS );
			glTexCoord2f( 0.f, 1.f  );
			glVertex3f( 0.f, 0.f, 0.0f );
			glTexCoord2f( 1.f, 1.f  );
			glVertex3f( 1.f, 0.f, 0.0f );
			glTexCoord2f( 1.f, 0.f  );
			glVertex3f( 1.f, 1.f, 0.0f );
			glTexCoord2f( 0.f, 0.f  );
			glVertex3f( 0, 1.f, 0.0f );
			glEnd();
			glEndList();
		}

		///////////////////////////////////////////////////////////////////////////

		void TextWriter::makeGLTexture() {
			if ( _textID != 0 ) glDeleteTextures( 1, &_textID );
			glGenTextures( 1, &_textID );
		}

		///////////////////////////////////////////////////////////////////////////

		void TextWriter::sdlSurfaceToGLTex( SDL_Surface *surf ) {
			assert( surf->format->BytesPerPixel == 4 && "Texture not RGBA" );
			assert( _textID > 0 && "Can't bind surface to a texture of 0" );

			GLint nOfColors = surf->format->BytesPerPixel;
			GLenum texture_format;
			if (surf->format->Rmask == 0x000000ff)
				texture_format = GL_RGBA;
			else
				texture_format = GL_BGRA;

			// Bind the texture object
			glBindTexture( GL_TEXTURE_2D, _textID );
		 
			// Set the texture's stretching properties
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE   );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE   );
		 
			// Edit the texture object's image data using the information SDL_Surface gives us
			glTexImage2D( GL_TEXTURE_2D, 0, 4, surf->w, surf->h, 0,
							  texture_format, GL_UNSIGNED_BYTE, surf->pixels );

		}

	}	// namespace SceneGraph
}	// namespace Menge
