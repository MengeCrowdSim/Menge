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

#include "ObstacleSets/ListObstacleSet.h"
#include "tinyxml.h"
#include <vector>

namespace Menge {

	namespace Agents {
		
		////////////////////////////////////////////////////////////////////////////
		//			Implementation of ListObstacleSet
		////////////////////////////////////////////////////////////////////////////

		ListObstacleSet::ListObstacleSet() : ObstacleSet(), _obstacles() {
		}

		////////////////////////////////////////////////////////////////////////////

		ListObstacleSet::~ListObstacleSet() {
			_obstacles.empty();
		}

		////////////////////////////////////////////////////////////////////////////

		Obstacle * ListObstacleSet::getObstacle( size_t i ) {
			if ( i >= _obstacles.size() ) {
				throw ObstacleSetFatalException("Trying to access obstacle with invalid index value");
			}
			return _obstacles[ i ];
		}

		//////////////////////////////////////////////////////////////////////////////
			
		bool  ListObstacleSet::addObstacle(Obstacle *o) {
			o->_class = _class;
			o->_id = _obstacles.size();
			_obstacles.push_back(o);	
			return true;
		};

		//////////////////////////////////////////////////////////////////////////////

		bool ListObstacleSet::addObstacle(ObstacleVertexList o){
			// TODO: confirm that class ID is truly a 2^k kind of value
			// TODO: Do something with the open/close thing
			if (o.vertices.size() < 2) {
				throw ObstacleSetFatalException("Obstacle with too few vertices");
			}

			size_t firstId = _obstacles.size();
			size_t prevId = firstId-1;
			const size_t VCOUNT = o.vertices.size();

			for (size_t i = 0; i < VCOUNT - 1; ++i) {
				Obstacle* obstacle = new Obstacle();
				obstacle->setClosedState( o.closed );
				
				obstacle->_point = o.vertices[i];
				if (i > 0) {
					obstacle->_prevObstacle = _obstacles[prevId];
					obstacle->_prevObstacle->_nextObstacle = obstacle;
				}
		 
				Vector2 dir = o.vertices[ i+1 ] - o.vertices[ i ];
				float length = abs( dir );
				obstacle->_length = length;
				obstacle->_unitDir = dir / length;
				// default case is that it is convex.
				obstacle->_isConvex = true;
				if ( i > 0 && VCOUNT > 2 ) {
					obstacle->_isConvex = leftOf( o.vertices[ i-1 ], o.vertices[ i ], o.vertices[ i+1 ] ) >= 0;
				}

				addObstacle(obstacle);
				prevId = obstacle->_id;
			}

			if ( o.closed ) {
				// create an obstacle connecting the last vertex to the first vertex
				Obstacle* obstacle = new Obstacle();
				obstacle->setClosedState( o.closed );
				obstacle->_point = o.vertices[ VCOUNT - 1 ];
				obstacle->_prevObstacle = _obstacles[prevId];
				obstacle->_prevObstacle->_nextObstacle = obstacle;
				obstacle->_nextObstacle = _obstacles[ firstId ];
				obstacle->_nextObstacle->_prevObstacle = obstacle;
		 
				Vector2 dir = o.vertices[ 0 ] - o.vertices[ VCOUNT - 1 ];
				float length = abs( dir );
				obstacle->_length = length;
				obstacle->_unitDir = dir / length;
				
				// default case is that it is convex.
				obstacle->_isConvex = true;
				if ( VCOUNT > 2 ) {
					obstacle->_isConvex = leftOf( o.vertices[ VCOUNT - 2 ], o.vertices[ VCOUNT - 1 ], o.vertices[ 0 ] ) >= 0;
					Obstacle * obst = obstacle->_nextObstacle;
					obst->_isConvex = leftOf( o.vertices[ VCOUNT - 1 ], o.vertices[ 0 ], o.vertices[ 1 ] ) >= 0;
				}

				addObstacle(obstacle);
			}

			return true;
			
		};
		
	}	// namespace Agents
}	// namespace Menge
