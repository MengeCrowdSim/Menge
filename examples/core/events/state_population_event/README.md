# StatePopulationTrigger Examples

This provides examples whose purpose is to show how the StatePopulationTrigger class is exercised.
The class has six behaviors and there will be, correspondingly, six scenarios (although, for now,
only four of the behaviors are demonstrated). The behaviors are logically paired and so are the
examples.

## `do_increase` and `do_decrease`

A group of agents move from the right to a goal on the left side. Each time an agent reaches its
goal, the agents who have not yet reached their goal get larger and faster.

These are the behaviors that cause the trigger to evaluate to true every time it detects that the
target state's population has increased (or decreased) by any amount since last evaluated. As such
the same scenario is used in both cases. 

In the `do_increase` case, increasing population changes are monitored in the final "GoalReached"
state. In the `do_decrease` case, decreasing popluation changes are monitored in the initial "Walk"
state. Because there are only two states, agents are either in one state or the other, so the two
formulations are functionally identical (a decrease in the initial state implies an increase in the
final state).

## `rise_to` and `drop_to`

Two hallways cross at right angles. One group of agents move from top to bottom, the other moves
from right to left. The agents moving from right to left _wait_ to start moving until most of the
agents have crossed the intersection.

These are the behaviors that cause the trigger to evaluate to true when the state population
exceeds (or falls below) a target value for the first time. The two behaviors are complementary and
the same scenario is used in both cases.

In the `rise_to` case, the final state for the agents moving from top to bottom is monitored. When
its population meets or exceeds a target threshold which represents most of the agents, the trigger
reports true. In the `drop_to` case, the initial state of the vertically moving agents is monitored
for when its population falls to or below a threshold. In both cases, the trigger causes the
right-to-left moving agents to begin moving.

## `while_higher` and `while_lower`

No examples for these scenarios yet. Feel free to contribute.
