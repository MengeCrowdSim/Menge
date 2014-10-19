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
 *	@file		Logger.h
 *	@brief		The specificaiton of a message logger for menge, such that
 *				all messages to the system get properly recorded.
 */

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include "CoreConfig.h"
#include <fstream>

namespace Menge {

	/////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		An html logger - writes messages to a formatted html file.
	 */
	class Logger {
	public:
		/*!
		 *	@brief		Classfies interpretation of the subsequent 
		 *				streaming elements.
		 */
		enum LogType {
			UNDEF_LOG,		///< No interpretation provided.
			INFO_MSG,		///< Benign information - reports status.
			WARN_MSG,		///< Error encountered and handled.
			ERR_MSG			///< Error encountered but *not* handled.
		};

		/*!
		 *	@brief		Default constructor.
		 */
		MENGE_API Logger();

		/*!
		 *	@brief		Destructor
		 */
		MENGE_API ~Logger();

		/*!
		 *	@brief		Closes the logger down.
		 */
		MENGE_API void close();

		/*! 
		 *	@brief		Writes a solid line to the logger.
		 */
		MENGE_API void line();

		/*!
		 *	@brief		Sets the logger's output file
		 *
		 *	If there is an error in opening the file, the logger will default to
		 *	writing to the console.
		 *
		 *	@param		fileName		The file name to write the html to.
		 */
		MENGE_API void setFile( const std::string & fileName );

		/*!
		 *	@brief		Writes strings to the logger based on current status.
		 *
		 *	@param		logger		A reference to a logger.
		 *	@param		msg			The string to write.
		 *	@returns	A reference to the logger streamed to.
		 */
		friend MENGE_API Logger & operator<<( Logger & logger, const std::string & msg );

		/*!
		 *	@brief		Writes strings to the logger based on current status.
		 *
		 *	@param		logger		A reference to a logger.
		 *	@param		msg			The string to write.
		 *	@returns	A reference to the logger streamed to.
		 */
		friend MENGE_API Logger & operator<<( Logger & logger, const char * msg );


			/*!
		 *	@brief		Writes long unsigned int to the logger based on current status.
		 *
		 *	@param		logger		A reference to a logger.
		 *	@param		value		The value to write.
		 *	@returns	A reference to the logger streamed to.
		 */
		friend MENGE_API Logger & operator<<( Logger & logger, long unsigned int value );
	        
		#if defined(_MSC_VER)
		   /*!
		 *	@brief		Writes a size_t to the logger based on current status. Only done in windows, as GCC complains.
		 *
		 *	@param		logger		A reference to a logger.
		 *	@param		value		The value to write.
		 *	@returns	A reference to the logger streamed to.
		 */
		friend MENGE_API Logger & operator<<( Logger & logger, size_t value );
		#endif

		/*!
		 *	@brief		Writes long unsigned int to the logger based on current status.
		 *
		 *	@param		logger		A reference to a logger.
		 *	@param		value		The value to write.
		 *	@returns	A reference to the logger streamed to.
		 */
		friend MENGE_API Logger & operator<<( Logger & logger, unsigned int value );

		/*!
		 *	@brief		Writes int to the logger based on current status.
		 *
		 *	@param		logger		A reference to a logger.
		 *	@param		value		The value to write.
		 *	@returns	A reference to the logger streamed to.
		 */
		friend MENGE_API Logger & operator<<( Logger & logger, int value );

		/*!
		 *	@brief		Writes float to the logger based on current status.
		 *
		 *	@param		logger		A reference to a logger.
		 *	@param		value		The value to write.
		 *	@returns	A reference to the logger streamed to.
		 */
		friend MENGE_API Logger & operator<<( Logger & logger, float value );

		/*!
		 *	@brief		Writes double to the logger based on current status.
		 *
		 *	@param		logger		A reference to a logger.
		 *	@param		value		The value to write.
		 *	@returns	A reference to the logger streamed to.
		 */
		friend MENGE_API Logger & operator<<( Logger & logger, double value );

		/*!
		 *	@brief		Changes the message status of the logger
		 *
		 *	@param		logger		A reference to a logger.
		 *	@param		type		The value to write.
		 *	@returns	A reference to the logger streamed to.
		 */
		friend MENGE_API Logger & operator<<( Logger & logger, Logger::LogType type );

	protected:
		/*!
		 *	@brief		Writes the html header information to the given file.
		 */
		void writeHeader();

		/*!
		 *	@brief		Writes the html tail information to the given file.
		 */
		void writeTail();

		/*!
		 *	@brief		Process text.
		 *
		 *	Modifies the string in place to make an html version of the text if the output
		 *	file is valid, otherwise, simply returns leaving the string untouched.
		 *
		 *	@param		input		The input text to write to the log.  The string
		 */
		void processText( std::string & input );

		/*!
		 *	@brief		Indicates if the output file is valid.
		 */
		bool	_validFile;

		/*!
		 *	@brief		The file object for the html to be written to.
		 */
		std::ofstream _file;

		/*!
		 *	@brief		The current message type
		 */
		LogType _streamType;
	};


	/*!
	  *  @brief Globally available Logger
	  *
	  * The single, globally availably logger. This allows core files 
	  * and plugins to use a single consistant logger object.
	  */
	extern MENGE_API Logger logger;

}	// namespace Menge

#endif // __LOGGER_H__
