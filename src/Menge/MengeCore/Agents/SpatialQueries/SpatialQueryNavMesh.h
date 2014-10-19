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
 *	@file		SpatialQueryNavMesh.h
 *	@brief		Definition of a spatial query structure based on a navigation mesh.
 */

#ifndef __SPATIAL_QUERY_NAV_MESH_H__
#define __SPATIAL_QUERY_NAV_MESH_H__

// Menge Base
#include "SpatialQueries/SpatialQuery.h"
#include "SpatialQueries/SpatialQueryFactory.h"

// Resources
#include "NavMesh.h"
#include "NavMeshLocalizer.h"

// STL
#include <vector>

namespace Menge {

	// forward declaration
	namespace BFSM {
		class Task;
	}

	namespace Agents {

		/*!
		 *	@brief		A spatial query structure based on a navigation mesh.
		 */
		class MENGE_API NavMeshSpatialQuery : public SpatialQuery {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			NavMeshSpatialQuery();

			/*!
			 *  @brief      Define the set of agents on which <i>k</i>d-tree will query.
			 */
			virtual void setAgents( const std::vector< BaseAgent * > & agents );	

			/*!
			 *  @brief      Allows the spatial query structure to update its
			 *				knowledge of the agent positions.
			 *
			 *				This happens by the NavMeshLocalizer as an FSM task.
			 */
			virtual void updateAgents() {}	

			/*!
			 *  @brief      gets agents within a range, and passes them to the supplied filter
			 *  @param      query           a pointer to the proximity query to be performed
			 */
			virtual void agentQuery( ProximityQuery *query) const;

			/*!
			 *  @brief      performs an agent based proximity query
			 *
			 *  @param      query           a pointer to the proximity query to be performed
			 *  @param      rangeSq           the range to search (extracted and mutable)
			 */
			virtual void agentQuery( ProximityQuery *query, float &rangeSq) const;

			// Obstacle operations

			/*!
			 *  @brief      Do the necessary pre-computation to support obstacle
			 *				definitions.
			 */
			virtual void processObstacles();

			/*!
			 *  @brief      performs an obstacle based proximity query
			 *  @param      query           a pointer to the proximity query to be performed
			 */
			virtual void obstacleQuery( ProximityQuery *query) const;

			/*!
			 *  @brief      performs an obstacle based proximity query
			 *  @param      query           a pointer to the proximity query to be performed
			 *  @param      rangeSq           the range to search (extracted and mutable)
			 */
			virtual void obstacleQuery( ProximityQuery *query, float rangeSq) const;


			/*!
			 *  @brief      Queries the visibility between two points within a
			 *              specified radius.  NOT CURRENTLY IMPLEMENTED!
			 *  @param      q1              The first point between which visibility is
			 *                              to be tested.
			 *  @param      q2              The second point between which visibility is
			 *                              to be tested.
			 *  @param      radius          The radius within which visibility is to be
			 *                              tested.
			 *  @returns    True if q1 and q2 are mutually visible within the radius;
			 *              false otherwise.
			 */
			virtual bool queryVisibility(const Vector2& q1, const Vector2& q2, float radius) const;

			/*!
			 *	@brief		Sets the navigation mesh localizer pointer.
			 *
			 *	@param		nml		The managed pointer to the navigation mesh localizer.
			 */
			void setNavMeshLocalizer( const NavMeshLocalizerPtr & nml ) { _localizer = nml; }

			/*!
			 *	@brief		Returns a pointer to the nav mesh localizer task.
			 *
			 *	@returns		A pointer to the nav mesh localizer task.  It is the responsibility
			 *					of the caller to free the memory of the provided task by
			 *					calling its destroy method.
			 */
			virtual BFSM::Task * getTask();

			// TODO: Another version of this would be good where the inputs are an agent, and
			//		a point, and it uses the agent's position and radius.
		protected:

			/*!
			 *	@brief		A vector of pointers to all the agents in the simulation
			 */
			std::vector< BaseAgent * > _agents;

			/*!
			 *	@brief		The localizer tied to the given navigation mesh.
			 */
			NavMeshLocalizerPtr _localizer;
		};

		//////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Factory for the NavMeshSpatialQuery.
		 */
		class MENGE_API NavMeshSpatialQueryFactory : public SpatialQueryFactory {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			NavMeshSpatialQueryFactory();

			/*!
			 *	@brief		The name of the spatial query implemenation.
			 *
			 *	The spatial query's name must be unique among all registered 
			 *	spatial query components.  Each spatial query factory must override 
			 *	this function.
			 *
			 *	@returns	A string containing the unique spatial query name.
			 */
			virtual const char * name() const { return "nav_mesh"; }

			/*!
			 *	@brief		A description of the spatial query.
			 *
			 *	Each spatial query factory must override this function.
			 *
			 *	@returns	A string containing the spatial query description.
			 */
			virtual const char * description() const {
				return "Performs spatial queries by operating on a navigation mesh.";
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
			SpatialQuery * instance() const { return new NavMeshSpatialQuery(); }

			/*!
			 *	@brief		Given a pointer to an SpatialQuery instance, sets the appropriate fields
			 *				from the provided XML node.
			 *
			 *	It is assumed that the value of the `type` attribute is this SpatialQuery's type.
			 *	(i.e. SpatialQueryFactory::thisFactory has already been called and returned true.)
			 *	If sub-classes of SpatialQueryFactory introduce *new* SpatialQuery parameters, 
			 *	then the sub-class should override this method but explicitly call the parent 
			 *	class's version.
			 *
			 *	@param		sq			A pointer to the spatial query whose attributes are to be set.
			 *	@param		node		The XML node containing the elevation attributes.
			 *	@param		specFldr	The path to the specification file.  If the SpatialQuery 
			 *							references resources in the file system, it should be defined relative
			 *							to the specification file location.  This is the folder containing
			 *							that path. 
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			virtual bool setFromXML( SpatialQuery * sq, TiXmlElement * node, const std::string & specFldr ) const;
		
			/*!
			 *	@brief		The identifier for the "file_name" string attribute.
			 */
			size_t	_fileNameID;
		};
	}	// namespace Agents
}	// namespace Menge
#endif	 // __SPATIAL_QUERY_NAV_MESH_H__
