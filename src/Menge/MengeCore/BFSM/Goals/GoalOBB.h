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
 *	@file		GoalOBB.h
 *	@brief		The definition of a BFSM oriented bounding box goal.  
 */

#ifndef __GOAL_OBB_H__
#define __GOAL_OBB_H__

#include "CoreConfig.h"
#include "fsmCommon.h"
#include "Goals/Goal.h"
#include "Goals/GoalFactory.h"

namespace Menge {

	namespace BFSM {
		/*!
		 *	@brief		An oriented bounding box goal region with uniform probability.
		 *
		 *	The oriented bounding box (OBB) is defined by a pivot point, a size, and an orientation.
		 *	An OBB with no rotation is the same as an AABB whose minimum point is the pivot
		 *	point and which extends along the x-axis and the y-axis the given width and 
		 *	height, respectively.  Positive angle causes counter-clockwise rotation.
		 */
		class MENGE_API OBBGoal : public Goal {
		public:
			/*!
			 *	@brief		Default constructor
			 */
			OBBGoal();

			/*!
			 *	@brief		Reports the *squared* distance from the given point to the goal.
			 *
			 *	@param		pt			The query point.
			 *	@returns	The squared distance from the point to the goal.
			 */
			virtual float squaredDistance( const Vector2 & pt ) const;

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
			virtual Vector2 getTargetPoint( const Vector2 & q, float r ) const;

			/*!
			 *	@brief		Return the centroid of the goal.
			 */
			virtual Vector2 getCentroid() const;

			/*!
			 *	@brief		Set the OBB's pivot point.
			 *
			 *	@param		x		The x-value of the OBB's pivot point.
			 *	@param		y		The y-value of the OBB's pivot point.
			 */
			inline void setPivot( float x, float y ) { _pivot.set( x, y ); }

			/*!
			 *	@brief		Set the OBB's pivot point.
			 *
			 *	@param		p		The position of the OBB's pivot point.
			 */
			inline void setPivot( const Vector2 & p ) { _pivot.set( p ); }

			/*!
			 *	@brief		Set the AABB's size.
			 *
			 *	@param		w		The width (extent along the x-axis) of the AABB.
			 *	@param		h		The height (extent along the y-axis) of the AABB.
			 */
			inline void setSize( float w, float h ) { _size.set( w, h ); }

			/*!
			 *	@brief		Set the AABB's size.
			 *
			 *	@param		size	The size of the AABB (width, height).
			 */
			inline void setSize( const Vector2 & size ) { _size.set( size ); }

			/*!
			 *	@brief		Set the OBB's rotation angle (in radians).
			 *
			 *	@param		angle	The angle of rotation (in radians).
			 */
			void setRotation( float angle );

		protected:
			/*!
			 *	@brief		Draws the goal geometry.
			 */
			virtual void drawGLGeometry() const;

			/*!
			 *	@brief		The minimum point in the box region.
			 */
			Vector2	_pivot;

			/*!
			 *	@brief		The size of the box region beyond the minimum point.
			 */
			Vector2	_size;

			/*!
			 *	@brief		The first column of the rotation matrix
			 *				A point, p, can be rotated to the box's
			 *				orientation as: Vector( p * _rot0, p * _rot1 );
			 */
			Vector2	_rot0;

			/*!
			 *	@brief		The second column of the rotation matrix
			 *				A point, p, can be rotated to the box's
			 *				orientation as: Vector( p * _rot0, p * _rot1 );
			 */
			Vector2	_rot1;
		};

		/*!
		 *	@brief		Factory for the OBBGoal.
		 */
		class MENGE_API OBBGoalFactory : public GoalFactory { 
		public:
			/*!
			 *	@brief		Constructor.
			 */
			OBBGoalFactory();

			/*!
			 *	@brief		The name of the goal type.
			 *
			 *	The goal's name must be unique among all registered goals.
			 *	Each goal factory must override this function.
			 *
			 *	@returns	A string containing the unique goal name.
			 */
			virtual const char * name() const { return "OBB"; }

			/*!
			 *	@brief		A description of the goal.
			 *
			 *	Each goal factory must override this function.
			 *
			 *	@returns	A string containing the goal description.
			 */
			virtual const char * description() const {
				return "An agent goal consisting of an oriented bounding box in two-dimensional space";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's goal.
			 *
			 *	@returns		A pointer to a newly instantiated Goal class.
			 */
			Goal * instance() const { return new OBBGoal(); }	

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
		
			/*!
			 *	@brief		The identifier for the "width" float attribute.
			 */
			size_t	_wID;

			/*!
			 *	@brief		The identifier for the "height" float attribute.
			 */
			size_t	_hID;
		
			/*!
			 *	@brief		The identifier for the "angle" float attribute.
			 */
			size_t	_aID;
		};
	}	// namespace BFSM
}	// namespace Menge

#endif // __GOAL_OBB_H__