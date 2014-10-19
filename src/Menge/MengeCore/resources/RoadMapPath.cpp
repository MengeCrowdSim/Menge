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

#include "RoadMapPath.h"
#include "BaseAgent.h"
#include "PrefVelocity.h"
#include "Core.h"
#include "SpatialQueries/SpatialQuery.h"
#include "Goals/Goal.h"
#include <cassert>

namespace Menge {

	/////////////////////////////////////////////////////////////////////
	//					Implementation of RoadMapPath
	/////////////////////////////////////////////////////////////////////

	RoadMapPath::RoadMapPath( size_t pointCount ): _targetID(0), _goal(0x0), _validPos(), _wayPointCount(pointCount) {
		_wayPoints = new Vector2[ pointCount ];
	}

	/////////////////////////////////////////////////////////////////////

	RoadMapPath::~RoadMapPath() {
		delete [] _wayPoints;
	}

	/////////////////////////////////////////////////////////////////////

	void RoadMapPath::setWayPoint( size_t i, const Vector2 & pos ) {
		assert( i < _wayPointCount && "Tried to set the position of an invalid waypoint" );
		_wayPoints[i].set( pos );
	}

	/////////////////////////////////////////////////////////////////////

	void RoadMapPath::setPrefDirection( const Agents::BaseAgent * agent, Agents::PrefVelocity & pVel ) {
		// Assume that when I'm overlapping one node, that I can see the next
		// Test to see if I can advance target way point
		//	while I'm overlapping current target, advance it
		//		assert that it is visible
		//
		// test to see if current target is visible
		//	if visible
		//			direction towards it
		//			current position last valid
		//   else
		//			direction towards last valid position
		// TODO: This has flaws because the ObstacleKDTree is finding segments visible when it should not.
		bool isVisible = false;
		// TODO: Should I compute this blindly?  Although it is used in potentially three places
		//		mostly, it won't be used.
		Vector2 target = _goal->getTargetPoint( agent->_pos, agent->_radius );
		if ( _targetID < _wayPointCount ) {
			isVisible = Menge::SPATIAL_QUERY->queryVisibility( agent->_pos, _wayPoints[ _targetID ], agent->_radius );
		} else {
			isVisible = Menge::SPATIAL_QUERY->queryVisibility( agent->_pos, target, agent->_radius );
		}
		size_t testID = _targetID + 1;
		while ( testID < _wayPointCount && Menge::SPATIAL_QUERY->queryVisibility( agent->_pos, _wayPoints[ testID ], agent->_radius ) ) {
			_targetID = testID;
			isVisible = true;
			++testID;
		}
		if ( _targetID == _wayPointCount - 1 ) {
			if ( Menge::SPATIAL_QUERY->queryVisibility( agent->_pos, target, agent->_radius ) ) {
				++_targetID;
				isVisible = true;
			}
		}
		// Visibility test
		Vector2 dir;
		if ( isVisible ) {
			Vector2 curr( _targetID < _wayPointCount ? _wayPoints[ _targetID ] : target );
			dir = norm( curr - agent->_pos );
			_validPos = agent->_pos;
			pVel.setTarget( curr );
		} else {
			// This should never be the zero vector.
			//	_validPos is set when the current waypoint is visible
			//  this code is only achieved when it is NOT visible
			//	POSSIBLY, something weird could happen where the next waypoint isn't visible, but
			//		that breaks the earlier assertion.
			dir = norm( _validPos - agent->_pos );
			pVel.setTarget( _validPos );
		}
		pVel.setSingle( dir );
	}

	/////////////////////////////////////////////////////////////////////

	Vector2 RoadMapPath::getWayPoint( size_t i ) const {
		assert( i <= _wayPointCount && "Requesting a waypoint with an invalid index" );
		return _wayPoints[ i ];
	}

}	// namespace Menge
