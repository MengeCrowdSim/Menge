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
 @file    RectGridGenerator.h
 @brief    An agent generator which creates a set of agents based on the definition of a rectangular
          lattice, with an agent at each point.
 */

#ifndef __RECT_GRID_GENERATOR_H__
#define __RECT_GRID_GENERATOR_H__

#include "MengeCore/Agents/AgentGenerators/AgentGenerator.h"
#include "MengeCore/Agents/AgentGenerators/AgentGeneratorFactory.h"
#include "MengeCore/mengeCommon.h"

#include <vector>

namespace Menge {

namespace Agents {
/*!
 @brief    Definition of an agent generator class which produces agents based on the positions of
          intersections on a lattice--one agent per intersection.

 To specify an rectangular grid generator, use the following syntax:

 ```xml
 <Generator type="rect_grid"
   anchor_x="float" anchor_y="float"
   offset_x="float" offset_y="float"
   count_x="int" count_y="int"
   rotation="float"
 />
 ```

 The various parameters have the following interpretation:
 - `anchor_x` and `anchor_y` represent the *anchor* point of a rectangular area. All other
    parameters are defined relative to this point.
 - `offset_x` and `offset_y` represent the distance between adjacent agents in the grid along the x-
    and y-axes, respectively. The first agent will be placed at the anchor point. The rest of the
    agents will be offset from this point, by the given amounts.
 - `count_x` and `count_y` determine the number of rows and columns of agents in the grid. The total
    number of agents will be `count_x` * `count_y`.
 - `rotation` rotates the rectangle off of the world axes the given number of *degrees*. This
    parameter is optional and, if excluded, defaults to a zero-degree rotation. The rotation is
    counter-clockwise for positive values of rotation.
 */
class MENGE_API RectGridGenerator : public AgentGenerator {
 public:
  /*!
   @brief    Constructor
   */
  RectGridGenerator();

  /*!
   @brief    Reports the number of agents created.

   @returns  The number of agents this generator creates.
   */
  virtual size_t agentCount() { return _xCount * _yCount; }

  /*!
   @brief    Sets the ith position to the given agent.

   @param    i      The index of the requested position in the sequence.
   @param    agt    A pointer to the agent whose position is to be set.
   @throws    AgentGeneratorException if the index, i, is invalid.
   */
  virtual void setAgentPosition(size_t i, BaseAgent* agt);

  /*!
   @brief    Sets the anchor position.

   @param    p    The anchor position.
   */
  void setAnchor(const Vector2& p) { _anchor.set(p); }

  /*!
   @brief    Sets the offset value.

   @param    o    The offset value.
   */
  void setOffset(const Vector2& o) { _offset.set(o); }

  /*!
   @brief    Sets the number of agents in the local x-direction.

   @param    count    The count of agents.
   */
  void setXCount(size_t count) { _xCount = count; }

  /*!
   @brief    Sets the number of agents in the local y-direction.

   @param    count    The count of agents.
   */
  void setYCount(size_t count) { _yCount = count; }

  /*!
   @brief    Sets the number of agents in the local x- and y-directions.

   @param    xCount    The count of agents in the x-direction.
   @param    yCount    The count of agents in the y-direction.
   */
  void setAgentCounts(size_t xCount, size_t yCount) {
    _xCount = xCount;
    _yCount = yCount;
  }

  /*!
   @brief    Sets the lattice rotation.

   @param    angle    The rotation angle (in degrees).
   */
  void setRotationDeg(float angle);

 protected:
  /*!
   @brief    The anchor point of the lattice.
   
   One agent will be positioned at this world coordainte.
   */
  Vector2 _anchor;

  /*!
   @brief    The offset from one agent to the next agent (along the local x- and y-axes,
            respectively.
   */
  Vector2 _offset;

  /*!
   @brief    The number of agents along the local x-axis.
   */
  size_t _xCount;

  /*!
   @brief    The number of agents along the local y-axis.
   */
  size_t _yCount;

  /*!
   @brief    The cosine of the amount the lattice is rotated around its anchor point.
   
   Positive rotation values are counter-clockwise rotation.
   */
  float _cosRot;

  /*!
   @brief    The sine of the amount the lattice is rotated around its anchor point.

   Positive rotation values are counter-clockwise rotation.
   */
  float _sinRot;
};

//////////////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for RectGridGenerator
 */
class MENGE_API RectGridGeneratorFactory : public AgentGeneratorFactory {
 public:
  /*!
   @brief    Constructor.
   */
  RectGridGeneratorFactory();

  /*!
   @brief    The name of the generator type.

   The generator's name must be unique among all registered agent generator components. Each agent
   generator factory must override this function.

   @returns  A string containing the unique elevation name.
   */
  virtual const char* name() const { return "rect_grid"; }

  /*!
   @brief    A description of the agent generator.

   Each agent generator factory must override this function.

   @returns  A string containing the agent generator description.
   */
  virtual const char* description() const {
    return "Agent generation is done via the specification of a rectangular grid.";
  };

 protected:
  /*!
   @brief    Create an instance of this class's agent generator implementation.

   All AgentGeneratorFactory sub-classes must override this by creating (on the heap) a new instance
   of its corresponding generator type. The various field values of the instance will be set in a
   subsequent call to AgentGeneratorFactory::setFromXML. The caller of this function takes ownership
   of the memory.

   @returns    A pointer to a newly instantiated EleAgentGenerator class.
   */
  AgentGenerator* instance() const { return new RectGridGenerator(); }

  /*!
   @brief    Given a pointer to a Goal Selector instance, sets the appropriate fields from the
            provided XML node.

   It is assumed that the value of the `type` attribute is this Goal Selector's type (i.e.
   GoalSelectorFactory::thisFactory has already been called and returned true). If sub-classes of
   GoalSelectorFactory introduce *new* GoalSelector parameters, then the sub-class should override
   this method but explicitly call the parent class's version.

   @param    gen            A pointer to the goal selector whose attributes are to be set.
   @param    node          The XML node containing the goal attributes.
   @param    behaveFldr    The path to the behavior file.  If the condition references resources in
                          the file system, it should be defined relative to the behavior file
                          location. This is the folder containing that path.
   @returns  A boolean reporting success (true) or failure (false).
   */
  virtual bool setFromXML(AgentGenerator* gen, TiXmlElement* node,
                          const std::string& behaveFldr) const;

  /*!
   @brief    The identifier for the "anchor_x" float parameter.
   */
  size_t _anchorXID;

  /*!
   @brief    The identifier for the "anchor_y" float parameter.
   */
  size_t _anchorYID;

  /*!
   @brief    The identifier for the "offset_x" float parameter.
   */
  size_t _offsetXID;

  /*!
   @brief    The identifier for the "offset_y" float parameter.
   */
  size_t _offsetYID;

  /*!
   @brief    The identifier for the "count_x" size_t parameter.
   */
  size_t _xCountID;

  /*!
   @brief    The identifier for the "count_y" size_t parameter.
   */
  size_t _yCountID;

  /*!
   @brief    The identifier for the "rotation" float parameter.
   */
  size_t _rotID;
};
}  // namespace Agents
}  // namespace Menge

#endif  // __RECT_GRID_GENERATOR_H__
