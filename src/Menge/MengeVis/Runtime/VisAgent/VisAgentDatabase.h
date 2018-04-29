/*!
 @file    VisAgentDatabase.h
 @brief   The definition of the database for all agent visualizations.
*/

#ifndef __VIS_AGENT_DATABASE_H__
#define __VIS_AGENT_DATABASE_H__

#include "MengeVis/Runtime/VisElementDatabase.h"

// forward declaration
namespace Menge {
namespace Agents {
class BaseAgent;
}
}  // namespace Menge

namespace MengeVis {
namespace Runtime {
class VisAgent;
/*!
 @brief   The database of registered agent visualization types.
 */
typedef VisElementDB<VisAgent, Menge::Agents::BaseAgent> VisAgentDB;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Declrations of explicit specializations
template <>
VisAgent* Runtime::VisElementDB<VisAgent, Menge::Agents::BaseAgent>::getDefaultElement();

template <>
void Runtime::VisElementDB<VisAgent, Menge::Agents::BaseAgent>::addBuiltins();

template <>
std::string Runtime::VisElementDB<VisAgent, Menge::Agents::BaseAgent>::getElementName();
#endif  // DOXYGEN_SHOULD_SKIP_THIS

}  // namespace Runtime
}  // namespace MengeVis

#endif  // __VIS_AGENT_DATABASE_H__
