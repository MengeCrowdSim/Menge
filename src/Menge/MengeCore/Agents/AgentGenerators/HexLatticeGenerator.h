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
 @file    HexLatticeGenerator.h
 @brief    An agent generator which creates a set of agents based on the definition of a hexagonal
          packed lattice fit into a rectangle. (i.e., rows are offset for maximal packing).
 */

#ifndef __LATTICE_AGENT_GENERATOR_H__
#define __LATTICE_AGENT_GENERATOR_H__

#include "MengeCore/Agents/AgentGenerators/AgentGenerator.h"
#include "MengeCore/Agents/AgentGenerators/AgentGeneratorFactory.h"
#include "MengeCore/mengeCommon.h"

namespace Menge {

namespace Agents {
/*!
 @brief    Definition of an agent generator class which produces agents based on the positions of
          intersections on a hexagonal lattice bounded by a rectangle.

 Circles can be maximally packed into a hexagonal lattice. In this case, an open rectangular region
 is defined (relative to an anchor point) and agents are placed in a hexagonal lattice, bounded by
 the rectangular region on three sides.

 The rectangular region is defined in "local" space. In its own local space, the rectangular region
 is axis aligned and extends into the positive y-direction. The "front" of the region lies on the
 x-axis and the left and right sides extend into the positive y-direction. The alignment of the
 rectangular region relative to the anchor point depends on the AnchorAlignEnum value.

 The hexagonal grid lattice size is based on the target average density. The lattice points are
 placed to achieve that average density. The lattice will either be aligned with the x-axis or the
 y-axis, depending on the LatticeRowEnum value.

 To specify a hexagonal lattice generator, use the following syntax:

 ```xml
 <Generator type="hex_lattice"
            anchor_x="float" anchor_y="float"
            alignment="string" row_direction="string"
            density="float" width="float"
            population="int" rotation="float"/>
 ```

 The various parameters have the following interpretation:
   - `anchor_x` and `anchor_y` represent the *anchor* point of the local rectangular area. All other
      parameters are defined relative to this point.
   - `alignment` is one of three values: `{ center | left | right }`. It defines how the front line
      of the rectangular is positioned relative to the anchor point. If `center`, the front edge of
      the rectangle is centered on the anchor point. If `left` or `right`, one end or the other will
      be positioned at the anchor point.
   - `row_direction` is one of two values: `{ x | y }`. The lattice's intrinsic rows can either be
      aligned with the local x-axis or the local y.
   - `density` defines the target average density of the agents.  As density increases, the distance
      between agents decreases.
   - `width` defines the approximate length of the front edge of the rectangular region. This width
      will be populated with the maximum number of agents based on the `density` and `row_direction`
      values.
   - `population` indicates the target population value. The actual population may differ. If the
      given population number doesn't exactly fill up all rows of the lattice, agents will be added
      to create a "complete" lattice.
   - `rotation` rotates the rectangle off of the world axes the given number of *degrees*. This
      parameter is optional and, if excluded, defaults to a zero-degree rotation. The rotation is
      counter-clockwise for positive values of rotation.
 */
class MENGE_API HexLatticeGenerator : public AgentGenerator {
 public:
  /*!
   @brief    Enumeration specifying arrangement relative to the anchor point.
   */
  enum AnchorAlignEnum {
    CENTER,       ///< The front boundary is centered on the anchor
    LEFT_CORNER,  ///< The front boundary's "left" corner is on the anchor
    RIGHT_CORNER  ///< The front boundary's "right" corner is on the anchor
  };

  /*!
   @brief    Enumeration indicating which axis the regular rows occur.
   */
  enum LatticeRowEnum {
    ROW_X,  ///< The rows run parallel with the local x-axis
    ROW_Y   ///< The rows run parallel with the local y-axis
  };

  /*!
   @brief    Constructor
   */
  HexLatticeGenerator();

  /*!
   @brief    Reports the number of agents created.

   @returns  The number of agents this generator creates.
   */
  virtual size_t agentCount() { return _totalPop; }

  /*!
   @brief    Sets the ith position to the given agent.

   @param    i     The index of the requested position in the sequence.
   @param    agt   A pointer to the agent whose position is to be set.
   @throws    AgentGeneratorException if the index, i, is invalid.
   */
  virtual void setAgentPosition(size_t i, BaseAgent* agt);

  /*!
   @brief    Sets the properties of the generator

   @param    anchor          The anchor position of the lattice.
   @param    align           The target alignment.
   @param    dir             The desired row direction.
   @param    width           The width of the bounding region
   @param    density         The target density (agent /m^2).
   @param    tgtPopulation   The target population (agents).
   @param    angle           The rotation angle (in degrees).
   */
  void set(const Vector2& anchor, AnchorAlignEnum align, LatticeRowEnum dir, float width,
           float density, size_t tgtPopulation, float angle);

  /*!
   @brief    Sets the lattice rotation.

   */
  void setRotationDeg(float angle);

 protected:
  /*!
   @brief    The anchor point of the lattice. One agent will be positioned at this world coordinate.
   */
  Vector2 _anchor;

  /*!
   @brief    The direction of the row layout.
   */
  LatticeRowEnum _rowDir;

  /*!
   @brief    The cosine of the amount the lattice is rotated around its anchor point. 
   
   Positive values represent counter-clockwise rotation.
   */
  float _cosRot;

  /*!
   @brief    The sine of the amount the lattice is rotated around its anchor point.

   Positive values represent counter-clockwise rotation.
   */
  float _sinRot;

  /*!
   @brief    The actual number of agents to create based on run-time parameters.

   This value is only valid AFTER finalize has been called.
   */
  size_t _totalPop;

  /*!
   @brief    The distance between rows for the given target density.
   */
  float _rowDist;

  /*!
   @brief    The distance between neighbors in a single row for the given target density.
   */
  float _nbrDist;

  /*!
   @brief    The population of the major row (interpreted differently depending on lattice row
            direction.
   */
  size_t _rowPop;

  /*!
   @brief    The number of rows to create.
   */
  size_t _rowCount;
};

//////////////////////////////////////////////////////////////////////////////

/*!
 @brief    Factory for HexLatticeGenerator
 */
class MENGE_API HexLatticeGeneratorFactory : public AgentGeneratorFactory {
 public:
  /*!
   @brief    Constructor.
   */
  HexLatticeGeneratorFactory();

  /*!
   @brief    The name of the generator type.

   The generator's name must be unique among all registered agent generator components. Each agent
   generator factory must override this function.

   @returns  A string containing the unique elevation name.
   */
  virtual const char* name() const { return "hex_lattice"; }

  /*!
   @brief    A description of the agent generator.

   Each agent generator factory must override this function.

   @returns  A string containing the agent generator description.
   */
  virtual const char* description() const {
    return "Agent generation is done via the specification of a bounded hexagonal "
           "lattice.";
  };

 protected:
  /*!
   @brief    Create an instance of this class's agent generator implementation.

   All AgentGeneratorFactory sub-classes must override this by creating (on the heap) a new instance
   of its corresponding generator type. The various field values of the instance will be set in a
   subsequent call to AgentGeneratorFactory::setFromXML(). The caller of this function takes
   ownership of the memory.

   @returns    A pointer to a newly instantiated EleAgentGenerator class.
   */
  AgentGenerator* instance() const { return new HexLatticeGenerator(); }

  /*!
   @brief    Given a pointer to a Goal Selector instance, sets the appropriate fields from the
            provided XML node.

   It is assumed that the value of the `type` attribute is this Goal Selector's type (i.e.
   GoalSelectorFactory::thisFactory has already been called and returned true). If sub-classes of
   GoalSelectorFactory introduce *new* GoalSelector parameters, then the sub-class should override
   this method but explicitly call the parent class's version.

   @param    gen         A pointer to the goal selector whose attributes are to be set.
   @param    node        The XML node containing the goal attributes.
   @param    behaveFldr  The path to the behavior file.  If the condition references resources in
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
   @brief    The identifier for the "alignment" string parameter.
   */
  size_t _alignID;

  /*!
   @brief    The identifier for the "row_direction" string parameter.
   */
  size_t _rowDirID;

  /*!
   @brief    The identifier for the "density" float parameter.
   */
  size_t _densityID;

  /*!
   @brief    The identifier for the "width" float parameter.
   */
  size_t _widthID;

  /*!
   @brief    The identifier for the "count" size_t parameter.
   */
  size_t _popID;

  /*!
   @brief    The identifier for the "rotation" float parameter.
   */
  size_t _rotID;
};
}  // namespace Agents
}  // namespace Menge
#endif  // __LATTICE_AGENT_GENERATOR_H__
