/*
 Menge Crowd Simulation Framework

 Copyright and trademark 2012-17 University of North Carolina at Chapel Hill

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0
 or
    LICENSE.txt in the root of the Menge repository.

 Any questions or comments should be sent to the authors menge@cs.unc.edu

 <http://gamma.cs.unc.edu/Menge/>
*/

/*!
 @file    Select.h
 @brief   Functionality to make nodes in the scene graph selectable by mouse clicking.
 */

#ifndef __SELECT_H__
#define __SELECT_H__

#include <string>
#include <vector>
#include "MengeVis/VisConfig.h"

namespace MengeVis {

namespace SceneGraph {

/*!
 @brief   The class interface for selectable objects -- objects which can be selectedc in the OpenGL
          context by clicking on them.
 */
class MENGEVIS_API Selectable {
 public:
  /*!
   @brief   Constructor.
   */
  Selectable();

  /*!
   @brief   Performs the OpenGL task to make this object selectable.
   */
  void loadSelectName() const;

  /*!
   @brief   Returns this object's globally unique *selection* id.

   @returns   The selection id - a positive value.
   */
  unsigned int getID() const { return _id; }

  /*!
   @brief   A dummy function that makes Selectable polymorphic.

   It enables the use of dynamic_cast.  Otherwise, it is a no-op.
   */
  virtual void dummy() { return; }

  /*!
   @brief   Retrives a pointer to the currently selected node.

   @returns   A pointer to the selected object -- NULL if no object is selected.
   */
  static Selectable* getSelectedObject() { return _selectedObject; }

  /*!
   @brief   Returns the "name" of the currently selected object.

   In this case, "name" refers to the OpenGL identifier. It corresponds to a *selectable* object's
   id (see Selectable::getID).

   @returns The OpenGL name (selectable id) of the currently selected object. If no object is
            selected, it returns zero.
   */
  static unsigned int getSelectedName() { return _selectedName; }

  /*!
   @brief   Clears the current selection
   */
  static void clearSelectedObject() {
    if (_selectedObject) _selectedObject->_selected = false;
    _selectedObject = 0x0;
    _selectedName = 0;
  }

  /*!
   @brief   Forces an arbitrary selectable to be selected

   @param   obj   A selectable object to set as selected.
   */
  static void setSelectedObject(Selectable* obj) {
    clearSelectedObject();
    _selectedName = obj->_id;
    _selectedObject = obj;
    obj->_selected = true;
  }

  /*!
   @brief   The selection set up.

   To perform selection, this needs to be called prior to drawing the scene of selectable objects.
   */
  static void selectStart();

  /*!
   @brief   The selection take down.

   When performing selection, after calling Selectable::selectStart and drawing the scene, this must
   be called to conclude the selection process.

   @returns A boolean reporting if the selection *changed* (true) or not (false).
   */
  static bool selectEnd();

  /*!
   @brief   Reports the next available selection name
   */
  static unsigned int nextSelectName() { return ID; }

 protected:
  /*!
   @brief   Globally unique OpenGL name for selection.
   */
  unsigned int _id;

  /*!
   @brief   Reports if this node is selcted.

   This can be used by contexts or objects that change how they are drawn based on selection state.
   */
  bool _selected;

 private:
  /*!
   @brief   An OpenGL construct; the size of a buffer to hold selection candiadtes.
   */
  static const unsigned int BUFFER_SIZE;

  /*!
   @brief The OpenGL buffer for holding selection candidates.
   */
  static unsigned int* _buffer;

  /*!
   @brief   A gobal counter of the number of selectable objects in the scene.

   Used to assign a new globally unique selection id to the next Selectable.
   */
  static unsigned int ID;

  /*!
   @brief   The currently selected object

   This system currently only supports selection of one item at a time. TODO(curds01): support
   multiple selection as required.
   */
  static Selectable* _selectedObject;

  /*!
   @brief   The OpenGL Selection name of the currently selected object.
   */
  static unsigned int _selectedName;

  /*!
   @brief   Set of all selectable objects (their ids serve as indices into the list).
   */
  static std::vector<Selectable*> _selectables;
};

/*!
 @brief   An STL Vector of Selectable objects.
 */
typedef std::vector<Selectable*> SelectableVector;

/*!
 @brief   An iterator for the SelectableVector.
 */
typedef SelectableVector::iterator SelectableVectorItr;

/*!
 @brief   A const iterator for the SelectableVector.
 */
typedef SelectableVector::const_iterator SelectableVectorCItr;

}  // namespace SceneGraph
}  // namespace MengeVis
#endif  // __SELECT_H__
