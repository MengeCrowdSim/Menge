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
 @file    Geometry2D.h
 @brief    Definition of various shapes for defining spatial relationships.
 */

#ifndef __GEOMETRY_2D_H__
#define __GEOMETRY_2D_H__

#include "MengeCore/mengeCommon.h"

// forward declaration
class TiXmlElement;

namespace Menge {

// forward declaration
namespace Agents {
class PrefVelocity;
}

namespace Math {

/*!
 @brief    Abstract 2d Geometry class for FSM queries.

 Supports queries to determine if points lie inside/outside a particular shape.
 */
class MENGE_API Geometry2D {
 public:
  /*!
   @brief    Constructor
   */
  Geometry2D() {}

  /*!
   @brief    Simple destructor
   */
  virtual ~Geometry2D() {}

  /*!
   @brief    Determine if the point is inside the shape based on the instance properties.

   @param    pt    The point to test.
   @returns  True if the point is inside the shape.
   */
  virtual bool containsPoint(const Vector2& pt) const = 0;

  /*!
   @brief    Determine if the point is inside the shape, not located at the instance value, but at
            the given position.  Definition of "at the given position" is defined by each shape.

   @param    pt    The point to test.
   @param    pos    The "position" of the shape.
   @returns  True if the point is inside the shape, false otherwise.
   */
  virtual bool containsPoint(const Vector2& pt, const Vector2& pos) const = 0;

  /*!
   @brief    Reports the *squared* distance from the given point to the goal.

   @param    pt      The query point.
   @returns  The squared distance from the point to the goal.
   */
  virtual float squaredDistance(const Vector2& pt) const = 0;

  /*!
   @brief    Set the preferred velocity directions w.r.t. the goal: left, right, and preferred.

   The Agents::PrefVelocity class represents a span of velocities that will reach the goal. For a
   goal that covers a 2D region, the directions in the Agents::PrefVelocity should span the arc
   subtended by the goal from the query point's perspective.  Furthermore, it should have
   sufficient clearance for a disk with the given radius to pass through. This should be overridden
   by subclasses to account for their unique geometry.

   The arc subtends an angle formed by a cone.  The apex of the cone is at the point `q`. The legs
   of the cone move from the apex in directions _towards_ the goal. The legs bound the minkowski
   _difference_ of the goal geometry with a circle of radius `r`.  In other words, the cone
   subtends the region such that if the _center_ of a disk of radius `r` were in that region, the
   whole disk would be contained in the geometry region.

   There is a degenerate case when the cone apex, `q`, lies _inside_ the goal geometry. Directions
   are now ill-defined.  The geometry will assign the _zero_ vectors to the three directions (left,
   right, and preferred). The target point will be `q`.

   @param    q              The query point.
   @param    r              The radius of clearance.
   @param    directions    An instance of Agents::PrefVelocity.
   */
  virtual void setDirections(const Vector2& q, float r, Agents::PrefVelocity& directions) const = 0;

  // TODO: Delete this function= transition uses it determine distance to goal
  //    I would be better off simply returning "squared distance to goal"
  /*!
   @brief    Returns the closest "target" point in the goal to the given query point.

   A "valid" target point is the nearest point to the query point that is sufficiently inside the
   goal region that a disk with the given radius is completely inside the goal. It need not be
   literally the *best* value, an approximation is sufficient.

   In the case where the goal region is too small to hold the agent, then the "deepest" point in the
   region is a good approximation.

   @param    q    The query point.
   @param    r    The radius of clearance.
   @returns  A 2D position representing the target point.
   */
  virtual Vector2 getTargetPoint(const Vector2& q, float r) const = 0;

  /*!
   @brief    Return the centroid of the goal.
   */
  virtual Vector2 getCentroid() const = 0;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Point "shape"
 */
class MENGE_API PointShape : public Geometry2D {
 public:
  /*!
   @brief    Default constructor
   */
  PointShape() : _position(0.f, 0.f) {}

  /*!
   @brief    Constructor

   @param    pos    The position of the point.
   */
  PointShape(const Vector2& pos) : Geometry2D(), _position(pos) {}

  /*!
   @brief    Copy constructor

   @param    shape    The shape to copy from.
   */
  PointShape(const PointShape& shape);

  /*!
   @brief    Initializes this shape as an translated version of the input shape.

   @param    shape      The shape to copy from.
   @param    offset    The displacement from this point to the new.
   */
  PointShape(const PointShape& shape, const Vector2& offset);

  /*!
   @brief    Construct an offset version of this shape.

   @param    pt    The offset value.
   @returns  A new PointShape offset from this one by the vector pt.
   */
  PointShape operator+(const Vector2& pt);

  /*!
   @brief   Sets the point's position.

   @param    pos    The new position.
   */
  void setPosition(const Vector2& pos) { _position.set(pos); }

  /*!
   @brief    Reports the point position.

   @returns  The point position.
   */
  const Vector2& getPosition() const { return _position; }

  /*!
   @brief    Determine if the point is inside the shape based on the instance properties.

   @param    pt    The point to test.
   @returns  True if the point is inside the shape.
   */
  virtual bool containsPoint(const Vector2& pt) const;

  /*!
   @brief    Determine if the given point is at the given position.

   @param    pt    The point to test.
   @param    pos    The position of the test point.
   @returns  True if the point is inside the shape.
   */
  virtual bool containsPoint(const Vector2& pt, const Vector2& pos) const;

  /*!
   @brief    Reports the *squared* distance from the given point to this shape.

   @param    pt      The query point.
   @returns  The squared distance from the point to this shape.
   */
  virtual float squaredDistance(const Vector2& pt) const;

  /*!
   @brief    Set the preferred velocity directions w.r.t. the goal: left, right, and preferred.

   The Agents::PrefVelocity class represents a span of velocities that will reach the goal. For a
   goal that covers a 2D region, the directions in the Agents::PrefVelocity should span the arc
   subtended by the goal from the query point's perspective. Furthermore, it should have sufficient
   clearance for a disk with the given radius to pass through. This should be overridden by
   subclasses to account for their unique geometry.

   @param    q              The query point.
   @param    r              The radius of clearance.
   @param    directions    An instance of Agents::PrefVelocity.
   */
  virtual void setDirections(const Vector2& q, float r, Agents::PrefVelocity& directions) const;

  // TODO: Delete this function= transition uses it determine distance to goal
  //    I would be better off simply returning "squared distance to goal"
  /*!
   @brief    Returns the closest "target" point in the goal to the given query point.

   A "valid" target point is the nearest point to the query point that is sufficiently inside the
   goal region that a disk with the given radius is completely inside the goal. It need not be
   literally the *best* value, an approximation is sufficient.

   In the case where the goal region is too small to hold the agent, then the "deepest" point in the
   region is a good approximation.

   @param    q    The query point.
   @param    r    The radius of clearance.
   @returns  A 2D position representing the target point.
   */
  virtual Vector2 getTargetPoint(const Vector2& q, float r) const;

  /*!
   @brief    Return the centroid of the goal.
   */
  virtual Vector2 getCentroid() const;

 protected:
  /*! @brief    Position */
  Vector2 _position;
};

/*!
 @brief    Circle shape
 */
class MENGE_API CircleShape : public Geometry2D {
 public:
  /*!
   @brief    Default constructor
   */
  CircleShape() : _center(0.f, 0.f), _radius(1.f) {}

  /*!
   @brief    Constructor

   @param    center    The position of the circle's center.
   @param    radius    The radius of the circle.
   */
  CircleShape(const Vector2& center, float radius)
      : Geometry2D(), _center(center), _radius(radius) {}

  /*!
   @brief    Copy constructor

   @param    shape    The shape to copy from.
   */
  CircleShape(const CircleShape& shape);

  /*!
   @brief    Initializes this shape as an translated version of the input shape.

   @param    shape    The shape to copy from.
   @param    offset    defines an offset from the copied CircleShape for the new
   CircleShape center
   */
  CircleShape(const CircleShape& shape, const Vector2& offset);

  /*!
   @brief    Construct an offset version of this shape.

   @param    pt    The offset value.
   @returns  A new CircleShape offset from this one by the vector pt.
   */
  CircleShape operator+(const Vector2& pt);

  /*!
   @brief    Reports the circle's radius.

   @returns  The circle's radius.
   */
  float getRadius() const { return _radius; }

  /*!
   @brief    Sets the radius of the circle.

   @param    radius    The circle's radius.
   */
  void setRadius(float radius) { _radius = radius; }

  /*!
   @brief    Reports the circle's center.

   @returns  The circle center.
   */
  const Vector2& getCenter() const { return _center; }

  /*!
   @brief    Sets the center of the circle.

   @param    center    The circle's center.
   */
  void setCenter(const Vector2& center) { _center.set(center); }

  /*!
   @brief    Sets the properties of the circle.

   @param    center    The circle's center.
   @param    radius    The circle's radius.
   */
  void set(const Vector2& center, float radius) {
    _center.set(center);
    _radius = radius;
  }

  /*!
   @brief    Determine if the point is inside the shape based on the instance properties.

   @param    pt    The point to test.
   @returns  True if the point is inside the shape, false otherwise.
   */
  virtual bool containsPoint(const Vector2& pt) const;

  /*!
   @brief    Determine if the point is inside a circle, centered on the given position.

   @param    pt    The point to test.
   @param    pos    The position of the circle's center.
   @returns  True if the point is inside the shape, false otherwise.
   */
  virtual bool containsPoint(const Vector2& pt, const Vector2& pos) const;

  /*!
   @brief    Reports the *squared* distance from the given point to the goal.

   @param    pt      The query point.
   @returns  The squared distance from the point to the goal.
   */
  virtual float squaredDistance(const Vector2& pt) const;

  /*!
   @brief    Set the preferred velocity directions w.r.t. the goal: left, right, and preferred.

   The Agents::PrefVelocity class represents a span of velocities that will reach the goal. For a
   goal that covers a 2D region, the directions in the Agents::PrefVelocity should span the arc
   subtended by the goal from the query point's perspective.  Furthermore, it should have sufficient
   clearance for a disk with the given radius to pass through. This should be overridden by
   subclasses to account for their unique geometry.

   @param    q              The query point.
   @param    r              The radius of clearance.
   @param    directions    An instance of Agents::PrefVelocity.
   */
  virtual void setDirections(const Vector2& q, float r, Agents::PrefVelocity& directions) const;

  // TODO: Delete this function= transition uses it determine distance to goal
  //    I would be better off simply returning "squared distance to goal"
  /*!
   @brief    Returns the closest "target" point in the goal to the given query point.

   A "valid" target point is the nearest point to the query point that is sufficiently inside the
   goal region that a disk with the given radius is completely inside the goal. It need not be
   literally the *best* value, an approximation is sufficient.

   In the case where the goal region is too small to hold the agent, then the "deepest" point in the
   region is a good approximation.

   @param    q    The query point.
   @param    r    The radius of clearance.
   @returns  A 2D position representing the target point.
   */
  virtual Vector2 getTargetPoint(const Vector2& q, float r) const;

  /*!
   @brief    Return the centroid of the goal.
   */
  virtual Vector2 getCentroid() const;

 protected:
  /*!
   @brief    Center of the circle
   */
  Vector2 _center;

  /*!
   @brief    Radius of the circle
   */
  float _radius;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Axis-aligned bounding box
 */
class MENGE_API AABBShape : public Geometry2D {
 public:
  /*!
   @brief    Default constructor
   */
  AABBShape();

  /*!
   @brief    Constructor

   @param    minPt    The minimum values of the bounding box along the x- and y-axes, respectively.
   @param    maxPt    The maximum values of the bounding box along the x- and y-axes, respectively.
   */
  AABBShape(const Vector2& minPt, const Vector2& maxPt);

  /*!
   @brief    Copy constructor

   @param    shape    The shape to copy from.
   */
  AABBShape(const AABBShape& shape);

  /*!
   @brief    Initializes this shape as an translated version of the input shape.

   @param    shape      The shape to copy from.
   @param    offset    defines an offset from the copied AABBShape for the new AABBShape.
   */
  AABBShape(const AABBShape& shape, const Vector2& offset);

  /*!
   @brief    Construct an offset version of this shape.

   @param    pt    The offset value.
   @returns  A new AABBShape offset from this one by the vector pt.
   */
  AABBShape operator+(const Vector2& pt);

  /*!
   @brief    Determine if the point is inside the shape based on the instance properties.

   @param    pt    The point to test.
   @returns  True if the point is inside the shape.
   */
  virtual bool containsPoint(const Vector2& pt) const;

  /*!
   @brief    Determine if the point is inside a AABB, centered on the given position.

   @param    pt    The point to test.
   @param    pos    The position of the circle.
   @returns  True if the point is inside the shape, false otherwise.
   */
  virtual bool containsPoint(const Vector2& pt, const Vector2& pos) const;

  /*!
   @brief    Reports the box's size (width by height)

   @returns  The box size.
   */
  const Vector2 getSize() const { return _maxPt - _minPt; }

  /*!
   @brief    Reports the box's maximal point (in x & y directions).

   @returns  The box's maximal extents.
   */
  const Vector2& getMaxPoint() const { return _maxPt; }

  /*!
   @brief    Reports the box's minimal point (in x & y directions).

   @returns  The box's minimal extents.
   */
  const Vector2& getMinPoint() const { return _minPt; }

  /*!
   @brief    Sets the extent of the AABB.

   @param    minPt    The minimum point (along the x- and y-axes).
   @param    maxPt    The maximum point (along the x- and y-axes).
   */
  void set(const Vector2& minPt, const Vector2& maxPt);

  /*!
   @brief    Sets the size of the AABB.

   It implicitly changes the maximum extent of the box, keeping the minimum point where it is.

   @param    size    The width and height of the box.
   */
  void setSize(const Vector2& size);

  /*!
   @brief    Reports the *squared* distance from the given point to the goal.

   @param    pt      The query point.
   @returns  The squared distance from the point to the goal.
   */
  virtual float squaredDistance(const Vector2& pt) const;

  /*!
   @brief    Set the preferred velocity directions w.r.t. the goal: left, right, and preferred.

   The Agents::PrefVelocity class represents a span of velocities that will reach the goal. For a
   goal that covers a 2D region, the directions in the Agents::PrefVelocity should span the arc
   subtended by the goal from the query point's perspective.  Furthermore, it should have sufficient
   clearance for a disk with the given radius to pass through. This should be overridden by
   subclasses to account for their unique geometry.

   @param    q             The query point.
   @param    r              The radius of clearance.
   @param    directions    An instance of Agents::PrefVelocity.
   */
  virtual void setDirections(const Vector2& q, float r, Agents::PrefVelocity& directions) const;

  // TODO: Delete this function= transition uses it determine distance to goal
  //    I would be better off simply returning "squared distance to goal"
  /*!
   @brief    Returns the closest "target" point in the goal to the given query point.

   A "valid" target point is the nearest point to the query point that is sufficiently inside the
   goal region that a disk with the given radius is completely inside the goal. It need not be
   literally the *best* value, an approximation is sufficient.

   In the case where the goal region is too small to hold the agent, then the "deepest" point in the
   region is a good approximation.

   @param    q    The query point.
   @param    r    The radius of clearance.
   @returns  A 2D position representing the target point.
   */
  virtual Vector2 getTargetPoint(const Vector2& q, float r) const;

  /*!
   @brief    Return the centroid of the goal.
   */
  virtual Vector2 getCentroid() const;

 protected:
  /*!
   @brief    The minimum extent of the bounding box
   */
  Vector2 _minPt;

  /*!
   @brief    The maximum extent of the bounding box
   */
  Vector2 _maxPt;

  /*!
   @brief    The size of the bounding box (along the x- and y- axes).
   */
  Vector2 _halfSize;
};

/////////////////////////////////////////////////////////////////////

/*!
 @brief    Oriented bounding box
 */
class MENGE_API OBBShape : public Geometry2D {
 public:
  /*!
   @brief    Default constructor
   */
  OBBShape();

  /*!
   @brief    Constructor

   @param    pivot    The minimum values of the unrotated bounding box along the x- and y-axes,
                    respectively.  The bounding box gets rotated around this point.
   @param    size    The size of the bounding box along the LOCAL x- and y-axes, respectively.
   @param    angle    The angle of rotation (in radians).
   */
  OBBShape(const Vector2& pivot, const Vector2& size, float angle);

  /*!
   @brief    Copy constructor

   @param    shape    The shape to copy from.
   */
  OBBShape(const OBBShape& shape);

  /*!
   @brief    Initializes this shape as an translated version of the input shape.

   @param    shape      The shape to copy from.
   @param    offset    offset vector from the copied shape
   */
  OBBShape(const OBBShape& shape, const Vector2& offset);

  /*!
   @brief    Construct an offset version of this shape.

   @param    pt    The offset value.
   @returns  A new OBBShape offset from this one by the vector pt.
   */
  OBBShape operator+(const Vector2& pt);

  /*!
   @brief    Determine if the point is inside the shape based on the instance properties.

   @param    pt    The point to test.
   @returns  True if the point is inside the shape, false otherwise.
   */
  virtual bool containsPoint(const Vector2& pt) const;

  /*!
   @brief    Determine if the point is inside a OBB, centered on the given position.

   @param    pt    The point to test.
   @param    pos    The position of the circle.
   @returns  True if the point is inside the shape, false otherwise.
   */
  virtual bool containsPoint(const Vector2& pt, const Vector2& pos) const;

  /*!
   @brief    Sets the extent of the OBB.

   @param    pivot    The pivot point of the OBB
   @param    width    The width of the unrotated box (length along the x-axis).
   @param    height    The width of the unrotated box (length along the x-axis).
   @param    angle    The angle of rotation (in radians) around the pivot point.
   */
  void set(const Vector2& pivot, float width, float height, float angle);

  /*!
   @brief    Sets the size of the OBB.

   The pivot point and angle remain unchanged.

   @param    size    The width and height of the box.
   */
  void setSize(const Vector2& size) { _size.set(size); }

  /*!
   @brief    Sets the angle of the OBB.

   The pivot point and size remain unchanged.

   @param    angle    The angle (in radians) the OBB is rotated around its pivot.
   */
  void setAngle(float angle);

  /*!
   @brief    Reports the *squared* distance from the given point to the goal.

   @param    pt      The query point.
   @returns  The squared distance from the point to the goal.
   */
  virtual float squaredDistance(const Vector2& pt) const;

  /*!
   @brief    Set the preferred velocity directions w.r.t. the goal: left, right, and preferred.

   The Agents::PrefVelocity class represents a span of velocities that will reach the goal. For a
   goal that covers a 2D region, the directions in the Agents::PrefVelocity should span the arc
   subtended by the goal from the query point's perspective.  Furthermore, it should have sufficient
   clearance for a disk with the given radius to pass through. This should be overridden by
   subclasses to account for their unique geometry.

   @param    q              The query point.
   @param    r              The radius of clearance.
   @param    directions    An instance of Agents::PrefVelocity.
   */
  virtual void setDirections(const Vector2& q, float r, Agents::PrefVelocity& directions) const;

  // TODO: Delete this function= transition uses it determine distance to goal
  //    I would be better off simply returning "squared distance to goal"
  /*!
   @brief    Returns the closest "target" point in the goal to the given query point.

   A "valid" target point is the nearest point to the query point that is sufficiently inside the
   goal region that a disk with the given radius is completely inside the goal. It need not be
   literally the *best* value, an approximation is sufficient.

   In the case where the goal region is too small to hold the agent, then the "deepest" point in the
   region is a good approximation.

   @param    q    The query point.
   @param    r    The radius of clearance.
   @returns  A 2D position representing the target point.
   */
  virtual Vector2 getTargetPoint(const Vector2& q, float r) const;

  /*!
   @brief    Return the centroid of the goal.
   */
  virtual Vector2 getCentroid() const;

  /*!
   @brief   Converts a position vector from the geometry origin to P, expressed in the geometry
            frame (`r_GP`) to a position vector from the world origin to P, expressed in the
            world frame (`r_WP`).

   @param  r_GP    The position vector from the geometry origin to a point P, expressed in the
                   geometry frame.
   @retval r_WP_W  The position vector from world origin to the point P, expressed in the world
                  frame.
   */
  Vector2 convertToWorld(const Vector2& r_GP) const;

  /*!
  @brief  Converts a position vector from the world origin to P, expressed in the world frame
          (`r_WP`) to a position vector from the geometry origin to P, expressed in the
          geometry frame (`r_GP`).

  @param  r_WP    The position vector from the world origin to a point P, expressed in the world
                  frame.
  @retval r_GP_G  The position vector from geometry origin to the point P, expressed in the
                  geometry frame.
  */
  Vector2 convertToGeometry(const Vector2& r_WP) const;

  /*!
  @brief    Returns the x-axis of the OBB's local frame expressed in the world frame.

  If we say that Bx = getXBasis() and By = getYBasis(), are *column* vectors, we can define the
  2x2 matrix R_WG = [Bx By] - a rotation taking vectors expressed in the geometry frame and
  expressing them in the world frame.

  For example, the position vector from the pivot to the far corner, expressed in the geometry
  frame would be `r_PC_G = [w h]^T`. We can express that same vector in the world frame as:
  `r_PC_W = R_WG * r_PC_G`.

  Conversely, the matrix `R_GW = R_WG^T = [Bx By]^T` re-expresses in the *geometry* frame a
  vector that was previously expressed in the world frame.

  In practice, for a position vector from the world origin to point Q expressed in the world
  frame, r_WQ_W, we can calculate a position vector from the geometry frame's origin r_WG (the
  OBB's pivot) and expressed in the geometry frame as:
  `r_GQ = [(r_WQ - r_WG) * Bx, (r_WQ - r_WG) * By]^T` or
  `r_GQ = R_GW * (r_WQ - r_WG)`.

  Functionally, that becomes:

  ```
  Vector2 r_WQ_W;
  Vector2 Bx = getXBasis();
  Vector2 By = getYBasis();
  Vector2 r_GQ_W = r_WQ - _pivot;
  Vector2 r_GQ_G(r_GQ_W * Bx, r_GQ_W * By);
  ```

  Mapping the opposite direction is a bit trickier (in the absence of a 2x2 matrix).

  ```
  Vector2 r_GQ_G;
  Vector2 Bx = getXBasis();
  Vector2 By = getYBasis();
  Vector2 r_GQ_W(r_GQ_G * Vector2(Bx._x, By._x), r_GQ_G * Vector2(Bx._y, By._y));
  Vector2 r_WQ_W = _pivot + r_GQ_W;
  ```

  See [Drake's monogram
  notation](http://drake.mit.edu/doxygen_cxx/group__multibody__notation__basics.html) for details on
  how to interpret the symbols, `r_WQ_W`, `R_WG`, etc.

  @returns  The direction of the obb's x-axis expressed in the world frame.
  */
  Vector2 getXBasis() const { return Vector2(_cosTheta, _sinTheta); }

  /*!
  @brief    Returns the y-axis of the OBB's local frame expressed in the world frame

  If we say that Bx = getXBasis() and By = getYBasis(), are *column* vectors, we can define the
  2x2 matrix R_WG = [Bx By] - a rotation taking vectors expressed in the geometry frame and
  expressing them in the world frame.

  For example, the position vector from the pivot to the far corner, expressed in the geometry
  frame would be `r_PC_G = [w h]^T`. We can express that same vector in the world frame as:
  `r_PC_W = R_WG * r_PC_G`.

  Conversely, the matrix `R_GW = R_WG^T = [Bx By]^T` re-expresses in the *geometry* frame a
  vector that was previously expressed in the world frame.

  In practice, for a position vector from the world origin to point Q expressed in the world
  frame, r_WQ_W, we can calculate a position vector from the geometry frame's origin r_WG (the
  OBB's pivot) and expressed in the geometry frame as:
  `r_GQ = [(r_WQ - r_WG) * Bx, (r_WQ - r_WG) * By]^T` or
  `r_GQ = R_GW * (r_WQ - r_WG)`.

  Functionally, that becomes:

  ```
  Vector2 r_WQ_W;
  Vector2 r_GQ_W = r_WQ - _pivot;
  Vector2 Bx = getXBasis();
  Vector2 By = getYBasis();
  Vector2 r_GQ_G(r_GQ_W * Bx, r_GQ_W * By);
  ```

  Mapping the opposite direction is a bit trickier (in the absence of a 2x2 matrix).

  ```
  Vector2 r_GQ_G;
  Vector2 Bx = getXBasis();
  Vector2 By = getYBasis();
  Vector2 r_GQ_W(r_GQ_G * Vector2(Bx._x, By._x), r_GQ_G * Vector2(Bx._y, By._y));
  Vector2 r_WQ_W = _pivot + r_GQ_W;
  ```

  See [Drake's monogram
  notation](http://drake.mit.edu/doxygen_cxx/group__multibody__notation__basics.html) for details on
  how to interpret the symbols, `r_WQ_W`, `R_WG`, etc.

  @returns  The direction of the obb's y-axis expressed in the world frame.
  */
  Vector2 getYBasis() const { return Vector2(-_sinTheta, _cosTheta); }

  /*!
   @brief    Returns the size of the obb (w, h)

   @returns  The box's size.
   */
  const Vector2& getSize() const { return _size; }

  /*!
   @brief    Return the pivot point of the obb.

   @returns  The box's pivot.
   */
  const Vector2& getPivot() const { return _pivot; }

 protected:
  /*!
   @brief    The minimum corners of the bounding box (the box is rotated around this point).
   */
  Vector2 _pivot;

  /*!
   @brief    The width and height of the box (along its local coordinate system).
   */
  Vector2 _size;

  /*!
   @brief    The half width and height of the box.
   */
  Vector2 _halfSize;

  /*!
   @brief    The cosine of the box's angle of rotation (used for accelerating queries).
   */
  float _cosTheta;

  /*!
   @brief    The sine of the box's angle of rotation (used for accelerating queries).
   */
  float _sinTheta;
};

/*!
 @brief    Reads the attributes of an XML element to determine if it contains the definition of a
          known Geoemtry2D instance -- instantiating the shape if possible.

 @param [in,out]  node    The node containing xml attributes.
 @param           prefix  (Optional) A prefix that can be pre-pended to the shape attribute names.

 @return  Null if it fails, else the new geometry.  The caller is responsible for destroying the
          instance.
 */
MENGE_API Geometry2D* createGeometry(TiXmlElement* node, const std::string& prefix = "");

/**
 @brief  Creates a point from the attributes of an XML element.

 @param [in,out]  node    The node containing xml attributes.
 @param           prefix  (Optional) A prefix that can be pre-pended to the shape attribute names.

 @return  Null if it fails, else the new point.  The caller is responsible for destroying the
          instance.
 */
MENGE_API PointShape* createPoint(TiXmlElement* node, const std::string& prefix = "");

/**
 @brief  Creates a circle from the attributes of an XML element.

 @param [in,out]  node    The node containing xml attributes.
 @param           prefix  (Optional) A prefix that can be pre-pended to the shape attribute names.

 @return  Null if it fails, else the new circle.  The caller is responsible for destroying the
          instance.
 */
MENGE_API CircleShape* createCircle(TiXmlElement* node, const std::string& prefix = "");

/**
 @brief  Creates an axis-aligned bounding box from the attributes of an XML element.

 @param [in,out]  node    The node containing xml attributes.
 @param           prefix  (Optional) A prefix that can be pre-pended to the shape attribute names.

 @return  Null if it fails, else the new AABB.  The caller is responsible for destroying the
          instance.
 */
MENGE_API AABBShape* createAABB(TiXmlElement* node, const std::string& prefix = "");

/**
 @brief  Creates an oriented bounding box (OBB) from the attributes of an XML element.

 @param [in,out]  node    The node containing xml attributes.
 @param           prefix  (Optional) A prefix that can be pre-pended to the shape attribute names.

 @return  Null if it fails, else the new OBB.  The caller is responsible for destroying
 the instance.
 */
MENGE_API OBBShape* createOBB(TiXmlElement* node, const std::string& prefix = "");

}  // namespace Math
}  // namespace Menge

#endif  // __GEOMETRY_2D_H__
