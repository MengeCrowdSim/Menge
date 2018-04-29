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
 @file    GoalPoint.h
 @brief    The definition of a BFSM point goal.
 */

#ifndef __GOAL_POINT_H__
#define __GOAL_POINT_H__

#include "MengeCore/BFSM/Goals/Goal.h"
#include "MengeCore/BFSM/Goals/GoalFactory.h"
#include "MengeCore/BFSM/fsmCommon.h"
#include "MengeCore/CoreConfig.h"

namespace Menge {

namespace BFSM {
/*!
 @brief    A simple point goal.  The goal is trivially this point.
 */
class MENGE_API PointGoal : public Goal {
 public:
  /*! Default constructor */
  PointGoal();

  /*!
   @brief    Full constructor.

   @param    p    The position of the goal.
   */
  PointGoal(const Vector2& p);

  /*!
   @brief    Full constructor - component-wise.

   @param    x    The x-position of the goal.
   @param    y    The x-position of the goal.
   */
  PointGoal(float x, float y);

  /*!
   @brief    Used by the plugin system to know what artifacts to associate with agents of this type.
   
   Every sub-class of must return a globally unique value if it should be associated with unique
   artifacts.
   */
  virtual std::string getStringId() const { return NAME; }

  /*! The unique identifier used to register this type with run-time components. */
  static const std::string NAME;
};

/*!
 @brief    Factory for the PointGoal.
 */
class MENGE_API PointGoalFactory : public GoalFactory {
 public:
  /*!
   @brief    The name of the goal type.

   The goal's name must be unique among all registered goals. Each goal factory must override this
   function.

   @returns  A string containing the unique goal name.
   */
  virtual const char* name() const { return PointGoal::NAME.c_str(); }

  /*!
   @brief    A description of the goal.

   Each goal factory must override this function.

   @returns  A string containing the goal description.
   */
  virtual const char* description() const {
    return "An agent goal consisting of a single point in two-dimensional space";
  };

 protected:
  /*!
   @brief    Create an instance of this class's goal.

   @returns    A pointer to a newly instantiated Goal class.
   */
  Goal* instance() const { return new PointGoal(); }

  /*!
   @brief    Given a pointer to a Goal instance, sets the appropriate fields from the provided XML
            node.

   @param    goal        A pointer to the goal whose attributes are to be set.
   @param    node        The XML node containing the action attributes.
   @param    behaveFldr  The path to the behavior file.  If the action references resources in the
                        file system, it should be defined relative to the behavior file location.
                        This is the folder containing that path.
   @returns  A boolean reporting success (true) or failure (false).
   */
  virtual bool setFromXML(Goal* goal, TiXmlElement* node, const std::string& behaveFldr) const;
};

}  // namespace BFSM
}  // namespace Menge

#endif  // __GOAL_POINT_H__
