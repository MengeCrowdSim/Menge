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
 *	@file	SCBWriter.h
 *	@brief	Functionality for writing the crowd trajectories to a binary file.
 */

#ifndef __SCB_WRITER_H__
#define __SCB_WRITER_H__

#include <string>
#include <fstream>
#include "mengeCommon.h"
#include "FSM.h"
#include "BaseAgent.h"

namespace Menge {

	namespace Agents {

		/////////////////////////////////////////////////////////////////////
		
		/*!
		 *	@brief		Base exception class for scb writers
		 */
		class MENGE_API SCBException : public virtual Menge::MengeException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			SCBException() : Menge::MengeException() {}		

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			SCBException( const std::string & s ): Menge::MengeException(s) {}
		};

		/*!
		 *	@brief		The fatal scb writers exception.
		 */
		class MENGE_API SCBFatalException : public SCBException, public Menge::MengeFatalException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			SCBFatalException() : Menge::MengeException(), SCBException(), Menge::MengeFatalException() {}

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			SCBFatalException( const std::string & s ): Menge::MengeException(s), SCBException(), Menge::MengeFatalException() {}
		};

		/////////////////////////////////////////////////////////////////////
		
		/*!
		 *	@brief		Exception raised for invalid scb version.
		 */
		class MENGE_API SCBVersionException : public SCBFatalException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			SCBVersionException() : SCBFatalException() {}		

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			SCBVersionException( const std::string & s ): SCBFatalException(s) {}
		};

		/////////////////////////////////////////////////////////////////////
		
		/*!
		 *	@brief		Exception raised for file I/O errors
		 */
		class MENGE_API SCBFileException : public SCBException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			SCBFileException() : SCBException() {}		

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			SCBFileException( const std::string & s ): SCBException(s) {}
		};

		// Forward declaration
		class SCBFrameWriter;
		class SimulatorInterface;

		/////////////////////////////////////////////////////////////////////
		
		/*!
		 *	@brief		Class responsible for writing the agent state of the simulator and
		 *				fsm into a file.
		 */
		class SCBWriter {
		public:
			/*!
			 *	@brief		Constructor for SCBWriter
			 *
			 *	@param		pathName		The path for the desired output file.
			 *	@param		version			A string representing the version to write out.
			 *	@param		sim				A pointer to the simulator to process
			 *	@throws		SCBVersionException		If the version string is not considered to be
			 *										a valid version.
			 *	@throws		SCBFileException		If there is a problem opening the given path
			 *										for writing.
			 */
			SCBWriter( const std::string & pathName, const std::string & version, SimulatorInterface * sim );

			/*!
			 *	@brief		Destructor.
			 */
			~SCBWriter();

			/*!
			 *	@brief		Writes the current frame of the stored simulator to the file.
			 *
			 *	@param		fsm		A pointer to the simulator's fsm
			 */
			void writeFrame( BFSM::FSM * fsm );
			
		protected:
			/*!
			 *	@brief		The frame writer -- defines the format of the frame's data.
			 */
			SCBFrameWriter * _frameWriter;

			/*!
			 *	@brief		The version of the scb file to be written.  
			 *				Version is represented by the integer _verstion[0]._version[1]
			 */
			int	_version[2];	

			/*!
			 *	@brief		A pointer to the simulator to write to the file
			 */
			SimulatorInterface * _sim;

			/*!
			 *	@brief	The file object for the scb stream to be written to.
			 */
			std::ofstream _file;

			/*!
			 *	@brief		Confirms that the given version is valid.
			 *				Function has side-effects.  This must be called for the SCBWriter to work.
			 *
			 *	@param		version		A string of the format "major"."minor" (e.g., 2.1)
			 *							for the desired output format.
			 *	@returns	A boolean reporting if the version is valid (true) or invalid (false).
			 */
			bool validateVersion( const std::string & version );

			/*!
			 *	@brief		Writes the appropriate header to the open file.
			 */
			void writeHeader();

			/*!
			 *	@brief		Writes the header appropriate to major version 1 formats.
			 */
			void writeHeader1_0();

			/*!
			 *	@brief		Writes the header appropriate to major version 2 formats.
			 */
			void writeHeader2_0();
		};

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of SCBFrameWriters
		/////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		This base class for writing a single frame of simulation data
		 *				to the scb file.
		 */
		class SCBFrameWriter {
		public:
			/*!
			 *	@brief		Simple static variable for writing binary zeros to the file.
			 */
			static const int ZERO;

			/*!
			 *	@brief		Function to write current frame's state to the file.
			 *
			 *	@param		file		The file object to write to.
			 *	@param		sim			A pointer to the simulator.
			 *	@param		fsm			A pointer to the behavior fsm for the simulator.
			 */
			virtual void writeFrame( std::ofstream & file, SimulatorInterface * sim, BFSM::FSM * fsm ) = 0;
		};

		
		/////////////////////////////////////////////////////////////////////
		
		/*!
		 *	@brief		Writer for version 1.0
		 *
		 *				The data for an agent consists of: 
		 *					4-byte float x-pos
		 *					4-byte float y-pos
		 *					4-byte float orientation (radians)
		 */
		class SCBFrameWriter1_0 : public SCBFrameWriter{
		public:
			virtual void writeFrame( std::ofstream & file, SimulatorInterface * sim, BFSM::FSM * fsm );
		};

		/////////////////////////////////////////////////////////////////////
		
		/*!
		 *	@brief		Writer for version 2.0
		 *
		 *				The data for an agent consists of: 
		 *					4-byte float x-pos
		 *					4-byte float y-pos
		 *					4-byte float orientation (radians)
		 */
		class SCBFrameWriter2_0 : public SCBFrameWriter {
		public:
			virtual void writeFrame( std::ofstream & file, SimulatorInterface * sim, BFSM::FSM * fsm );
		};

		/////////////////////////////////////////////////////////////////////
		
		/*!
		 *	@brief		Writer for version 2.1
		 *
		 *				The data for an agent consists of: 
		 *					4-byte float x-pos
		 *					4-byte float y-pos
		 *					4-byte float orientation (radians)
		 *					4-byte float stateID - although stored as a float, the value will be
		 *						an integer value.
		 */
		class SCBFrameWriter2_1 : public SCBFrameWriter {
		public:
			virtual void writeFrame( std::ofstream & file, SimulatorInterface * sim, BFSM::FSM * fsm );
		};

		/////////////////////////////////////////////////////////////////////
		
		/*!
		 *	@brief		Writer for version 2.2
		 *
		 *				The data for an agent consists of: 
		 *					4-byte float x-pos
		 *					4-byte float y-pos
		 *					4-byte float orientation (radians)
		 *					4-byte float stateID - although stored as a float, the value will be
		 *						an integer value.
		 *					4-byte float x-vPref (the x-component of the agent's preferred velocity).
		 *					4-byte float y-vPref (the y-component of the agent's preferred velocity).
		 *					4-byte float x-vel (the x-component of the agent's current velocity).
		 *					4-byte float y-vel (the y-component of the agent's current velocity).
		 */
		class SCBFrameWriter2_2 : public SCBFrameWriter {
		public:
			virtual void writeFrame( std::ofstream & file, SimulatorInterface * sim, BFSM::FSM * fsm );
		};

		/////////////////////////////////////////////////////////////////////
		
		/*!
		 *	@brief		Writer for version 2.3
		 *
		 *				The data for an agent consists of: 
		 *					4-byte float x-pos
		 *					4-byte float y-pos
		 *					4-byte float x-direction - the x-component of the unit vector
		 *						pointing in the direction of orientation.
		 *					4-byte float y-direction - the y-component of the unit vector
		 *						pointing in the direction of orientation.
		 */
		class SCBFrameWriter2_3 : public SCBFrameWriter {
		public:
			virtual void writeFrame( std::ofstream & file, SimulatorInterface * sim, BFSM::FSM * fsm );
		};

		/////////////////////////////////////////////////////////////////////
		
		/*!
		 *	@brief		Writer for version 2.4
		 *
		 *				The data for an agent consists of: 
		 *					4-byte float x-pos 
		 *					4-byte float y-pos (elevation)
		 *					4-byte float z-pos
		 *					4-byte float orientation (radians)
		 */
		class SCBFrameWriter2_4 : public SCBFrameWriter {
		public:
			virtual void writeFrame( std::ofstream & file, SimulatorInterface * sim, BFSM::FSM * fsm );
		};

	}	// namespace Agents
}	// namespace Menge
#endif // __SCB_WRITER_H__
