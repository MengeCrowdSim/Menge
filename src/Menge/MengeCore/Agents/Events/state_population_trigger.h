#pragma once
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

/**
 @file   TriggerStatePopulation.h
 @brief  The definitin of event trigger which depends on the population of a BFSM state.
 */

#include "MengeCore/Agents/Events/StateEvtTrigger.h"

#include <string>

namespace Menge {

// Forward declaration.
class StatePopulationTriggerFactory;

/** @brief  An event trigger that depends on the change of population of a specified state.
  
   There are three parameters that define the trigger: its state, a population threshold, and
   the trigger behavior.

   The trigger is invoked in XML as follows (note: the details for the specific details of the
   `EventSystem`, `Target`, and `Effect` tags have been left out):

   @code{xml}
   <BFSM>
     <EventSystem ...>
       <Target name="my_target" .../>
       <Effect name="my_effect" .../>
       <Event name="change_on_population">
         <Trigger type="state_population" name="my_trigger_name" behavior="<behavior>" 
                  threshold="10" state="<state to monitor> />
         <Response effect="my_effect" target="my_target" />
       </Event>
     </EventSystem>
   </BFSM>
   @endcode

   This trigger type has three unique parameters (with the following interpretation):

     - `behavior`: a string that determines the behavior for the trigger evaluating to true. It
        must be one of the following case-sensitive strings: "on_increase", "on_decrease",
        "rise_to", "drop_to", "while_higher", "while_lower". (See below for description of the
        behaviors.
    - `state`: the name of the state whose population is monitored.
    - `threshold`: the threshold value for the trigger behavior. This value is _not_ required for
       the 'on_increase' and 'on_decrease' behavior types, but is required for all others.

   Examples of this trigger can be found in: 

     - `examples/core\events/state_population_event/`.
  
   If we look at the changing population of a state with respect to time, we can imagine a chart
   like this. As time passes, we see agents *entering* the state and then gradually leaving it. Each
   step represents a change in the population from one timestep to the next. A threshold value is
   indicated by the dashed, horizontal line.

      p  │               ┏┅┅┅┅┅┅┅┅┅┐
      o  │               ┇         └┅┅┅┅┐
      p  │               ┇              ┇
      u  │      ┏┅┅┅┅┅┅┅┅┛              ┇
      l  │┄┄┄┄┄┄┇┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┇┄┄┄┄┄┄┄┄┄┄┄┄┄   <-- threshold
      a  │  ┏┅┅┅┛                       ┇
      t  │  ┇                           ┕┅┅┅┅┅┐
      i  │┅┅┛                                 ┇
      o  │                                    ┇
      n  └━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━
                              t 

   The particular behavior chosen, will cause different triggers depending on the reltionahip
   between the size of the population and the change in the size of population. Each behavior
   is shown below, showing when the trigger would evaluate to true for the hypothetical sequence
   shown above.

   <h2>Behaviors</h2>

   _On increase_: type = "on_increase". Every time the population increases, the trigger evalutes to
   true for *one* timestep.

      t  │
      r  │
      i  │  ┇   ┇        ┇
      g  │  ┇   ┇        ┇
      g  │  ┇   ┇        ┇
      e  │  ┇   ┇        ┇
      r  └━━┻━━━┻━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
                              t

   _On decrease_: type = "on_decrease". Every time the population decreases, the trigger evaluets to
   true for *one* timestep.

      t  │
      r  │
      i  │                         ┇    ┇      ┇
      g  │                         ┇    ┇      ┇
      g  │                         ┇    ┇      ┇
      e  │                         ┇    ┇      ┇
      r  └━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━┻━━━━━━┻━━━━━━━
                              t

   _Rise to_: type = "rise_to". When the population is below the `threshold` value, the timestep at
   which it reaches (or rises above) the `threshold` value is reached, the trigger evaluates to 
   true. This true evaluation is only at the moment of *change* and doesn't persist just because
   the state's population stays high.

      t  │
      r  │
      i  │      ┇
      g  │      ┇
      g  │      ┇
      e  │      ┇
      r  └━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
                              t

   _Drop to_: type = "drop_to". The reverse of _Rise to_. In this case, it's when the population is
   _higher_ than the threshold and then drops to or below the threshodl that the trigger evalutes
   to true.

      t  │
      r  │
      i  │                              ┇
      g  │                              ┇
      g  │                              ┇
      e  │                              ┇
      r  └━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━
                              t

   _While higher_: type = "while_higher". For every time step in which the state's population is
   _stritly_ above `threshold`, the trigger evaluats to true.

      t  │
      r  │
      i  │      ┏┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┅┐
      g  │      ┇                       ┇
      g  │      ┇                       ┇
      e  │      ┇                       ┇
      r  └━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━

   _While lower_: type = "while_lower". FOr every time step win which the state's population is
   _strictly_ below `threshold`, the trigger evaluates to true.

      t  │
      r  │
      i  │┅┅┅┅┅┅┐                       ┏┅┅┅┅┅┅┅┅┅┅┅┅┅┅
      g  │      ┇                       ┇
      g  │      ┇                       ┇
      e  │      ┇                       ┇
      r  └━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━

 */
class MENGE_API StatePopulationTrigger : public StateEvtTrigger {
public:
  /** @brief  Constructor.  */
  StatePopulationTrigger();

  /** @brief  Allows the trigger to finish initializing itself from its parsed state to its running
              state.
   @throws    EventException if there is a problem finalizing.  */
  void finalize() override;

  /** @brief  The various modes the trigger can operate in.  */
  enum Mode {
    UNDEFINED,
    ON_INCREASE,
    ON_DECREASE,
    RISE_TO,
    DROP_TO,
    WHILE_HIGHER,
    WHILE_LOWER
  };

  Mode mode() const { return _mode;  }

  /** @brief  Sets the trigger's behavior node from a valid string. Reports if the given `mode`
              requires a threshold value (true).
   @throws    EventException if the string does not represent a valid mode.  */
  bool set_mode(const std::string& mode_name);

  /** @brief  Sets the threshold value for the trigger.  */
  void set_threshold(size_t t) { _threshold = t;  }

  friend class StatePopulationTriggerFactory;

protected:

  /** @brief  Evaluates the condition to see if it has been met.

   This must be implemented by all sub-classes.

   @returns   True if the condition has been met, false otherwise.  */
  bool testCondition() override;

  /** @brief  The population last time the state was examined.  */
  size_t _lastPop{0};

  /** @brief  The threshold for the behavior.  */
  size_t _threshold{0};

  /** @brief  The mode the trigger is operating in.  */
  Mode _mode;
};

//////////////////////////////////////////////////////////////////////////

/** @brief  The factory for StatePopulationTrigger event triggers.  */
class MENGE_API StatePopulationTriggerFactory final : public StateEvtTriggerFactory {
public:
  /** @brief  Constructor.  */
  StatePopulationTriggerFactory();

  /** @brief  The name of the trigger type.

   The trigger's name must be unique among all registered triggers. Each trigger factory must
   override this function.

   @returns   A string containing the unique trigger name.  */
  const char* name() const override { return "state_population"; }

  /** @brief  A description of the event trigger.

   Each trigger factory must override this function.

   @returns  A string containing the trigger description.  */
  const char* description() const override {
    return "Event trigger which fires based on a state's population.";
  };

protected:
  /**
   @brief  Given a pointer to an EventTrigger instance, sets the appropriate fields from the 
           provided XML node.

   It is assumed that the value of the `type` attribute is this EventTrigger's type. (i.e. 
   EventTrigger::thisFactory has already been called and returned true.) If sub-classes of 
   EventTriggerFactory introduce *new* EventTrigger parameters, then the sub-class should override
   this method but explicitly call the parent class's version.

   @param    trigger    A pointer to the event trigger whose attributes are to be set.
   @param    node       The XML node containing the event trigger attributes.
   @param    specFldr   The path to the specification file. If the EventTrigger references resources
                        in the file system, it should be defined relative to the specification file
                        location.  This is the folder containing that path.
   @returns  A boolean reporting success (true) or failure (false).  */
  bool setFromXML(EventTrigger * trigger, TiXmlElement * node,
                  const std::string & specFldr) const override;

  /** @brief  Create an instance of this class's event trigger implementation.

   All EventTriggerFactory sub-classes must override this by creating (on the heap) a new instance
   of its corresponding trigger type.  The various field values of the instance will be set in a
   subsequent call to EventTriggerFactory::setFromXML. The caller of this function takes ownership
   of the memory.

   @returns  A pointer to a newly instantiated EventTrigger class.  */
  EventTrigger * instance() const override { return new StatePopulationTrigger(); }

private:
  /** @brief  The identifier for the "behavior" string attribute.  */
  size_t _behaviorID;

  /** @brief  The identifier for the "threshold" int attribute.  */
  size_t _thresholdID;
};

}  // namespace Menge
