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
 @file       NavMeshPoly.h
 @brief      Defines the geometric, convex polygon for each navigation mesh node
 */

#ifndef __NAV_MESH_POLY_H__
#define __NAV_MESH_POLY_H__

#include "MengeCore/BFSM/fsmCommon.h"

#include <fstream>
#include <string>

namespace Menge {

// FORWARD DECLARATIONS
class NavMeshNode;
class NavMesh;

/*!
 @brief    The polygon used in each node of a navigation mesh graph
 */
class MENGE_API NavMeshPoly {
 public:
  /*!
   @brief    Constructor
   */
  NavMeshPoly();

  /*!
   @brief    Destructor
   */
  ~NavMeshPoly();

  /*!
   @brief    Assignment operator
   */
  NavMeshPoly& operator=(const NavMeshPoly& n);

  /*!
   @brief    Reports if the point lies inside the polygon.

   NOTE: This is limited to 2D polygons.

   @param    point    The point to test.
   @returns  A boolean reporting if the point is inside (true) or outside (false) the polygon
   */
  // TODO: Figure out what I do w.r.t. boundaries.
  bool containsPoint(const Math::Vector2& point) const;

  /*!
   @brief    Computes the elevation of the polygon at the given point.

   @param    point    A point on the polygon to evaluate. The function does not test to see if the
                    point lies inside the polygon.
   @returns  The elevation of the polygon at the given point.
   */
  float getElevation(const Math::Vector2& point) const;

  /*!
   @brief    Reports the gradient of the polygon.

   Because it is a plane, the gradient is constant at all positions.

   @returns  The gradient of the plane.
   */
  Math::Vector2 getGradient() const { return Math::Vector2(_A, _B); }

  /*!
   @brief    Sets the polygon properties from a polygon definition in the given ascii file stream.

   @param    f    The input file stream.
   @returns  A boolean indicating successful parsing (true) or failure (false).
   */
  bool loadFromAscii(std::ifstream& f);

  /*!
   @brief    Sets the polygon properties from a polygon definition in the given binary file stream.

   @param    f    The input file stream.
   @returns  A boolean indicating successful parsing (true) or failure (false).
   */
  bool loadFromBinary(std::ifstream& f);

  /*!
   @brief    Initialize the polygon with vertices and equation of plane

   @param    vCount    Number of vertices in the polygon.
   @param    ids      An array of index values into the global set of vertices in the navigation mesh.
   @param    A      The first coefficient of the planar equation: f( x, y ) = Ax + By + C
   @param    B      The second coefficient of the planar equation: f( x, y ) = Ax + By + C
   @param    C      The third coefficient of the planar equation: f( x, y ) = Ax + By + C
   */
  void initialize(size_t vCount, unsigned int* ids, float A = 0.f, float B = 0.f, float C = 0.f);

  friend class NavMeshNode;
  friend class NavMesh;

 protected:
  /*!
   @brief    An array of indices into the vertex list defining the polygon.
   
   The indices must be ordered such that the vertices are visited in a counter-clockwise order.
   */
  // TODO: These are unsigned ints because of the binary file format. It would be better to decouple
  // the file format from the representation.
  unsigned int* _vertIDs;

  /*!
   @brief    The number of vertices in the polygon.
   */
  size_t _vertCount;

  /*!
   @brief    A pointer to the mesh vertex information for performing geometric tests.
   */
  const Math::Vector2* _vertices;

  // TODO: Replace the individual bounding box with a BVH outside of the
  //    polygon.  And then change the "is inside" function assuming
  //    that BVH tests have already been done.
  /*!
   @brief    Minimum extent on the x-axis of the polygon.
   */
  float _minX;

  /*!
   @brief    Maximum extent on the x-axis of the polygon.
   */
  float _maxX;

  /*!
   @brief    Minimum extent on the y-axis of the polygon.
   */
  float _minY;

  /*!
   @brief    Maximum extent on the y-axis of the polygon.
   */
  float _maxY;

  /*!
   @brief    Compute the bounding box for this polygon.

   @param    vertices    The actucal vertices used for geometric queries.
   */
  void setBB(const Math::Vector2* vertices);

  /*!
   @brief    The first coefficient of the planar equation: f( x, y ) = Ax + By + C. 

   Used to compute the elevation of the point on the polygon.
   */
  float _A;

  /*!
   @brief    The second coefficient of the planar equation: f( x, y ) = Ax + By + C.

   Used to compute the elevation of the point on the polygon.
   */
  float _B;

  /*!
   @brief    The third coefficient of the planar equation: f( x, y ) = Ax + By + C.

   Used to compute the elevation of the point on the polygon.
   */
  float _C;
};
}  // namespace Menge

#endif  // __NAV_MESH_POLY_H__
