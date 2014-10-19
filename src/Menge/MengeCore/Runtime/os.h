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
 *	@file		os.h
 *	@brief		Various operating-system-dependent file-system operations.
 *
 *	NOTE: The interface is modeled on python's os module.
 */

#ifndef __OS_H__
#define __OS_H__

#include <string>
#include <vector>
#include <list>
#include "CoreConfig.h"

#ifdef _WIN32
#include "windows.h"
/*!
 *	@brief		The type for reporting file sizes.
 */
typedef DWORD FILE_SIZE;
#else
/*!
 *	@brief		The type for reporting file sizes.
 */
typedef unsigned int FILE_SIZE;
#endif	// _WIN32

/*!
 *	@brief		A list of strings.
 */
typedef std::list< std::string > StringList;

/*!
 *	@brief		An interator into a list of strings.
 */
typedef StringList::iterator StringListItr;

/*!
 *	@brief		An const interator into a list of strings.
 */
typedef StringList::const_iterator StringListCItr;

namespace Menge {

	/*!
	 *	@namespace		os
	 *	@brief			A compilation of file system operations.
	 *	
	 *	These are based loosely on the pythong os module and the interface
	 *	is similar.
	 */
	namespace os {
		/*!
		 *	@brief		Creates a list of all of the files in the given directory.
		 *
		 *	@param		path		The path to a folder to search in.
		 *	@param		contents	A vector of strings to populate with file names.
		 *	@param		wildcard	The wild card to apply -- defaults to *.
		 *	@returns	True if the folder exists and was successfully read and the 
		 *				contents of the StringList are meaningful.  False otherwise.
		 */
		MENGE_API bool listdir( const std::string & path, StringList & contents, const std::string & wildcard="*" );

		/*!
		 *	@brief		Returns the file size (in bytes) of the file in the given path
		 *
		 *	@param		path		The path to the file in question.
		 *	@returns	The size of the file or zero, if it is not a file or if there
		 *				is an error in determining the size.
		 */
		MENGE_API FILE_SIZE fileSize( const std::string & path );

		/*!
		 *	@brief		Formats the file size (in bytes) into an easily readable string.
		 *
		 *	@param		size		The number of bytes.
		 *	@returns	A readable string.  Limits the significant digits to just a few
		 *				and scales the units (bytes, kb, Mb, Gb, etc.) appropriately.
		 */
		MENGE_API std::string fileSizeStr( FILE_SIZE size );

		/*!
		 *	@brief		Removes the file located at the given path.
		 *
		 *	@param		path		The path to the file to remove.
		 *	@returns	True if the file was successfully removed, false otherwise.
		 */
		MENGE_API bool remove( const std::string & path );


		/*!
		 *	@brief		Creates the indicated folder.
		 *
		 *	This assumes that in the path given, all folders exist
		 *	except the final folder.
		 *
		 *	@param		path		The path of the folder to create.
		 *	@returns	True if the folder was successfully created, 
		 *				false otherwise.
		 */
		MENGE_API bool mkdir( const std::string & path );

		/*!
		 *	@brief		Creates the folder given, and all missing
		 *				folders in the path to that folder.
		 *
		 *	@param		path		The path of the folder to create.
		 *	@returns	True if the folder was successfully created, 
		 *				false otherwise.
		 */
		MENGE_API bool makedirs( const std::string & path );


		/*!
		 *	@namespace		os::path
		 *	@brief			Aggregation of path-oriented, file system operations.
		 */
		namespace path {
			/*!
			 *	@brief		Returns the absolute path of the given file resource
			 *
			 *	e.g. if '.' is given, c:\\program files\test might be returned.
			 *
			 *	@param		path		The path to the target resource.
			 *	@param		fullPath	A string which will contain the full path
			 *	@returns	True if the full path could be found and placed in
			 *				fullPath, false otherwise.
			 */
	  		MENGE_API bool absPath( const std::string & path, std::string & fullPath );

			/*!
			 *	@brief		Reports if the path is a directory
			 *
			 *	Bad files are considered to NOT be directories.
			 *
			 *	@param		path	The path to a file location
			 *	@returns	True if the path resource is a directory, false
			 *				otherwise.
			 */
			MENGE_API bool isdir( const std::string & path );

			/*!
			 *	@brief		Joins multiple strings into a single path
			 *
			 *	@param		pathCount		The number of strings to join.
			 *	@param		...				The strings.  They must be char * (and not strings).
			 *	@returns	A string containing the joined path.
			 */
			MENGE_API std::string join( unsigned int pathCount, ... );

			/*!
			 *	@brief		Reports if a file object exists
			 *
			 *	@param		path		The path to the object in question.
			 *	@returns	True if the object indicated by path exists.
			 */
			MENGE_API bool exists( const std::string & path );

			/*!
			 *	@brief		Splits the path at the last separator and returns
			 *				the two parts.
			 *
			 *	This is useful for popping levels off of a path to work closer
			 *	and closer to the root directory.
			 *
			 *	@param		path		The path to split.
			 *	@param		head		The "head" of the path.
			 *	@param		tail		The "tail" of the path (the element beyond
			 *							the last separator.
			 */
			MENGE_API void split( const std::string & path, std::string & head, std::string & tail );

			/*!
			 *	@brief		The separator for paths based on OS
			 *
			 *	@returns	A character representing the path separater for the
			 *				OS.
			 */
			inline char pathSep() {
	#ifdef _WIN32
			  return '\\';
	#else
			  return '/';
	#endif
			} 
		}
	}	// namespace os
}	//namespace Menge

#endif  // __OS_H__
