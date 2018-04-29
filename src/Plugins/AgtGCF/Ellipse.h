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
#ifndef __ELLIPSE_H__
#define __ELLIPSE_H__

#include "MengeCore/Math/vector.h"

// forward declarations
namespace Menge {
namespace Agents {
class Obstacle;
}
}  // namespace Menge

/*!
 Ellipse.h
 Definition of ellipse geometry and queries
 */

namespace GCF {
/*!
 @brief      Definition of an ellipse.
 */
class Ellipse {
 public:
  /*!
   @brief		Basic constructor with default values. By default, the Ellipse is a circle, centered on
            the origin.
   */
  Ellipse();

  /*!
   @brief		Constructor with ellipse center definition.
   @param		center		The position of the ellipse's center in R2.
   */
  Ellipse(const Menge::Math::Vector2& center);

  /*!
   @brief		Constructor with ellipse center definition and axes.
   @param		center		The position of the ellipse's center in R2.
   @param		axes		The semi-major and semi-minor axis values stored in a two-dimensional vector.
   */
  Ellipse(const Menge::Math::Vector2& center, const Menge::Math::Vector2& axes);

  /*!
   @brief		Constructor with ellipse center definition, axes, and orientation.
   @param		center		The position of the ellipse's center in R2.
   @param		axes		  The semi-major and semi-minor axis values stored in a two-dimensional vector.
   @param		angle		  The angle (in radians) defining the orientation of the *ellipse.
   */
  Ellipse(const Menge::Math::Vector2& center, const Menge::Math::Vector2& axes, float angle);

  /*!
   @brief		Compute the distance between the centers of this ellipse and the provided ellipse.
   @param		other		The other ellipse
   @return		The center distance.
   */
  inline float ellipseCenterDistance(const Ellipse& other) const {
    return abs(other._center - _center);
  }

  /*!
   @brief		Computes the displacement from this ellipse's center to the other ellipse's center.
   @param		other		The other ellipse
   @return		The center displacement.
   */
  inline Menge::Math::Vector2 ellipseCenterDisplace(const Ellipse& other) const {
    return _center - other._center;
  }

  /*!
   @brief		Compute the distance of closest approach between two ellipses.
   @param		other		The other ellipse
   @return		The distance of closest approach.
   */
  float distanceOfClosestApproach(const Ellipse& other) const;

  /*!
   @brief		Compute the effective distance between this ellipse and the provided ellipse.

   Effective distance is the distance between two ellipse boundaries along the line that connects
   the ellipse centers.

   @param		other		The other ellipse
   @return	The effective distance.
   */
  float approxDistanceOfClosestApproach(const Ellipse& other) const;

  /*!
   @brief		Computes the closest point on the ellipse to a point. Also reports direction.
   @param		pt			The point to which we want the closest point.
   @return		The point on the ellipse that was closest.
   */
  Menge::Math::Vector2 closestPoint(const Menge::Math::Vector2& pt) const;

  /*!
   @brief		Computes the minimum distance between the obstacle (a line segment) and the ellipse.
   
   Also provides the direction of that minimum displacement.

   @param		obstacle		A pointer to the obstacle to test
   @param		dir				  The direction of minimum displacment is set into this parameter.
   @return	The distance (in dir) from the nearest point on the obstacle to the ellipse.
   */
  float minimumDistance(const Menge::Agents::Obstacle* obstacle, Menge::Math::Vector2& dir) const;

  /*!
   @brief		Computes the distance of closest approach.

   It is the distance between the center and the line after the obstacle has been moved along its
   normal until it makes contact. The obstacle is interpreted as an INFINITE line.

   @param		line		The line segment obstacle to compute against.
   @return	float representing the "distance of closest approach"
   */
  float distanceOfClosestApproach(const Menge::Agents::Obstacle* line) const;

  /*!
   @brief		Computes the approximate distance from point p to the ellipse.

   This is computed by finding the intersection of a line formed by the point and the origin with
   the ellipse. The distance is to that point. This is *not* the *actual* closest distance.

   @param		pt		The point whose distance is to be computed.
   @returns	A positive value if pt is outside the ellipse and a negative value if the point lies
            inside the ellipse.
   */
  float approximateMinimumDistance(const Menge::Math::Vector2& pt) const;

  /*!
   @brief		Computes the radius of the ellipse in the direction of the point.
   @param		pt		The point used to define the direction. Should be different from the ellipse
                  center and, ideally, outside the ellipse.
   @returns	The length of the radius in this direction
   */
  float radiusInPointDirection(const Menge::Math::Vector2& pt) const;

  /*!
   @brief		Computes the radius of the ellipse in the given direction.

   @param		dir		The direction from the ellipse center to determine the radius.
   @returns	The length of the radius in this direction
   */
  float radiusInDirection(const Menge::Math::Vector2& dir) const;

  /*!
   @brief		Translates the point into ellipse-space coordinates i.e. the transformation necessary to
            place the ellipse at the center, with its axes aligned with the world axes.
   @param		pt		The point to translate.
   @returns	The translated point.
   */
  Menge::Math::Vector2 toEllipseSpace(const Menge::Math::Vector2& pt) const;

  /*!
   @brief		Translates the point from ellipse-space coordinates into world coordinates.
   @param		pt		The point to translate.
   @returns	The translated point.
   */
  Menge::Math::Vector2 fromEllipseSpace(const Menge::Math::Vector2& pt) const;

  /*!
   @brief		Sets the orientation of the ellipse
   @param		angle		Rotation from x-axis in radians.
   */
  inline void setOrientation(float angle) {
    _cosPhi = cos(angle);
    _sinPhi = sin(angle);
  }

  /*!
   @brief		Sets the orientation of the ellipse
   @param		dir		A unit-length vector pointing in the direction
   */
  inline void setOrientation(const Menge::Math::Vector2& dir) {
    _cosPhi = dir.x();
    _sinPhi = dir.y();
  }

  /*!
   @brief		Returns the "facing" direction of the ellipse
   @returns	The ellipse's facing direction.
   */
  inline Menge::Math::Vector2 getOrientation() const {
    return Menge::Math::Vector2(_cosPhi, _sinPhi);
  }

  /*!
   @brief		Set center of the ellipse.
   @param		pos			The two-dimensional vector specifying center position.
   */
  inline void setCenter(const Menge::Math::Vector2& pos) { _center.set(pos); }

  /*!
   @brief		Returns the center of the ellipse.
   @returns	A const reference to the ellipse's center.
   */
  inline const Menge::Math::Vector2& getCenter() const { return _center; }

  /*!
   @brief		Sets the major and minor axes of the ellipse.
   @param		axes		A two dimensional vector containg the major and minor axis sizes in the x and y
                    values, respectively.
   */
  inline void setAxes(const Menge::Math::Vector2& axes) {
    _majorAxis = axes.x();
    _minorAxis = axes.y();
  }

  /*!
   @brief		Sets the major and minor axes of the ellipse.
   @param		major		The value of the semi-major axis.
   @param		minor		The value of the semi-minor axis.
   */
  inline void setAxes(float major, float minor) {
    _majorAxis = major;
    _minorAxis = minor;
  }

  /*!
   @brief		Sets the major axis of the ellipse.
   @param		length		The value of the semi-major axis.
   */
  inline void setMajorAxis(float length) { _majorAxis = length; }

  /*!
   @brief		Returns the length of the major axis
   @returns	The major axis
   */
  inline float getMajor() const { return _majorAxis; }

  /*!
   @brief		Sets the minor axis of the ellipse.
   @param		length		The value of the semi-minor axis.
   */
  inline void setMinorAxis(float length) { _minorAxis = length; }

  /*!
   @brief		Returns the length of the minor axis
   @returns	The major axis
   */
  inline float getMinor() const { return _minorAxis; }

  /*!
   @brief		Returns the smaller of the axes.
   */
  inline float getSmallerAxis() const { return _minorAxis < _majorAxis ? _minorAxis : _majorAxis; }

  /*!
   @brief		Returns the larger of the axes.
   */
  inline float getLargerAxis() const { return _minorAxis < _majorAxis ? _majorAxis : _minorAxis; }

 protected:
  /*!
   @brief		The center of the ellipse.
   */
  Menge::Math::Vector2 _center;

  /*!
   @brief		Part of the orientation of the ellipse.

   Oriented with angle phi. This is cosine of that angle.
   */
  float _cosPhi;

  /*!
   @brief		Part of the orientation of the ellipse.

   Oriented with angle phi. This is sine of that angle.
   */
  float _sinPhi;

  /*!
   @brief		Length of semi-major axis.

   The semi-major axis is the "direction" the ellipse is oriented. So, when phi is zero, the
   semi-major axis is aligned with the x-axis. It need not be a bigger value than the semi-minor
   axis.
   */
  float _majorAxis;

  /*!
   @brief		Length of semi-minor axis.

   The semi-minor axis is perpendicular to the "direction" the ellipse is oriented. So, when phi is
   zero, the semi-minor axis is aligned with the y-axis. It need not be a smaller value than the
   semi-major axis.
   */
  float _minorAxis;
};
}  // namespace GCF
#endif  //__ELLIPSE_H__
