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

#include "SimSystem.h"
// SceneGraph
#include "GLScene.h"
// MengeBase
#include "SimulatorInterface.h"
#include "SpatialQueries/SpatialQuery.h"
#include "Obstacle.h"
#include "SCBWriter.h"
// MengeRuntime
#include "VisAgent.h"
#include "VisObstacle.h"
// BFSM
#include "FSM.h"
// STL
#include <set>

namespace Menge {

	////////////////////////////////////////////////////////////////////////////
	//			Implementation of SimSystem
	////////////////////////////////////////////////////////////////////////////

	SimSystem::SimSystem( bool visualize ): SceneGraph::System(), _forVis(visualize), _sim(0x0), _fsm(0x0), _scbWriter(0x0),_lastUpdate(0.f), _isRunning(true), _maxDuration(100.f) {
	}

	////////////////////////////////////////////////////////////////////////////

	SimSystem::SimSystem( bool visualize, float duration ): SceneGraph::System(), _forVis(visualize), _sim(0x0), _fsm(0x0), _scbWriter(0x0),_lastUpdate(0.f), _isRunning(true), _maxDuration(duration) {
	}

	////////////////////////////////////////////////////////////////////////////

	SimSystem::~SimSystem() {
			if ( _sim ) delete _sim;
			if ( _fsm ) delete _fsm;
			if ( _scbWriter ) delete _scbWriter;
		}

	////////////////////////////////////////////////////////////////////////////

	bool SimSystem::updateScene( float time ) {
		const int agtCount = static_cast<int>( _sim->getNumAgents() );
		if ( _isRunning ) {
			if ( _scbWriter ) _scbWriter->writeFrame( _fsm );	
			_lastUpdate = _sim->getGlobalTime();
			if ( _lastUpdate > _maxDuration ) {
				_isRunning = false;
			} else {
				for ( size_t i = 0; i <= _sim->getSubSteps(); ++i ) {
                                        if(_sim->getGlobalTime()>=time) break;
					try {
						_isRunning = !_fsm->doStep();
					} catch ( BFSM::FSMFatalException & e ) {
						logger << Logger::ERR_MSG << "Error in updating the finite state machine -- stopping!\n";
						logger << "\t" << e.what() << "\n";
						throw SceneGraph::SystemStopException();
					}

					_sim->doStep();
					if ( _forVis ) {
						updateAgentPosition( agtCount );
					}
					try {
						_fsm->doTasks();
					} catch ( BFSM::FSMFatalException &e ) {
						logger << Logger::ERR_MSG << e.what() << "\n";
						throw SceneGraph::SystemStopException();
					}
				}
			}
		}
		if ( !_isRunning ) {
			// TODO: WHy is this here??
			throw SceneGraph::SystemStopException();
			return false;
		}
		return true;
	}

	////////////////////////////////////////////////////////////////////////////

	bool SimSystem::isFinished() const { 
		return _fsm->allFinal(); 
	}

	////////////////////////////////////////////////////////////////////////////

	void SimSystem::setSimulator( Agents::SimulatorInterface * sim, BFSM::FSM * fsm ) {
		if ( _sim ) {
			std::string msg( "Simulator already assigned to SimSystem" );
			throw SimSystemFatalException( msg );
		}
		_sim = sim;
		_fsm = fsm;
	}

	////////////////////////////////////////////////////////////////////////////

	void SimSystem::setSimulator( Agents::SimulatorInterface * sim, BFSM::FSM * fsm, const std::string & outFileName, const std::string & scbVersion ) {
		setSimulator( sim, fsm );
		try {
			_scbWriter = new Agents::SCBWriter( outFileName, scbVersion, sim );
		} catch ( Agents::SCBFileException ) {
			std::string msg( "Unable to create SCB file: ");
			msg += outFileName;
			throw SimSystemFatalException( msg );
		}
	}

	////////////////////////////////////////////////////////////////////////////

	void SimSystem::addObstacleToScene( SceneGraph::GLScene * scene ) {
			// TODO: If the bsptree (ObstacleKDTree.h) chops up the obstacles, this isn't doing the
			//		right thing.  Currently, the bsptree chops them
			//	THIS IS A HACK to address the issues of the ObstacleKDTree
			//		The right thing to do is modify things so that they are not chopped up.
			std::set< const Agents::Obstacle * > handled;
			const std::vector< Agents::Obstacle * > & obstacles = _sim->getSpatialQuery()->getObstacles();
			for ( size_t o = 0; o < obstacles.size(); ++o ) {
				
				const Agents::Obstacle * obst = obstacles[ o ];
				if ( handled.find( obst ) == handled.end() ) {
					Vector2 p0a = obst->getP0();
					Vector2 p1a = obst->getP1();
					const Agents::Obstacle * next = obst->_nextObstacle;
					while ( next && next->_unitDir * obst->_unitDir >= 0.99999f ) {
						handled.insert( next );
						p1a.set( next->getP1() );
						next = next->_nextObstacle;
					}
					Vector3 p0( p0a.x(), _sim->getElevation( p0a ), p0a.y() );
					Vector3 p1( p1a.x(), _sim->getElevation( p1a ), p1a.y() );
					VisObstacle * vo = new VisObstacle( p0, p1 );
					
					scene->addNode( vo );
					handled.insert( obst );
				}
			}
		}

	////////////////////////////////////////////////////////////////////////////

	void SimSystem::addAgentsToScene( SceneGraph::GLScene * scene ) {
		_visAgents = new VisAgent * [ _sim->getNumAgents() ];
		for ( size_t a = 0; a < _sim->getNumAgents(); ++a ) {
			Agents::BaseAgent * agt =  _sim->getAgent( a );
			VisAgent * agtNode = new VisAgent( agt );
			float h = _sim->getElevation( agt );
			agtNode->setPosition( agt->_pos.x(), h, agt->_pos.y() );
			scene->addNode( agtNode );
			_visAgents[ a ] = agtNode;
		}
	}

	////////////////////////////////////////////////////////////////////////////

	void SimSystem::populateScene( SceneGraph::GLScene * scene ) {
		assert( _sim !=  0x0 && "Can't add SimSystem to scene when no simulator is connected" );

		addAgentsToScene( scene );
		addObstacleToScene( scene );
	}

	////////////////////////////////////////////////////////////////////////////

	void SimSystem::updateAgentPosition( int agtCount ) {
		#pragma omp parallel for
		for ( int a = 0; a < agtCount; ++a ) {
			const Agents::BaseAgent * agt = _visAgents[ a ]->getAgent();
			float h = _sim->getElevation( agt );
			_visAgents[ a ]->setPosition( agt->_pos.x(), h, agt->_pos.y() );
		}
	}

	////////////////////////////////////////////////////////////////////////////

	size_t SimSystem::getAgentCount() const { 
		return _sim->getNumAgents(); 
	}

	////////////////////////////////////////////////////////////////////////////

}	// namespace Menge
