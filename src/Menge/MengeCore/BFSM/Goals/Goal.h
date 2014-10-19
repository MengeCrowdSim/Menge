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
 *  @file       Goal.h
 *  @brief      Defines the goal classes for agent behaviors
 */

#ifndef __GOALS_H__
#define	__GOALS_H__

#include "CoreConfig.h"
#include "fsmCommon.h"
#include "Element.h"
#include "ReadersWriterLock.h"
#include "MengeException.h"

// forward declaration
class TiXmlElement;

namespace Menge {

	namespace Agents {
		class PrefVelocity;
	}

	namespace BFSM {

		// Forward declaration
		class GoalSet;

		/*!
		 *	@brief		Exception class for BFSM goals.
		 */
		class MENGE_API GoalException : public virtual MengeException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			GoalException() : MengeException() {}		

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			GoalException( const std::string & s ): MengeException(s) {}

		};
		
		/*!
		 *	@brief		Exception thrown when the goal has an error which cannot be
		 *				recovered from.
		 */
		class GoalFatalException : public GoalException, public MengeFatalException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			GoalFatalException() : MengeException(), GoalException(), MengeFatalException() {}

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			GoalFatalException( const std::string & s ): MengeException(s), GoalException(), MengeFatalException() {}
		};

		/////////////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		The base, abstract class defining goals
		 */
		class MENGE_API Goal : public Element {
		public:
			/*!
			 *	@brief		Basic constructor
			 *
			 */
			Goal():Element(),_weight(1.f),_capacity(MAX_CAPACITY),_id(-1),_goalSet(0x0),_population(0){}	// -1 is the biggest value for size_t

		protected:
			/*!
			 *	@brief		Destructor. 
			 */
			virtual ~Goal(){}

		public:
			/*!
			 *	@brief		Reports the *squared* distance from the given point to the goal.
			 *
			 *	@param		pt			The query point.
			 *	@returns	The squared distance from the point to the goal.
			 */
			virtual float squaredDistance( const Vector2 & pt ) const = 0;

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
			virtual void setDirections( const Vector2 & q, float r, Agents::PrefVelocity & directions ) const = 0;

			// TODO: Delete this function= transition uses it determine distance to goal
			//		I would be better off simply returning "squared distance to goal"
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
			virtual Vector2 getTargetPoint( const Vector2 & q, float r ) const = 0;

			/*!
			 *	@brief		Return the centroid of the goal.
			 */
			virtual Vector2 getCentroid() const = 0;

			/*!
			 *	@brief		Reports if the goal still has capacity.
			 *
			 *	@returns	True if the goal has remaining capacity, false otherwise.
			 */
			bool hasCapacity() const;

			/*!
			 *	@brief		Inform the goal that it has been assigned.
			 *
			 *	@param		agent		The agent that has been assigned to this goal.
			 */
			void assign( const Agents::BaseAgent * agent );

			/*!
			 *	@brief		Inform the goal that an assignment has been removed.
			 */
			void free();

			/*!
			 *	@brief		Sets this goal's goal set.
			 *
			 *	@param		goalSet		A pointer to the goal set to which this goal belongs.
			 */
			inline void setGoalSet( GoalSet * goalSet ) { _goalSet = goalSet; }

			/*!
			 *	@brief		Returns a pointer to this agent's goal set.
			 *
			 *	A dynamic goal (such as would be created by a hold-position goal
			 *	will return NULL, which means it isn't owned by a goal set and should
			 *	be destroyed when finished.  
			 *
			 *	// TODO: Handle goal sharing and persistence.
			 *
			 *	@returns	A pointer to the goal set to which this goal belongs.
			 */
			inline GoalSet * getGoalSet() { return _goalSet; }

			/*!
			 *	@brief		Returns a const pointer to this agent's goal set.
			 *
			 *	A dynamic goal (such as would be created by a hold-position goal
			 *	will return NULL, which means it isn't owned by a goal set and should
			 *	be destroyed when finished.  
			 *
			 *	// TODO: Handle goal sharing and persistence.
			 *
			 *	@returns	A const pointer to the goal set to which this goal belongs.
			 */
			inline const GoalSet * getGoalSet() const { return _goalSet; }

			/*!
			 *	@brief		Sets the goal's weight.
			 *
			 *	@param		weight		The weight value for this goal.
			 */
			inline void setWeight( float weight ) { _weight = weight; }

			/*!
			 *	@brief		Retrieves the goal's weight.
			 *
			 *	@returns	This goal's weight.
			 */
			inline float getWeight() const { return _weight; }

			/*!
			 *	@brief		Sets the goal's capacity.
			 *
			 *	@param		capacity		The target capacity for this goal.
			 */
			inline void setCapacity( size_t capacity ) { _capacity = capacity; }

			/*!
			 *	@brief		Returns this goal's capacity.
			 *
			 *	@returns	The goal's capacity.
			 */
			inline size_t getCapacity() const { return _capacity; }

			/*!
			 *	@brief		Sets the id of the goal.
			 *
			 *	@param		id		The goal's new id.
			 */
			inline void setID( size_t id ) { _id = id; }

			/*!
			 *	@brief		Gets the id of the goal.
			 *
			 *	@returns	The goal's id.
			 */
			inline size_t getID() const { return _id; }

			/*!
			 *	@brief		Draws the goal into an OpenGL context.
			 */
			virtual void drawGL() const;

			/*!
			 *	@brief		The maximum capacity any goal can hold.
			 */
			static const size_t MAX_CAPACITY;

			friend class GoalSet;

		protected:
			/*!
			 *	@brief		Draws the goal geometry.
			 */
			virtual void drawGLGeometry() const {}

			/*!
			 *	@brief		The relative weight of this goal.
			 */
			float	_weight;

			/*!
			 *	@brief		The maximum capacity of this goal.
			 */
			size_t	_capacity;

			/*!
			 *	@brief		The id of this goal in its goal set.
			 */
			size_t	_id;

			/*!
			 *	@brief		The goal set to which this goal belongs.
			 */
			GoalSet * _goalSet;

			/*!
			 *	@brief		The current "population" of this goal.
			 *
			 *				In other words, it is the number of times "getGoalPoint"
			 *				has been called on this goal.  If a goal is to support
			 */
			mutable size_t	_population;

			/*!
			 *	@brief		The lock to maintain readers-writer access to the
			 *				structure which control available goals
			 */
			ReadersWriterLock	_lock;
		};

		/*!
		 *	@brief		Parses a TinyXML element containing a goal description
		 *
		 *	@param		node			The TinyXML element
		 *	@param		behaveFldr		The folder in which the behavior is defined -- all resources
		 *								are defined relative to this folder.
		 *	@returns	A pointer to the new goal description (NULL if the definition is invalid).
		 */
		Goal * parseGoal( TiXmlElement * node, const std::string & behaveFldr );

	}	// namespace BFSM

}	// namespace Menge
#endif	 //__GOALS_H__
