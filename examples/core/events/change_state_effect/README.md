# ChangeStateEffect Examples

This provides a single example whose purpose is to show how the ChangeStateEffect class is 
exercised. The example doesn't show the "force_reentry" parameter. And the scenario duplicates that
shown in other examples (pedModelSwap, rise_to, and drop_to). The comparison with `rise_to` is
particularly significant.

## change_state

Two hallways cross at right angles. One group of agents move from top to bottom, the other moves
from right to left. The agents moving from right to left _wait_ to start moving until most of the
agents have crossed the intersection.

The agents moving left to right begin in a "wait" state -- a state with a zero velocity component
and _no_ transitions out of it. The only way an agent can leave that state would be a mechanism
outside of the standard FSM mechanism -- such as this event effect.

When enough agents moving from top to bottom have crossed the intersection, all agents in the wait
state are moved to a walking state.
