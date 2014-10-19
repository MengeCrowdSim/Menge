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

// UTILS
#include "KNearestQuery.h"

namespace Menge {

	namespace Agents {
		
		////////////////////////////////////////////////////////////////////////
		//                IMPLEMENTATION OF KNearestQuery CLASS
		////////////////////////////////////////////////////////////////////////


		void KNearestQuery::filterAgent(const BaseAgent * agent, float distanceSquared){
			
			//if the agent is too far, and we have enough agents, just exit
			if (distanceSquared >= _maxAgentResultDistance && _agentResults.size() == _maxAgentResults)
				return;

			//if not, add the agent
			if (_agentResults.size() < _maxAgentResults) {

				_agentResults.push_back(NearAgent(distanceSquared,agent));
			}
			
			//we are trusting that results are in order
			size_t i = _agentResults.size() - 1;
			while (i != 0 && distanceSquared < _agentResults[i-1].distanceSquared) {
				_agentResults[i] = _agentResults[i-1];
				--i;
			}

			_agentResults[i] = NearAgent(distanceSquared,agent);

			if (distanceSquared > _maxAgentResultDistance)
				_maxAgentResultDistance = distanceSquared;

		};


		///////////////////////////////////////////////////////////

		void KNearestQuery::filterObstacle(const Obstacle * obstacle, float distanceSquared) {
				
			//make sure that the obstacle is not already disqualified
			if (distanceSquared >= _maxObstacleResultDistance && _obstacleResults.size() == _maxObstacleResults)
				return;

			if (_obstacleResults.size() != _maxObstacleResults){
				_obstacleResults.push_back(NearObstacle(distanceSquared, obstacle) );
			}
				
			size_t i = _obstacleResults.size() - 1;
			while ( i != 0 && distanceSquared < _obstacleResults[i-1].distanceSquared) {
				_obstacleResults[i] = _obstacleResults[i-1];
				--i;
			}
			
			_obstacleResults[i] = NearObstacle(distanceSquared, obstacle);

			if (distanceSquared > _maxObstacleResultDistance)
				_maxObstacleResultDistance = distanceSquared;	
		};

		///////////////////////////////////////////////////////////

		void KNearestQuery::startQuery() {
			_agentResults.clear();
			_obstacleResults.clear();
			_maxAgentResultDistance = 9999;
			_maxObstacleResultDistance = 9999;
			_queryPoint = Vector2(0,0);
		}

		///////////////////////////////////////////////////////////

		float KNearestQuery::getMaxAgentRange() {
			if (_agentResults.size() == _maxAgentResults)
				return _maxAgentResultDistance;

			return _initialRange;
		}

	}	// namespace Agents
}	// namespace Menge


