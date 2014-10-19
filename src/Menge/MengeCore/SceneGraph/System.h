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

// A system drives attributes of a graph
//		Before drawing a graph, systems get evaluated to make sure what's being drawn
//		is "correct" and "updated".
// Systems are relatively lazy.  Their update can be called, but only if the system KNOWS it needs
//     to be updated, will it update.

/*!
 *	@file		System.h
 *	@brief		The mechanism for evolving a scene w.r.t. time.
 */

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "CoreConfig.h"
#include "MengeException.h"
#include <list>
#include <string>

namespace Menge {

	namespace SceneGraph {

		/*!
		 *	@brief		Exception class for indicating that a system is done.
		 *
		 *	When a system will no longer change the the scene with calls to
		 *	System::updateScene, it throws the SystemStopException.  The caller
		 *	is responsible for catching and reacting appropriately.
		 */
		class MENGE_API SystemStopException : public Menge::MengeException {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			SystemStopException() : Menge::MengeException() {}		
		};

		/*!
		 *	@brief		An animation system.  Responsible for updating the scene based
		 *				based on increasing time values.
		 *
		 *	This is a purely virtual class.  It must be sub-classed to be used.
		 */
		class MENGE_API System {
		public:
			/*!
			 *	@brief		Default constructor.
			 */
			System() {}

			/*!
			 *	@brief		Virtual destructor.
			 */
			virtual ~System() {}

			/*!
			 *	@brief		Finalizing function for the system.
			 *
			 *	When the viewer wants to exit, it calls this method on the scene to give the
			 *	scene any last minute things it needs to do (such as outputting data)
			 *	The scene calls the systems finish method.
			 */
			virtual void finish(){}

			/*!
			 *	@brief		Updates the state of the scene graph with the global time.
			 *
			 *	@param		time		The time for which the system should be set.
			 *	@returns	True if the system changed the scene such that it should be redrawn.
			 */
			virtual bool updateScene( float time ) = 0;
		};

		/*!
		 *	@brief		An STL list of System objects.
		 */
		typedef std::list< System * > SystemList;

		/*!
		 *	@brief		An iterator for the SystemList.
		 */
		typedef SystemList::iterator SystemListItr;
		
		/*!
		 *	@brief		A const iterator for the SystemList.
		 */
		typedef SystemList::const_iterator SystemListCItr;

	}	// namespace SceneGraph
}	// namespace Menge
#endif	// __SYSTEM_H__
