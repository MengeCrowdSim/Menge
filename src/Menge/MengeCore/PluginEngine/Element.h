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
 *	@file		Element.h
 *	@brief		Base class for all Menge elements.
 */

#ifndef __ELEMENT_H__
#define	__ELEMENT_H__

#include "CoreConfig.h"

namespace Menge {

	// forward declarations
	namespace SceneGraph {
		class GLNode;
	}

	namespace BFSM {
		class Task;
	}

	/*!
	 *	@brief		The basic interface of extendible Menge Elements.
	 *
	 *	A Menge element is a component of the Menge framework. A particular
	 *	crowd simulator is defined by the instantiation of various types of 
	 *	elements.
	 */
	class MENGE_API Element {
	public:
		/*!
		 *	@brief		This supplants the destructor.
		 *
		 *	In order to avoid potential problems in windows when
		 *	dlls do not share the same c-runtime library, the destructor
		 *	is held to be protected.  To garbage collect a Condition,
		 *	the destroy method should be called (which in turn, will call
		 *	the destructor from its own memory space, averting run-time
		 *  crashes).
		 *
		 *	Once this has been called, the TransitionTarget no longer exists.  
		 *	Calling methods or accessing members will produce indetermine behavior 
		 *	(most likely errors).
		 */
		void destroy() { delete this; }
			
		/*!
		 *	@brief		Return an optional task associated with this element.
		 *
		 *	An element can have an accompanying Task for performing synchronized work.
		 *	Most element implementations will not have a task.  If a task is required,
		 *	override this function to return an appropriate instance of the required
		 *	task.
		 *
		 *	@returns		A pointer to the required task.  It is the responsibility
		 *					of the caller to free the memory of the provided task by
		 *					calling its destroy method.
		 */
		virtual BFSM::Task * getTask() { return 0x0; }

		/*!
		 *	@brief		Returns an optional visualization element associated with the element.
		 *
		 *	This element will simply be added to the scene graph and will be destroyed by
		 *	the scene graph.  It should not be used if the visualization is context dependent.
		 *
		 *	@returns		A pointer to the scene graph node element.
		 */
		virtual SceneGraph::GLNode * getSGNode() { return 0x0; }
	};
}	// namespace Menge

#endif	 // __ELEMENT_H__