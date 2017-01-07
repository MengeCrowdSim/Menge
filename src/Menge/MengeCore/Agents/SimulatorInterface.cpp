/*

License

Menge
Copyright � and trademark � 2012-14 University of North Carolina at Chapel Hill. 
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

#include "MengeCore/Agents/SimulatorInterface.h"

#include "MengeCore/Core.h"
#include "MengeCore/Agents/Obstacle.h"
#include "MengeCore/Agents/Elevations/ElevationFlat.h"
#include "MengeCore/Agents/SCBWriter.h"
#include "MengeCore/Agents/SpatialQueries/SpatialQuery.h"
#include "MengeCore/BFSM/FSM.h"

namespace Menge {

	namespace Agents {

		////////////////////////////////////////////////////////////////////////////
		//			Implementation of SimulatorInterface
		////////////////////////////////////////////////////////////////
		
		float SimulatorInterface::LOGICAL_TIME_STEP = 0.1f;

		float SimulatorInterface::TIME_STEP = 0.1f;

		size_t SimulatorInterface::SUB_STEPS = 0;

		////////////////////////////////////////////////////////////////////////////

		SimulatorInterface::SimulatorInterface() :
			XMLSimulatorBase(), _globalTime( 0.f ), _elevation( 0x0 ), _spatialQuery( 0x0 ),
			_fsm( 0x0 ), _scbWriter( 0x0 ), _isRunning( true ), _maxDuration( 100.f ) {
		}

		////////////////////////////////////////////////////////////////////////////

		SimulatorInterface::~SimulatorInterface() {
			if ( _fsm ) delete _fsm;
			if ( _spatialQuery != 0x0 ) _spatialQuery->destroy();
			if ( _elevation ) _elevation->destroy();
		}

		////////////////////////////////////////////////////////////////////////////

		void SimulatorInterface::setBFSM( BFSM::FSM * fsm ) {
			_fsm = fsm;
		}

		////////////////////////////////////////////////////////////////////////////

		bool SimulatorInterface::step() {
			const int agtCount = static_cast<int>( getNumAgents() );
			if ( _isRunning ) {
				if ( _scbWriter ) _scbWriter->writeFrame( _fsm );
				if ( _globalTime >= _maxDuration ) {
					_isRunning = false;
				} else {
					for ( size_t i = 0; i <= SUB_STEPS; ++i ) {
						try {
							// TODO: doStep for FSM is a *bad* name; it should be "evaluate".
							_isRunning = !_fsm->doStep();
							doStep();
							_fsm->doTasks();
						} catch ( BFSM::FSMFatalException & e ) {
							logger << Logger::ERR_MSG << "Error in updating the finite state ";
							logger << "machine -- stopping!\n";
							logger << "\t" << e.what() << "\n";
							_isRunning = false;
						}
					}
				}
			}
			return _isRunning;
		}

		////////////////////////////////////////////////////////////////////////////

		float SimulatorInterface::getElevation( const BaseAgent * agent ) const { 
			return _elevation->getElevation( agent ); 
		}

		////////////////////////////////////////////////////////////////////////////

		float SimulatorInterface::getElevation( const Vector2 & point ) const { 
			return _elevation->getElevation( point ); 
		}
		
		////////////////////////////////////////////////////////////////////////////

		void SimulatorInterface::setElevationInstance( Elevation * elevation ) {
			assert( _elevation == 0x0 && "Trying to set the elevation that already exists" );
			_elevation = elevation;
		}
		
		////////////////////////////////////////////////////////////////

		void SimulatorInterface::setSpatialQuery( SpatialQuery * spatialQuery ) {
			assert( _spatialQuery == 0x0 &&
					"Trying to set the spatial query when one already exists" );
			_spatialQuery = spatialQuery;
		}


		////////////////////////////////////////////////////////////////

		bool SimulatorInterface::queryVisibility( const Vector2& point1, const Vector2& point2,
												  float radius ) const {
			return _spatialQuery->queryVisibility( point1, point2, radius );
		}

		////////////////////////////////////////////////////////////////

		void SimulatorInterface::finalize() {
			if ( _fsm == 0x0 ) throw BFSM::FSMFatalException( "No BFSM defined for simulation." );
			if ( _elevation == 0x0 ) {
				logger << Logger::WARN_MSG << "No elevation implementation specified.  "
					"Using \"flat\" implementation.";
				_elevation = new FlatElevation();
				Menge::ELEVATION = _elevation;
			}
		}

		////////////////////////////////////////////////////////////////

		bool SimulatorInterface::setOutput( const std::string & outFileName,
									const std::string & scbVersion ) {
			try {
				_scbWriter = new SCBWriter( outFileName, scbVersion, this );
				return true;
			} catch ( SCBFileException ) {
				logger << Logger::WARN_MSG << "Error preparing output trajectory file: ";
				logger << outFileName << ".";
				return false;
			}
		}

		////////////////////////////////////////////////////////////////////////////
		
	}	// namespace Agents
}	// namespace Menge
