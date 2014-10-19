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
 *	@file	Select.h
 *	@brief	Functionality to make nodes in the scene graph selectable by mouse clicking.
 */

#ifndef __SELECT_H__
#define __SELECT_H__

#include "CoreConfig.h"
#include <vector>
#include <string>

namespace Menge {

	namespace SceneGraph {

		/*!
		 *	@brief	The class interface for selectable objects -- objects which can be selectedc
		 *			in the OpenGL context by clicking on them.
		 */
		class MENGE_API Selectable {
		public:
			/*!
			 *	@brief		Constructor.
			 */
			Selectable();

			/*!
			 *	@brief		Performs the OpenGL task to make this object selectable.
			 */
			void loadSelectName() const;

			/*!
			 *	@brief		Returns this object's globally unique *selection* id.
			 *
			 *	@returns	The selection id - a positive value.
			 */
			unsigned int getID() const { return _id; }
			
			/*!
			 *	@brief		A dummy function that makes Selectable polymorphic.
			 *				It enables the use of dynamic_cast.  Otherwise, it is a no-op.
			 */
			virtual void dummy() { return; }

			/*!
			 *	@brief		Retrives a pointer to the currently selected node.
			 *
			 *	@returns	A pointer to the selected object -- NULL if no object is selected.
			 */
			static Selectable * getSelectedObject() { return _selectedObject; }

			/*!
			 *	@brief		Returns the "name" of the currently selected object.
			 *
			 *	In this case, "name" refers to the OpenGL identifier.  It corresponds to
			 *	a *selectable* object's id (see Selectable::getID).
			 *
			 *	@returns	The OpenGL name (selectable id) of the currently selected object.
			 *				If no object is selected, it returns zero.
			 */
			static unsigned int getSelectedName() { return _selectedName; }

			/*!
			 *	@brief		Clears the current selection
			 */
			static void clearSelectedObject() { 
				if ( _selectedObject ) _selectedObject->_selected = false;
				_selectedObject = 0x0; 
				_selectedName = 0;
			}

			/*!
			 *	@brief		Forces an arbitrary selectable to be selected
			 *
			 *	@param		obj		A selectable object to set as selected.
			 */
			static void setSelectedObject( Selectable * obj ) {
				clearSelectedObject();
				_selectedName = obj->_id;
				_selectedObject = obj;
				obj->_selected = true;
			}

			/*!
			 *	@brief		The selection set up.
			 *
			 *	To perform selection, this needs to be called prior to drawing the scene of
			 *	selectable objects.
			 */
			static void selectStart();

			/*!
			 *	@brief		The selection take down.
			 *
			 *	When performing selection, after calling Selectable::selectStart and drawing the
			 *	scene, this must be called to conclude the selection process.
			 *
			 *	@returns	A boolean reporting if the selection *changed* (true) or not (false).
			 */
			static bool selectEnd();

			/*!
			 *	@brief		Reports the next available selection name
			 */
			static unsigned int nextSelectName() { return ID; }

		protected:
			/*!
			 *	@brief		Globally unique OpenGL name for selection.
			 */
			unsigned int	_id;

			/*!
			 *	@brief		Reports if this node is selcted.
			 *
			 *	This can be used by contexts or objects that change how they are drawn based
			 *	on selection state.
			 */
			bool			_selected;

		private:
			/*!
			 *	@brief		An OpenGL construct.  The size of a buffer to hold selection candiadtes.
			 */
			static const unsigned int BUFFER_SIZE;

			/*!
			 *	@brief		The OpenGL buffer for holding selection candidates.
			 */
			static unsigned int * _buffer;

			/*!
			 *	@brief		A gobal counter of the number of selectable objects in the scene.
			 *		
			 *	Used to assign a new globally unique selection id to the next Selectable.
			 */
			static unsigned int ID;

			/*!
			 *	@brief		The currently selected object 
			 *
			 *	This system currently only supports selection of one item at a time.  
			 *	TODO: support multiple selection as required.
			 */
			static Selectable *	_selectedObject;

			/*!
			 *	@brief		The OpenGL Selection name of the currently selected object.
			 */
			static unsigned int	_selectedName;

			/*!
			 *	@brief		Set of all selectable objects (their ids serve as indices into the list).
			 */
			static std::vector< Selectable * >	_selectables;

		};

		/*!
		 *	@brief		An STL Vector of Selectable objects.
		 */
		typedef std::vector< Selectable * > SelectableVector;
		
		/*!
		 *	@brief		An iterator for the SelectableVector.
		 */
		typedef SelectableVector::iterator SelectableVectorItr;
		
		/*!
		 *	@brief		A const iterator for the SelectableVector.
		 */
		typedef SelectableVector::const_iterator SelectableVectorCItr;

	}	// namespace SceneGraph
}	// namespace Menge
#endif	// __SELECT_H__
