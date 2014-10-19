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
 *	@file		NavMeshObstacle.h
 *	@brief		Specification for obstacles in a navigation mesh file.
 */

#ifndef __NAV_MESH_OBSTACLE__
#define	__NAV_MESH_OBSTACLE__

#include "Obstacle.h"
#include <fstream>

namespace Menge {

	// FORWARD DECLARATIONS
	class NavMeshNode;
	class NavMesh;

	/*!
	 *	@brief		Specification of an obstacle.  It is the same as a pedModel
	 *				specification but includes a pointer to a node to which it is
	 *				attached.
	 */
	class NavMeshObstacle : public Agents::Obstacle {
	public:
		/*!
		 *	@brief		The index value if the obstacle has no neighboring obstacle
		 */
		static unsigned int NO_NEIGHBOR_OBST;

		/*!
		 *	@brief		Constructor
		 */
		NavMeshObstacle(): Agents::Obstacle(), _node(0x0) {}

		/*!
		 *	@brief		Sets the obstacle properties from an obstacle definition
		 *				in the given ascii file stream.
		 *	
		 *	@param		f			The input file stream.
		 *	@param		vertices	The array of vertices into which the definition
		 *							indexes.
		 *	@returns	A boolean indicating successful parsing (true) or failure
		 *				(false).
		 */
		bool loadFromAscii( std::ifstream & f, Vector2 * vertices );

		/*!
		 *	@brief		Retrieve the nav mesh node to which this obstacle is adjacent.
		 *
		 *	@returns	A pointer to the node.
		 */
		inline const NavMeshNode * getNode() const { return _node; }

		friend class NavMeshNode;
		friend class NavMesh;

	protected:
		/*!
		 *	@brief		A nav mesh node to which this obstacle is attached.
		 *				It could be attached to multiple, but this is the node
		 *				for which it serves as an edge.
		 */
		NavMeshNode *	_node;
	};
}	// namespace Menge

#endif	 // __NAV_MESH_OBSTACLE__