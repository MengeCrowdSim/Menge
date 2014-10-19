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

#include "GLNode.h"

namespace Menge {

	namespace SceneGraph {

		///////////////////////////////////////////////////////////////////////////
		//				IMPLEMENTATION FOR GLNode
		///////////////////////////////////////////////////////////////////////////

		GLNode::GLNode( GLDagNode * parent ):_parent(0x0), _visible( true ) {
			if ( parent ) {
				parent->addChild( this );
			}
		}

		///////////////////////////////////////////////////////////////////////////

		GLNode::~GLNode() {
		}

		///////////////////////////////////////////////////////////////////////////
		//				IMPLEMENTATION FOR GLDagNode
		///////////////////////////////////////////////////////////////////////////

		GLDagNode::GLDagNode( GLDagNode * parent ):GLNode(parent), _children(0x0), _childCount(0) {
		}

		///////////////////////////////////////////////////////////////////////////

		GLDagNode::~GLDagNode() {
			if ( _children ) {
				for ( size_t i = 0; i < _childCount; ++i ) {
					delete _children[i];
				}
				delete [] _children;
			}
		}

		///////////////////////////////////////////////////////////////////////////

		// This isn't the most efficient memory management, time-wise, but if you assume
		//	that the construction of the graph is pre-processing then it optimizes evaluation over construction
		void GLDagNode::addChild( GLNode * child ) {
			GLNode ** newChildren = new GLNode *[ _childCount + 1 ];
			for ( size_t i = 0; i < _childCount; ++i ) {
				newChildren[i] = _children[i];
			}
			newChildren[ _childCount ] = child;
			++_childCount;
			if ( _children ) delete [] _children;
			_children = newChildren;
			child->setParent( this );
		}

		///////////////////////////////////////////////////////////////////////////

		void GLDagNode::drawGL( bool select ) {
			for ( size_t i = 0; i < _childCount; ++i ) {
				_children[i]->drawGL( select );
			}
		}

		///////////////////////////////////////////////////////////////////////////

		void GLDagNode::getWorldMatrix( Matrix4x4 & mat ) {
			if ( _parent ) {
				_parent->getWorldMatrix( mat );
			} else {
				mat.identity();
			}
		}

		///////////////////////////////////////////////////////////////////////////

		void GLDagNode::getWorldInverseMatrix( Matrix4x4 & mat ) {
			if ( _parent ) {
				_parent->getWorldInverseMatrix( mat );
			} else {
				mat.identity();
			}
		}

		///////////////////////////////////////////////////////////////////////////

		void GLDagNode::getParentMatrix( Matrix4x4 & mat ) {
			if ( _parent ) {
				_parent->getWorldInverseMatrix( mat );
			} else {
				mat.identity();
			}
		}

		///////////////////////////////////////////////////////////////////////////

		void GLDagNode::getParentInverseMatrix( Matrix4x4 & mat ) {
			if ( _parent ) {
				_parent->getWorldInverseMatrix( mat );
			} else {
				mat.identity();
			}
		}

		///////////////////////////////////////////////////////////////////////////

		void GLDagNode::newContext() {
			for ( size_t i = 0; i < _childCount; ++i ) {
				_children[i]->newContext();
			}
		}

		///////////////////////////////////////////////////////////////////////////

	}	// namespace SceneGraph
}	// namespace Menge