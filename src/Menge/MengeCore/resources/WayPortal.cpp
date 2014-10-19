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

#include "WayPortal.h"

namespace Menge {

	/////////////////////////////////////////////////////////////////////
	//					Implementation of WayPortal
	/////////////////////////////////////////////////////////////////////

	WayPortal::WayPortal( const NavMeshEdge * edge, unsigned int nodeID, bool p0IsLeft ): _edge(edge), _nodeID(nodeID), _p0IsLeft(p0IsLeft) {
	}

	/////////////////////////////////////////////////////////////////////

	void WayPortal::setPreferredDirection( const Vector2 & pos, float radius, const Vector2 & dir, Agents::PrefVelocity & pVel ) const {
		_edge->setClearDirections( pos, radius, dir, pVel );

	}

	/////////////////////////////////////////////////////////////////////

	Vector2 WayPortal::intersectionPoint( const Vector2 & point, const Vector2 & dir ) const {
		Vector2 pDir = _edge->getDirection();
		Vector2 p0 = _edge->getP0();
		float denom = det( pDir, dir );
		//if ( fabs( denom ) <= EPS ) {
		//	// If the line is parallel with the portal, just use the point
		//	// This is a horrible, horrible hack.
		//	return point;
		//}
		assert( fabs( denom ) > EPS && "Parallel lines don't intersect" );

		float num = det( dir, p0 - point );
		float s = num / denom;
		return p0 + s * pDir;
	}

	/////////////////////////////////////////////////////////////////////
}	// namespace Menge