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

#include "os.h"
#include "Logger.h"
#include <iostream>
#include <cstdarg>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef _MSC_VER
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
// For linux absolute path
#include <sys/param.h>
#include <stdlib.h>
#endif

namespace Menge {

	namespace os {
		bool listdir( const std::string & path, StringList & contents, const std::string & wildcard ) {
			if ( ! path::exists( path ) ) {
				logger << Logger::ERR_MSG << "Path does not exist: " << path << "\n";
				return false;
			}
	#ifdef _MSC_VER
			WIN32_FIND_DATA FindFileData;
			HANDLE hFind = INVALID_HANDLE_VALUE;

			std::string wildPath = path + path::pathSep() + wildcard;
			hFind = FindFirstFile( wildPath.c_str(), &FindFileData);

			if( hFind == INVALID_HANDLE_VALUE ) {
				logger << Logger::ERR_MSG << "No files in path: " << path << "\n";
				return false;
			}

			std::string file = FindFileData.cFileName;
			if ( file != "." && file != ".." ) {
				contents.push_back( std::string( file ) );
			}


			while(FindNextFile(hFind, &FindFileData) != 0) {
				file = std::string( FindFileData.cFileName );
				if ( file != "." && file != ".." ) {
					contents.push_back( file );
				}
			}

			FindClose(hFind);
			return true;
	#else
			DIR *dp;
			struct dirent *dirp;
			struct stat st;

			if ( ( dp = opendir( path.c_str() ) ) == NULL ) {
				logger << Logger::ERR_MSG << "Error: invalid path\n";
				return false;
			}

			while ( ( dirp = readdir( dp ) ) != NULL ) {
				std::string name( dirp->d_name );
				std::string fullName = path + "/" + name;

				if ( name[0] == '.' ) continue;

				if ( stat( fullName.c_str(), &st ) == -1 ) continue;
				
				const bool isDirectory = ( st.st_mode & S_IFDIR ) != 0;
				if ( isDirectory ) continue;

				// TODO: I need to use the wildcard
				contents.push_back( name );

			}
			closedir( dp );
			return true;
	#endif

		}

		///////////////////////////////////////////////////////////////////////////////////

		FILE_SIZE fileSize( const std::string & path ) {
	#ifdef _MSC_VER
			WIN32_FIND_DATA FindFileData; 
			HANDLE hFind = INVALID_HANDLE_VALUE; 

			hFind = FindFirstFile( path.c_str(), &FindFileData );
			if ( hFind != INVALID_HANDLE_VALUE ) {
				return FindFileData.nFileSizeLow;
			} else {
				return 0;
			}
			FindClose(hFind);
		return 0;
	#else

			struct stat filestatus;
			if (-1 != stat(path.c_str(), &filestatus)) {
				return filestatus.st_size;
			} else {
				return 0;
			}

	#endif
	}

		///////////////////////////////////////////////////////////////////////////////////

		std::string fileSizeStr( FILE_SIZE size ) {
			static const char * SUFFIXES[] = { "b ", "Kb", "Mb", "Gb", "Tb", "Pb" };
			static const int MAX_SUF = 5;
			int sIndex = 0;
			double sz = size;
			
			while ( sz > 1000.0 ) {
				sz /= 1000.0;
				sIndex += 1;
			}
			std::stringstream ss;
			ss.precision( 3 );
			if ( sIndex > MAX_SUF ) {
				
				ss << size << " b";
			} else {
				ss << sz << " " << SUFFIXES[ sIndex ];
			}
			return ss.str();
		}

		///////////////////////////////////////////////////////////////////////////////////

		bool remove( const std::string & path ) {
	#ifdef _MSC_VER
			return 0 == DeleteFile( path.c_str() );
	#else
			return (0==remove ( path.c_str() ));
	#endif
		}

		///////////////////////////////////////////////////////////////////////////////////
	#ifdef _WIN32
		bool mkdir( const std::string & path ) {
			BOOL retval = CreateDirectory( path.c_str(), 0x0 );
			if ( !retval ) {
				DWORD err = GetLastError();
				if ( err == ERROR_ALREADY_EXISTS ) {
					// This is a funny case -- it prints an error message bug reports true
					//		because the folder actually exists.
					logger << Logger::ERR_MSG << "Cannot make directory.  Path already exists: " << path << "\n";
					return true;
				} else if ( err == ERROR_PATH_NOT_FOUND ) {
					logger << Logger::ERR_MSG << "Cannot make directory.  Intermediate directories missing: " << path << "\n";
					return false;
				}
			}
			return true;
		}

		///////////////////////////////////////////////////////////////////////////////////

		bool makedirs( const std::string & path ) {

			std::string fullpath;
			path::absPath( path, fullpath );

			if ( path::exists( fullpath ) ) {
				return true;
			}

			size_t pos = fullpath.find( path::pathSep() );
			while ( pos != std::string::npos ) {
				std::string parent = fullpath.substr( 0, pos );
				if ( !path::exists( parent ) ) {
					if ( ! mkdir( parent ) ) {
						return false;
					}
				}
				pos = fullpath.find( path::pathSep(), pos + 1 );
			}
			return mkdir( fullpath );
		}
	#else

		bool mkdir( const std::string & path ) {
			return ::mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		}

		///////////////////////////////////////////////////////////////////////////////////

		bool makedirs( const std::string & path ) {
			size_t err;
			std::string fullpath;
			path::absPath( path, fullpath );
			size_t pos = 0;
			bool ret_val = true;

			while((ret_val == true || err == EEXIST) && pos != std::string::npos)
			{
				pos = fullpath.find(path::pathSep(), pos + 1);
				ret_val = mkdir(fullpath.substr(0, pos));
				err = errno;
			}

			return !ret_val;
	    
		}
	    	

	#endif
		///////////////////////////////////////////////////////////////////////////////////

		namespace path {
			
			///////////////////////////////////////////////////////////////////////////////////

			bool absPath( const std::string & path, std::string & fullPath ) {
	            
				const unsigned int BUFSIZE = 4096;
				char BUFFER[ BUFSIZE ];
				
	#ifdef _MSC_VER

				
				char ** lppPart= {0x0};
				DWORD retval = GetFullPathName( path.c_str(), BUFSIZE, BUFFER, lppPart );


	#else
				std::string storage;
				std::string mutpath = path;
				std::string mutpath2;
				std::string splitStr;
				bool iterative = false;
				size_t err = 0;

				char * retval = realpath( path.c_str(), BUFFER );
				err = errno;
				
				if (retval == 0 && errno == ENOENT){
	            
					//split the path until we find a valid path
					//base case
					iterative = true;
					split(path,mutpath,splitStr);
					storage = splitStr;
					retval = realpath( mutpath.c_str(), BUFFER );
					err = errno;
					while (retval == 0 && errno == ENOENT){
	                
						//strip and split the string
						split(mutpath,mutpath2,splitStr);
						storage = join(2,splitStr.c_str(),storage.c_str());
						retval = realpath( mutpath2.c_str(), BUFFER );
						err = errno;
						mutpath = mutpath2;
					}
				}
	#endif           
				if (retval == 0){
					logger << Logger::ERR_MSG << "Invalid path: " << path << "\n";
					return false;
				} else {
					fullPath = std::string( BUFFER );
	#ifdef _MSC_VER    
	#else
					if (iterative)
						fullPath = join(2,fullPath.c_str(),storage.c_str()) ;
	#endif
					return true;
				}
	        
			}

			///////////////////////////////////////////////////////////////////////////////////

			bool isdir( const std::string & path ) { 

	#ifdef _MSC_VER
				DWORD attr = GetFileAttributes( path.c_str() );
				if ( attr == INVALID_FILE_ATTRIBUTES ) {
					logger << Logger::ERR_MSG << "Invalid file: " << path << "\n";
					return false;
				} else {
					return (attr & FILE_ATTRIBUTE_DIRECTORY) > 0;
				}
	#else
		struct stat s;
		if (stat(path.c_str(), &s) == 0) {
			if (s.st_mode & S_IFDIR) {
				return true;
			} else if (s.st_mode & S_IFREG) {
				return false;
			} else {
				logger << Logger::ERR_MSG << "Invalid file: " << path << "\n";
				return false;
			}
		}
		return false;

	#endif
		}

			///////////////////////////////////////////////////////////////////////////////////

			std::string join( unsigned int pathCount, ... ) {
				if ( pathCount == 0 ) return "";

				va_list arguments;
				va_start ( arguments, pathCount );

				if ( pathCount == 1 ) {
					return va_arg( arguments, const char* );
				} else {
					std::string fullPath = va_arg( arguments, const char* );
					for ( unsigned int i = 1; i < pathCount; ++i ) {
					std::string level( va_arg( arguments, const char* ) );
						fullPath += pathSep() + level;
					}
					va_end( arguments );
					return fullPath;
				}

				va_end ( arguments );
				return "";
			}

			///////////////////////////////////////////////////////////////////////////////////

			bool exists( const std::string & path ) {
	#ifdef _MSC_VER
				WIN32_FIND_DATA FindFileData; 
				HANDLE hFind = INVALID_HANDLE_VALUE; 

				hFind = FindFirstFile( path.c_str(), &FindFileData );

				bool valid = hFind != INVALID_HANDLE_VALUE;
				FindClose(hFind);
				return valid;
	#else
				struct stat s;
				if (stat(path.c_str(), &s) == 0) {
					if (s.st_mode & S_IFDIR) {
						return true;
					} else if (s.st_mode & S_IFREG) {
						return true;
					} else {
						logger << Logger::ERR_MSG << "Invalid file: " << path << "\n";
						return false;
					}
				}
				return false;
	#endif
			}

			///////////////////////////////////////////////////////////////////////////////////

			void split( const std::string & path, std::string & head, std::string & tail ) {
				size_t n = path.find_last_of('/');
				if ( n == std::string::npos ) {
					n = path.find_last_of( '\\' );
					if ( n == std::string::npos ) {
						head = "";
						tail = path;
						return;
					} 
				}
				head = path.substr( 0, n );
				tail = path.substr( n+1 );
			}

			///////////////////////////////////////////////////////////////////////////////////
		} // namespace os::path
	}   // namespace os
}	// namespace Menge