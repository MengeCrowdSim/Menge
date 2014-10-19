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
 *	@file		GLContextManager.h
 *	@brief		The mechanism by which OpenGL memory structures are restored after
 *				an OpenGL context change..
 */

#ifndef __GL_CONTEXT_MANAGER_H__
#define __GL_CONTEXT_MANAGER_H__

#include "CoreConfig.h"
#include <list>

namespace Menge {

	/*!
	 *	@brief		Handles updating the scene graph when there is a new OpenGL context.
	 *
	 *	Through this class and its static members, all of the resources used in the scene graph 
	 *	are refreshed when the OpenGL context changes.  
	 *
	 *	Each resource registers a refresh callback  to the manager and then the viewer, when 
	 *	changin GL Contexts, calls newGLContext() to have the resources update.
	 *
	 *	If any new resources are added to the SceneGraph which have OpenGL-dependent memory
	 *	(such as textures, display lists, arrays of data, etc.) that class must provide
	 *	a static callback method and register it so its data can be restored in the new
	 *	context (e.g. ImageData, Circle, Cylinder, etc.).
	 */
	class MENGE_API GLContextManager {
		/*!
		 *	@brief		A list of callbacks for performing context changes.
		 */
		static std::list< void (*)() > _rsrcCallbacks;

	public:
		/*!
		 *	@brief		Register a new class callback to the context manager.
		 *
		 *	@param		newGLContext	A pointer to the callback function.
		 */
		static void addCallback( void (*newGLContext)() );

		/*!
		 *	@brief		Called when the OpenGL context changes - triggers all of 
		 *				the registered callbacks.
		 */
		static void newGLContext();
	};
}	// namespace Menge
#endif  // __GL_CONTEXT_MANAGER_H__
