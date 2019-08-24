/*
 Menge Crowd Simulation Framework

 Copyright and trademark 2012-19 University of North Carolina at Chapel Hill

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0
 or
    LICENSE.txt in the root of the Menge repository.

 Any questions or comments should be sent to the authors menge@cs.unc.edu

 <http://gamma.cs.unc.edu/Menge/>
*/

#pragma once

#include "MengeCore/Math/Geometry2D.h"
#include "MengeCore/Math/Matrix.h"

namespace MengeVis {
namespace Runtime {
namespace GoalVis {

// TODO(curds01): This generic R^2 transform should be in MengeCore/Math.

class RigidTransform2 {
 public:
  /** @brief  Constructs the identity transform.  */
  RigidTransform2();

  /** @brief Constructs a strictly translation transform.  */
  RigidTransform2(Menge::Math::Vector2 pos);

  /** @brief  Right multiplies this transform (X_AB) by the point `p_BC` producing the value p_AC --
              the point C measured and expressed in frame A.
   @retval p_AC
   */
  Menge::Math::Vector2 operator*(const Menge::Math::Vector2& p_BC) const;

 private:
  Menge::Math::Vector2 _pos;

  // TDODO(curds01): Support rotations.
};

/** @name   Utilities for drawing shapes to an OpenGl context.

 The following functions provide the ability to draw the given geometric shape in the active
 OpenGL context. Each takes a geometry (whose properties are defined in the geometry's own
 frame) and a transform from the geometry frame G and the world frame W, X_WG.
 */
//@{

/** @brief    The variant that takes a geometry of undetermined type.
 */
void drawGeometry(const Menge::Math::Geometry2D& geometry, const RigidTransform2& X_WG);
void drawPoint(const Menge::Math::Vector2& point, const RigidTransform2& X_WG);
void drawGeometry(const Menge::Math::PointShape& point, const RigidTransform2& X_WG);
void drawGeometry(const Menge::Math::CircleShape& circle, const RigidTransform2& X_WG);
void drawGeometry(const Menge::Math::AABBShape& aabb, const RigidTransform2& X_WG);
void drawGeometry(const Menge::Math::OBBShape& obb, const RigidTransform2& X_WG);

//@}
}  // namespace GoalVis
}  // namespace Runtime
}  // namespace MengeVis
