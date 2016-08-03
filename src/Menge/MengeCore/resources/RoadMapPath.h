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
 *	@file		RoadMapPath.h
 *	@brief		A path along a roadmap.
 */

#ifndef __ROADMAP_PATH_H__
#define __ROADMAP_PATH_H__

#include "mengeCommon.h"

namespace Menge {

	class RoadMapPath;

	/*!
	 *	@brief		A map from agent id to its path.
	 */
	typedef HASH_MAP< size_t, RoadMapPath * > PathMap;

	namespace Agents {
		class BaseAgent;
		class PrefVelocity;
	}

	namespace BFSM {
		class Goal;
	}

	/*!
	 *	@brief		A path on a roadmap between vertices
	 */
	class RoadMapPath {
	public:
		/*!
		 *	@brief		Constructor
		 *
		 *	@param		pointCount		The number of waypoints in the path.
		 */
		RoadMapPath( size_t pointCount );

		/*!
		 *	@brief		Destructor.
		 */
		~RoadMapPath();

		/*!
		 *	@brief		Sets the position of the ith waypoint.
		 *
		 *	@param		i		The index of the waypoint.
		 *	@param		pos		The location of the ith waypoint.
		 */
		void setWayPoint( size_t i, const Vector2 & pos );

		/*!
		 *	@brief		Sets the ultimate goal.
		 *
		 *	@param		goal		The ultimate goal
		 */
		inline void setGoalPos( const BFSM::Goal * goal ) { _goal = goal; }

		/*!
		 *	@brief		Sets the direction of the preferred velocity (and target).
		 *
		 *	@param		agent		The agent to compute the preferred direciton for.
		 *	@param		pVel		The preferred velocity to set.
		 */
		void setPrefDirection( const Agents::BaseAgent * agent, Agents::PrefVelocity & pVel ); 

		/*!
		 *	@brief		Reports the number of waypoints in the path.
		 *
		 *	@returns	The number of way points.  This does *not* include the final
		 *				goal point.  So, at its simplest, this could be zero.
		 */
		inline size_t getWayPointCount() const { return _wayPointCount; }

		/*!
		 *	@brief		Returns the position of the ith waypoint.
		 *
		 *	The index i is only validated in debug mode with an assertion.
		 *	The ultimate goal is not included in the count.
		 *
		 *	@param		i		The index of the desired waypoint.
		 *	@returns	The position of the ith waypoint.
		 */
		Vector2 getWayPoint( size_t i ) const;

		/*!
		 *	@brief		Returns the ultimate goal.
		 *
		 *	@returns	A pointer to the ultimate goal.
		 */
		const BFSM::Goal * getGoal() const { return _goal; }

		/*!
		 *	@brief		Returns the current target waypoint index
		 *
		 *	@returns	The index of the target waypoint.
		 */
		inline size_t getTargetID() const { return _targetID; }

	protected:
		/*!
		 *	@brief		The ultimate goal.
		 */
		const BFSM::Goal * _goal;

		/*!
		 *	@brief		The last valid position -- validity means the target 
		 *				goal was visible.
		 */
		Vector2	_validPos;

		/*!
		 *	@brief		The index of the current target.
		 */
		size_t	_targetID;

		/*!
		 *	@brief		The number of way points in the path.
		 */
		size_t	_wayPointCount;

		/*!
		 *	@brief		The way points along the path.
		 */
		Vector2 * _wayPoints;

	};
}	// namespace Menge
#endif	// __ROADMAP_PATH_H__
