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
 *	@file		Watermark.h
 *	@brief		Data for controlling a watermark.
 */

#ifndef __WATER_MARK_H__
#define __WATER_MARK_H__

#include <string>

// forward declaration
class TiXmlElement;

namespace Menge {

	class Image;

	namespace Vis {
		/*!
		 *	@brief		The potential alignment of the watermark.
		 */
		enum WatermarkAlign {
			NO_ALIGN,		///< Undefined alignment.
			CENTERED,		///< Center the image in the screen.
			BOTTOM_LEFT,	///< Place the image in the bottom-left corner of the screen.
			BOTTOM_RIGHT,	///< Place the image in the bottom-right corner of the screen.
			TOP_LEFT,		///< Place the image in the top-left corner of the screen.
			TOP_RIGHT		///< Place the image in the top-right corner of the screen.
		};

		/*!
		 *	@brief		A class for handling the watermark functionality.
		 *	
		 *	A watermark is a semi-transparent image drawn over the top of the scene.
		 */
		class Watermark {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			Watermark();

			/*!
			 *	@brief		Destructor.
			 */
			~Watermark();

			/*!
			 *	@brief		Returns the file name for the water mark.
			 */
			const std::string & getFilename() const { return _fileName; }

			/*!
			 *	@brief		Draws the watermark to the screen.
			 *
			 *	@param		w		The width of the screen (in pixels).
			 *	@param		h		The height of the screen (in pixels).
			 */
			void drawGL( float w, float h ) const;

			friend Watermark *	parseWatermark( TiXmlElement * node, const std::string & viewFldr );
			
		protected:
			/*!
			 *	@brief		File name of the watermark.
			 */
			std::string		_fileName;

			/*!
			 *	@brief		A pointer to the image the watermark uses.
			 */
			Image	*	_img;

			/*!
			 *	@brief		The alignment of the watermark.
			 */
			WatermarkAlign	_alignment; 

			/*!
			 *	@brief		The opacity of the watermark.  Should be a value
			 *				in the range [0, 1], where 0 is completely transparent
			 *				and 1 is completely opaque.
			 */
			float	_opacity;

			/*!
			 *	@brief		The scale of the watermark.
			 *				If the alignment is FULL_SCREEN, this can cause tiling.
			 */
			float	_scale;
		};

		/*!
		 *	@brief		Parses a water mark xml specification and returns the corresponding
		 *				instance.
		 *
		 *	@param		node		The XML node containing the watermark definition.
		 *	@param		viewFldr	The path to the view configuration file.
		 *	@returns	A pointer to a water mark, if the definition was valid (NULL otherwise).
		 */
		Watermark *	parseWatermark( TiXmlElement * node, const std::string & viewFldr );
	}	// namespace Vis
}	// namespace Menge
#endif // __WATER_MARK_H__