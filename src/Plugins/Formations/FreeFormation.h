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
 @file		FreeFormation.h
 @brief		Implementatin of freestyle formations.

 An implemenatation of the paper at:
 http://graphics.cs.uh.edu/wp-content/papers/2013/2011_CGA-crowd-formation-generation-preprint.pdf
 */

#ifndef __FREE_FORMATION_H__
#define __FREE_FORMATION_H__

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Agents/PrefVelocity.h"
#include "MengeCore/BFSM/FSM.h"
#include "MengeCore/mengeCommon.h"
#include "MengeCore/resources/Resource.h"

#include <map>
#include <vector>

/*!
 @namespace		Formations
 @brief			The name space for the Formation Model

 This formation model is based off of
 http://graphics.cs.uh.edu/wp-content/papers/2013/2011_CGA-crowd-formation-generation-preprint.pdf

 We use input files which contain the formation coordinates to define a formation, and a task and
 velocity modifier to execute them.
 */

namespace Formations {

/*!
 @brief		The data structure representing a point in the formation.
 */
struct FormationPoint {
  size_t _id;                 ///< The id of the sentinel point
  Menge::Math::Vector2 _pos;  ///< The position of the sentinel point (in formation space)
  float _dist;                ///< The distance of the sentinel point to the formation center
  Menge::Math::Vector2 _dir;  ///< The direction of the sentinel point to the formation center
  bool _border;               ///< Flag indicating if this is a border point (true) or not (false).
  float _weight;              ///< The weight of the sentinel point
};

/*!
 @brief		The class for modeling a freestyle formation.
 */
class FreeFormation : public Menge::Resource {
 public:
  /*!
   @brief		Constructor

   @param	name		The name of the file containing the target formation.
   */
  FreeFormation(const std::string& name);

  /*!
   @brief		Destructor
   */
  ~FreeFormation();

  /*!
   @brief		Reports the message label for this resource.

   @returns	The label for formations.
   */
  virtual const std::string& getLabel() const { return LABEL; }

  /*!
   @brief		Parses a formation definition and returns a pointer to it.

   This function works in conjunction with the ResourceManager. That is why it returns a pointer,
   not to a Formation, but to a Resource. The ResourceManager uses it to load and instantiate
   Resource instances.

   @param		fileName		The path to the file containing the Formation definition.
   @returns	A pointer to the new Formation (if the file is valid), NULL if invalid.
   */
  static Menge::Resource* load(const std::string& fileName);

  /*!
   @brief		The unique label for this data type to be used with resource management.
   */
  static const std::string LABEL;

  /*!
   @brief		Adds an agent to this formation.

   Only agents "added" to the formation will be mapped considered.
   ?? What happens if more agents than formation points are added?

   @param		agt		The agent to add to the formation.
   */
  void addAgent(const Menge::Agents::BaseAgent* agt);

  /*!
   @brief		Removes an agent from the formation.

   @param		agt		The agent to remove.
   */
  void removeAgent(const Menge::Agents::BaseAgent* agt);

  /*!
   @brief		Computes the mapping from tracked agents to formation points.

   @param		fsm		A pointer to the FSM.
   */
  void mapAgentsToFormation(const Menge::BFSM::FSM* fsm);

  /*!
   @brief		Provides an intermediate goal for the agent.

   This assumes that the agent provided is a member of the formation. It only checks in debug mode
   (via an assertion).

   @param		agt		  The agent for whom the goal is provided.
   @param		pVel	  The agent's preferred velocity...does it change??
   @param		target	The intermediate goal value is stored in this vector.
   @returns	True if the an intermediate goal exists and is set in target.
   */
  bool getGoalForAgent(const Menge::Agents::BaseAgent* agt, Menge::Agents::PrefVelocity& pVel,
                       Menge::Math::Vector2& target);

 protected:
  /*!
   @brief		Maps a single agent to a sentinel point

   @param		agt		The agent to map to a sentinel point.
   */
  void mapAgentToPoint(const Menge::Agents::BaseAgent* agt);

  /*!
   @brief		Maps a border point to one of the agents in the formation.

   @param		pt		A border point to map to an agent.
   */
  void mapPointToAgent(FormationPoint* pt);

  /*!
   @brief		Adds a point to the formation.

   @param		pt				    A point (in formation space).
   @param		borderPoint		True if the point should be considered a border point.
   @param		weight			  The weight of the point.
   */
  void addFormationPoint(Menge::Math::Vector2 pt, bool borderPoint, float weight);

  /*!
   @brief		Adds an agent to the formation.

   @param		agt		The agent to add to the formation.
   */
  void addAgentPoint(const Menge::Agents::BaseAgent* agt);

  /*!
   @brief		Finalize the formation representation for use.

   Normalizing the formation defines distances between the formation center and formation points
   relative to the *size* of the formation. This allows for arbitrary scales.
   */
  void normalizeFormation();

  /*!
   @brief		A custom distance metric to apply to formation points. Used for evaluating "similarity"
   between formation points.

   @param		pt1		The first point to check.
   @param		pt2		The other point to check.
   @returns	The "distance" between the two formatin points.
   */
  float formationDistance(FormationPoint* pt1, FormationPoint* pt2);

  /*!
   @brief		Maps formation point identifiers to agent identifiers. Why identifiers?
   */
  std::map<size_t, size_t> _agent_formationPoint;

  /*!
   @brief		Maps agent identifiers to formation point identifiers. Why identifiers?
   */
  std::map<size_t, size_t> _formationPoint_agent;

  /*!
   @brief		Maps agent identifiers to formation points.

   This lets me seach over a space instead of doing a lot of vector math each timestep
   ? WHAT ?
   */
  std::map<size_t, FormationPoint*> _agentPoints;

  /*!
   @brief		The formation points defining the formation.
   */
  std::vector<FormationPoint*> _formationPoints;

  /*!
   @brief		A separate cache of border points -- this is a subset of _formationPoints.
   */
  std::vector<FormationPoint*> _borderPoints;

  /*!
   @brief		The formation's direction of travel.
   */
  Menge::Math::Vector2 _direction;

  /*!
   @brief		The preferred speed of the formation.
   */
  float _speed;

  /*!
   @brief		The location of the formation center in world space (0,0).
   */
  Menge::Math::Vector2 _pos;

  /*!
   @brief		The instantaneous max distance from the center of the formation to normalized agent
   positions.  ??
   */
  float _agentRadius;

  /*!
   @brief		A cache of previoius agent preferred directions.

   Maps agent identifiers to directions.
   */
  std::map<size_t, Menge::Math::Vector2> _agentPrefDirs;

  /*!
   @brief		A cache of previoius agent preferred velocities.

   Maps agent identifiers to velocities.
   */
  std::map<size_t, Menge::Math::Vector2> _agentPrefVels;

  /*!
   @brief		A cache of previoius agent preferred weights.

   Maps agent identifiers to weights.

   TODO: Currently, these are *always* 1.0.  Provide mechanism
   for defininig non-unit weights.
   */
  std::map<size_t, float> _agentWeights;

  /*!
   @brief		A cache of previoius agent preferred directions.

   Maps agent identifiers to agents.
   */
  std::map<size_t, const Menge::Agents::BaseAgent*> _agents;
};

/*!
 @brief		The definition of the managed pointer for formation data
 */
typedef Menge::ResourcePtr<FreeFormation> FormationPtr;

/*!
 @brief       load a formation
 @param	fileName	The name of the file containing the formation definition.
 @returns	The FormationPtr containing the data.
 @throws		A ResourceException if the data is unable to be instantiated.
 */
FormationPtr loadFormation(const std::string& fileName) throw(Menge::ResourceException);
}  // namespace Formations
#endif  // __FREE_FORMATION_H__
