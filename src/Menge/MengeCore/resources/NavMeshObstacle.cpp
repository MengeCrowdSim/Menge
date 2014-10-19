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

#include "NavMeshObstacle.h"
#include "Logger.h"
#include <iostream>
#include <limits>

namespace Menge {

	/*!
	 *	@brief		The minimum width for an edge to be considered valid.
	 */
	const float MIN_EDGE_WIDTH = 0.00001f;

	unsigned int NavMeshObstacle::NO_NEIGHBOR_OBST = std::numeric_limits< unsigned int >::max();

	////////////////////////////////////////////////////////////////
	//			Implementation of NavMeshObstacle
	////////////////////////////////////////////////////////////////

	#ifdef _WIN32
	// This disables a 64-bit compatibility warning - pushing a 32-bit value into a 64-bit value.
	// This can cause problems with SIGN EXTENSION.
	// In this case, I know the value in being put into the pointer slot is an unsigned
	//	int, so sign extension is not a problem.  Plus, they never get interpreted as
	//	pointers.  These indices are eventually mapped to REAL pointers.
	#pragma warning( disable : 4312 )
	#endif

	bool NavMeshObstacle::loadFromAscii( std::ifstream & f, Vector2 * vertices ) {
		size_t v0, v1, node;
		int nextObst;
		if ( ! ( f >> v0 >> v1 >> node >> nextObst ) ) {
			logger << Logger::ERR_MSG << "\tError in parsing nav mesh: missing edge data.\n";
			return false;
		} else {
			_point.set( vertices[ v0 ] );
			Vector2 disp = vertices[ v1 ] - vertices[ v0 ];
			_length = abs( disp );
			if ( _length <= MIN_EDGE_WIDTH ) {
				logger << Logger::ERR_MSG << "\tError in parsing nav mesh: obstacle is too narrow (length = " << _length << ").\n";
				return false;
			}
			_unitDir.set( disp / _length );
			// Stash indices as pointers
			if ( nextObst >= 0 ) {
				_nextObstacle = ( Obstacle * )nextObst;
			} else {
				_nextObstacle = ( Obstacle * )NO_NEIGHBOR_OBST;
			}
			_node = ( NavMeshNode * )node;
		}
		return true;
	}
	#ifdef _WIN32
	#pragma warning( default : 4312 )
	#endif
}	// namespace Menge