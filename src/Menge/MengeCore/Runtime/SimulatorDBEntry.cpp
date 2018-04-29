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

#include "MengeCore/Runtime/SimulatorDBEntry.h"

#include "MengeCore/Agents/AgentInitializer.h"
#include "MengeCore/Agents/SimulatorInterface.h"
#include "MengeCore/BFSM/FSM.h"
#include "MengeCore/BFSM/FSMDescrip.h"
#include "MengeCore/MengeException.h"
#include "MengeCore/Runtime/Logger.h"

namespace Menge {

/////////////////////////////////////////////////////////////////////////////
//                     Implementation of SimulatorDBEntry
////////////////////////////////////////////////////////////////////////////

Agents::SimulatorInterface* SimulatorDBEntry::initSimulator(const std::string& sceneFileName,
                                                            bool VERBOSE) {
  Agents::SimulatorInterface* sim = getNewSimulator();
  Agents::AgentInitializer* agentInit = getAgentInitalizer();
  Agents::SimXMLLoader loader(sim);
  logger.line();
  try {
    if (!loader.loadFromXML(sceneFileName, agentInit, VERBOSE)) {
      logger << Logger::ERR_MSG << "Couldn't initialize scene from xml.";
      return 0x0;
    }
  } catch (MengeFatalException e) {
    logger << Logger::ERR_MSG << e._msg;
    return 0x0;
  }
  return sim;
}

////////////////////////////////////////////////////////////////////////////

BFSM::FSM* SimulatorDBEntry::initFSM(const std::string& behaveFile, Agents::SimulatorInterface* sim,
                                     bool VERBOSE) {
  logger.line();
  BFSM::FSMDescrip fsmDescrip;

  if (!fsmDescrip.loadFromXML(behaveFile, VERBOSE)) {
    logger << Logger::ERR_MSG << "Problems loading behavior specification!";
    return 0x0;
  }
  if (VERBOSE) logger << fsmDescrip << "\n";

  BFSM::FSM* fsm = BFSM::buildFSM(fsmDescrip, sim, VERBOSE);
  if (!fsm) {
    logger << Logger::ERR_MSG << "Error instantiating FSM from description.";
  }
  return fsm;
}

////////////////////////////////////////////////////////////////////////////

bool SimulatorDBEntry::finalize(Agents::SimulatorInterface* sim, BFSM::FSM* fsm) {
  sim->setBFSM(fsm);
  // older versions of OpenMP require signed for loop counters
  int agtCount = (int)sim->getNumAgents();
#pragma omp parallel for
  for (int a = 0; a < agtCount; ++a) {
    Agents::BaseAgent* agt = sim->getAgent(a);
    fsm->computePrefVelocity(agt);
  }
  try {
    sim->finalize();
  } catch (Menge::MengeException& e) {
    logger << Logger::ERR_MSG << "Problem in finalizing the simulator.\n";
    logger << "\t" << e.what();
    delete fsm;
    return false;
  }
  try {
    fsm->finalize();
  } catch (Menge::MengeFatalException& e) {
    logger << Logger::ERR_MSG << "Fatal error finalizing the finite state machine!\n";
    logger << "\t" << e.what();
    delete fsm;
    return false;
  } catch (Menge::MengeException& e) {
    logger << Logger::WARN_MSG
           << "There were non-fatal errors in finalizing the finite "
              "state machine!\n";
    logger << "\t" << e.what();
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////

Agents::SimulatorInterface* SimulatorDBEntry::getSimulator(
    size_t& agentCount, float& simTimeStep, size_t subSteps, float simDuration,
    const std::string& behaveFile, const std::string& sceneFile, const std::string& outFile,
    const std::string& scbVersion, bool verbose) {
  _sim = initSimulator(sceneFile, verbose);
  if (!_sim) {
    return 0x0;
  }
  // TODO: Remove time step from the simulator specification!
  float specTimeStep = _sim->getTimeStep();

  _fsm = initFSM(behaveFile, _sim, verbose);
  if (!_fsm) {
    return 0x0;
  }
  if (!finalize(_sim, _fsm)) {
    delete _sim;
    delete _fsm;
    return 0x0;
  }

  if (simTimeStep > 0.f) {
    if (verbose) {
      logger << Logger::INFO_MSG;
      logger << "Simulation time step set by command-line argument: ";
      logger << simTimeStep << ".";
    }
    _sim->setTimeStep(simTimeStep);
  } else {
    simTimeStep = specTimeStep;
    if (verbose) {
      logger << Logger::INFO_MSG << "Simulation time step set by specification file: ";
      logger << specTimeStep << ".";
    }
  }
  _sim->setSubSteps(subSteps);
  float effTimeStep = simTimeStep / (1.f + subSteps);
  logger << Logger::INFO_MSG << "For logical time step: " << simTimeStep << " and ";
  logger << subSteps << " sub step";
  if (subSteps != 1) {
    logger << "s";
  }
  logger << ", effective time step is: " << effTimeStep;

  _sim->setMaxDuration(simDuration);
  if (outFile != "") {
    _sim->setOutput(outFile, scbVersion);
  }
  agentCount = _sim->getNumAgents();
  return _sim;
}

////////////////////////////////////////////////////////////////////////////

float SimulatorDBEntry::simDuration() const { return _sim != 0x0 ? _sim->getGlobalTime() : -1.f; }

////////////////////////////////////////////////////////////////////////////
#if 0
  BaseAgentContext * SimulatorDBEntry::contextFromSystem( SimSystem * simSystem ) {
    return new BaseAgentContext( simSystem->getVisAgents(), simSystem->getAgentCount() ); 
  }

  ////////////////////////////////////////////////////////////////////////////
#endif
Agents::AgentInitializer* SimulatorDBEntry::getAgentInitalizer() const {
  return new Agents::AgentInitializer();
}
}  // namespace Menge
