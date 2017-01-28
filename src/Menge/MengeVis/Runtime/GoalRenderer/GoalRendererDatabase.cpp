#include "MengeVis/Runtime/GoalRenderer/GoalRendererDatabase.h"

#include "MengeVis/Runtime/GoalRenderer/AABBGoalRenderer.h"
#include "MengeVis/Runtime/GoalRenderer/CircleGoalRenderer.h"
#include "MengeVis/Runtime/GoalRenderer/GoalRenderer.h"
#include "MengeVis/Runtime/GoalRenderer/OBBGoalRenderer.h"
#include "MengeVis/Runtime/GoalRenderer/PointGoalRenderer.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace MengeVis {

	namespace {
		// TODO: Make it so this works despite the fact that I can't use "delete" on this global
		//	member.
		Runtime::GoalVis::GoalRenderer _defaulRenderer;
	}	// namespace

	// Specialization
	template<> Runtime::GoalVis::GoalRenderer *
		Runtime::VisElementDB<Runtime::GoalVis::GoalRenderer,
							  Menge::BFSM::Goal>::getDefaultElement() {
		return &_defaulRenderer;
	}

	template<> void
		Runtime::VisElementDB<Runtime::GoalVis::GoalRenderer,
		                      Menge::BFSM::Goal>::addBuiltins() {
		addVisElement( "aabb", new Runtime::GoalVis::AABBGoalRenderer() );
		addVisElement( "point", new Runtime::GoalVis::PointGoalRenderer() );
		addVisElement( "obb", new Runtime::GoalVis::OBBGoalRenderer() );
		addVisElement( "circle", new Runtime::GoalVis::CircleGoalRenderer() );
	}

	template<> std::string
		Runtime::VisElementDB<Runtime::GoalVis::GoalRenderer,
							  Menge::BFSM::Goal>::getElementName() {
		return "goal_renderer";
	}
}
#endif	// DOXYGEN_SHOULD_SKIP_THIS