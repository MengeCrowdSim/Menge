#include "MengeVis/Runtime/VisAgent/VisAgentDatabase.h"

#include "MengeVis/Runtime/VisAgent/VisAgent.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace MengeVis {

namespace {
// TODO: Make it so this works despite the fact that I can't use "delete" on this global
//  member.
Runtime::VisAgent _defaulRenderer;
}  // namespace

// Specialization
template <>
Runtime::VisAgent*
Runtime::VisElementDB<Runtime::VisAgent, Menge::Agents::BaseAgent>::getDefaultElement() {
  return &_defaulRenderer;
}

template <>
void Runtime::VisElementDB<Runtime::VisAgent, Menge::Agents::BaseAgent>::addBuiltins() {
  // addVisElement( new Runtime::GoalVis::AABBGoalRenderer() );
}

template <>
std::string Runtime::VisElementDB<Runtime::VisAgent, Menge::Agents::BaseAgent>::getElementName() {
  return "vis_agent";
}
}  // namespace MengeVis
#endif  // DOXYGEN_SHOULD_SKIP_THIS
