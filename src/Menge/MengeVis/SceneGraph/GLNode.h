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
 @file    GLNode.h
 @brief   The basic scene graph node. Any object which can be placed into the scene graph is an
          instance or sub-class of this node.
 */

#ifndef __GLNODE_H__
#define __GLNODE_H__

#include <list>
#include "MengeVis/SceneGraph/graphCommon.h"
#include "MengeVis/VisConfig.h"

namespace MengeVis {

namespace SceneGraph {

class GLDagNode;

/*!
 @brief   An abstact class -- a generic, scene graph node.

 A GLNode can be included in a hierarchy, but cannot take children. It is the basic entity that
 forms the "graph" of the scene graph. It cannot be added to the scene but must be sub-classed.
 */
class MENGEVIS_API GLNode {
 public:
  /*!
   @brief   Constructor.

   @param   parent    A pointer to the optional parent node in the graph.
   */
  GLNode(GLDagNode* parent = 0x0);

  /*!
   @brief   Virtual destructor.
   */
  virtual ~GLNode();

  /*!
   @brief   Sets the visible state of the node.

   A node which is not visible does not draw in the scene. Nor does any child or child-tree that
   the node has.

   @param   state   The desired visible state. True for a visible node that will be drawn, false for
                    invisible.
   */
  void setVisible(bool state) { _visible = state; }

  /*!
   @brief   Causes this node to draw itself to the scene.

   This is a purely virtual function. Every node in the graph *must* implement this function.

   @param   select    Determines if the draw call is being performed for the purpose of selection
                      (true) or for visualization (false).
   */
  virtual void drawGL(bool select = false) = 0;

  /*!
   @brief   Allows the node to recreate any unique OpenGL objects based on the acquisition of a new
            OpenGL context.

   This should be over-ridden by nodes that have their own unique OpenGL constructs (display lists,
   textures, etc.)
   */
  virtual void newContext() { return; }

  /*!
   @brief   Returns a pointer to the node's parent (possibly NULL).

   @returns A pointer to a parent node -- if no parent, it returns NULL (0x0).
   */
  GLDagNode* getParent() { return _parent; }

  friend class GLDagNode;

 protected:
  /*!
   @brief   Assigns this node to a parent GLDagNode.

   Nodes should be linked by calling GLDagNode::addChild.

   @param   p   The pointer to the new parent node.
   */
  void setParent(GLDagNode* p) { _parent = p; }

  /*!
   @brief   The GLDagNode that serves as this node's parent.
   */
  GLDagNode* _parent;

  /*!
   @brief   The visibility state of this node.
   
   If visible (true) the node and its children will be drawn into the scene, if invisible (false)
   it will not be drawn.
   */
  bool _visible;
};

/*!
 @brief   An STL list of GLNode objects.
 */
typedef std::list<GLNode*> GLNodeList;

/*!
 @brief   An iterator for the GLNodeList.
 */
typedef GLNodeList::iterator GLNodeListItr;

/*!
 @brief   A const iterator for the GLNodeList.
 */
typedef GLNodeList::const_iterator GLNodeListCItr;

/*!
 @brief   The node that provides the basis for a "hierarchy" in the scene graph.

 The scene graph is, ultimately, not truly a graph.  It is a directed acyclic graph, best thought of
 as a tree with the GLScene at the root and all other nodes inserted into sub-strees below the
 GLScene.

 The GLDagNode serves as the basis for this. It is a GLNode that can accept children, providing the
 mechanism for creating trees
 */
class MENGEVIS_API GLDagNode : public GLNode {
 public:
  /*!
   @brief   Constructor.

   @param   parent    A pointer to the optional parent node in the graph.
   */
  GLDagNode(GLDagNode* parent = 0x0);

  /*!
   @brief   Virtual destructor.
   */
  virtual ~GLDagNode();

  /*!
   @brief   Adds a child node to this node.

   Nodes can be linked either by calling GLNode::setParent() or GLDagNode::addChild().

   @param   child   The node to add as a child to this node.
   */
  void addChild(GLNode* child);

  /*!
   @brief   Performs any work to recreate OpenGL constructs for this node and its children nodes.
   */
  virtual void newContext();

  /*!
   @brief   Causes this node's child nodes to draw themselves to the scene.

   @param   select    Determines if the draw call is being performed for the purpose of selection
                      (true) or for visualization (false).
   */
  virtual void drawGL(bool select = false);

  /*!
   @brief   Reports the local object transform matrix.

   By definition, because this node can provide no transformation its transform matrix is the
   identity matrix.

   @param   mat   The 4x4 matrix into which the identity matrix is set.
   */
  virtual void getMatrix(Menge::Math::Matrix4x4& mat) { mat.identity(); }

  /*!
   @brief   Reports the local object inverse transform matrix.

   By definition, because this node can provide no transformation its inverse transform matrix is
   the identity matrix.

   @param   mat   The 4x4 matrix into which the identity matrix is set.
   */
  virtual void getInverseMatrix(Menge::Math::Matrix4x4& mat) { mat.identity(); }

  /*!
   @brief   Reports the world object transform matrix.

   This is the concatenation of all transforms experienced by this node and any node in its
   ancestors up to the GLSCene. This is essentially the transform that transforms elements in this
   object's space into world space.

   @param   mat   The 4x4 matrix into which the world matrix is set.
   */
  virtual void getWorldMatrix(Menge::Math::Matrix4x4& mat);

  /*!
   @brief   Reports the world object inverse transform matrix.

   This is the concatenation of all transforms experienced by this node and any node in its
   ancestors up to the GLSCene. This is essentially the transform that transforms elements in world
   space into this object's space.

   @param   mat   The 4x4 matrix into which the inverse world matrix is set.
   */
  virtual void getWorldInverseMatrix(Menge::Math::Matrix4x4& mat);

  /*!
   @brief    The world matrix of this node's parent matrix (see GLDagNode::getWorldMatrix()).

   @param   mat   The 4x4 matrix into which the result is written. If this node has no parent
                  matrix, the identity matrix is stored.
   */
  virtual void getParentMatrix(Menge::Math::Matrix4x4& mat);

  /*!
   @brief   The inverse world matrix of this node's parent matrix (see 
            GLDagNode::getWorldInverseMatrix()).

   @param   mat   The 4x4 matrix into which the result is written. If this node has no parent
                  matrix, the identity matrix is stored.
   */
  virtual void getParentInverseMatrix(Menge::Math::Matrix4x4& mat);

 protected:
  /*!
   @brief   An array of child GLNodes.
   */
  GLNode** _children;

  /*!
   @brief   The number of child nodes this node contains.
   */
  size_t _childCount;
};

/*!
 @brief   An STL list of GLDagNode objects.
 */
typedef std::list<GLDagNode*> GLDagNodeList;

/*!
 @brief   An iterator for the GLDagNodeList.
 */
typedef GLDagNodeList::iterator GLDagNodeListItr;

/*!
 @brief   A const iterator for the GLDagNodeList.
 */
typedef GLDagNodeList::const_iterator GLDagNodeListCItr;

}  // namespace SceneGraph
}  // namespace MengeVis
#endif  // __GLNODE_H__
