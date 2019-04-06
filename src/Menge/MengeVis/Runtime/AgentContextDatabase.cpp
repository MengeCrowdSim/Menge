#include "MengeVis/Runtime/AgentContext/AgentContextDatabase.h"
#include "MengeVis/Runtime/AgentContext/ORCAAgentContext.h"
#include "MengeVis/Runtime/AgentContext/PedVOAgentContext.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace MengeVis {

namespace {
Runtime::BaseAgentContext _defaultContext;
}  // namespace

// Specialization
template <>
Runtime::BaseAgentContext*
Runtime::VisElementDB<Runtime::BaseAgentContext, Runtime::VisAgent>::getDefaultElement() {
  return &_defaultContext;
}

template <>
void Runtime::VisElementDB<Runtime::BaseAgentContext, Runtime::VisAgent>::addBuiltins() {
  addVisElement(new Runtime::OrcaAgentContext());
  addVisElement(new Runtime::PedVOAgentContext());
}

template <>
std::string Runtime::VisElementDB<Runtime::BaseAgentContext, Runtime::VisAgent>::getElementName() {
  return "agent_context";
}
}  // namespace MengeVis
#endif  // DOXYGEN_SHOULD_SKIP_THIS