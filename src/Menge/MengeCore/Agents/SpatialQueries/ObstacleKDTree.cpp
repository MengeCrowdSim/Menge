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

#include "ObstacleKDTree.h"
#include "BaseAgent.h"
#include "Math/consts.h"
#include <algorithm>

namespace Menge {

	namespace Agents {

		/////////////////////////////////////////////////////////////////////////////
		//                     Implementation of ObstacleKDTree
		/////////////////////////////////////////////////////////////////////////////

		ObstacleKDTree::ObstacleKDTree(): _obstacles(), _tree(0x0) {
		}

		/////////////////////////////////////////////////////////////////////////////

		ObstacleKDTree::~ObstacleKDTree() {
			deleteTree();
		}

		/////////////////////////////////////////////////////////////////////////////

		void ObstacleKDTree::buildTree( const std::vector< Obstacle * > obstacles ) {
			deleteTree();
			
			_obstacles.assign( obstacles.begin(), obstacles.end() );
			if ( _obstacles.size() > 0 ) {
				std::vector< Obstacle * > temp;
				temp.assign( _obstacles.begin(), _obstacles.end() );
				_tree = buildTreeRecursive( temp );
			}
		}

		/////////////////////////////////////////////////////////////////////////////

		void ObstacleKDTree::obstacleQuery( ProximityQuery *filter ) const {
			float range = filter->getMaxObstacleRange();
			queryTreeRecursive(filter, filter->getQueryPoint(), range, _tree );
		}

		/////////////////////////////////////////////////////////////////////////////

		bool ObstacleKDTree::queryVisibility(const Vector2& q1, const Vector2& q2, float radius) const {
			return queryVisibilityRecursive(q1, q2, radius, _tree);
		}

		/////////////////////////////////////////////////////////////////////////////

		ObstacleTreeNode* ObstacleKDTree::buildTreeRecursive( const std::vector<Obstacle*>& obstacles) {
			if ( obstacles.empty() ) {
				return 0x0;
			} else {
				ObstacleTreeNode* const node = new ObstacleTreeNode;

				size_t optimalSplit = 0;
				size_t minLeft = obstacles.size();
				size_t minRight = obstacles.size();

				for (size_t i = 0; i < obstacles.size(); ++i) {
					size_t leftSize = 0;
					size_t rightSize = 0;

					const Obstacle* const obstacleI = obstacles[i];
					const Vector2 I0 = obstacleI->getP0();
					const Vector2 I1 = obstacleI->getP1();
					
					/* Compute optimal split node. */
					for (size_t j = 0; j < obstacles.size(); ++j) {
						if (i == j) {
							continue;
						}

						const Obstacle* const obstacleJ = obstacles[j];
						const Vector2 J0 = obstacleJ->getP0();
						const Vector2 J1 = obstacleJ->getP1();

						const float j1LeftOfI = leftOf( I0, I1, J0 );
						const float j2LeftOfI = leftOf( I0, I1, J1 );

						if (j1LeftOfI >= -EPS && j2LeftOfI >= -EPS) {
							++leftSize;
						} else if (j1LeftOfI <= EPS && j2LeftOfI <= EPS ) {
							++rightSize;
						} else {
							++leftSize;
							++rightSize;
						}

						if (std::make_pair(std::max(leftSize, rightSize), std::min(leftSize, rightSize)) >= std::make_pair(std::max(minLeft, minRight), std::min(minLeft, minRight))) {
							break;
						}
					}

					if (std::make_pair(std::max(leftSize, rightSize), std::min(leftSize, rightSize)) < std::make_pair(std::max(minLeft, minRight), std::min(minLeft, minRight))) {
						minLeft = leftSize;
						minRight = rightSize;
						optimalSplit = i;
					}
				}

				/* Build split node. */
				std::vector<Obstacle*> leftObstacles(minLeft);
				std::vector<Obstacle*> rightObstacles(minRight);

				size_t leftCounter = 0;
				size_t rightCounter = 0;
				const size_t i = optimalSplit;

				const Obstacle* const obstacleI = obstacles[i];
				const Vector2 I0 = obstacleI->getP0();
				const Vector2 I1 = obstacleI->getP1();

				for (size_t j = 0; j < obstacles.size(); ++j) {
					if (i == j) {
						continue;
					}

					Obstacle* const obstacleJ = obstacles[j];
					const Vector2 J0 = obstacleJ->getP0();
					const Vector2 J1 = obstacleJ->getP1();

					const float j1LeftOfI = leftOf( I0, I1, J0 );
					const float j2LeftOfI = leftOf( I0, I1, J1 );

					if (j1LeftOfI >= -EPS && j2LeftOfI >= -EPS ) {
						leftObstacles[leftCounter++] = obstacles[j];
					} else if (j1LeftOfI <= EPS && j2LeftOfI <= EPS ) {
						rightObstacles[rightCounter++] = obstacles[j];
					} else {
						/* Split obstacle j. */
						const float t = det( I1 - I0, J0 - I0 ) / det( I1 -I0, J0 - J1 );

						const Vector2 splitpoint = J0 + t * ( J1 - J0 );

						Obstacle* const newObstacle = new Obstacle();
						newObstacle->_point = splitpoint;
						newObstacle->_prevObstacle = obstacleJ;
						newObstacle->_nextObstacle = obstacleJ->_nextObstacle;
						if ( newObstacle->_nextObstacle ) {
							obstacleJ->_nextObstacle = newObstacle;
						}
						newObstacle->_isConvex = true;
						newObstacle->_unitDir = obstacleJ->_unitDir;
						newObstacle->_length = abs( J1 - newObstacle->_point );

						newObstacle->_id = _obstacles.size();
						newObstacle->_class = obstacleJ->_class;

						_obstacles.push_back(newObstacle);

						obstacleJ->_nextObstacle = newObstacle;
						obstacleJ->_length = abs( J0 - newObstacle->_point );

						if (j1LeftOfI > 0.0f) {
							leftObstacles[leftCounter++] = obstacleJ;
							rightObstacles[rightCounter++] = newObstacle;
						} else {
							rightObstacles[rightCounter++] = obstacleJ;
							leftObstacles[leftCounter++] = newObstacle;
						}
					}
				}

				node->_obstacle = obstacleI;
				node->_left = buildTreeRecursive(leftObstacles);
				node->_right = buildTreeRecursive(rightObstacles);
				return node;
			}
		}

		/////////////////////////////////////////////////////////////////////////////

		void ObstacleKDTree::queryTreeRecursive( ProximityQuery *filter, Vector2 pt, float& rangeSq, const ObstacleTreeNode* node) const {
			if (node == 0) {
				return;
			} else {
				const Obstacle* const obstacle1 = node->_obstacle;
				
				const Vector2 P0 = obstacle1->getP0();
				const Vector2 P1 = obstacle1->getP1();

				const float agentLeftOfLine = leftOf( P0, P1, pt);

				queryTreeRecursive(filter, pt, rangeSq, (agentLeftOfLine >= 0.0f ? node->_left : node->_right));

				const float distSqLine = sqr(agentLeftOfLine) / absSq(P1 - P0);

				if (distSqLine < rangeSq) {
					if ( obstacle1->_doubleSided || agentLeftOfLine < 0.0f) {
						/*
						* Try obstacle at this node only if agent is on right side of
						* obstacle (and can see obstacle).
						*/
						float distSq = distSqPointLineSegment(node->_obstacle->getP0(), node->_obstacle->getP1(), pt);

						filter->filterObstacle(node->_obstacle, distSq);
						
						rangeSq = filter->getMaxObstacleRange();
					}

					/* Try other side of line. */
					queryTreeRecursive(filter, pt, rangeSq, (agentLeftOfLine >= 0.0f ? node->_right : node->_left));
				}
			}
		}

		/////////////////////////////////////////////////////////////////////////////

		bool ObstacleKDTree::queryVisibilityRecursive(const Vector2& q1, const Vector2& q2,
										  float radius, 
										  const ObstacleTreeNode* node) const {
			if (node == 0) {
				return true;
			} else {
				const Obstacle* const obstacle1 = node->_obstacle;
				const Obstacle* const obstacle2 = obstacle1->_nextObstacle;

				const float q1LeftOfI = leftOf(obstacle1->_point, obstacle2->_point, q1);
				const float q2LeftOfI = leftOf(obstacle1->_point, obstacle2->_point, q2);
				const float invLengthI = 1.0f / absSq(obstacle2->_point - obstacle1->_point);

				if (q1LeftOfI >= 0.0f && q2LeftOfI >= 0.0f) {
					return queryVisibilityRecursive(q1, q2, radius, node->_left) && ((sqr(q1LeftOfI) * invLengthI >= sqr(radius) && sqr(q2LeftOfI) * invLengthI >= sqr(radius)) || queryVisibilityRecursive(q1, q2, radius, node->_right));
				} else if (q1LeftOfI <= 0.0f && q2LeftOfI <= 0.0f) {
					return queryVisibilityRecursive(q1, q2, radius, node->_right) && ((sqr(q1LeftOfI) * invLengthI >= sqr(radius) && sqr(q2LeftOfI) * invLengthI >= sqr(radius)) || queryVisibilityRecursive(q1, q2, radius, node->_left));
				} else if (q1LeftOfI >= 0.0f && q2LeftOfI <= 0.0f) {
					/* One can see through obstacle from left to right. */
					return queryVisibilityRecursive(q1, q2, radius, node->_left) && queryVisibilityRecursive(q1, q2, radius, node->_right);
				} else {
					const float point1LeftOfQ = leftOf(q1, q2, obstacle1->_point);
					const float point2LeftOfQ = leftOf(q1, q2, obstacle2->_point);
					const float invLengthQ = 1.0f / absSq(q2 - q1);

					return (point1LeftOfQ * point2LeftOfQ >= 0.0f && sqr(point1LeftOfQ) * invLengthQ > sqr(radius) && sqr(point2LeftOfQ) * invLengthQ > sqr(radius) && queryVisibilityRecursive(q1, q2, radius, node->_left) && queryVisibilityRecursive(q1, q2, radius, node->_right));
				}
			}
		}

		/////////////////////////////////////////////////////////////////////////////

		void ObstacleKDTree::deleteTree() {
			deleteSubTree( _tree );
			_tree = 0x0;
		}

		/////////////////////////////////////////////////////////////////////////////

		void ObstacleKDTree::deleteSubTree( ObstacleTreeNode * node ) {
			if ( node != 0x0 ) {
				deleteSubTree( node->_left );
				deleteSubTree( node->_right );
				delete node;
			}
		}
	}	// namespace Agents
}	// namespace Menge