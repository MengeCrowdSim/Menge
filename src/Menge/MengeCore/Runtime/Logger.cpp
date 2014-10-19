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

#include "Logger.h"
#include <iostream>
#include "os.h"

namespace Menge {

	/////////////////////////////////////////////////////////////////////
	//					Implementation of Logger
	/////////////////////////////////////////////////////////////////////

	Logger logger;

	/////////////////////////////////////////////////////////////////////

	Logger::Logger():_validFile(false), _file(), _streamType(UNDEF_LOG) {
		// test for the existence of the style sheet
		if ( ! os::path::exists( "log.css" ) ) {
			std::ofstream cssFile;
			cssFile.open( "log.css", std::ios::out );
			if ( cssFile.is_open() ) {
				cssFile << "table, th, td {\n";
				cssFile << "\tborder-collapse:collapse;\n";
				cssFile << "\tborder: 1px solid grey;\n";
				cssFile << "}\n";
				cssFile << "table {\n";
				cssFile << "\ttable-layout : fixed;\n";
				cssFile << "\twidth: 950 px;\n";
				cssFile << "\tmax-width: 900 px;\n";
				cssFile << "\tbackground-color : #292929;\n";
				cssFile << "}\n";
				cssFile << "td {\n";
				cssFile << "\tmin-width: 200px;\n";
				cssFile << "}\n";
				cssFile << "#content {\n";
				cssFile << "\tposition : relative;\n";
				cssFile << "}\n";
				cssFile << "body, html {\n";
				cssFile << "\tbackground: #000000;\n";
				cssFile << "\twidth: 1000px;\n";
				cssFile << "\tfont-family: Arial;\n";
				cssFile << "\tfont-size: 16px;\n";
				cssFile << "\tcolor: #C0C0C0;\n";
				cssFile << "}\n";
				cssFile << "h1 {\n";
				cssFile << "\tfont-size : 50px;\n";
				cssFile << "\tline-height : 100px;\n";
				cssFile << "\tcolor : #FFFFFF;\n";
				cssFile << "\tborder-bottom : 1px dotted #888888;\n";
				cssFile << "}\n";
				cssFile << "#logo {\n";
				cssFile << "\tposition: absolute;\n";
				cssFile << "\ttop: 0px;\n";
				cssFile << "\tright: 0px;\n";
				cssFile << "}\n";
				cssFile << ".divider {\n";
				cssFile << "\tbackground : #DDD;\n";
				cssFile << "}\n";
				cssFile << ".box {\n";
				cssFile << "\tpadding : 0px;\n";
				cssFile << "}\n";
				cssFile << ".inf {\n";
				cssFile << "\tcolor: #C0C0C0;\n";
				cssFile << "\tmin-width : 1000px;\n";
				cssFile << "\tmax-width : 1000px;\n";
				cssFile << "}\n";
				cssFile << ".err {\n";
				cssFile << "\tcolor: #EE1100;\n";
				cssFile << "\tfont-weight: bold\n";
				cssFile << "}\n";
				cssFile << ".war {\n";
				cssFile << "\tcolor: #FFCC00;\n";
				cssFile << "\tfont-weight: bold\n";
				cssFile << "}";
				
				cssFile.close();
			} else {
				std::cout << "Unable to create style sheet for logger\n";
			}
		}
	}

	/////////////////////////////////////////////////////////////////////

	Logger::~Logger() {
		close();
	}

	/////////////////////////////////////////////////////////////////////

	void Logger::close() {
		if ( _validFile ) {
			writeTail();
			_file.close();
			_validFile = false;
		}
	}

	/////////////////////////////////////////////////////////////////////

	void Logger::line() {
		if ( _validFile ) {
			if ( _streamType != UNDEF_LOG ) {
				_file << "</td>\n\t</tr>\n";
			}
			_file << "\t<tr>\n\t\t<td class=\"divider\"/>\n\t</tr>\n";
		} else {
			if ( _streamType != Logger::UNDEF_LOG ) {
				std::cout << "\n";
			}
			std::cout << "============================\n";
		}
		_streamType = UNDEF_LOG;
	}

	/////////////////////////////////////////////////////////////////////

	void Logger::setFile( const std::string & fileName ) {
		_file.open( fileName.c_str(), std::ios::out );
		_validFile = _file.is_open();
		if ( _validFile ) {
			writeHeader();
		} else {
			std::cout << "Error opening file for writing a log\n";
			std::cout << "\tAll output will be written to the console\n";
		}
	}

	/////////////////////////////////////////////////////////////////////

	void Logger::writeHeader() {
		_file << "<html>\n";
		_file << "<head>\n";
		_file << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n";
		_file << "<title>Menge Log</title>\n";
		_file << "<link rel=\"stylesheet\" type=\"text/css\" href=\"log.css\" media=\"screen\"/>\n";
		_file << "</head>\n\n";

		_file << "<body>\n";
		_file << "<div id=\"content\">\n";
		_file << "<h1>Menge Log</h1>\n";
		_file << "<img id=\"logo\" src=\"mengeLogo.png\"/>\n";
		_file << "<div class=\"box\">\n";
		_file << "<table>\n";

	}

	/////////////////////////////////////////////////////////////////////

	void Logger::writeTail() {
		if ( _streamType != UNDEF_LOG ) {
			_file << "</td>\n\t</tr>\n";
		}
		_file << "</table>\n";
		_file << "</div>\n";
		_file << "</div>\n";
		_file << "</body>\n";
		_file << "</html>\n";
	}

	/////////////////////////////////////////////////////////////////////

	/*!
	 *	@brief		Simple helper function for doing text search and replace
	 *
	 *	@param		source		The string to change.
	 *	@param		find		The sub-strings to find and replace in the main string.
	 *	@param		replace		The sub-string to input into the main string.
	 */
	inline void findAndReplace( std::string & source, const std::string & find, const std::string & replace)
	{
		size_t fLen = find.size();
		size_t rLen = replace.size();
		for ( size_t pos = 0; ( pos = source.find( find, pos ) ) != std::string::npos; pos += rLen ) {
			source.replace(pos, fLen, replace);
		}
	}

	/////////////////////////////////////////////////////////////////////

	void Logger::processText( std::string & input ) {
		if ( _validFile ) {
			// tags
			findAndReplace( input, std::string( "<" ), std::string( "&lt;" ) );
			findAndReplace( input, std::string( ">" ), std::string( "&gt;" ) );
			// carriage returns
			findAndReplace( input, std::string( "\n" ), std::string( "<br>" ) );
			// tabs
			findAndReplace( input, std::string( "\t" ), std::string( "&nbsp;&nbsp;&nbsp;&nbsp;" ) );
		}
	}

	/////////////////////////////////////////////////////////////////////

	Logger & operator<<( Logger & logger, const std::string & msg ) {
		std::string msgStr( msg );
		logger.processText( msgStr );
		if ( logger._validFile ) {
			logger._file << msgStr;
		} else {
			std::cout << msgStr;
		}
		return logger;
	}

	/////////////////////////////////////////////////////////////////////

	Logger & operator<<( Logger & logger, const char * msg ) {
		std::string msgStr( msg );
		logger.processText( msgStr );
		if ( logger._validFile ) {
			logger._file << msgStr;
		} else {
			std::cout << msgStr;
		}
		return logger;
	}

	/////////////////////////////////////////////////////////////////////

	Logger & operator<<( Logger & logger, long unsigned int value ) {
		if ( logger._validFile ) {
			logger._file << value;
		} else {
			std::cout << value;
		}
		return logger;
	}
	 
	#if defined(_MSC_VER)

	/////////////////////////////////////////////////////////////////////

	Logger & operator<<( Logger & logger, size_t value ) {
		if ( logger._validFile ) {
			logger._file << value;
		} else {
			std::cout << value;
		}
		return logger;
	}

	#else 
	/////////////////////////////////////////////////////////////////////

	Logger & operator<<( Logger & logger, unsigned int value ) {
		if ( logger._validFile ) {
			logger._file << value;
		} else {
			std::cout << value;
		}
		return logger;
	}

	#endif
	/////////////////////////////////////////////////////////////////////


	Logger & operator<<( Logger & logger, int value ) {
		if ( logger._validFile ) {
			logger._file << value;
		} else {
			std::cout << value;
		}
		return logger;
	}

	/////////////////////////////////////////////////////////////////////

	Logger & operator<<( Logger & logger, float value ) {
		if ( logger._validFile ) {
			logger._file << value;
		} else {
			std::cout << value;
		}
		return logger;
	}

	/////////////////////////////////////////////////////////////////////

	Logger & operator<<( Logger & logger, double value ) {
		if ( logger._validFile ) {
			logger._file << value;
		} else {
			std::cout << value;
		}
		return logger;
	}

	/////////////////////////////////////////////////////////////////////

	Logger & operator<<( Logger & logger, Logger::LogType type ) {
		if ( logger._validFile ) {
			if ( logger._streamType != Logger::UNDEF_LOG ) {
				logger._file << "</td>\n\t</tr>\n";
			} else {
				type = Logger::INFO_MSG;
			}
			logger._file << "\t<tr>\n\t\t<td class=\"";
			switch( type ) {
				case Logger::UNDEF_LOG:
				case Logger::INFO_MSG:
					logger._file << "inf";
					break;
				case Logger::WARN_MSG:
					logger._file << "war";
					break;
				case Logger::ERR_MSG:
					logger._file << "err";
					break;
			}
			logger._file << "\">";
			
		} else {
			if ( logger._streamType != Logger::UNDEF_LOG ) {
				std::cout << "\n";
			} else {
				type = Logger::INFO_MSG;
			}
			switch( type ) {
				case Logger::UNDEF_LOG:
					std::cout << "?  ";
					break;
				case Logger::INFO_MSG:
					std::cout << "-  ";
					break;
				case Logger::WARN_MSG:
					std::cout << "!  ";
					break;
				case Logger::ERR_MSG:
					std::cout << "X  ";
					break;
			}
		}
		logger._streamType = type;
		return logger;
	}

}	// namespace Menge