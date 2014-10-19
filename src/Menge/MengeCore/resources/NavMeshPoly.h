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
 *  @file       NavMeshPoly.h
 *  @brief      Defines the geometric, convex polygon for each navigation
 *				mesh node
 */

#ifndef __NAV_MESH_POLY_H__
#define __NAV_MESH_POLY_H__

#include "fsmCommon.h"
#include "string.h"
#include <fstream>

namespace Menge {

	// FORWARD DECLARATIONS
	class NavMeshNode;
	class NavMesh;

	/*!
	 *	@brief		The polygon used in each node of a navigation mesh graph
	 */
	class NavMeshPoly {
	public:
		/*!
		 *	@brief		Constructor
		 */
		NavMeshPoly();

		/*!
		 *	@brief		Destructor
		 */
		~NavMeshPoly();

		/*!
		 *	@brief		Assignment operator
		 */
		NavMeshPoly & operator=( const NavMeshPoly & n );

		/*!
		 *	@brief		Reports if the point lies inside the polygon.
		 *				NOTE: This is limited to 2D polygons.
		 *
		 *	@param		point		The point to test.
		 *	@returns	A boolean reporting if the point is inside (true)
		 *				or outside (false) the polygon
		 *				TODO: Figure out what I do w.r.t. boundaries.
		 */
		bool containsPoint( const Vector2 & point ) const;

		/*!
		 *	@brief		Computes the elevation of the polygon at the
		 *				given point.  
		 *
		 *	@param		point		A point on the polygon to evaluate.
		 *							The function does not test to see if the point
		 *							lies inside the polygon.
		 *	@returns	The elevation of the polygon at the given point.
		 */
		float getElevation( const Vector2 & point ) const;

		/*!
		 *	@brief		Reports the gradient of the polygon.
		 *				Because it is a plane, the gradient is constant at all positions.
		 *
		 *	@returns	The gradient of the plane.
		 */
		Vector2 getGradient() const { return Vector2( _A, _B ); }

		/*!
		 *	@brief		Sets the polygon properties from a polygon definition
		 *				in the given ascii file stream.
		 *	
		 *	@param		f		The input file stream.
		 *	@returns	A boolean indicating successful parsing (true) or failure
		 *				(false).
		 */
		bool loadFromAscii( std::ifstream & f );

		/*!
		 *	@brief		Sets the polygon properties from a polygon definition
		 *				in the given binary file stream.
		 *	
		 *	@param		f		The input file stream.
		 *	@returns	A boolean indicating successful parsing (true) or failure
		 *				(false).
		 */
		bool loadFromBinary( std::ifstream & f );

		/*!
		 *	@brief		Initialize the polygon with vertices and equation of plane
		 *
		 *	@param		vCount		Number of vertices in the polygon.
		 *	@param		ids			An array of index values into the global
		 *							set of vertices in the navigation mesh.
		 *	@param		A			The first coefficient of the planar equation:
		 *							f( x, y ) = Ax + By + C
		 *	@param		B			The second coefficient of the planar equation:
		 *							f( x, y ) = Ax + By + C
		 *	@param		C			The third coefficient of the planar equation:
		 *							f( x, y ) = Ax + By + C
		 */
		void initialize( size_t vCount, unsigned int * ids, float A=0.f, float B=0.f, float C=0.f );

		friend class NavMeshNode;
		friend class NavMesh;

	protected:
		/*!
		 *	@brief		An array of indices into the vertex list defining the
		 *			polygon.  The indices must be ordered such that the
		 *			vertices are visited in a counter-clockwise order.
		 *
		 * TODO: These are unsigned ints because of the binary file format.  It would
		 * be better to decouple the file format from the representation.
		 */
		unsigned int	*_vertIDs;

		/*!
		 *	@brief		The number of vertices in the polygon.
		 */
		size_t	 _vertCount;

		/*!
		 *	@brief		A pointer to the mesh vertex information for performing
		 *				geometric tests.
		 */	
		const Vector2 *  _vertices;

		// TODO: Replace the individual bounding box with a BVH outside of the
		//		polygon.  And then change the "is inside" function assuming
		//		that BVH tests have already been done.
		/*!
		 *	@brief		Minimum extent on the x-axis of the polygon.
		 */
		float _minX;	
		
		/*!
		 *	@brief		Maximum extent on the x-axis of the polygon.
		 */
		float _maxX;
		
		/*!
		 *	@brief		Minimum extent on the y-axis of the polygon.
		 */
		float _minY;
		
		/*!
		 *	@brief		Maximum extent on the y-axis of the polygon.
		 */
		float _maxY;

		/*!
		 *	@brief		Compute the bounding box for this polygon.
		 *
		 *	@param		vertices		The actucal vertices used for
		 *								geometric queries.
		 */
		void setBB( const Vector2 * vertices );

		/*!
		 *	@brief		The first coefficient of the planar equation:
		 *				f( x, y ) = Ax + By + C
		 *				Used to compute the elevation of the point on the polygon.
		 */
		float	_A;
		
		/*!
		 *	@brief		The second coefficient of the planar equation:
		 *				f( x, y ) = Ax + By + C
		 *				Used to compute the elevation of the point on the polygon.
		 */
		float	_B;
		
		/*!
		 *	@brief		The third coefficient of the planar equation:
		 *				f( x, y ) = Ax + By + C
		 *				Used to compute the elevation of the point on the polygon.
		 */
		float	_C;
	};
}	// namespace Menge

#endif // __NAV_MESH_POLY_H__
