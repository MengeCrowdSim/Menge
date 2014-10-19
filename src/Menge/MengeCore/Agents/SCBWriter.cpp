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

#include "SCBWriter.h"
#include "SimulatorInterface.h"
#include "Core.h"

namespace Menge {

	namespace Agents {

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of SCBWriter
		/////////////////////////////////////////////////////////////////////

		SCBWriter::SCBWriter( const std::string & pathName, const std::string & version, SimulatorInterface * sim ):_frameWriter(0x0) {
			if ( !validateVersion( version ) ) {
				logger << Logger::ERR_MSG << "Invalid SCB version: " << version << "\n";
				throw SCBVersionException();
			}
			logger << Logger::INFO_MSG << "SCBWRITER: version: " << _version[0] << "." << _version[1] << "\n";
			_file.open( pathName.c_str(), std::ios::out | std::ios::binary );
			if ( ! _file.is_open() ) {
				throw SCBFileException();
			}
			_sim = sim;
			writeHeader();
		}

		/////////////////////////////////////////////////////////////////////

		SCBWriter::~SCBWriter() {
			if ( _file.is_open() ) _file.close();
			if ( _frameWriter ) delete _frameWriter;
		}

		/////////////////////////////////////////////////////////////////////

		bool SCBWriter::validateVersion( const std::string & version ) {
			bool valid = ( version == "1.0" ||	// a simple exhaustive list of valid versions
				version == "2.0" ||
				version == "2.1" ||
				version == "2.2" ||
				version == "2.3" ||
				version == "2.4" );
			if ( valid ) {
				// convert string to ints
				size_t dotPos = version.find_first_of( "." );
				_version[0] = atoi( version.substr( 0, dotPos ).c_str() );
				_version[1] = atoi( version.substr( dotPos + 1 ).c_str() );
				if ( _version[0] == 1 && _version[1] == 0 ) {
					_frameWriter = new SCBFrameWriter1_0();
				} else if ( _version[0] == 2 ) {
					if ( _version[1] == 0 ) {
						_frameWriter = new SCBFrameWriter2_0();
					} else if ( _version[1] == 1 ) {
						_frameWriter = new SCBFrameWriter2_1();
					} else if ( _version[1] == 2 ) {
						_frameWriter = new SCBFrameWriter2_2();
					} else if ( _version[1] == 3 ) {
						_frameWriter = new SCBFrameWriter2_3();
					} else if ( _version[1] == 4 ) {
						_frameWriter = new SCBFrameWriter2_4();
					}
				}
				assert( _frameWriter != 0x0 && "Valid version didn't produce a frame writer" );
			}
			return valid;
		}

		/////////////////////////////////////////////////////////////////////

		void SCBWriter::writeFrame( BFSM::FSM * fsm ) {
			_frameWriter->writeFrame( _file, _sim, fsm );
		}

		/////////////////////////////////////////////////////////////////////

		void SCBWriter::writeHeader() {
			_file << _version[0] << "." << _version[1] << (char)0x0;
			if ( _version[0] == 1 ) {
				writeHeader1_0();
			} else if ( _version[ 0 ] == 2 ) {
				writeHeader2_0();
			}
		}

		/////////////////////////////////////////////////////////////////////

		void SCBWriter::writeHeader1_0() {
			const size_t AGT_COUNT = _sim->getNumAgents();
			_file.write( (char*)&AGT_COUNT, sizeof(int) );
		}

		/////////////////////////////////////////////////////////////////////

		void SCBWriter::writeHeader2_0() {
			const size_t AGT_COUNT = _sim->getNumAgents();
			_file.write( (char*)&AGT_COUNT, sizeof(int) );
			float step = _sim->getTimeStep();
			_file.write( (char*)&step, sizeof( float ) );
			// write ids
			for ( size_t i = 0; i < AGT_COUNT; ++i ) {
				Agents::BaseAgent * agt = _sim->getAgent( i );
				unsigned int cID = static_cast< unsigned int >( agt->_class );
				_file.write( (char*)&cID, sizeof(unsigned int) );
			}
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of SCBFrameWriter
		/////////////////////////////////////////////////////////////////////

		const int SCBFrameWriter::ZERO = 0;

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of SCBFrameWriter1_0
		/////////////////////////////////////////////////////////////////////

		void SCBFrameWriter1_0::writeFrame( std::ofstream & file, SimulatorInterface * sim, BFSM::FSM * fsm ) {
			const size_t AGT_COUNT = sim->getNumAgents();
			for ( size_t i = 0; i < AGT_COUNT; ++i ) {
				Agents::BaseAgent * agt = sim->getAgent( i );
				const Vector2 & p = agt->_pos;
				file.write( (char*)&p, 2 * sizeof(float) );
				float orient = atan2( agt->_orient.y(), agt->_orient.x() );
				file.write( (char*)&orient, sizeof(float) );
			}
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of SCBFrameWriter2_0
		/////////////////////////////////////////////////////////////////////

		void SCBFrameWriter2_0::writeFrame( std::ofstream & file, SimulatorInterface * sim, BFSM::FSM * fsm ) {
			const size_t AGT_COUNT = sim->getNumAgents();
			for ( size_t i = 0; i < AGT_COUNT; ++i ) {
				Agents::BaseAgent * agt = sim->getAgent( i );
				const Vector2 & p = agt->_pos;
				file.write( (char*)&p, 2 * sizeof(float) );
				float orient = atan2( agt->_orient.y(), agt->_orient.x() );
				file.write( (char*)&orient, sizeof(float) );
			}
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of SCBFrameWriter2_1
		/////////////////////////////////////////////////////////////////////

		void SCBFrameWriter2_1::writeFrame( std::ofstream & file, SimulatorInterface * sim, BFSM::FSM * fsm ) {
			const size_t AGT_COUNT = sim->getNumAgents();
			for ( size_t i = 0; i < AGT_COUNT; ++i ) {
				Agents::BaseAgent * agt = sim->getAgent( i );
				const Vector2 & p = agt->_pos;
				file.write( (char*)&p, 2 * sizeof(float) );
				float orient = atan2( agt->_orient.y(), agt->_orient.x() );
				file.write( (char*)&orient, sizeof(float) );
				float state = (float)fsm->getAgentStateID( i );
				file.write( (char*)&state, sizeof(float) );
			}
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of SCBFrameWriter2_2
		/////////////////////////////////////////////////////////////////////

		void SCBFrameWriter2_2::writeFrame( std::ofstream & file, SimulatorInterface * sim, BFSM::FSM * fsm ) {
			const size_t AGT_COUNT = sim->getNumAgents();
			for ( size_t i = 0; i < AGT_COUNT; ++i ) {
				Agents::BaseAgent * agt = sim->getAgent( i );
				const Vector2 & p = agt->_pos;
				file.write( (char*)&p, 2 * sizeof(float) );
				float orient = atan2( agt->_orient.y(), agt->_orient.x() );
				file.write( (char*)&orient, sizeof(float) );
				float state = (float)fsm->getAgentStateID( i );
				file.write( (char*)&state, sizeof(float) );
				// pref velocity
				// NOTE: This does not use _velPref.getSpeed() because it may be modified
				//		by intention filters.  This factors those out.
				const Vector2 vDir = agt->_velPref.getPreferredVel();
				file.write( (char*)&vDir, 2 * sizeof(float) );
				// velocity
				const Vector2 & v = agt->_vel;
				file.write( (char*)&v, 2 * sizeof(float) );
			}
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of SCBFrameWriter2_3
		/////////////////////////////////////////////////////////////////////

		void SCBFrameWriter2_3::writeFrame( std::ofstream & file, SimulatorInterface * sim, BFSM::FSM * fsm ) {
			const size_t AGT_COUNT = sim->getNumAgents();
			for ( size_t i = 0; i < AGT_COUNT; ++i ) {
				Agents::BaseAgent * agt = sim->getAgent( i );
				const Vector2 & p = agt->_pos;
				file.write( (char*)&p, 2 * sizeof(float) );
				file.write( (char*)&agt->_orient, 2 * sizeof(float) );
			}
		}

		/////////////////////////////////////////////////////////////////////
		//                   Implementation of SCBFrameWriter2_4
		/////////////////////////////////////////////////////////////////////

		void SCBFrameWriter2_4::writeFrame( std::ofstream & file, SimulatorInterface * sim, BFSM::FSM * fsm ) {
			const size_t AGT_COUNT = sim->getNumAgents();
			for ( size_t i = 0; i < AGT_COUNT; ++i ) {
				Agents::BaseAgent * agt = sim->getAgent( i );
				const Vector2 & p = agt->_pos;
				float elevation = sim->getElevation( agt );
				file.write( (char*)&p._x, sizeof(float) );
				file.write( (char*)&elevation, sizeof(float) );
				file.write( (char*)&p._y, sizeof(float) );
				float orient = atan2( agt->_orient.y(), agt->_orient.x() );
				file.write( (char*)&orient, sizeof(float) );
			}
		}

		/////////////////////////////////////////////////////////////////////

		

	}	// namespace Agents
}	// namespace Menge