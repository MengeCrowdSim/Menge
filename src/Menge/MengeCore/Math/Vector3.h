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
 @file    Vector3.h
 @brief    Definition of a vector in R3
 */

#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#ifdef _MSC_VER
// To export templated classes and functions, requires declaring specializations
// extern.  This became standard in C++11, but prior to that it is considered a
//  "microsoft extension".  This silences the warnings.
#pragma warning(disable : 4231)
#endif

#include "MengeCore/CoreConfig.h"
#include "MengeCore/Math/consts.h"
#include "MengeCore/Runtime/Logger.h"

#include <cassert>
#include <cmath>
#include <ostream>

namespace Menge {

namespace Math {

// forward declaration
template <class Type>
class Vector3d;

template <class Type>
inline MENGE_API Type abs(const Vector3d<Type>& v);

/*!
 @brief    Templated vector in R3
 */
template <class Type>
class MENGE_API Vector3d {
 public:
  /////////////////////////////    CONSTRUCTORS      ////////////////////////////
  /*!
   @brief    Default constructor.  It does NOT initialize the fields
   */
  inline Vector3d() {}

  /*!
   @brief    Constructor with arguments

   @param    x    The vector's x-component.
   @param    y    The vector's y-component.
   @param    z    The vector's z-component.
   */
  inline Vector3d(Type x, Type y, Type z) : _x(x), _y(y), _z(z) {}

  /*!
   @brief    Copy constructor

   @param    v    The vector to copy from.
   */
  inline Vector3d(const Vector3d& v) : _x(v._x), _y(v._y), _z(v._z) {}

  /////////////////////////////    GETTERS/SETTERS      ////////////////////////////

  /*!
   @brief    Get the x-value

   @returns  The x-value.
   */
  inline Type x() const { return _x; }

  /*!
   @brief    Get the y-value

   @returns  The y-value.
   */
  inline Type y() const { return _y; }

  /*!
   @brief    Get the z-value

   @returns  The z-value.
   */
  inline Type z() const { return _z; }

  /*!
   @brief    Set the x- and y-values from scalar values

   @param    x    The value for the x-component
   @param    y    The value for the y-component
   @param    z    The value for the z-component
   */
  inline void set(Type x, Type y, Type z) {
    _x = x;
    _y = y;
    _z = z;
  }

  /*!
   @brief    Set the x-, y- and z-values from a vector

   @param    v    The vector containing the  newvalues
   */
  inline void set(const Vector3d& v) {
    _x = v._x;
    _y = v._y;
    _z = v._z;
  }

  /*!
   @brief    Set the x-value

   @param    x    The x-value.
   */
  inline void setX(Type x) { _x = x; }

  /*!
   @brief    Set the y-value

   @param    y    The y-value.
   */
  inline void setY(Type y) { _y = y; }

  /*!
   @brief    Set the z-value

   @param    z    The z-value.
   */
  inline void setZ(Type z) { _z = z; }

  /*!
   @brief    Index-style access to vector components

   @param    i    The index of the desired component. Should be in the range [0, 2].
   @returns  The ith component of the vector.
   */
  inline Type operator[](const int i) const {
    assert(i >= 0 && i <= 2 && "Invalid index for Vector3");
    return i == 0 ? _x : (i == 1 ? _y : _z);
  }

  /*!
   @brief    Index-style access to vector components as a reference

   @param    i    The index of the desired component. Should be in the range [0, 2].
   @returns  A reference to the ith component of the vector.
   */
  inline Type& operator[](const int i) {
    assert(i >= 0 && i <= 2 && "Invalid index for Vector3");
    return i == 0 ? _x : (i == 1 ? _y : _z);
  }

  /*!
   @brief    Set the vector to zero
   */
  inline void zero() { _x = _y = _z = (Type)0; }

  /////////////////////////////    MATH OPERATIONS      ////////////////////////////

  /*!
   @brief    Vector negation.
   
   Creates a new vector which is the negation of this vector

   @returns  The vector in the opposite direction of this vector
   */
  inline Vector3d<Type> operator-() const { return Vector3d<Type>(-_x, -_y, -_z); }

  /*!
   @brief    Computes the dot product of this vector with the given vector.

   @param    v    The with which the dot product should be computed.
   @returns    The dot product of this vector with the given vector
   */
  inline float operator*(const Vector3d& v) const { return _x * v._x + _y * v._y + _z * v._z; }

  /*!
   @brief    Computes the scalar multiplication of this vector with the given scalar value.

   @param    s     The scalar value
   @returns    The scalar multiplication of this vector with a specified scalar value.
   */
  inline Vector3d<Type> operator*(float s) const { return Vector3d<Type>(_x * s, _y * s, _z * s); }

  /*!
   @brief    Computes the scalar division of this vector with the given scalar value.

   @param    s     The scalar value
   @returns    The scalar division of this vector with a specified scalar value.
   */
  inline Vector3d<Type> operator/(float s) const {
    const float invS = 1.f / s;
    return Vector3d<Type>(_x * invS, _y * invS, _z * invS);
  }

  /*!
   @brief    Computes the vector sum of this vector with the given vector.

   @param    v        The second operand.
   @returns    The vector sum of this vector with the given vector.
   */
  inline Vector3d operator+(const Vector3d& v) const {
    return Vector3d<Type>(_x + v._x, _y + v._y, _z + v._z);
  }

  /*!
   @brief    Computes the vector difference of this vector with the given vector.

   @param    v        The second operand.
   @returns    The vector difference of this vector with the given vector.
   */
  inline Vector3d operator-(const Vector3d& v) const {
    return Vector3d<Type>(_x - v._x, _y - v._y, _z - v._z);
  }

  /*!
   @brief    Reports if this vector is the same as the given vector.

   @param    v        The second operand.
   @returns    True if the vectors are exactly identical.
   */
  inline bool operator==(const Vector3d& v) const { return _x == v._x && _y == v._y && _z == v._z; }

  /*!
   @brief    Reports if this vector is the different from the given vector.

   @param    v        The second operand.
   @returns    True if the vectors are at all different.
   */
  inline bool operator!=(const Vector3d& v) const { return _x != v._x || _y != v._y || _z != v._z; }

  /////////////////////////////    IN-PLACE MATH OPERATIONS      ////////////////////////

  /*!
   @brief    Perform in-place scalar multiplication on this vector.

   @param    s      The scalar mutliplicand.
   @return    A reference to the vector.
   */
  inline Vector3d& operator*=(float s) {
    _x *= s;
    _y *= s;
    _z *= s;
    return *this;
  }

  /*!
   @brief    Perform in-place scalar division on this vector.

   @param    s      The scalar divisor.
   @return    A reference to the vector.
   */
  inline Vector3d& operator/=(float s) {
    const float invS = 1.f / s;
    _x *= invS;
    _y *= invS;
    _z *= invS;
    return *this;
  }

  /*!
   @brief    Perform in-place vector addition on this vector.

   @param    v      The second vector operand.
   @return    A reference to the vector.
   */
  inline Vector3d& operator+=(const Vector3d& v) {
    _x += v._x;
    _y += v._y;
    _z += v._z;
    return *this;
  }

  /*!
   @brief    Perform in-place vector subtraction on this vector.

   @param    v      The second vector operand.
   @return    A reference to the vector.
   */
  inline Vector3d& operator-=(const Vector3d& v) {
    _x -= v._x;
    _y -= v._y;
    _z -= v._z;
    return *this;
  }

  /*!
   @brief    Negate the vector in place
   */
  inline void negate() {
    _x = -_x;
    _y = -_y;
    _z = -_z;
  }

  /*!
   @brief    Normalize the vector in place
   */
  inline void normalize() {
    float len = sqrtf(_x * _x + _y * _y + _z * _z);
    if (len > EPS) {
      _x /= len;
      _y /= len;
      _z /= len;
    } else {
      _x = _y = _z = 0.f;
    }
  }

  /*!
   @brief    Adds in a scaled version of another vector: `this += s * v`

   @param    s      The scale factor for the vector v
   @param    v      The vector to scale
   */
  inline void SumScale(Type s, const Vector3d& v) {
    _x += v._x * s;
    _y += v._y * s;
    _z += v._z * s;
  }

  /////////////////////////////    ROTATION OPERATIONS      ////////////////////////////

  /*!
   @brief    Rotate the vector around the x axis

   @param    angle    The amoutn of rotations (in radians)
   @returns  The rotated vector
   */
  Vector3d rotateX(float angle) const {
    float newY = _y, newZ = _z;
    rotatePair(angle, &newY, &newZ);
    return Vector3d(_x, newY, newZ);
  }

  /*!
   @brief    Rotate the vector around the y axis

   @param    angle    The amoutn of rotations (in radians)
   @returns  The rotated vector
   */
  Vector3d rotateY(float angle) const {
    float newX = _x, newZ = _z;
    rotatePair(angle, &newX, &newZ);
    return Vector3d(newX, _y, newZ);
  }

  /*!
   @brief    Rotate the vector around the z axis

   @param    angle    The amoutn of rotations (in radians)
   @returns  The rotated vector
   */
  Vector3d rotateZ(float angle) const {
    float newX = _x, newY = _y;
    rotatePair(-angle, &newX, &newY);
    return Vector3d(newX, newY, _z);
  }

  /*!
   @brief    Rotate the vector around an arbitrary vector

   @param    angle    The amoutn of rotations (in radians)
   @param    v        The axis of rotation - it should be normalized
   @returns  The rotated vector
   */
  Vector3d rotateV(Type angle, const Vector3d& v) const {
    assert(abs(v) > 0.999 && abs(v) < 1.001);
    Type c = cos(angle);
    Type s = sin(angle);
    Type omc = 1 - c;
    // This is a hack -- it should be done with a matrix.....
    Type vx = _x * (v._x * v._x * omc + c) + _y * (v._y * v._x * omc + v._z * s) +
              _z * (v._z * v._x * omc - v._y * s);
    Type vy = _x * (v._x * v._y * omc - v._z * s) + _y * (v._y * v._y * omc + c) +
              _z * (v._z * v._y * omc + v._x * s);
    Type vz = _x * (v._x * v._z * omc + v._y * s) + _y * (v._y * v._z * omc - v._x * s) +
              _z * (v._z * v._z * omc + c);
    return Vector3d(vx, vy, vz);
  }

  /*!
   @brief    Rotate the vector around an arbitrary vector - change the vector in place

   @param    angle    The amoutn of rotations (in radians)
   @param    v        The axis of rotation - it should be normalized
   */
  void rotateV_ip(Type angle, const Vector3d& v) {
    assert(abs(v) > 0.999 && abs(v) < 1.001);
    Type c = cos(angle);
    Type s = sin(angle);
    Type omc = 1 - c;
    // This is a hack -- it should be done with a matrix.....
    Type vx = _x * (v._x * v._x * omc + c) + _y * (v._y * v._x * omc + v._z * s) +
              _z * (v._z * v._x * omc - v._y * s);
    Type vy = _x * (v._x * v._y * omc - v._z * s) + _y * (v._y * v._y * omc + c) +
              _z * (v._z * v._y * omc + v._x * s);
    Type vz = _x * (v._x * v._z * omc + v._y * s) + _y * (v._y * v._z * omc - v._x * s) +
              _z * (v._z * v._z * omc + c);
    _x = vx;
    _y = vy;
    _z = vz;
  }

  /////////////////////////////    GEOMETRIC OPERATIONS      ////////////////////////////

  /*!
   @brief    Compute the magnitude (aka length) of the vector

   @returns  The magnitude of the vector.
   */
  inline Type Length() const { return sqrt(_x * _x + _y * _y + _z * _z); }

  /*!
   @brief    Cross product of this vector with the given vector.

   @param    v    The second opearand.
   @returns  The vector representing the cross product of this with v.
   */
  inline Vector3d cross(const Vector3d<Type>& v) {
    return Vector3d<Type>(_y * v._z - v._y * _z, _z * v._x - _x * v._z, _x * v._y - _y * v._x);
  }

  /*!
   @brief    Compute the distance from this vector to another point.

   @param    p      The point whose distance from this vector (interpreted as a point) is to be
                  computed.
   @returns  The distance between this vector and p.
   */
  inline float distance(const Vector3d& p) const {
    float dx = _x - p._x;
    float dy = _y - p._y;
    float dz = _z - p._z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
  }

  /*!
   @brief    Compute the distance from this vector to another point.

   @param    x      The x-value of the other point.
   @param    y      The y-value of the other point.
   @param    z      The z-value of the other point.
   @returns  The distance between this vector and (x, y).
   */
  inline float distance(float x, float y, float z) const {
    float dx = _x - x;
    float dy = _y - y;
    float dz = _z - z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
  }

  /*!
   @brief    Compute the squared-distance from this vector to another point.

   @param    p      The point whose distance from this vector (interpreted as a point) is to be
                  computed.
   @returns  The squared-distance between this vector and p.
   */
  inline float distanceSq(const Vector3d& p) const {
    float dx = _x - p._x;
    float dy = _y - p._y;
    float dz = _z - p._z;
    return dx * dx + dy * dy + dz * dz;
  }

  /*!
   @brief    Compute the squared-distance from this vector to another point

   @param    x      The x-value of the other point.
   @param    y      The y-value of the other point.
   @param    z      The z-value of the other point.
   @returns  The squared-distance between this vector and (x, y).
   */
  inline float distanceSq(float x, float y, float z) const {
    float dx = _x - x;
    float dy = _y - y;
    float dz = _z - z;
    return dx * dx + dy * dy + dz * dz;
  }

  /*!
   @brief    x-component of the vector
   */
  Type _x;

  /*!
   @brief    y-component of the vector
   */
  Type _y;

  /*!
   @brief    z-component of the vector
   */
  Type _z;

 private:
  /*!
   @brief    Rotate a 2D vector.

   @param    angle    The amount of rotation (in radians).
   @param    x      The x-component of the vector.
   @param    y      The y-component of the vector.
   */
  void rotatePair(float angle, float* x, float* y) const {
    float c = cos(angle);
    float s = sin(angle);
    float newX = c * (*x) + s * (*y);
    float newY = c * (*y) - s * (*x);
    *x = newX;
    *y = newY;
  }
};

/*!
 @brief    Explicit specialization for shared library export.
 @internal
 */
MATHEXTERN template class MENGE_API Vector3d<float>;

/*!
 @brief    Declaration of a floating point vector in R3.
 */
typedef Vector3d<float> Vector3;

/////////////////////////////    UTILITY FUNCTIONS      ////////////////////////////

/*!
 @brief      Computes the left scalar multiplication of a scalar and 2d vector.

 @param     s    The scalar value with which the scalar multiplication should be computed.
 @param     v    The two-dimensional vector with which the scalar multiplication should be computed.
 @returns   The scalar multiplication of the two-dimensional vector with the scalar value.
 */
template <class Type>
inline MENGE_API Vector3d<Type> operator*(Type s, const Vector3d<Type>& v) {
  return Vector3d<Type>(s * v._x, s * v._y, s * v._z);
}

/*!
 @brief    Explicit specialization for shared library export.
 @internal
 */
MATHEXTERN template MENGE_API Vector3d<float> operator*(float s, const Vector3d<float>& v);

/*!
 @brief      Inserts the specified two-dimensional vector into the logger.

 @param      logger   The logger into which the two-dimensional vector should be inserted.
 @param      v        The two-dimensional vector which to insert into the output stream.
 @returns    A reference to the output stream.
 */
template <class Type>
inline MENGE_API Logger& operator<<(Logger& logger, const Vector3d<Type>& v) {
  logger << "(" << v.x() << "," << v.y() << ", " << v.z() << ")";
  return logger;
}

/*!
 @brief      Inserts the specified two-dimensional vector into the specified output stream.

 @param      os     The output stream into which the two-dimensional vector should be inserted.
 @param      v      The two-dimensional vector which to insert into the output stream.
 @returns    A reference to the output stream.
 */
template <class Type>
inline MENGE_API std::ostream& operator<<(std::ostream& os, const Vector3d<Type>& v) {
  os << "(" << v.x() << "," << v.y() << ", " << v.z() << ")";
  return os;
}

/*!
 @brief    Explicit specialization for shared library export.
 @internal
 */
MATHEXTERN template MENGE_API Logger& operator<<(Logger& os, const Vector3d<float>& v);

/*!
 @brief      Computes the length of a specified two-dimensional vector.

 @param      v    The two-dimensional vector whose length is to be computed.
 @returns    The length of the two-dimensional vector.
 */
template <class Type>
inline MENGE_API Type abs(const Vector3d<Type>& v) {
  return std::sqrt(v * v);
}

/*!
 @brief    Explicit specialization for shared library export.
 @internal
 */
MATHEXTERN template MENGE_API float abs(const Vector3d<float>& v);

/*!
 @brief      Computes the squared length of a specified two-dimensional vector.

 @param      v        The two-dimensional vector whose squared length is to be computed.
 @returns    The squared length of the two-dimensional vector.
 */
template <class Type>
inline MENGE_API Type absSq(const Vector3d<Type>& v) {
  return v * v;
}

/*!
 @brief    Explicit specialization for shared library export.
 @internal
 */
MATHEXTERN template MENGE_API float absSq(const Vector3d<float>& v);

/*!
 @brief      Computes the normalization of the specified two-dimensional
 vector.

 @param      vector          The two-dimensional vector whose normalization is to be computed.
 @returns    The normalization of the two-dimensional vector.
 */
template <class Type>
inline MENGE_API Vector3d<Type> norm(const Vector3d<Type>& vector) {
  Type mag = abs(vector);
  if (mag < EPS) {
    // This may not be the "right" behavior.  I do this because the "normalized" vector
    //  has unit length.  This guarantees that the result is always unit length.
    //  Although it introduces otherissues.
    return Vector3d<Type>(1.f, 0.f, 0.f);
  } else {
    return vector / mag;
  }
}

/*!
 @brief    Explicit specialization for shared library export.
 @internal
 */
MATHEXTERN template MENGE_API Vector3d<float> norm(const Vector3d<float>& vector);

/*!
 @brief  Determines if two vectors are equal to within a threshhold

 @param  v1          The first of two-dimensional vectors to be compared.
 @param  v2          The second of two-dimensional vectors to be compared.
 @param  threshSqd    The squared threshhold to test against defaults to 3 decimal places (0.001)^2.
 @returns  A boolean: true if they are equivalent within a threshhold.
 */
template <class Type>
inline MENGE_API bool equivalent(const Vector3d<Type>& v1, const Vector3d<Type>& v2,
                                 float threshSqd = 0.000001f) {
  return absSq(v1 - v2) < threshSqd;
}

/*!
 @brief    Explicit specialization for shared library export.
 @internal
 */
MATHEXTERN template MENGE_API bool equivalent(const Vector3d<float>& v1, const Vector3d<float>& v2,
                                              float threshSqd);

// Possible operations
//  Normal to a trio of points (i.e. normal of triangle)
// Optimized math operations
//    add in scaled vector
//    scale this add in scaled vector
// Getter/setter from Type pointer
// Linear interpolation
// reflection of one vector around another
// Compute plane from three points

}  // namespace Math
}  // namespace Menge
#ifdef _MSC_VER
   // To export templated classes and functions, requires declaring specializations
// extern.  This became standard in C++11, but prior to that it is considered a
//  "microsoft extension".  This silences the warnings.
#pragma warning(default : 4231)
#endif

#endif
