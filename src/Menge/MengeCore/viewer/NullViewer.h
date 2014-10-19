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
 *	@file		NullViewer.h
 *	@brief		Specification for a do-nothing viewer.  This is the
 *				offline simulator.
 */

#ifndef __NULL_VIEWER_H__
#define __NULL_VIEWER_H__

#include "CoreConfig.h"
#include "Profiler.h"

namespace Menge {

	// forward declarations
	namespace SceneGraph {
		class GLScene;
	}

	namespace Vis {

		/*!
		 *	@brief		The off-line context for running a simulation.
		 *
		 *				The NullViewer is a mock viewer, similar to the GLViewer
		 *				It takes a SceneGraph node and repeatedly updates the scene
		 *				However, there is no visualization.  It's purpose is simply
		 *				to run the scene.
		 *
		 *				The POINT of this type of thing is to load up an empty scene
		 *				with a non-trivial system that has some secondary, non-visual
		 *				side effects and simply exercise it.
		 */
		class MENGE_API NullViewer {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			NullViewer();

			/*!
			 *	@brief		Destructor.
			 */
			~NullViewer();

			/*!
			 *	@brief		The scene to run.
			 */
			void setScene( SceneGraph::GLScene * scene );

			/*!
			 *	@brief		The main loop.
			 */
			void run();

			/*!
			 *	@brief		Sets the simulator to use a fixed time step, with the given value.
			 *
			 *	@param		stepSize		The size of the fixed step the viewer should advance
			 *								its GLScene.
			 */
			void setFixedStep( float stepSize );

		protected:
			/*!
			 *	@brief		The GLScene to draw.
			 */
			SceneGraph::GLScene *	_scene;

			/*!
			 *	@brief		The step size for fixed-step simulation.
			 */
			float	_stepSize;

			/*!
			 *	@brief		Timer for determining computation time.
			 */
			LapTimer	_fpsTimer;
		};
	}	// namespace Vis
}	// namespace Menge
#endif	// __NULL_VIEWER_H__
