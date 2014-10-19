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
 *	@file		ExplicitObstacleSet.h
 *	@brief		An obstacleset which creates obstacles based on explicit definitions in XML
 */

#ifndef __NAVMESH_OBSTACLE_SET_H__
#define __NAVMESH_OBSTACLE_SET_H__

#include "mengeCommon.h"
#include "ObstacleSets/ListObstacleSet.h"
#include "ObstacleSets/ObstacleSetFactory.h"
#include "Obstacle.h"
#include "NavMesh.h"
#include <vector>

namespace Menge {

	namespace Agents {

		class NavMeshObstacleSetFactory;
		/*!
		 *	@brief		Definition of obstacle set class which produces obstacles based on
		 *				explicit definition in an XML file.
		 */
		class MENGE_API NavMeshObstacleSet : public ListObstacleSet {
		public:
			/*!
			 *	@brief		Constructor
			 */
			NavMeshObstacleSet();

			/*!
			 *	@brief		Extract obstacles from a provided navigation_mesh
			 *
			 *  @param      nm		The navigation mesh from which to extract obstacles
			 *  @returns    True if the obstacles were extracted successfully, false otherwise.
			 */
			bool extractFromNavMesh(NavMeshPtr nm);

		};

		//////////////////////////////////////////////////////////////////////////////

		/*!
		 *	@brief		Factory for NavMeshObstacleSet
		 */
		class MENGE_API NavMeshObstacleSetFactory : public ObstacleSetFactory {
		public:

			/*!
			 *	@brief		constrcutor
			 */
			NavMeshObstacleSetFactory();

			/*!
			 *	@brief		The name of the obstacleset type.
			 *
			 *	The set's name must be unique among all registered obstacleset components.  
			 *	Each obstacleset factory must override this function.
			 *
			 *	@returns	A string containing the unique obstacleset name.
			 */
			virtual const char * name() const { return "nav_mesh"; }

			/*!
			 *	@brief		A description of the obstacle set
			 *
			 *	Each obstacleset factory must override this function.
			 *
			 *	@returns	A string containing the obstacleset description.
			 */
			virtual const char * description() const {
				return "Obstacle definitions extracted from a provided nav_mesh.";
			};

		protected:
			/*!
			 *	@brief		Create an instance of this class's obstacleset implementation.
			 *
			 *	All ObstacleSetFactory sub-classes must override this by creating (on the heap)
			 *	a new instance of its corresponding ObstacleSet type.  The various field values
			 *	of the instance will be set in a subsequent call to ObstacleSetFactory::setFromXML.
			 *	The caller of this function takes ownership of the memory.
			 *
			 *	@returns		A pointer to a newly instantiated ExplicitObstacleSet class.
			 */
			ObstacleSet * instance() const { return new NavMeshObstacleSet(); }

			/*!
			 *	@brief		Given a pointer to an ObstacleSet instance, sets the appropriate fields
			 *				from the provided XML node.
			 *
			 *	It is assumed that the value of the `type` attribute is this ObstacleSet's type.
			 *	(i.e. ObstacleSetFactory::thisFactory has already been called and returned true.)
			 *	If sub-classes of ObstacleSetFactory introduce *new* ObstacleSet parameters, then the
			 *	sub-class should override this method but explicitly call the parent class's
			 *	version.
			 *
			 *	@param		gen			A pointer to the ObstacleSet whose attributes are to be set.
			 *	@param		node		The XML node containing the ObstacleSet attributes.
			 *	@param		specFldr	The path to the specification file.  If the ObstacleSet references
			 *							resources in the file system, it should be defined relative
			 *							to the specification file location.  This is the folder containing
			 *							that path. 
			 *	@returns	A boolean reporting success (true) or failure (false).
			 */
			virtual bool setFromXML( ObstacleSet * gen, TiXmlElement * node, const std::string & specFldr ) const;


			/*!
			 *	@brief		The identifier for the "file_name" string attribute.
			 */
			size_t	_fileNameID;

		};
	}	// namespace Agents
}	// namespace Menge

#endif	// __EXPLICIT_OBSTACLE_SET_H__