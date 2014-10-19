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

#ifndef __VIS_OBSTACLE_H__
#define __VIS_OBSTACLE_H__

/*!
 *	@file		VisObstacle.h
 *	@brief		The node for visualizing a simulation obstacle.
 */
#include "CoreConfig.h"
#include "Math/Vector3.h"
#include "GLNode.h"

namespace Menge {
	using namespace Math;

	/*!
	 *	@brief		A simple class for drawing a simulation obstacle (line segment).
	 *				The obstacle is drawn with lines in 3D space.
	 */
	class MENGE_API VisObstacle : public SceneGraph::GLNode {
	public:
		/*!
		 *	@brief		Constructor.
		 *
		 *	@param		p0		The first end point of the line segment.
		 *	@param		p1		The second end point of the line segment.
		 */
		VisObstacle( const Vector3 & p0, const Vector3 & p1 );

		/*!
		 *	@brief		Draws the object into the OpenGL context.
		 *
		 *	@param		select		Determines if the object is being drawn
		 *							in a selection context (true) or visualization (false).
		 */
		void drawGL( bool select=false );

	protected:
		/*!
		 *	@brief		The line segment's first end point.
		 */
		Vector3 _p0;

		/*!
		 *	@brief		The line segment's second end point.
		 */
		Vector3 _p1;
	};
}	// namespace Menge
#endif	//__VIS_OBSTACLE_H__