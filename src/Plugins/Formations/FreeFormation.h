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
 *	@file		FreeFormation.h
 *	@brief		Implementatin of freestyle formations.
 *				
 * An implemenatation of the paper at:
 * http://graphics.cs.uh.edu/wp-content/papers/2013/2011_CGA-crowd-formation-generation-preprint.pdf
 */ 

#ifndef __FREE_FORMATION_H__
#define __FREE_FORMATION_H__

#include <map>
#include <vector>
#include "FSM.h"
#include "BaseAgent.h"
#include "resources/Resource.h"
#include "mengeCommon.h"
#include "PrefVelocity.h"

using namespace Menge;


/*!
 *	@namespace		Formations
 *	@brief			The name space for the Formation Model
 *
 *	This formation model is based off of 
 *http://graphics.cs.uh.edu/wp-content/papers/2013/2011_CGA-crowd-formation-generation-preprint.pdf
 *
 * We use input files which contain the formation coordinates to
 * define a formation, and a task and velocity modifier to execute them.
 */

namespace Formations {

	/*!
	 * @brief		The data structure representing a point in the formation.
	 */
	struct FormationPoint {
		size_t _id;		///< The id of the sentinel point
		Vector2 _pos;	///< The position of the sentinel point (in formation space) 
		float _dist;	///< The distance of the sentinel point to the formation center
		Vector2 _dir;	///< The direction of the sentinel point to the formation center
		bool _border;	///< Flag indicating if this is a border point (true) or not (false).
		float _weight;	///< The weight of the sentinel point 
	};

	/*!
	 * @brief		The class for modeling a freestyle formation. 
	 */
	class FreeFormation : public Resource{
	public:
		/*!
		 *  @brief		Constructor
		 *
		 *  @param	name		The name of the file containing the target formation.
		 */
		FreeFormation(const std::string & name);

		/*!
		 *	@brief		Destructor
		 */
		~FreeFormation();

		/*!
		 *	@brief		Reports the message label for this resource.
		 *
		 *	@returns	The label for formations.
		 */
		virtual const std::string & getLabel() const { return LABEL; }

		/*!
		 *	@brief		Parses a formation definition and returns a pointer to it.
		 *
		 *	This function works in conjunction with the ResourceManager.  That is why it
		 *	returns a pointer, not to a Formation, but to a Resource.  The ResourceManager
		 *	uses it to load and instantiateResource instances.
		 *
		 *	@param		fileName		The path to the file containing the Formation
		 *								definition.
		 *	@returns	A pointer to the new Formation (if the file is valid), NULL if
		 *				invalid.
		 */
		static Resource * load( const std::string & fileName );

		/*!
		 *	@brief		The unique label for this data type to be used with 
		 *				resource management.
		 */
		static const std::string LABEL;

		/*!
		 *	@brief		Adds an agent to this formation. 
		 *
		 *	Only agents "added" to the formation will be mapped considered.
		 *	?? What happens if more agents htan formation points are added?
		 *
		 *	@param		agt		The agent to add to the formation.
		 */
		void addAgent( const Agents::BaseAgent * agt );

		/*!
		 *	@brief		Removes an agent from the formation.
		 *
		 *	@param		agt		The agent to remove.
		 */
		void removeAgent(const Agents::BaseAgent *agt);
		
		/*!
		 *	@brief		Computes the mapping from tracked agents to formation points.
		 *
		 *	@param		fsm		A pointer to the FSM.
		 *
		 */
		void mapAgentsToFormation(const BFSM::FSM * fsm);
		
		/*!
		 *	@brief		Provides an intermediate goal for the agent.
		 *
		 *	This assumes that the agent provided is a member of the formation.  It only
		 *	checks in debug mode (via an assertion).
		 *
		 *	@param		agt		The agent for whom the goal is provided.
		 *	@param		pVel	The agent's preferred velocity...does it change??
		 *	@param		target	The intermediate goal value is stored in this vector.
		 *	@returns	True if the an intermediate goal exists and is set in target, false otherwise.
		 */
		bool getGoalForAgent(const Agents::BaseAgent * agt, Agents::PrefVelocity &pVel, Vector2 &target);

	protected:

		/*!
		 *	@brief		Maps a single agent to a sentinel point
		 *
		 *	@param		agt		The agent to map to a sentinel point.
		 *
		 */
		void mapAgentToPoint(const Agents::BaseAgent *agt);

		/*!
		 *	@brief		Maps a border point to one of the agents in the formation.
		 *
		 *	@param		pt		A border point to map to an agent.
		 *
		 */
		void mapPointToAgent(FormationPoint *pt);

		/*!
		 *	@brief		Adds a point to the formation.
		 *
		 *	@param		pt				A point (in formation space).
		 *	@param		borderPoint		True if the point should be considered a border point (false otherwise).
		 *	@param		weight			The weight of the point.
		 */
		void addFormationPoint(Vector2 pt, bool borderPoint, float weight ); 

		/*!
		 *	@brief		Adds an agent to the formation.
		 *
		 *	@param		agt		The agent to add to the formation.
		 */
		void addAgentPoint(const Agents::BaseAgent *agt); 

		/*!
		 *	@brief		Finalize the formation representation for use.
		 *				
		 *	Normalizing the formation defines distances between the formation center
		 *	and formation points relative to the *size* of the formation.  This allows
		 *	for arbitrary scales.
		 */
		void normalizeFormation();

		/*!
		 *	@brief		A custom distance metric to apply to formation points. Used
		 *				for evaluating "similarity" between formation points.
		 *
		 *	@param		pt1		the first point to check
		 *	@param		pt2		the other point to check
		 *	@returns	The "distance" between the two formatin points.
		 */
		float formationDistance( FormationPoint *pt1, FormationPoint *pt2 );

		/*!
		 *	@brief		Maps formation point identifiers to agent identifiers.
		 *				Why identifiers?
		 */
		std::map< size_t, size_t > _agent_formationPoint;

		/*!
		 *	@brief		Maps agent identifiers to formation point identifiers.
		 *				Why identifiers?
		 */
		std::map< size_t, size_t > _formationPoint_agent;

		/*!
		 *	@brief		Maps agent identifiers to formation points.
		 *				This lets me seach over a space instead of 
		 *				doing a lot of vector math each timestep
		 *				? WHAT ?
		 */
		std::map< size_t, FormationPoint * > _agentPoints;

		/*!
		 *	@brief		The formation points defining the formation.
		 */
		std::vector< FormationPoint * >  _formationPoints;

		/*!
		 *	@brief		A separate cache of border points -- this is a subset of _formationPoints.
		 */
		std::vector< FormationPoint * > _borderPoints;

		/*!
		 *	@brief		The formation's direction of travel.
		 */
		Vector2 _direction;

		/*!
		 *	@brief		The preferred speed of the formation.
		 */
		float _speed;

		/*!
		 *	@brief		The location of the formation center in world space (0,0).
		 */
		Vector2 _pos;

		/*!
		 *	@brief		The instantaneous max distance from the center of the formation to normalized agent positions
		 *				??
		 */
		float _agentRadius;

		/*!
		 *	@brief		A cache of previoius agent preferred directions.
		 *				Maps agent identifiers to directions.
		 */
		std::map< size_t, Vector2 > _agentPrefDirs;

		/*!
		 *	@brief		A cache of previoius agent preferred velocities.
		 *				Maps agent identifiers to velocities.
		 */
		std::map< size_t, Vector2 > _agentPrefVels;

		/*!
		 *	@brief		A cache of previoius agent preferred weights.
		 *				Maps agent identifiers to weights.
		 *
		 *	TODO: Currently, these are *always* 1.0.  Provide mechanism
		 *	for defininig non-unit weights.
		 */
		std::map< size_t, float > _agentWeights;

		/*!
		 *	@brief		A cache of previoius agent preferred directions.
		 *				Maps agent identifiers to agents.
		 */
		std::map< size_t, const Agents::BaseAgent * > _agents;
	};

	/*!
	 *	@brief		The definition of the managed pointer for formation data
	 */
	typedef ResourcePtr< FreeFormation > FormationPtr;

	/*!
	 *  @brief       load a formation
	 *	@param	fileName	The name of the file containing the formation definition.
     *	@returns	The FormationPtr containing the data.
     *	@throws		A ResourceException if the data is unable to be instantiated.
	 */
	FormationPtr loadFormation( const std::string & fileName ) throw ( ResourceException );
};

#endif
