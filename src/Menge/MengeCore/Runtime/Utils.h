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
 *	@file		Utils.h
 *	@brief		A collection of convenience utilities.
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include "CoreConfig.h"
#include "MengeException.h"
#include <string>

#if defined(_MSC_VER)
    /*!
	 *	This is visual studio non-ansi compliant functionality
	 *	It doesnt like functions which declare specific types of exceptions thrown
	 *	This turns off the silly visual studio warning.
	 */
    #pragma warning(disable:4290)
#endif

namespace Menge {

	/*!
	 *	@brief		Exception thrown when a utility function fails..
	 */
	class UtilException : public virtual MengeException {
	public:
		/*!
		 *	@brief		Default constructor.
		 */
		UtilException() : MengeException() {}		

		/*!
		 *	@brief		Constructor with message.
		 *
		 *	@param		s		The exception-specific message.
		 */
		UtilException( const std::string & s ): MengeException(s) {}

	};

	/*!
	 *	@brief		Exception thrown when the utility functions have an error which cannot be
	 *				recovered from.
	 */
	class UtilFatalException : public UtilException, public MengeFatalException {
	public:
		/*!
		 *	@brief		Default constructor.
		 */
		UtilFatalException() : MengeException(), UtilException(), MengeFatalException() {}

		/*!
		 *	@brief		Constructor with message.
		 *
		 *	@param		s		The exception-specific message.
		 */
		UtilFatalException( const std::string & s ): MengeException(s), UtilException(), MengeFatalException() {}
	};

	/*!
	 *	@brief	Converts a string to a float.
	 *
	 *	@param		value		A string representing a float value.
	 *	@throws		A UtilException if the string is improperly formatted.
	 *	@returns		The float representation of the value held in the string.
	 */
	MENGE_API float toFloat( const std::string & value ) throw( UtilException );


	/*!
	 *	@brief	Converts a string to an int.
	 *
	 *	@param		value		A string representing a int value.
	 *	@throws		A UtilException if the string is improperly formatted.
	 *	@returns	The int representation of the value held in the string.
	 */
	MENGE_API int toInt( const std::string & value ) throw( UtilException );



	/*!
	 *	@brief	Converts a string to a size_t.
	 *
	 *	@param		value		A string representing a size_t value.
	 *	@throws		A UtilException if the string is improperly formatted.
	 *	@returns		The size_t representation of the value held in the string.
	 */
	MENGE_API size_t toSize_t( const std::string & value ) throw( UtilException );
}	// namespace Menge

#endif	 // __UTILS_H__