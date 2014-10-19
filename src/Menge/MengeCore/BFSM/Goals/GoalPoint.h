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
 *	@file		GoalPoint.h
 *	@brief		The definition of a BFSM point goal. 
 */

#ifndef __GOAL_POINT_H__
#define __GOAL_POINT_H__

#include "CoreConfig.h"
#include "fsmCommon.h"
#include "Goals/Goal.h"
#include "Goals/GoalFactory.h"

namespace Menge {

	namespace BFSM {
		/*!
		 *	@brief		A simple point goal.  The goal is trivially this point.
		 */
		class MENGE_API PointGoal : public Goal {
		public:
			/*!
			 *	@brief		Default constructor
			 */
			PointGoal();

			/*!
			 *	@brief		Reports the *squared* distance from the given point to the goal.
			 *
			 *	@param		pt			The query point.
			 *	@returns	The squared distance from the point to the goal.
			 */
			virtual float squaredDistance( const Vector2 & pt ) const { return absSq( pt - _point ); }

			/*!
			 *	@brief		Set the preferred velocity directions w.r.t. the goal: left, right, and preferred.
			 *
			 *	The Agents::PrefVelocity class represents a span of velocities that will reach the goal.
			 *	For a goal that covers a 2D region, the directions in the Agents::PrefVelocity should span the arc 
			 *	subtended by the goal from the query point's perspective.  Furthermore, it should have sufficient clearance
			 *	for a disk with the given radius to pass through.
			 *	This should be overridden by subclasses to account for their unique geometry.
			 *
			 *	@param		q				The query point.
			 *	@param		r				The radius of clearance.
			 *	@param		directions		An instance of Agents::PrefVelocity.  
			 */
			virtual void setDirections( const Vector2 & q, float r, Agents::PrefVelocity & directions ) const;

			/*!
			 *	@brief		Returns the closest "target" point in the goal to the given
			 *				query point.
			 *
			 *	A "valid" target point is the nearest point to the query point that is sufficiently
			 *	inside the goal region that a disk with the given radius is completely inside the goal.
			 *	It need not be literally the *best* value, an approximation is sufficient.
			 *
			 *	In the case where the goal region is too small to hold the agent, then the "deepest"
			 *	point in the region is a good approximation.
			 *
			 *	@param		q		The query point.
			 *	@param		r		The radius of clearance.
			 *	@returns	A 2D position representing the target point.
			 */
			virtual Vector2 getTargetPoint( const Vector2 & q, float r ) const { return _point; }

			/*!
			 *	@brief		Return the centroid of the goal.
			 */
			virtual Vector2 getCentroid() const { return _point; }

			/*!
			 *	@brief		Draws the goal into an OpenGL context.
			 */
			virtual void drawGL() const;

			/*!
			 *	@brief		Sets the goal's position.
			 *
			 *	@param		p		The position value.
			 */
			void setPosition( const Vector2 & p ) { _point.set( p ); }

			/*!
			 *	@brief		Sets the goal's position.
			 *
			 *	@param		x		The x-value of the goal position.
			 *	@param		y		The y-value of the goal position.
			 */
			void setPosition( float x, float y ) { _point.set( x, y ); }

		protected:
			/*!
			 *	@brief		The point for this goal.
			 */
			Vector2	_point;
		};

		/*!
		 *	@brief		Factory for the PointGoal.
		 */
		class MENGE_API PointGoalFactory : public GoalFactory { 
		public:
			/*!
			 *	@brief		Constructor.
			 */
			PointGoalFactory();

			/*!
			 *	@brief		The name of the goal type.
			 *
			 *	The goal's name must be unique among all registered goals.
			 *	Each goal factory must override this function.
			 *
			 *	@returns	A string containing the unique goal name.
			 */
			virtual const char * name() const { return "point"; }

			/*!
			 *	@brief		A description of the goal.
			 *
			 *	Each goal factory must override this function.
			 *
			 *	@returns	A string containing the goal description.
			 */
			virtual const char * description() const {
				return "An agent goal consisting of a single point in two-dimensional space";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's goal.
			 *
			 *	@returns		A pointer to a newly instantiated Goal class.
			 */
			Goal * instance() const { return new PointGoal(); }	

			/*!
			 *	@brief		Given a pointer to a Goal instance, sets the appropriate fields
			 *				from the provided XML node.
			 *
			 *	@param		goal		A pointer to the goal whose attributes are to be set.
			 *	@param		node		The XML node containing the action attributes.
			 *	@param		behaveFldr	The path to the behavior file.  If the action references
			 *							resources in the file system, it should be defined relative
			 *							to the behavior file location.  This is the folder containing
			 *							that path. 
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			virtual bool setFromXML( Goal * goal, TiXmlElement * node, const std::string & behaveFldr ) const;
		
			/*!
			 *	@brief		The identifier for the "x" float attribute.
			 */
			size_t	_xID;

			/*!
			 *	@brief		The identifier for the "y" float attribute.
			 */
			size_t	_yID;
		};

	}	// namespace BFSM
}	// namespace Menge

#endif // __GOAL_POINT_H__