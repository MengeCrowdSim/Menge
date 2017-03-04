/*
 Menge Crowd Simulation Framework

 Copyright and trademark 2012-17 University of North Carolina at Chapel Hill

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0
 or
    LICENSE.txt in the root of the Menge repository.

 Any questions or comments should be sent to the authors menge@cs.unc.edu

 <http://gamma.cs.unc.edu/Menge/>
*/

/*!
 *	@file		Utils.h
 *	@brief		A collection of convenience utilities.
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include "MengeCore/CoreConfig.h"
#include "MengeCore/MengeException.h"
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
		UtilFatalException( const std::string & s ) : MengeException(s), UtilException(),
													  MengeFatalException() {}
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
