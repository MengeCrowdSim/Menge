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

/*!
 @file    GoalSelectorMirror.h
 @brief    The definition of the "mirror" goal selector.
 */

#ifndef __GOAL_SELECTOR_MIRROR_H__
#define __GOAL_SELECTOR_MIRROR_H__

#include "MengeCore/BFSM/GoalSelectors/GoalSelector.h"
#include "MengeCore/BFSM/GoalSelectors/GoalSelectorFactory.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/CoreConfig.h"

namespace Menge {

namespace BFSM {
/*!
 @brief    The mirror goal selector makes a goal by reflecting the agent's position around the
          world's origin. The reflection across the x- and y-axes can be individually specified.

 Setting the mirror settings to *not* reflect over either axis is akin to creating the
 IdentityGoalSelector.
 */
class MENGE_API MirrorGoalSelector : public GoalSelector {
 public:
  /*!
   @brief    Default constructor
   */
  MirrorGoalSelector();

  /*!
   @brief    Interface function for acquiring per-agent goals.

   @param    agent    The agent for whom a goal is extracted.
   @returns  A pointer to a goal.
   // TODO: Figure out who owns this goal.
   */
  virtual Goal* getGoal(const Agents::BaseAgent* agent) const;

  /*!
   @brief    Sets the mirroring of the goal selector.

   @param    mirrorX    Determines if the position is mirrored over the x-axis.
   @param    mirrorY    Determines if the position is mirrored over the y-axis.
   */
  inline void setMirror(bool mirrorX, bool mirrorY) {
    _mirrorX = mirrorX;
    _mirrorY = mirrorY;
  }

  /*!
   @brief    Sets the x-mirroring of the goal selector.

   @param    mirror    Determines if the position is mirrored over the axis.
   */
  inline void setMirrorX(bool mirror) { _mirrorX = mirror; }

  /*!
   @brief    Sets the y-mirroring of the goal selector.

   @param    mirror    Determines if the position is mirrored over the axis.
   */
  inline void setMirrorY(bool mirror) { _mirrorY = mirror; }

  /*!
   @brief    Returns the mirror x state.

   @returns  The mirror x state of the selector.
   */
  inline bool getMirrorX() const { return _mirrorX; }

  /*!
   @brief    Returns the mirror y state.

   @returns  The mirror y state of the selector.
   */
  inline bool getMirrorY() const { return _mirrorY; }

 protected:
  /*!
   @brief    Determines if the agent's position is reflected over the x-axis (true) or not (false).
   */
  bool _mirrorX;

  /*!
   @brief    Determines if the agent's position is reflected over the y-axis (true) or not (false).
   */
  bool _mirrorY;
};

/*!
 @brief    Factory for the MirrorGoalSelector.
 */
class MENGE_API MirrorGoalSelectorFactory : public GoalSelectorFactory {
 public:
  /*!
   @brief    Constructor.
   */
  MirrorGoalSelectorFactory();

  /*!
   @brief    The name of the goal selector type.

   The goal selector's name must be unique among all registered goal selectors. Each goal selector
   factory must override this function.

   @returns  A string containing the unique goal selector name.
   */
  virtual const char* name() const { return "mirror"; }

  /*!
   @brief    A description of the goal selector.

   Each goal selector factory must override this function.

   @returns  A string containing the goal selector description.
   */
  virtual const char* description() const {
    return "A goal selector.  The goal an agent gets is the reflection "
           "of the agent's position across the world's origin over the "
           "world's x- and/or y-axes as specified.";
  };

 protected:
  /*!
   @brief    Create an instance of this class's goal selector.

   @returns    A pointer to a newly instantiated GoalSelector class.
   */
  GoalSelector* instance() const { return new MirrorGoalSelector(); }

  /*!
   @brief    Given a pointer to a GoalSelector instance, sets the appropriate fields
   from the provided XML node.

   @param    selector    A pointer to the goal whose attributes are to be set.
   @param    node        The XML node containing the goal selector attributes.
   @param    behaveFldr  The path to the behavior file.  If the goal selector references resources in
                        the file system, it should be defined relative to the behavior file
                        location.  This is the folder containing that path.
   @returns  A boolean reporting success (true) or failure (false).
   */
  virtual bool setFromXML(GoalSelector* selector, TiXmlElement* node,
                          const std::string& behaveFldr) const;

  /*!
   @brief    The identifier for the "mirror_x" bool attribute.
   */
  size_t _mirrorXID;

  /*!
   @brief    The identifier for the "mirror_y" bool attribute.
   */
  size_t _mirrorYID;
};

}  // namespace BFSM
}  // namespace Menge

#endif  // __GOAL_SELECTOR_MIRROR_H__
