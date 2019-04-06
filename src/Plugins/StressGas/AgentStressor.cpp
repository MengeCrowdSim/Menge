#include "AgentStressor.h"

#include "MengeCore/Agents/BaseAgent.h"
#include "MengeCore/Orca/ORCAAgent.h"
#include "MengeCore/PedVO/PedVOAgent.h"

namespace StressGAS {

using Menge::Agents::BaseAgent;

// These values come from the GAS paper
//	Interactive Simulation of Dynamic Crowd Behaviors using General Adaptation Syndrome Theory
//		Sujeong Kim, Stephen J.Guy, Dinesh Manocha and Ming C.Lin
//		ACM SIGGRAPH Symposium on Interactive 3D Graphics and Games(I3D), 2012.
//
// It reports a stress vector defined as:
//      (PC1, PC2) . ( 0.95, -0.3)^T
// where PC1 and PC2 are defined in this paper:
//	Simulating Heterogeneous Crowd Behaviors Using Personality Trait Theory
//		Stephen J.Guy, Sujeong Kim, Ming C.Lin and Dinesh Manocha
//		ACM SIGGRAPH / Eurographics Symposium on Computer Animation(SCA), Aug 2011.
//
// The vectors are defined as:
//	(neighbor dist., max. neighbors, planning horiz., radius, pref_speed )
//	PC1 = ( 0, -0.04, 0.04, 0.75, 0.66 )
//	PC2 = (0.14, 0.5, 0.8, 0.15, -0.19 )
//
// This yields the final values:
// (-0.042, -0.188, -0.202, 0.6675, 0.684)
const float AgentStressor::DEFAULT_NEIGHBOR_DIST_DELTA = -0.0402f;
const float AgentStressor::DEFAULT_MAX_NEIGHBORS_DELTA = -0.188f;
const float AgentStressor::DEFAULT_TIME_HORIZON_DELTA = -0.202f;
const float AgentStressor::DEFAULT_RADIUS_DELTA = 0.6675f;
const float AgentStressor::DEFAULT_PREF_SPEED_DELTA = 0.684f;

/////////////////////////////////////////////////////////////////////
//                   Implementation of AgentStressor
/////////////////////////////////////////////////////////////////////

AgentStressor::AgentStressor()
    : _deltaNeighborDist(DEFAULT_NEIGHBOR_DIST_DELTA),
      _deltaMaxNeighbors(DEFAULT_MAX_NEIGHBORS_DELTA),
      _deltaRadius(DEFAULT_RADIUS_DELTA),
      _deltaPrefSpeed(DEFAULT_PREF_SPEED_DELTA),
      _deltaTimeHorizon(DEFAULT_TIME_HORIZON_DELTA) {}

/////////////////////////////////////////////////////////////////////

AgentStressor::AgentStressor(float neighborDistDelta, float maxNeighborDelta, float radiusDelta,
                             float prefSpeedDelta, float timeHorizonDelta)
    : _deltaNeighborDist(neighborDistDelta),
      _deltaMaxNeighbors(maxNeighborDelta),
      _deltaRadius(radiusDelta),
      _deltaPrefSpeed(prefSpeedDelta),
      _deltaTimeHorizon(timeHorizonDelta) {}

/////////////////////////////////////////////////////////////////////

void AgentStressor::applyBaseline(BaseAgent* agt) {
  agt->_neighborDist = _baseNeighborDist;
  agt->_maxNeighbors = static_cast<size_t>(_baseMaxNeighbors);
  agt->_radius = _baseRadius;
  agt->_prefSpeed = _basePrefSpeed;

  // handle the orca-derived classes specially
  ORCA::Agent* oAgt = dynamic_cast<ORCA::Agent*>(agt);
  if (oAgt != 0x0) {
    oAgt->_timeHorizon = _baseTimeHorizon;
  } else {
    PedVO::Agent* oAgt = dynamic_cast<PedVO::Agent*>(agt);
    if (oAgt != 0x0) {
      oAgt->_timeHorizon = _baseTimeHorizon;
    }
  }
}

/////////////////////////////////////////////////////////////////////

void AgentStressor::setBaseline(const AgentStressor* stressor) {
  _baseNeighborDist = stressor->_baseNeighborDist;
  _baseMaxNeighbors = stressor->_baseMaxNeighbors;
  _baseRadius = stressor->_baseRadius;
  _basePrefSpeed = stressor->_basePrefSpeed;
  _baseTimeHorizon = stressor->_baseTimeHorizon;
}

/////////////////////////////////////////////////////////////////////

void AgentStressor::setBaseline(const BaseAgent* agt) {
  _baseNeighborDist = agt->_neighborDist;
  _baseMaxNeighbors = static_cast<float>(agt->_maxNeighbors);
  _baseRadius = agt->_radius;
  _basePrefSpeed = agt->_prefSpeed;

  // handle the orca-derived classes specially
  const ORCA::Agent* oAgt = dynamic_cast<const ORCA::Agent*>(agt);
  if (oAgt != 0x0) {
    _baseTimeHorizon = oAgt->_timeHorizon;
  } else {
    const PedVO::Agent* oAgt = dynamic_cast<const PedVO::Agent*>(agt);
    if (oAgt != 0x0) {
      _baseTimeHorizon = oAgt->_timeHorizon;
    }
  }
}

/////////////////////////////////////////////////////////////////////

void AgentStressor::applyStress(float stressLevel, BaseAgent* agt) {
  agt->_neighborDist = _baseNeighborDist + stressLevel * _deltaNeighborDist;
  // the additional 0.5f uses truncation to do rounding.
  agt->_maxNeighbors =
      static_cast<size_t>(_baseMaxNeighbors + stressLevel * _deltaMaxNeighbors + 0.5f);
  agt->_prefSpeed = _basePrefSpeed + stressLevel * _deltaPrefSpeed;
  agt->_radius = _baseRadius + stressLevel * _deltaRadius;

  // handle the orca-derived classes specially
  ORCA::Agent* oAgt = dynamic_cast<ORCA::Agent*>(agt);
  if (oAgt != 0x0) {
    oAgt->_timeHorizon = _baseTimeHorizon + stressLevel * _deltaTimeHorizon;
  } else {
    PedVO::Agent* oAgt = dynamic_cast<PedVO::Agent*>(agt);
    if (oAgt != 0x0) {
      oAgt->_timeHorizon = _baseTimeHorizon + stressLevel * _deltaTimeHorizon;
    }
  }
}

/////////////////////////////////////////////////////////////////////

}  // namespace StressGAS
