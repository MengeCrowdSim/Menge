/*

License

Menge
Copyright © and trademark ™ 2012-14 University of North Carolina at Chapel Hill.
All rights reserved.

Permission to use, copy, modify, and distribute this software and its documentation
for educational, research, and non-profit purposes, without fee, and without a
written agreement is hereby granted, provided that the above copyright notice,
this paragraph, and the following four paragraphs appear in all copies.

This software program and documentation are copyrighted by the University of North
Carolina at Chapel Hill. The software program and documentation are supplied "as is,"
without any accompanying services from the University of North Carolina at Chapel
Hill or the authors. The University of North Carolina at Chapel Hill and the
authors do not warrant that the operation of the program will be uninterrupted
or error-free. The end-user understands that the program was developed for research
purposes and is advised not to rely exclusively on the program for any reason.

IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE AUTHORS
BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
DOCUMENTATION, EVEN IF THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE
AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY
DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY STATUTORY WARRANTY
OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND
THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS HAVE NO OBLIGATIONS
TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

Any questions or comments should be sent to the authors {menge,geom}@cs.unc.edu

*/

#include "MengeCore/Math/Geometry2D.h"

#include "MengeCore/Agents/PrefVelocity.h"
#include "MengeCore/PluginEngine/Attribute.h"

#include "thirdParty/tinyxml.h"

#include <algorithm>
#include <cassert>

namespace Menge {

namespace Math {

/////////////////////////////////////////////////////////////////////
//                   Implementation of PointShape
/////////////////////////////////////////////////////////////////////

PointShape::PointShape(const PointShape& shape) { _position = shape._position; }

/////////////////////////////////////////////////////////////////////

PointShape::PointShape(const PointShape& shape, const Vector2& offset) {
  _position = shape._position + offset;
}

/////////////////////////////////////////////////////////////////////

PointShape PointShape::operator+(const Vector2& pt) { return PointShape(*this, pt); }

/////////////////////////////////////////////////////////////////////

bool PointShape::containsPoint(const Vector2& pt) const {
  float distSq = absSq(pt - _position);
  return distSq < 1e-6f;
}

/////////////////////////////////////////////////////////////////////

bool PointShape::containsPoint(const Vector2& pt, const Vector2& pos) const {
  float distSq = absSq(pt - pos);
  return distSq < 1e-6f;
}

/////////////////////////////////////////////////////////////////////

float PointShape::squaredDistance(const Vector2& pt) const { return absSq(pt - _position); }

/////////////////////////////////////////////////////////////////////

void PointShape::setDirections(const Vector2& q, float r, Agents::PrefVelocity& directions) const {
  Vector2 disp = _position - q;
  const float distSq = absSq(disp);
  Vector2 dir;
  if (distSq > 1e-8) {
    // Distant enough that I can normalize the direction.
    dir.set(disp / sqrtf(distSq));
  } else {
    dir.set(0.f, 0.f);
  }
  directions.setSingle(dir);
  directions.setTarget(_position);
}

/////////////////////////////////////////////////////////////////////

Vector2 PointShape::getTargetPoint(const Vector2& q, float r) const { return _position; }

/////////////////////////////////////////////////////////////////////

Vector2 PointShape::getCentroid() const { return _position; }

/////////////////////////////////////////////////////////////////////
//                   Implementation of CircleShape
/////////////////////////////////////////////////////////////////////

CircleShape::CircleShape(const CircleShape& shape) {
  _center = shape._center;
  _radius = shape._radius;
}

/////////////////////////////////////////////////////////////////////

CircleShape::CircleShape(const CircleShape& shape, const Vector2& offset) {
  _center = shape._center + offset;
  _radius = shape._radius;
}

/////////////////////////////////////////////////////////////////////

CircleShape CircleShape::operator+(const Vector2& pt) {
  CircleShape circle(*this);
  circle._center += pt;
  return circle;
}

/////////////////////////////////////////////////////////////////////

bool CircleShape::containsPoint(const Vector2& pt) const {
  float distSq = absSq(pt - _center);
  return distSq < _radius * _radius;
}

/////////////////////////////////////////////////////////////////////

bool CircleShape::containsPoint(const Vector2& pt, const Vector2& pos) const {
  float distSq = absSq(pt - pos);
  return distSq < _radius * _radius;
}

/////////////////////////////////////////////////////////////////////

float CircleShape::squaredDistance(const Vector2& pt) const {
  float d = abs(pt - _center);
  float perimD = d - _radius;
  return perimD * perimD;
}

/////////////////////////////////////////////////////////////////////

void CircleShape::setDirections(const Vector2& q, float r, Agents::PrefVelocity& directions) const {
  const float TARGET_R = _radius - r;
  if (TARGET_R < 0) {
    // circle isn't big enough to fit agent -- treat it like a point goal
    Vector2 dir(norm(_center - q));
    directions.setSingle(dir);
    directions.setTarget(_center);
  } else {
    // Circle is large enough to form a span
    const float TARGET_R_SQD = TARGET_R * TARGET_R;
    Vector2 relPos = _center - q;
    const float distSq = absSq(relPos);
    if (distSq < TARGET_R_SQD) {
      // Goal reached -- I'm inside the effective circle -- current position is the
      //  goal and no movement is necessary
      directions.setSingle(Vector2(0.f, 0.f));
      directions.setTarget(q);
    } else {
      // Outside the effective circle, a span is possible.
      float leg = sqrtf(distSq - TARGET_R_SQD);
      Vector2 left = Vector2(relPos.x() * leg - relPos.y() * TARGET_R,
                             relPos.x() * TARGET_R + relPos.y() * leg) /
                     distSq;
      Vector2 right = Vector2(relPos.x() * leg + relPos.y() * TARGET_R,
                              -relPos.x() * TARGET_R + relPos.y() * leg) /
                      distSq;
      const float dist = sqrtf(distSq);
      Vector2 dir = relPos / dist;
      directions.setSpan(left, right, dir);
      directions.setTarget(q + dir * (dist - TARGET_R));
    }
  }
}

/////////////////////////////////////////////////////////////////////

Vector2 CircleShape::getTargetPoint(const Vector2& q, float r) const {
  const float THRESH = _radius - r;
  if (THRESH > 0) {
    Vector2 disp(q - _center);
    float distSqd = absSq(disp);
    if (distSqd < THRESH * THRESH) {
      return q;
    } else {
      float dist = sqrtf(distSqd);
      Vector2 offset = disp * (THRESH / dist);
      return _center + offset;
    }
  } else {
    return _center;
  }
}

/////////////////////////////////////////////////////////////////////

Vector2 CircleShape::getCentroid() const { return _center; }

/////////////////////////////////////////////////////////////////////
//                   Implementation of AABBShape
/////////////////////////////////////////////////////////////////////

AABBShape::AABBShape() : Geometry2D(), _minPt(0.f, 0.f), _maxPt(0.f, 0.f), _halfSize(0.f, 0.f) {}

/////////////////////////////////////////////////////////////////////

AABBShape::AABBShape(const Vector2& minPt, const Vector2& maxPt)
    : Geometry2D(), _minPt(minPt), _maxPt(maxPt) {
  assert(_minPt.x() <= _maxPt.x() && _minPt.y() <= _maxPt.y() && "Improper definition of AABB");
  _halfSize = (maxPt - minPt) * 0.5f;
}

/////////////////////////////////////////////////////////////////////

AABBShape::AABBShape(const AABBShape& shape) {
  _minPt = shape._minPt;
  _maxPt = shape._maxPt;
  _halfSize = shape._halfSize;
}

/////////////////////////////////////////////////////////////////////

AABBShape::AABBShape(const AABBShape& shape, const Vector2& offset) {
  _minPt = shape._minPt + offset;
  _maxPt = shape._maxPt + offset;
  _halfSize = shape._halfSize;
}

/////////////////////////////////////////////////////////////////////

AABBShape AABBShape::operator+(const Vector2& pt) {
  AABBShape aabb(*this);
  aabb._minPt += pt;
  aabb._maxPt += pt;
  return aabb;
}

/////////////////////////////////////////////////////////////////////

bool AABBShape::containsPoint(const Vector2& pt) const {
  const float x = pt.x();
  const float y = pt.y();
  return (x >= _minPt.x() && x <= _maxPt.x() && y >= _minPt.y() && y <= _maxPt.y());
}

/////////////////////////////////////////////////////////////////////

bool AABBShape::containsPoint(const Vector2& pt, const Vector2& pos) const {
  const float x = pt.x();
  const float y = pt.y();
  Vector2 minPt = pos - _halfSize;
  Vector2 maxPt = pos + _halfSize;
  return (x >= minPt.x() && x <= maxPt.x() && y >= minPt.y() && y <= maxPt.y());
}

/////////////////////////////////////////////////////////////////////

void AABBShape::set(const Vector2& minPt, const Vector2& maxPt) {
  _minPt.set(minPt);
  _maxPt.set(maxPt);
  _halfSize = (_maxPt - _minPt) * 0.5f;
}

/////////////////////////////////////////////////////////////////////

void AABBShape::setSize(const Vector2& size) {
  _maxPt = _minPt + size;
  _halfSize = size * 0.5f;
}
/////////////////////////////////////////////////////////////////////

float AABBShape::squaredDistance(const Vector2& pt) const {
  // determine quadrant
  float minX = _minPt._x;
  float maxX = _maxPt._x;
  const int xCoord = (int)(pt._x > maxX) - (int)(pt._x < minX);
  float minY = _minPt._y;
  float maxY = _maxPt._y;
  const int yCoord = (int)(pt._y > maxY) - (int)(pt._y < minY);

  if (xCoord == 0 && yCoord == 0) {
    return 0.f;
  } else {
    // the x- and y- coordinates of the nearest point
    float X, Y;
    X = (xCoord == -1) * minX + (xCoord == 1) * maxX;
    Y = (yCoord == -1) * minY + (yCoord == 1) * maxY;
    return absSq(Vector2(X, Y) - pt);
  }
}

/////////////////////////////////////////////////////////////////////

void AABBShape::setDirections(const Vector2& q, float r, Agents::PrefVelocity& directions) const {
  // based on the voronoi regions of the AABB
  //   (-1,1)  |        (0,1)        |  (1,1)
  // ----------------------------------------
  //           |                     |
  //   (-1, 0) |        (0,0)        |  (1,0)
  //           |                     |
  // ----------------------------------------
  //   (-1,-1) |       (0,-1)        |  (1,-1)
  //

  // the x- and y- coordinates of the nearest point
  float X, Y;
  // the x- and y-coordintes of the left and right directions
  float xL, xR, yL, yR;
  Vector2 size = _maxPt - _minPt;

  // determine quadrant
  const float D = 2.f * r;
  float minX = _minPt._x + r;
  float maxX = minX + size._x - D;
  const int xCoord = (int)(q._x > maxX) - (int)(q._x < minX);
  float minY = _minPt._y + r;
  float maxY = minY + size._y - D;
  const int yCoord = (int)(q._y > maxY) - (int)(q._y < minY);

  if (xCoord == 0 && yCoord == 0) {
    // inside the region already
    directions.setSingle(Vector2(0.f, 0.f));
    directions.setTarget(q);
  } else {
    Vector2 size = _halfSize * 2;
    int dimensions = 2;
    if (size._x < D) {
      X = _minPt._x + size._x * 0.5f;
      xL = X;
      xR = X;
      --dimensions;
    } else {
      int LMaxXtest = (yCoord == 1) | ((xCoord == 1) & (yCoord == 0));
      xL = maxX * LMaxXtest + (1 - LMaxXtest) * minX;
      int RMaxXtest = (yCoord == -1) | ((xCoord == 1) & (yCoord == 0));
      xR = maxX * RMaxXtest + (1 - RMaxXtest) * minX;
      X = (xCoord == -1) * minX + (xCoord == 1) * maxX + (xCoord == 0) * q._x;
    }
    if (size._y < D) {
      Y = _minPt._y + size._y * 0.5f;
      yL = Y;
      yR = Y;
      --dimensions;
    } else {
      int LMaxYtest = (xCoord == -1) | ((xCoord == 0) & (yCoord == 1));
      yL = maxY * LMaxYtest + (1 - LMaxYtest) * minY;
      int RMaxYtest = (xCoord == 1) | ((xCoord == 0) & (yCoord == 1));
      yR = maxY * RMaxYtest + (1 - RMaxYtest) * minY;
      Y = (yCoord == -1) * minY + (yCoord == 1) * maxY + (yCoord == 0) * q._y;
    }

    Vector2 targetPt = Vector2(X, Y);
    directions.setTarget(targetPt);
    Vector2 prefDir(norm(targetPt - q));
    if (dimensions) {
      // there is actually a span
      directions.setSpan(norm(Vector2(xL, yL) - q), norm(Vector2(xR, yR) - q), prefDir);
    } else {
      directions.setSingle(prefDir);
    }
  }
}

/////////////////////////////////////////////////////////////////////

Vector2 AABBShape::getTargetPoint(const Vector2& q, float r) const {
  // based on the voronoi regions of the AABB
  //   0 |         1          |  2
  // ----------------------------------------
  //     |                    |
  //   3 |         4          |  5
  //     |                    |
  // ----------------------------------------
  //   6 |         7          |  8
  //
  //  Approximated by simply offsetting the sides by r.

  Vector2 size = _maxPt - _minPt;
  float X = q._x;
  float Y = q._y;
  const float D = 2.f * r;

  if (size._x < D) {
    X = _minPt._x + size._x * 0.5f;
  } else {
    float minX = _minPt._x + r;
    float maxX = minX + size._x - D;
    if (q._x < minX) {
      X = minX;
    } else if (q._x > maxX) {
      X = maxX;
    }
  }
  if (size._y < D) {
    Y = _minPt._y + size._y * 0.5f;
  } else {
    float minY = _minPt._y + r;
    float maxY = minY + size._y - D;
    if (q._y < minY) {
      Y = minY;
    } else if (q._y > maxY) {
      Y = maxY;
    }
  }

  return Vector2(X, Y);
}

/////////////////////////////////////////////////////////////////////

Vector2 AABBShape::getCentroid() const { return (_minPt + _maxPt) * 0.5f; }

/////////////////////////////////////////////////////////////////////
//                   Implementation of OBBShape
/////////////////////////////////////////////////////////////////////

OBBShape::OBBShape()
    : Geometry2D(), _pivot(0.f, 0.f), _size(0.f, 0.f), _cosTheta(1.f), _sinTheta(0.f) {}

/////////////////////////////////////////////////////////////////////

OBBShape::OBBShape(const Vector2& pivot, const Vector2& size, float angle)
    : Geometry2D(), _pivot(pivot), _size(size), _cosTheta(cos(angle)), _sinTheta(sin(angle)) {}

/////////////////////////////////////////////////////////////////////

OBBShape::OBBShape(const OBBShape& shape) {
  _pivot = shape._pivot;
  _size = shape._size;
  _halfSize = shape._halfSize;
  _cosTheta = shape._cosTheta;
  _sinTheta = shape._sinTheta;
}

/////////////////////////////////////////////////////////////////////

OBBShape::OBBShape(const OBBShape& shape, const Vector2& offset) {
  _pivot = shape._pivot + offset;
  _size = shape._size;
  _halfSize = shape._halfSize;
  _cosTheta = shape._cosTheta;
  _sinTheta = shape._sinTheta;
}

/////////////////////////////////////////////////////////////////////

OBBShape OBBShape::operator+(const Vector2& pt) {
  OBBShape obb(*this);
  obb._pivot += pt;
  return obb;
}

/////////////////////////////////////////////////////////////////////

bool OBBShape::containsPoint(const Vector2& pt) const {
  // Offset position to the space of the oriented bounding box.
  Vector2 disp = pt - _pivot;
  // rotate NEGATIVELY
  const float x = disp.x() * _cosTheta + disp.y() * _sinTheta;
  const float y = disp.y() * _cosTheta - disp.x() * _sinTheta;
  return (x >= 0.f && x <= _size.x() && y >= 0.f && y <= _size.y());
}

/////////////////////////////////////////////////////////////////////

bool OBBShape::containsPoint(const Vector2& pt, const Vector2& pos) const {
  // point relative to the center
  //  Scaled by 2 so I can use the size value directly to do the test
  Vector2 p = (pt - pos) * 2.f;
  // rotate it
  const float x = p.x() * _cosTheta + p.y() * _sinTheta;
  const float y = p.y() * _cosTheta - p.x() * _sinTheta;

  return (x >= -_size.x() && x <= _size.x() && y >= -_size.y() && y <= _size.y());
}

/////////////////////////////////////////////////////////////////////

void OBBShape::set(const Vector2& pivot, float width, float height, float angle) {
  _pivot.set(pivot);
  _size.set(width, height);
  _cosTheta = cos(angle);
  _sinTheta = sin(angle);
}

/////////////////////////////////////////////////////////////////////

void OBBShape::setAngle(float angle) {
  _cosTheta = cos(angle);
  _sinTheta = sin(angle);
}

/////////////////////////////////////////////////////////////////////

float OBBShape::squaredDistance(const Vector2& pt) const {
  // First rotate so that the OBB is an AABB then use the same logic as
  //  with the AABB
  Vector2 disp = pt - _pivot;
  // the LOCAL x- and y- coordinates of the nearest point, initialized to the
  //  local value of the query point.
  float X = disp.x() * _cosTheta + disp.y() * _sinTheta;
  float Y = disp.y() * _cosTheta - disp.x() * _sinTheta;

  // determine quadrant
  const float minX = 0.f;
  const float maxX = _size._x;
  const int xCoord = (int)(X > maxX) - (int)(X < minX);
  const float minY = 0.f;
  const float maxY = _size._y;
  const int yCoord = (int)(Y > maxY) - (int)(Y < minY);

  if (xCoord == 0 && yCoord == 0) {
    return 0.f;
  } else {
    // the x- and y- coordinates of the nearest point
    X = (xCoord == -1) * minX + (xCoord == 1) * maxX;
    Y = (yCoord == -1) * minY + (yCoord == 1) * maxY;
    return absSq(Vector2(X, Y) - disp);
  }
}

/////////////////////////////////////////////////////////////////////

void OBBShape::setDirections(const Vector2& q, float r, Agents::PrefVelocity& directions) const {
  // First rotate so that the OBB is an AABB then use the same logic as
  //  with the AABB
  Vector2 disp = q - _pivot;
  // Rotation from world to geometry: R = [Bx By]^T.
  Vector2 Bx = getXBasis();
  Vector2 By = getYBasis();
  // the LOCAL x- and y- coordinates of the nearest point, initialized to the
  //  local value of the query point.
  float X = Bx * disp;
  float Y = By * disp;

  // based on the voronoi regions of the AABB
  //   (-1,1)  |        (0,1)        |  (1,1)
  // ----------------------------------------
  //           |                     |
  //   (-1, 0) |        (0,0)        |  (1,0)
  //           |                     |
  // ----------------------------------------
  //   (-1,-1) |       (0,-1)        |  (1,-1)
  //

  // the x- and y-coordintes of the left and right directions
  float xL, xR, yL, yR;

  // determine quadrant
  const float D = 2.f * r;
  const float minX = r;
  const float maxX = _size._x - r;
  const int xCoord = (int)(X > maxX) - (int)(X < minX);
  const float minY = r;
  const float maxY = _size._y - r;
  const int yCoord = (int)(Y > maxY) - (int)(Y < minY);

  if (xCoord == 0 && yCoord == 0) {
    // inside the region already
    directions.setSingle(Vector2(0.f, 0.f));
    directions.setTarget(q);
  } else {
    int dimensions = 2;
    if (_size._x < D) {
      X = _size._x * 0.5f;
      xL = X;
      xR = X;
      --dimensions;
    } else {
      int LMaxXtest = (yCoord == 1) | ((xCoord == 1) & (yCoord == 0));
      xL = maxX * LMaxXtest + (1 - LMaxXtest) * minX;
      int RMaxXtest = (yCoord == -1) | ((xCoord == 1) & (yCoord == 0));
      xR = maxX * RMaxXtest + (1 - RMaxXtest) * minX;
      X = (xCoord == -1) * minX + (xCoord == 1) * maxX + (xCoord == 0) * X;
    }
    if (_size._y < D) {
      Y = _size._y * 0.5f;
      yL = Y;
      yR = Y;
      --dimensions;
    } else {
      int LMaxYtest = (xCoord == -1) | ((xCoord == 0) & (yCoord == 1));
      yL = maxY * LMaxYtest + (1 - LMaxYtest) * minY;
      int RMaxYtest = (xCoord == 1) | ((xCoord == 0) & (yCoord == 1));
      yR = maxY * RMaxYtest + (1 - RMaxYtest) * minY;
      Y = (yCoord == -1) * minY + (yCoord == 1) * maxY + (yCoord == 0) * Y;
    }

    Vector2 localPt(X, Y);
    // Rotation from geometry to world is: R = [Bx, By]
    Vector2 rot0(Bx._x, By._x);
    Vector2 rot1(Bx._y, By._y);
    Vector2 targetPt(_pivot + Vector2(localPt * rot0, localPt * rot1));
    directions.setTarget(targetPt);
    Vector2 prefDir(norm(targetPt - q));
    if (dimensions) {
      // there is actually a span
      localPt.set(xL, yL);
      Vector2 leftPt(_pivot + Vector2(localPt * rot0, localPt * rot1));
      localPt.set(xR, yR);
      Vector2 rightPt(_pivot + Vector2(localPt * rot0, localPt * rot1));
      directions.setSpan(norm(leftPt - q), norm(rightPt - q), prefDir);
    } else {
      directions.setSingle(prefDir);
    }
  }
}

/////////////////////////////////////////////////////////////////////

Vector2 OBBShape::getTargetPoint(const Vector2& q, float r) const {
  // First rotate so that the OBB is an AABB then use the same logic as
  //  with the AABB
  Vector2 disp = q - _pivot;
  // Rotation from world to geometry: R = [Bx By]^T.
  Vector2 Bx = getXBasis();
  Vector2 By = getYBasis();
  // the LOCAL x- and y- coordinates of the nearest point, initialized to the
  //  local value of the query point.
  float X = Bx * disp;
  float Y = By * disp;

  // based on the voronoi regions of the AABB
  //   0 |         1          |  2
  // ----------------------------------------
  //     |                    |
  //   3 |         4          |  5
  //     |                    |
  // ----------------------------------------
  //   6 |         7          |  8
  //
  //  Approximated by simply offsetting the sides by r.

  const float D = 2.f * r;

  if (_size._x < D) {
    X = _size._x * 0.5f;
  } else {
    float minX = r;
    float maxX = _size._x - r;
    if (X < minX) {
      X = minX;
    } else if (X > maxX) {
      X = maxX;
    }
  }
  if (_size._y < D) {
    Y = _size._y * 0.5f;
  } else {
    float minY = r;
    float maxY = _size._y - r;
    if (Y < minY) {
      Y = minY;
    } else if (Y > maxY) {
      Y = maxY;
    }
  }

  Vector2 localPt(X, Y);
  // Rotation from geometry to world is: R = [Bx, By]
  Vector2 rot0(Bx._x, By._x);
  Vector2 rot1(Bx._y, By._y);

  return _pivot + Vector2(localPt * rot0, localPt * rot1);
}

/////////////////////////////////////////////////////////////////////

Vector2 OBBShape::getCentroid() const {
  return _pivot + getXBasis() * (_size.x() * 0.5f) + getYBasis() * (_size.y() * 0.5f);
}

/////////////////////////////////////////////////////////////////////

Vector2 OBBShape::convertToWorld(const Vector2& r_GP) const {
  Vector2 Bx = getXBasis();
  Vector2 By = getYBasis();
  Vector2 r_GP_W(r_GP * Vector2(Bx._x, By._x), r_GP * Vector2(Bx._y, By._y));
  return _pivot + r_GP_W;
}

/////////////////////////////////////////////////////////////////////

Vector2 OBBShape::convertToGeometry(const Vector2& r_WP) const {
  Vector2 r_GP_W = r_WP - _pivot;
  return Vector2(r_GP_W * getXBasis(), r_GP_W * getYBasis());
}

/////////////////////////////////////////////////////////////////////
//                   Implementation of XML Parsing
/////////////////////////////////////////////////////////////////////

Geometry2D* createGeometry(TiXmlElement* node, const std::string& prefix) {
  std::string fullName = prefix + "shape";
  const char* val = node->Attribute(fullName.c_str());
  if (val) {
    std::string shapeName(val);
    std::transform(shapeName.begin(), shapeName.end(), shapeName.begin(), ::tolower);
    if (shapeName == "point") {
      return createPoint(node, prefix);
    } else if (shapeName == "circle") {
      return createCircle(node, prefix);
    } else if (shapeName == "aabb") {
      return createAABB(node, prefix);
    } else if (shapeName == "obb") {
      return createOBB(node, prefix);
    } else {
      logger << Logger::ERR_MSG
             << "Attemtped to read shape attributes from a tag "
                "but the shape type was not recognized: "
             << val << ".\n";
    }
  } else {
    logger << Logger::ERR_MSG
           << "Attempted to read shape attributes from a tag but "
              "didn't find the shape declaration: \" "
           << fullName << "\" on line " << node->Row() << ".\n";
  }
  return 0x0;
}

/////////////////////////////////////////////////////////////////////

PointShape* createPoint(TiXmlElement* node, const std::string& prefix) {
  bool valid = true;
  FloatAttribute attrX(prefix + "x", true, 0.f);
  if (!attrX.extract(node)) {
    logger << Logger::ERR_MSG << "Missing \"x\" value from point definition on line ";
    logger << node->Row() << "\n";
    valid = false;
  }
  FloatAttribute attrY(prefix + "y", true, 0.f);
  if (!attrY.extract(node)) {
    logger << Logger::ERR_MSG << "Missing \"y\" value from point definition on line ";
    logger << node->Row() << "\n";
    valid = false;
  }
  if (valid) {
    return new PointShape(Vector2(attrX.getFloat(), attrY.getFloat()));
  }
  return 0x0;
}

/////////////////////////////////////////////////////////////////////

CircleShape* createCircle(TiXmlElement* node, const std::string& prefix) {
  bool valid = true;
  FloatAttribute attrX(prefix + "x", true, 0.f);
  if (!attrX.extract(node)) {
    logger << Logger::ERR_MSG << "Missing \"x\" value from circle definition on line ";
    logger << node->Row() << "\n";
    valid = false;
  }
  FloatAttribute attrY(prefix + "y", true, 0.f);
  if (!attrY.extract(node)) {
    logger << Logger::ERR_MSG << "Missing \"y\" value from circle definition on line ";
    logger << node->Row() << "\n";
    valid = false;
  }
  FloatAttribute radius(prefix + "radius", true, 0.f);
  if (!radius.extract(node)) {
    logger << Logger::ERR_MSG << "Missing \"radius\" value from circle definition on ";
    logger << "line " << node->Row() << "\n";
    valid = false;
  }
  if (valid) {
    return new CircleShape(Vector2(attrX.getFloat(), attrY.getFloat()), radius.getFloat());
  }
  return 0x0;
}

/////////////////////////////////////////////////////////////////////

AABBShape* createAABB(TiXmlElement* node, const std::string& prefix) {
  bool valid = true;
  FloatAttribute minX(prefix + "min_x", true, 0.f);
  if (!minX.extract(node)) {
    logger << Logger::ERR_MSG;
    logger << "Missing \"min_x\" value from AABB definition on line ";
    logger << node->Row() << "\n";
    valid = false;
  }
  FloatAttribute minY(prefix + "min_y", true, 0.f);
  if (!minY.extract(node)) {
    logger << Logger::ERR_MSG;
    logger << "Missing \"min_y\" value from AABB definition on line ";
    logger << node->Row() << "\n";
    valid = false;
  }
  FloatAttribute maxX(prefix + "max_x", true, 0.f);
  if (!maxX.extract(node)) {
    logger << Logger::ERR_MSG;
    logger << "Missing \"max_x\" value from AABB definition on line ";
    logger << node->Row() << "\n";
    valid = false;
  }
  FloatAttribute maxY(prefix + "max_y", true, 0.f);
  if (!maxY.extract(node)) {
    logger << Logger::ERR_MSG;
    logger << "Missing \"max_y\" value from AABB definition on line ";
    logger << node->Row() << "\n";
    valid = false;
  }
  if (valid) {
    return new AABBShape(Vector2(minX.getFloat(), minY.getFloat()),
                         Vector2(maxX.getFloat(), maxY.getFloat()));
  }
  return 0x0;
}

/////////////////////////////////////////////////////////////////////

OBBShape* createOBB(TiXmlElement* node, const std::string& prefix) {
  bool valid = true;
  FloatAttribute x(prefix + "x", true, 0.f);
  if (!x.extract(node)) {
    logger << Logger::ERR_MSG << "Missing \"x\" value from OBB definition on line ";
    logger << node->Row() << "\n";
    valid = false;
  }
  FloatAttribute y(prefix + "y", true, 0.f);
  if (!y.extract(node)) {
    logger << Logger::ERR_MSG << "Missing \"y\" value from OBB definition on line ";
    logger << node->Row() << "\n";
    valid = false;
  }
  FloatAttribute width(prefix + "width", true, 0.f);
  if (!width.extract(node)) {
    logger << Logger::ERR_MSG;
    logger << "Missing \"width\" value from OBB definition on line ";
    logger << node->Row() << "\n";
    valid = false;
  }
  FloatAttribute height(prefix + "height", true, 0.f);
  if (!height.extract(node)) {
    logger << Logger::ERR_MSG;
    logger << "Missing \"height\" value from OBB definition on line ";
    logger << node->Row() << "\n";
    valid = false;
  }
  FloatAttribute angle(prefix + "angle", true, 0.f);
  if (!angle.extract(node)) {
    logger << Logger::ERR_MSG;
    logger << "Missing \"angle\" value from OBB definition on line ";
    logger << node->Row() << "\n";
    valid = false;
  }
  if (valid) {
    return new OBBShape(Vector2(x.getFloat(), y.getFloat()),
                        Vector2(width.getFloat(), height.getFloat()),
                        angle.getFloat() * DEG_TO_RAD);
  }
  return 0x0;
}

}  // namespace Math
}  // namespace Menge
