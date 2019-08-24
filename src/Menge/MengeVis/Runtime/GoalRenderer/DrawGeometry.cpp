#include "MengeVis/Runtime/GoalRenderer/DrawGeometry.h"

#include <memory>

#include "MengeVis/SceneGraph/graphCommon.h"
#include "MengeVis/SceneGraph/shapes.h"

namespace MengeVis {
namespace Runtime {
namespace GoalVis {

using Menge::Math::AABBShape;
using Menge::Math::CircleShape;
using Menge::Math::Geometry2D;
using Menge::Math::PointShape;
using Menge::Math::OBBShape;
using Menge::Math::Vector2;

RigidTransform2::RigidTransform2() : RigidTransform2(Vector2(0.f, 0.f)) {}

RigidTransform2::RigidTransform2(Vector2 pos) : _pos(std::move(pos)) {}

Vector2 RigidTransform2::operator*(const Vector2& v) const {
  return v + _pos;
}

void drawGeometry(const Geometry2D& geometry, const RigidTransform2& X_WG) {
  // Note: this dynamic casting is kind of crap. But since it's only used for visualization,
  // we'll ignore the cost for now.
  if (dynamic_cast<const PointShape*>(&geometry)) {
    drawGeometry(dynamic_cast<const PointShape&>(geometry), X_WG);
  } else if (dynamic_cast<const CircleShape*>(&geometry)) {
    drawGeometry(dynamic_cast<const CircleShape&>(geometry), X_WG);
  } else if (dynamic_cast<const AABBShape*>(&geometry)) {
    drawGeometry(dynamic_cast<const AABBShape&>(geometry), X_WG);
  } else if (dynamic_cast<const OBBShape*>(&geometry)) {
    drawGeometry(dynamic_cast<const OBBShape&>(geometry), X_WG);
  } else {
    // If I have some otherwise unrecognized shape; draw it as a point.
    drawPoint(geometry.getCentroid(), X_WG);
  }
}

void drawPoint(const Vector2& point, const RigidTransform2& X_WG) {
  glBegin(GL_POINTS);
  const Vector2 p = X_WG * point;
  glVertex3f(p.x(), p.y(), 0.f);
  glEnd();
}

void drawGeometry(const PointShape& point, const RigidTransform2& X_WG) {
  drawPoint(point.getCentroid(), X_WG);
}

void drawGeometry(const CircleShape& circle, const RigidTransform2& X_WG) {
  glPushMatrix();
  const Vector2& pos = X_WG * circle.getCenter();
  const float radius = circle.getRadius();
  glTranslatef(pos.x(), pos.y(), 0.f);
  glScalef(radius, radius, radius);
  SceneGraph::Circle::drawUnit();
  glPopMatrix();
}

void drawGeometry(const AABBShape& aabb, const RigidTransform2& X_WG) {
  const Vector2& minPt = X_WG * aabb.getMinPoint();
  const Vector2& size = aabb.getSize();

  glBegin(GL_POLYGON);
  glVertex3f(minPt.x(), minPt.y(), 0.f);
  glVertex3f(minPt.x() + size.x(), minPt.y(), 0.f);
  glVertex3f(minPt.x() + size.x(), minPt.y() + size.y(), 0.f);
  glVertex3f(minPt.x(), minPt.y() + size.y(), 0.f);
  glVertex3f(minPt.x(), minPt.y(), 0.f);
  glEnd();
}

void drawGeometry(const OBBShape& obb, const RigidTransform2& X_WG) {
  const Vector2 size = obb.getSize();
  const Vector2 pivot = obb.getPivot();

  // Compute the four corners in the world frame:
  //
  // c3 ___________ c2
  //   |           |
  //   |___________|
  //   O            c1
  //
  // O = <0, 0, 0>
  // c1 = <w, 0, 0>
  // c2 = <w, h, 0>
  // c3 = <0, h, 0>
  Vector2 c(0.f, 0.f);
  // TODO(curds01): This should be called convertToGeometryFrame() or some such thing. The key is
  // that the geometry is no longer explicitly defined in the *world* frame.
  Vector2 c0 = X_WG * obb.convertToWorld(c);
  c.set(size.x(), 0.f);
  Vector2 c1 = X_WG * obb.convertToWorld(c);
  c.set(size);
  Vector2 c2 = X_WG * obb.convertToWorld(c);
  c.set(0.f, size.y());
  Vector2 c3 = X_WG * obb.convertToWorld(c);

  glBegin(GL_POLYGON);
  glVertex3f(c0.x(), c0.y(), 0.f);
  glVertex3f(c1.x(), c1.y(), 0.f);
  glVertex3f(c2.x(), c2.y(), 0.f);
  glVertex3f(c3.x(), c3.y(), 0.f);
  glEnd();
}

}  // namespace GoalVis
}  // namespace Runtime
}  // namespace MengeVis
