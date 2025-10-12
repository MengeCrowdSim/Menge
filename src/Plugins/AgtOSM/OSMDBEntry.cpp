/*
 OSM Simulation plugin

 Sergio Lozano Mateos
 UNED

*/

#include "OSMDBEntry.h"
#include "OSMInitializer.h"
#include "MengeCore/Runtime/SimulatorDB.h"

namespace OSM {

using Menge::Agents::SimulatorInterface;

/////////////////////////////////////////////////////////////////////////////
//                     Implementation of KaramouzasDBEntry
/////////////////////////////////////////////////////////////////////////////

::std::string DBEntry::briefDescription() const {
  return "Simulator based on the model proposed in 2012 by M. J. Seitz and G. Köster";
}
/////////////////////////////////////////////////////////////////////////////

::std::string DBEntry::longDescription() const {
  return "Simulator based on Seitz's 2012 pedestrian model\n"
         "\tThe model is based on M. J. Seitz's 2012 paper.\n";
}

/////////////////////////////////////////////////////////////////////////////

::std::string DBEntry::viewerName() const { return "Karamouzas 2009"; }

/////////////////////////////////////////////////////////////////////////////

SimulatorInterface* DBEntry::getNewSimulator() { return new Simulator(); }

/////////////////////////////////////////////////////////////////////////////

Menge::Agents::AgentInitializer* DBEntry::getAgentInitalizer() const {
  return new AgentInitializer();
}

/////////////////////////////////////////////////////////////////////////////

}  // namespace OSM
