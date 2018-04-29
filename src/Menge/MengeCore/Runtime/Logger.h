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
 @file    Logger.h
 @brief    The specificaiton of a message logger for menge, such that all messages to the system get
          properly recorded.
 */

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "MengeCore/CoreConfig.h"

#include <fstream>
#include <string>

namespace Menge {

/////////////////////////////////////////////////////////////////////

/*!
 @brief    An html logger - writes messages to a formatted html file.
 */
class Logger {
 public:
  /*!
   @brief    Classfies interpretation of the subsequent streaming elements.
   */
  enum LogType {
    UNDEF_LOG,  ///< No interpretation provided.
    INFO_MSG,   ///< Benign information - reports status.
    WARN_MSG,   ///< Error encountered and handled.
    ERR_MSG     ///< Error encountered but *not* handled.
  };

  /*!
   @brief    Default constructor.
   */
  MENGE_API Logger();

  /*!
   @brief    Destructor
   */
  MENGE_API ~Logger();

  /*!
   @brief    Closes the logger down.
   */
  MENGE_API void close();

  /*!
   @brief    Writes a solid line to the logger.
   */
  MENGE_API void line();

  /*!
   @brief    Sets the logger's output file

   If there is an error in opening the file, the logger will default to writing to the console.

   @param    fileName    The file name to write the html to.
   */
  MENGE_API void setFile(const std::string& fileName);

  /*!
   @brief    Writes strings to the logger based on current status.

   @param    logger    A reference to a logger.
   @param    msg       The string to write.
   @returns  A reference to the logger streamed to.
   */
  friend MENGE_API Logger& operator<<(Logger& logger, const std::string& msg);

  /*!
   @brief    Writes strings to the logger based on current status.

   @param    logger    A reference to a logger.
   @param    msg       The string to write.
   @returns  A reference to the logger streamed to.
   */
  friend MENGE_API Logger& operator<<(Logger& logger, const char* msg);

  /*!
   @brief    Writes long unsigned int to the logger based on current status.

   @param    logger    A reference to a logger.
   @param    value      The value to write.
   @returns  A reference to the logger streamed to.
   */
  friend MENGE_API Logger& operator<<(Logger& logger, long unsigned int value);

#ifdef _WIN32
  /*!
   @brief    Writes a size_t to the logger based on current status. Only done in windows, as GCC
            complains.

   @param    logger    A reference to a logger.
   @param    value      The value to write.
   @returns  A reference to the logger streamed to.
   */
  friend MENGE_API Logger& operator<<(Logger& logger, size_t value);
#endif  // _WIN32

#if !defined _MSC_VER || defined _M_X64
  /*!
   @brief    Writes long unsigned int to the logger based on current status.

   @param    logger    A reference to a logger.
   @param    value    The value to write.
   @returns  A reference to the logger streamed to.
   */
  friend MENGE_API Logger& operator<<(Logger& logger, unsigned int value);
#endif

  /*!
   @brief    Writes int to the logger based on current status.

   @param    logger    A reference to a logger.
   @param    value      The value to write.
   @returns  A reference to the logger streamed to.
   */
  friend MENGE_API Logger& operator<<(Logger& logger, int value);

  /*!
   @brief    Writes float to the logger based on current status.

   @param    logger    A reference to a logger.
   @param    value      The value to write.
   @returns  A reference to the logger streamed to.
   */
  friend MENGE_API Logger& operator<<(Logger& logger, float value);

  /*!
   @brief    Writes double to the logger based on current status.

   @param    logger    A reference to a logger.
   @param    value      The value to write.
   @returns  A reference to the logger streamed to.
   */
  friend MENGE_API Logger& operator<<(Logger& logger, double value);

  /*!
   @brief    Changes the message status of the logger

   @param    logger    A reference to a logger.
   @param    type      The value to write.
   @returns  A reference to the logger streamed to.
   */
  friend MENGE_API Logger& operator<<(Logger& logger, Logger::LogType type);

 protected:
  /*!
   @brief    Writes the html header information to the given file.
   */
  void writeHeader();

  /*!
   @brief    Writes the html tail information to the given file.
   */
  void writeTail();

  /*!
   @brief    Process text.

   Modifies the string in place to make an html version of the text if the output file is valid,
   otherwise, simply returns leaving the string untouched.

   @param    input    The input text to write to the log.  The string
   */
  void processText(std::string& input);

  /*!
   @brief    Indicates if the output file is valid.
   */
  bool _validFile;

  /*!
   @brief    The file object for the html to be written to.
   */
  std::ofstream _file;

  /*!
   @brief    The current message type
   */
  LogType _streamType;
};

/*!
  @brief Globally available Logger

  The single, globally availably logger. This allows core files and plugins to use a single
  consistant logger object.
  */
extern MENGE_API Logger logger;

}  // namespace Menge

#endif  // __LOGGER_H__
