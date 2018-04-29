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

#ifndef __AGENT_STRESSOR_H__
#define __AGENT_STRESSOR_H__

#include "StressGasConfig.h"

// forward declarations
namespace Menge {
namespace Agents {
class BaseAgent;
}
}  // namespace Menge

namespace StressGAS {

/**
 @brief		The class that applies stress to an agent.

 Each agent is associated with a unique stressor. The stressor knows the agent's baseline state and,
 given a stress level modifies the agent's properties to reflect the stress level.
 */
class EXPORT_API AgentStressor {
 public:
  /** Default change to neighbor distance at full stress. */
  const static float DEFAULT_NEIGHBOR_DIST_DELTA;
  /** Default change to maximum neighbors at full stress. */
  const static float DEFAULT_MAX_NEIGHBORS_DELTA;
  /** Default change to time horizon (for ORCA and PedVO) at full stress. */
  const static float DEFAULT_TIME_HORIZON_DELTA;
  /** Default change to agent radius at full stress. */
  const static float DEFAULT_RADIUS_DELTA;
  /** Default change to preferred speed at full stress. */
  const static float DEFAULT_PREF_SPEED_DELTA;

  /** @brief		Default constructor */
  AgentStressor();

  /**
   @brief	Constructor for fully specifying the agent parameter changes due to stress.

   @param	neighborDistDelta	  The amount the neighbor distance changes at full stress.
   @param	maxNeighborDelta 	  The amount the maximum neighbor changes at full stress.
   @param	radiusDelta		 	    The amount the radius changes at full stress.
   @param	prefSpeedDelta   	  The amount the preference speed changes at full stress.
   @param	timeHorizonDelta 	  The amount the time horizon changes at full stress. (Only affects ORCA
                              and PedVO agents).
   */
  AgentStressor(float neighborDistDelta, float maxNeighborDelta, float radiusDelta,
                float prefSpeedDelta, float timeHorizonDelta);

  /**
   @brief		Sets the baseline for this stressor by copying it from the provided stressor.

   @param		stressor		The stressor to copy the baseline from.
   */
  void setBaseline(const AgentStressor* stressor);

  /**
   @brief	Sets the stressor baseline based on the given agent.

   A stressor whose baseline set from one agent *can* technically be applied to another agent.
   However, not all agents will necessarily have the same baseline. That means the agent's stressed
   and restored neutral state may become corrupt. If there is any expected variability among agents,
   each should be associated with a unique stressor with its own initialized stressor.

   @param [in]	agt	If non-null, the agt.
   */
  void setBaseline(const Menge::Agents::BaseAgent* agt);

  /**
   @brief	Applies the stressor's baseline values to the given agent.

   @param [in,out]	agt		If non-null, the agent's stress-related parametes are set to the
                          stressor's baseline values.
   */
  void applyBaseline(Menge::Agents::BaseAgent* agt);

  /**
   @brief	Applies the stress level to the given agent.

   The changes to the agent will be relative to this stressor's baseline (@see setBaseline).

   @param[in] stressLevel		The level of stress. Ordinarily in the range [0, 1] representing the
                            range of baseline to fully stressed. However, values outside of this
                            range can be applied without guarantee as to the impact.
   @param[in,out]	agt			  The agent to apply the stress to.
   */
  void applyStress(float stressLevel, Menge::Agents::BaseAgent* agt);

 private:
  // Baseline values
  /** @brief	The baseline neighbor distance. */
  float _baseNeighborDist;

  /** @brief	The baseline maximum neighbors. */
  float _baseMaxNeighbors;

  /** @brief	The baseline radius. */
  float _baseRadius;

  /** @brief	The baseline preference speed. */
  float _basePrefSpeed;

  /** @brief	The baseline time horizon. */
  float _baseTimeHorizon;

  // Stress changes
  /** @brief	The fully stressed change to neighbor distance. */
  float _deltaNeighborDist;

  /** @brief	The fully stressed change to maximum neighbors. */
  float _deltaMaxNeighbors;

  /** @brief	The fully stressed change to radius. */
  float _deltaRadius;

  /** @brief	The fully stressed change to preference speed. */
  float _deltaPrefSpeed;

  /** @brief	The fully stressed change to time horizon. */
  float _deltaTimeHorizon;
};

}  // namespace StressGAS

#endif  // __AGENT_STRESSOR_H__
