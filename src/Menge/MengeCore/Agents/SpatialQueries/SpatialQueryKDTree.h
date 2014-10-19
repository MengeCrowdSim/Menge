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
 *  @file       SpatialQueryKDTree.h
 *  @brief      A spatial query object based on Jur van den Berg's <i>k</i>d-tree as defined
 *				in the RVO2 library (http://gamma.cs.unc.edu/RVO2).
 *
 *	This spatial query implementation uses a <i>k</i>d-tree for agents and a bsp-tree for
 *	obstacles.  The BSP Tree changes the input obstacle set.  Single line segments can end
 *	up cut into two or more pieces.  This *may* have a deleterious effect on simulation.
 */

#ifndef __SPATIAL_QUERY_KD_TREE_H__
#define	__SPATIAL_QUERY_KD_TREE_H__

// UTILS
#include "SpatialQueries/SpatialQuery.h"
#include "SpatialQueries/SpatialQueryFactory.h"
#include "SpatialQueries/AgentKDTree.h"
#include "SpatialQueries/ObstacleKDTree.h"

namespace Menge {

	namespace Agents {

		/*!
		 *	@brief		Spatial query object.  Used to determine obstacles
		 *				and agents near an agent -- based on a <i>k</i>d-tree.
		 */
		class MENGE_API BergKDTree : public SpatialQuery {
		public:
			/*!
			 *  @brief      Constructor.
			 */
			explicit BergKDTree(): SpatialQuery() {
			}

			// Agent operations

			/*!
			 *  @brief      Define the set of agents on which <i>k</i>d-tree will query.
			 *
			 *	@param		agents		The set of agents in the simulator to be managed.
			 */
			virtual void setAgents( const std::vector< BaseAgent * > & agents ) {
				_agentTree.setAgents( agents );
			}

			/*!
			 *  @brief      Allows the spatial query structure to update its
			 *				knowledge of the agent positions.
			 */
			virtual void updateAgents() {
				_agentTree.buildTree();
			};	

			/*!
			 *  @brief      performs an agent based proximity query
			 *
			 *  @param      query           a pointer to the proximity query to be performed
			 */
			virtual void agentQuery( ProximityQuery *query) const {
				_agentTree.agentQuery(query);
			}

			// Obstacle operations

			/*!
			 *  @brief      Do the necessary pre-computation to support obstacle
			 *				definitions.
			 */
			virtual void processObstacles() {
				_obstTree.buildTree( _obstacles );
			}

			/*!
			 *  @brief      perform an obstacle based proximity query
			 *
			 *  @param      query           a pointer to the proximity query to be performed
			 *
			 */
			virtual void obstacleQuery( ProximityQuery *query) const {
				_obstTree.obstacleQuery( query);
			}

			/*!
			 *  @brief      Queries the visibility between two points within a
			 *              specified radius.
			 *
			 *  @param      q1              The first point between which visibility is
			 *                              to be tested.
			 *  @param      q2              The second point between which visibility is
			 *                              to be tested.
			 *  @param      radius          The radius within which visibility is to be
			 *                              tested.
			 *  @returns    True if q1 and q2 are mutually visible within the radius;
			 *              false otherwise.
			 */
			virtual bool queryVisibility(const Vector2& q1, const Vector2& q2, float radius) const {
				return _obstTree.queryVisibility( q1, q2, radius );
			}

		protected:
			/*!
			 *  @brief      A kd-tree for the agent queries.
			 */
			AgentKDTree	_agentTree;

			/*!
			 *  @brief      A kd-tree for the obstacle queries.
			 */
			ObstacleKDTree	_obstTree;
			
		};

		//////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Factory for the BergKDTree.
		 */
		class MENGE_API BergKDTreeFactory : public SpatialQueryFactory {
		public:
			/*!
			 *	@brief		The name of the spatial query implemenation.
			 *
			 *	The spatial query's name must be unique among all registered 
			 *	spatial query components.  Each spatial query factory must override 
			 *	this function.
			 *
			 *	@returns	A string containing the unique spatial query name.
			 */
			virtual const char * name() const { return "kd-tree"; }

			/*!
			 *	@brief		A description of the spatial query.
			 *
			 *	Each spatial query factory must override this function.
			 *
			 *	@returns	A string containing the spatial query description.
			 */
			virtual const char * description() const {
				return "Performs spatial queries by creating a kd-tree on the agents and a bsp " \
						"tree on the obstacles.";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's spatial query implementation.
			 *
			 *	All SpatialQueryFactory sub-classes must override this by creating (on the heap)
			 *	a new instance of its corresponding spatial query type.  The various field values
			 *	of the instance will be set in a subsequent call to SpatialQueryFactory::setFromXML.
			 *	The caller of this function takes ownership of the memory.
			 *
			 *	@returns		A pointer to a newly instantiated SpatialQuery class.
			 */
			SpatialQuery * instance() const { return new BergKDTree(); }
		};
	}	// namespace Agents
}	// namespace Menge
#endif	//__SPATIAL_QUERY_KD_TREE_H__