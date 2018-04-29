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
 @file  SCBWriter.h
 @brief  Functionality for writing the crowd trajectories to a binary file.
 */

#ifndef __SCB_WRITER_H__
#define __SCB_WRITER_H__

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/BFSM/FSM.h"
#include "MengeCore/mengeCommon.h"

#include <fstream>
#include <string>

namespace Menge {

namespace Agents {

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Base exception class for scb writers
 */
class MENGE_API SCBException : public virtual Menge::MengeException {
 public:
  /*!
   @brief    Default constructor.
   */
  SCBException() : Menge::MengeException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  SCBException(const std::string& s) : Menge::MengeException(s) {}
};

/*!
 @brief    The fatal scb writers exception.
 */
class MENGE_API SCBFatalException : public SCBException, public Menge::MengeFatalException {
 public:
  /*!
   @brief    Default constructor.
   */
  SCBFatalException() : Menge::MengeException(), SCBException(), Menge::MengeFatalException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  SCBFatalException(const std::string& s)
      : Menge::MengeException(s), SCBException(), Menge::MengeFatalException() {}
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Exception raised for invalid scb version.
 */
class MENGE_API SCBVersionException : public SCBFatalException {
 public:
  /*!
   @brief    Default constructor.
   */
  SCBVersionException() : SCBFatalException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  SCBVersionException(const std::string& s) : SCBFatalException(s) {}
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Exception raised for file I/O errors
 */
class MENGE_API SCBFileException : public SCBException {
 public:
  /*!
   @brief    Default constructor.
   */
  SCBFileException() : SCBException() {}

  /*!
   @brief    Constructor with message.

   @param    s    The exception-specific message.
   */
  SCBFileException(const std::string& s) : SCBException(s) {}
};

// Forward declaration
class SCBFrameWriter;
class SimulatorInterface;

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Class responsible for writing the agent state of the simulator and fsm into a file.
 */
class SCBWriter {
 public:
  /*!
   @brief    Constructor for SCBWriter

   @param    pathName    The path for the desired output file.
   @param    version      A string representing the version to write out.
   @param    sim          A pointer to the simulator to process
   @throws  SCBVersionException  if the version string is not considered to be a valid version.
   @throws  SCBFileException if there is a problem opening the given path for writing.
   */
  SCBWriter(const std::string& pathName, const std::string& version, SimulatorInterface* sim);

  /*!
   @brief    Destructor.
   */
  ~SCBWriter();

  /*!
   @brief    Writes the current frame of the stored simulator to the file.

   @param    fsm    A pointer to the simulator's fsm
   */
  void writeFrame(BFSM::FSM* fsm);

 protected:
  /*!
   @brief    The frame writer -- defines the format of the frame's data.
   */
  SCBFrameWriter* _frameWriter;

  /*!
   @brief    The version of the scb file to be written.

   Version is represented by the integer _verstion[0]._version[1]
   */
  int _version[2];

  /*!
   @brief    A pointer to the simulator to write to the file
   */
  SimulatorInterface* _sim;

  /*!
   @brief  The file object for the scb stream to be written to.
   */
  std::ofstream _file;

  /*!
   @brief    Confirms that the given version is valid.

   Function has side-effects.  This must be called for the SCBWriter to work.

   @param    version    A string of the format "major"."minor" (e.g., 2.1) for the desired output
                      format.
   @returns  A boolean reporting if the version is valid (true) or invalid (false).
   */
  bool validateVersion(const std::string& version);

  /*!
   @brief    Writes the appropriate header to the open file.
   */
  void writeHeader();

  /*!
   @brief    Writes the header appropriate to major version 1 formats.
   */
  void writeHeader1_0();

  /*!
   @brief    Writes the header appropriate to major version 2 formats.
   */
  void writeHeader2_0();
};

/////////////////////////////////////////////////////////////////////
//                   Implementation of SCBFrameWriters
/////////////////////////////////////////////////////////////////////

/*!
 @brief    This base class for writing a single frame of simulation data to the scb file.
 */
class SCBFrameWriter {
 public:
  /*!
   @brief    Simple static variable for writing binary zeros to the file.
   */
  static const int ZERO;

  /*!
   @brief    Virtual destructor.
   */
  virtual ~SCBFrameWriter() {}

  /*!
   @brief    Function to write current frame's state to the file.

   @param    file    The file object to write to.
   @param    sim      A pointer to the simulator.
   @param    fsm      A pointer to the behavior fsm for the simulator.
   */
  virtual void writeFrame(std::ofstream& file, SimulatorInterface* sim, BFSM::FSM* fsm) = 0;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Writer for version 1.0

 The data for an agent consists of:
 4-byte float x-pos
 4-byte float y-pos
 4-byte float orientation (radians)
 */
class SCBFrameWriter1_0 : public SCBFrameWriter {
 public:
  virtual void writeFrame(std::ofstream& file, SimulatorInterface* sim, BFSM::FSM* fsm);
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Writer for version 2.0

 The data for an agent consists of:
 4-byte float x-pos
 4-byte float y-pos
 4-byte float orientation (radians)
 */
class SCBFrameWriter2_0 : public SCBFrameWriter {
 public:
  virtual void writeFrame(std::ofstream& file, SimulatorInterface* sim, BFSM::FSM* fsm);
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Writer for version 2.1

     The data for an agent consists of:
         4-byte float x-pos
         4-byte float y-pos
         4-byte float orientation (radians)
         4-byte float stateID - although stored as a float, the value will be an integer value.
 */
class SCBFrameWriter2_1 : public SCBFrameWriter {
 public:
  virtual void writeFrame(std::ofstream& file, SimulatorInterface* sim, BFSM::FSM* fsm);
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Writer for version 2.2

     The data for an agent consists of:
         4-byte float x-pos
         4-byte float y-pos
         4-byte float orientation (radians)
         4-byte float stateID - although stored as a float, the value will be an integer value.
         4-byte float x-vPref (the x-component of the agent's pref. velocity).
         4-byte float y-vPref (the y-component of the agent's pref. velocity).
         4-byte float x-vel (the x-component of the agent's current velocity).
         4-byte float y-vel (the y-component of the agent's current velocity).
 */
class SCBFrameWriter2_2 : public SCBFrameWriter {
 public:
  virtual void writeFrame(std::ofstream& file, SimulatorInterface* sim, BFSM::FSM* fsm);
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Writer for version 2.3

     The data for an agent consists of:
         4-byte float x-pos
         4-byte float y-pos
         4-byte float x-direction - the x-component of the unit vector pointing in the direction of
                                    orientation.
         4-byte float y-direction - the y-component of the unit vector pointing in the direction of
                                    orientation.
 */
class SCBFrameWriter2_3 : public SCBFrameWriter {
 public:
  virtual void writeFrame(std::ofstream& file, SimulatorInterface* sim, BFSM::FSM* fsm);
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Writer for version 2.4

 The data for an agent consists of:
 4-byte float x-pos
 4-byte float y-pos (elevation)
 4-byte float z-pos
 4-byte float orientation (radians)
 */
class SCBFrameWriter2_4 : public SCBFrameWriter {
 public:
  virtual void writeFrame(std::ofstream& file, SimulatorInterface* sim, BFSM::FSM* fsm);
};

}  // namespace Agents
}  // namespace Menge
#endif  // __SCB_WRITER_H__
