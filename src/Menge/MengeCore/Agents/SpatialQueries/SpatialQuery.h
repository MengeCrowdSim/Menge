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
 *  @file       SpatialQuery.h
 *  @brief      The base class for all objects which support agent spatial
 *				queries including: k-nearest agent neighbor, k-nearest
 *				obstacles and visibility queries.
 */
#ifndef __SPATIAL_QUERY_H__
#define	__SPATIAL_QUERY_H__


// UTILS
#include "CoreConfig.h"
#include "Element.h"
#include "Obstacle.h"
#include "SpatialQueries/ProximityQuery.h"
#include "MengeException.h"
#include <vector>

namespace Menge {

	namespace Agents {

		/*!
		 *	@brief		Exception class for spatial query operations
		 */
		class MENGE_API SpatialQueryException : public virtual MengeException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			SpatialQueryException() : MengeException() {}		

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			SpatialQueryException( const std::string & s ): MengeException(s) {}
		};

		/*!
		 *	@brief		The fatal spatial query exception.
		 */
		class MENGE_API SpatialQueryFatalException : public SpatialQueryException, public MengeFatalException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			SpatialQueryFatalException() : MengeException(), SpatialQueryException(), MengeFatalException() {}

			/*!
			 *	@brief		Constructor with message.
			 *
			 *	@param		s		The exception-specific message.
			 */
			SpatialQueryFatalException( const std::string & s ): MengeException(s), SpatialQueryException(), MengeFatalException() {}
		};

		// FORWARD DECLARATIONS
		class BaseAgent;

		/*!
		 *	@brief		The base class for performing spatial queries.
		 *
		 *	A spatial query implementation has to support several operations:
		 *		- Given a maximum distance, find agents within that distance from a point
		 *		- Given a maximum distance, find obstacles within that distance from a point
		 *		- Given two points and a width, determines if a straight link
		 *			between them is collision free (the link has the given width).
		 *		- Updates its internal structure based on current agent state.
		 *			// TODO: This should be done via a task.
		 *      
		 *      - The spatial query is also responsible for visiblity testing
		 *      Spatial Queries work in tandem with a Filter. The query itself is agnostic of the filter or the results
		 */
		class MENGE_API SpatialQuery : public Element {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			SpatialQuery();

		protected:
			/*!
			 *	@brief		Virtual destructor.
			 */
			virtual ~SpatialQuery() {}

			// Agent operations
		public:
			/*!
			 *  @brief      Define the set of agents on which query class will operate.
			 */
			virtual void setAgents( const std::vector< BaseAgent * > & agents ) = 0;	

			/*!
			 *  @brief      Allows the spatial query structure to update its
			 *				knowledge of the agent positions.
			 *	TODO: Replace this with a task.
			 */
			virtual void updateAgents() = 0;

			/*!
			 *  @brief      adds an obstacle to the internal list of the spatial query
			 *				
			 */
			virtual void addObstacle(Obstacle *obs);

			/*!
			 *  @brief      returns the collected obstacles of the spatial query for use in visualization
			 *  @returns    const pointer to the obstacle set
			 *				
			 */
			const std::vector< Obstacle * > getObstacles() {return _obstacles;};

			/*!
			 *  @brief      performs an agent based proximity query
			 *  @param      query          a pointer for the proximity query to be performed
			 */
			virtual void agentQuery( ProximityQuery *query) const = 0;

			// Obstacle operations

			/*!
			 *  @brief      Do the necessary pre-computation to support obstacle
			 *				definitions. Now uses the internal obstacle representation
			 */
			virtual void processObstacles() = 0;	

			/*!
			 *  @brief      performs an obstacle based proximity query
			 *  @param      query          a pointer for the proximity query to be performed
			 */
			virtual void obstacleQuery( ProximityQuery *query) const = 0;

			/*!
			 *  @brief      Queries the visibility between two points within a
			 *              specified radius.
			 *  @param      q1              The first point between which visibility is
			 *                              to be tested.
			 *  @param      q2              The second point between which visibility is
			 *                              to be tested.
			 *  @param      radius          The radius within which visibility is to be
			 *                              tested.
			 *  @returns    True if q1 and q2 are mutually visible within the radius;
			 *              false otherwise.
			 */
			virtual bool queryVisibility(const Vector2& q1, const Vector2& q2, float radius) const = 0;

			/*!
			 *	@brief		Sets the spatial query to include visibility in finding agent neighbors.
			 *
			 *	@param		state		If true, the spatial query has to consider visibility.
			 *							if false, it doesn't.
			 */
			virtual void setNeighborVisibleTest( bool state ) {}

			/*!
			 *	@brief		Sets the test visibility status of the neighbor functions.
			 *
			 *	@param		status		The new stats of the test visibility property.
			 */
			inline void setTestVisibility( bool status ) { _testVisibility = status; }

			/*!
			 *	@brief		Reports the test visibility property of the spatial query instance.
			 *
			 *	@returns	A boolean reporting if the spatial query instance uses visibility
			 *				tests when computing nearby agents and obstacles (true) or not (false).
			 */
			inline bool getTestVisibility() const { return _testVisibility; }

		protected:
			/*!
			 *	@brief		Dictates if visibility queries should be used (true) for neighbor
			 *				queries or not (false).
			 */
			bool	_testVisibility;

			/*!
			 *	@brief		an internal central list of obstacles
			 *				
			 */
			std::vector<Obstacle *> _obstacles;
		};
	}	// namespace Agents
}	 // namespace Menge
#endif	// __SPATIAL_QUERY_H__
