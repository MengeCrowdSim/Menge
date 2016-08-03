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

#include "AgentKDTree.h"
#include "BaseAgent.h"
#include <algorithm>

namespace Menge {

	namespace Agents {

		/////////////////////////////////////////////////////////////////////////////
		//                     Implementation of AgentKDTree
		/////////////////////////////////////////////////////////////////////////////

		AgentKDTree::AgentKDTree(): _agents(), _tree() {
		}

		/////////////////////////////////////////////////////////////////////////////

		void AgentKDTree::setAgents( const std::vector< BaseAgent * > & agents ) {
			const size_t AGT_COUNT = agents.size();
			_agents.resize( AGT_COUNT );
			for ( size_t i = 0; i < AGT_COUNT; ++i ) {
				_agents[ i ] = agents[i];
			}
			_tree.resize( 2 * AGT_COUNT - 1 );
			
			if ( AGT_COUNT > 0 ) {
				buildTreeRecursive( 0, AGT_COUNT, 0 );
			}
		}

		/////////////////////////////////////////////////////////////////////////////

		void AgentKDTree::buildTree() {
			if ( _agents.size() > 0 ) {
				buildTreeRecursive( 0, _agents.size(), 0 );
			}
		}

		/////////////////////////////////////////////////////////////////////////////

		void AgentKDTree::agentQuery( ProximityQuery *filter) const {
			float range = filter->getMaxAgentRange();
			queryTreeRecursive( filter, filter->getQueryPoint(), range, 0 );
		}

		/////////////////////////////////////////////////////////////////////////////

		void AgentKDTree::buildTreeRecursive(size_t begin, size_t end, size_t node) {
			_tree[node]._begin = begin;
			_tree[node]._end = end;
			const Vector2 & pos = _agents[begin]->_pos;
			_tree[node]._minX = _tree[node]._maxX = pos.x();
			_tree[node]._minY = _tree[node]._maxY = pos.y();
		    
			for (size_t i = begin + 1; i < end; ++i) {
				const Vector2 & posI = _agents[i]->_pos;
				_tree[node]._maxX = std::max(_tree[node]._maxX, posI.x());
				_tree[node]._minX = std::min(_tree[node]._minX, posI.x());
				_tree[node]._maxY = std::max(_tree[node]._maxY, posI.y());
				_tree[node]._minY = std::min(_tree[node]._minY, posI.y());
			}

			if (end - begin > MAX_LEAF_SIZE) {
				/* No leaf node. */
				const bool isVertical = (_tree[node]._maxX - _tree[node]._minX > _tree[node]._maxY - _tree[node]._minY);
				const float splitValue = (isVertical ? 0.5f * (_tree[node]._maxX + _tree[node]._minX) : 0.5f * (_tree[node]._maxY + _tree[node]._minY));

				size_t left = begin;
				size_t right = end;

				
				while (left < right) {
					Vector2 posL = _agents[left]->_pos;
					while (left < right && (isVertical ? posL.x() : posL.y()) < splitValue) {
						++left;
						posL = _agents[left]->_pos;
					}

					Vector2 posR = _agents[right-1]->_pos;
					while (right > left && (isVertical ? posR.x() : posR.y()) >= splitValue) {
						--right;
						posR = _agents[right-1]->_pos;
					}

					if (left < right) {
						  std::swap(_agents[left], _agents[right-1]);
						  ++left;
						  --right;
					}
				}

				size_t leftSize = left - begin;

				if (leftSize == 0) {
					++leftSize;
					++left;
					++right;
				}

				_tree[node]._left = node + 1;
				_tree[node]._right = node + 1 + (2 * leftSize - 1);

				buildTreeRecursive(begin, left, _tree[node]._left);
				buildTreeRecursive(left, end, _tree[node]._right);
			}
		}

		/////////////////////////////////////////////////////////////////////////////

		void AgentKDTree::queryTreeRecursive( ProximityQuery *filter, Vector2 pt, float& rangeSq, size_t node) const {
			if (_tree[node]._end - _tree[node]._begin <= MAX_LEAF_SIZE) {
				for (size_t i = _tree[node]._begin; i < _tree[node]._end; ++i) {
					float distance = pt.distanceSq(_agents[i]->_pos);
					if (distance < rangeSq){
						filter->filterAgent( _agents[i], distance );
					}
					rangeSq = filter->getMaxAgentRange();
				}
			} else {
				float x = pt.x();
				float y = pt.y();
				const float distSqLeft =  sqr(std::max(0.0f, _tree[_tree[node]._left ]._minX - x)) + 
										sqr(std::max(0.0f, x - _tree[_tree[node]._left ]._maxX)) + 
										sqr(std::max(0.0f, _tree[_tree[node]._left ]._minY - y)) + 
										sqr(std::max(0.0f, y - _tree[_tree[node]._left ]._maxY));

				const float distSqRight = sqr(std::max(0.0f, _tree[_tree[node]._right]._minX - x)) + 
										sqr(std::max(0.0f, x - _tree[_tree[node]._right]._maxX)) + 
										sqr(std::max(0.0f, _tree[_tree[node]._right]._minY - y)) + 
										sqr(std::max(0.0f, y - _tree[_tree[node]._right]._maxY));

				if (distSqLeft < distSqRight) {
					if (distSqLeft < rangeSq) {
						queryTreeRecursive(filter, pt, rangeSq, _tree[node]._left);

						if (distSqRight < rangeSq) {
							queryTreeRecursive(filter, pt, rangeSq, _tree[node]._right);
						}
					}
				} else if (distSqRight < rangeSq) {
					queryTreeRecursive(filter, pt,  rangeSq, _tree[node]._right);

					if (distSqLeft < rangeSq) {
						queryTreeRecursive(filter, pt, rangeSq, _tree[node]._left);
					}
				}
			}
		}

		/////////////////////////////////////////////////////////////////////////////
	}	// namespace Agents
}	// namespace Menge