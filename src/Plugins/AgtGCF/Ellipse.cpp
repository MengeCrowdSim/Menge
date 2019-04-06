#include "Ellipse.h"

#include "MengeCore/Agents/Obstacle.h"
#include "MengeCore/Math/geomQuery.h"

#include <float.h>
#include <cmath>
#include <complex>
#include <iostream>

////////////////////////////////////////////////////////////////
//					Implementation of Helper function
////////////////////////////////////////////////////////////////

std::complex<float> c_cbrt(std::complex<float> x) {
  using std::cos;
  float a, b, r, phi, rn;
  std::complex<float> I(0, 1);
  float s;
  a = real(x);
  b = imag(x);

  r = sqrtf(a * a + b * b);

  phi = atan2(b, a);
  phi /= 3.f;
  rn = cbrt(r);
  s = sin(phi);
  return rn * cos(phi) + I * rn * s;
}

namespace GCF {
using Menge::Agents::Obstacle;
using Menge::Math::Vector2;

////////////////////////////////////////////////////////////////
//					Implementation of Ellipse
////////////////////////////////////////////////////////////////

Ellipse::Ellipse()
    : _center(0.f, 0.f), _cosPhi(1.f), _sinPhi(0.f), _majorAxis(1.f), _minorAxis(0.5f) {}

////////////////////////////////////////////////////////////////

Ellipse::Ellipse(const Vector2& center)
    : _center(center), _cosPhi(1.f), _sinPhi(0.f), _majorAxis(1.f), _minorAxis(1.f) {}

////////////////////////////////////////////////////////////////

Ellipse::Ellipse(const Vector2& center, const Vector2& axes)
    : _center(center), _cosPhi(1.f), _sinPhi(0.f), _majorAxis(axes.x()), _minorAxis(axes.y()) {}

////////////////////////////////////////////////////////////////

Ellipse::Ellipse(const Vector2& center, const Vector2& axes, float angle)
    : _center(center),
      _cosPhi(cos(angle)),
      _sinPhi(sin(angle)),
      _majorAxis(axes.x()),
      _minorAxis(axes.y()) {}

////////////////////////////////////////////////////////////////

float Ellipse::approxDistanceOfClosestApproach(const Ellipse& other) const {
  //	Compute the radius in the direction of that center
  float r2 = other.radiusInPointDirection(_center);
  float r1 = radiusInPointDirection(other._center);

  return r2 + r1;
}

////////////////////////////////////////////////////////////////

float Ellipse::distanceOfClosestApproach(const Ellipse& other) const {
  /*----------------- INPUT Params --------------*/
  float a1, b1, a2, b2, cos1, sin1, cos2, sin2;
  // The values of cosPhi and sinPhi are the direction of _majorAxis
  //		However, it is possible for _majorAxis < _minorAxis
  //		The algorithm requires a > b, so the comparison has to be done
  //		and changes to the orientation as necessary
  if (_majorAxis > _minorAxis) {
    a1 = _majorAxis;
    b1 = _minorAxis;
    cos1 = _cosPhi;
    sin1 = _sinPhi;
  } else {
    a1 = _minorAxis;
    b1 = _majorAxis;
    cos1 = -_sinPhi;
    sin1 = _cosPhi;
  }
  if (other._majorAxis > other._minorAxis) {
    a2 = other._majorAxis;
    b2 = other._minorAxis;
    cos2 = other._cosPhi;
    sin2 = other._sinPhi;
  } else {
    a2 = other._minorAxis;
    b2 = other._majorAxis;
    cos2 = -other._sinPhi;
    sin2 = other._cosPhi;
  }
  // The algorithm below works better if the directional
  //	vectors are in the upper quadrants - otherwise the occasional error will pop up.
  if (sin1 < 0.f) {
    sin1 = -sin1;
    cos1 = -cos1;
  }
  if (sin2 < 0.f) {
    sin2 = -sin2;
    cos2 = -cos2;
  }

  float qa1 = a1 * a1;
  float qb1 = b1 * b1;
  float qa2 = a2 * a2;
  float qb2 = b2 * b2;

  Vector2 c1c2 = norm(other._center - _center);
  Vector2 e11(cos1, sin1);  // unit vector of the direction of E1
  Vector2 e12(cos2, sin2);  // unit vector of the direction of E2
  /*----------------------------------------------*/
  float eps1, eps2, k1dotd, k2dotd, k1dotk2, nu, Ap[2][2], lambdaplus, lambdaminus, bp2, ap2,
      cosphi, tanphi2, delta, dp;
  std::complex<float> A, B, C, D, E, alpha, beta, gamma, P, Q, U, y, qu;

  const float EPS = 0.00001f;
  eps1 = 1.f - qb1 / qa1;
  eps2 = 1.f - qb2 / qa2;
  k1dotd = e11 * c1c2;
  k2dotd = e12 * c1c2;
  k1dotk2 = e11 * e12;
  nu = a1 / b1 - 1.f;

  float qk1dotk2 = k1dotk2 * k1dotk2;
  float qk1dotd = k1dotd * k1dotd;
  float qnu = nu * nu;
  Ap[0][0] = qb1 / qb2 *
             (1.f + 0.5f * (1.0f + k1dotk2) *
                        (nu * (2.0f + nu) - eps2 * (1.0f + nu * k1dotk2) * (1.0f + nu * k1dotk2)));
  Ap[1][1] = qb1 / qb2 *
             (1.0f + 0.5f * (1.0f - k1dotk2) *
                         (nu * (2.0f + nu) - eps2 * (1.0f - nu * k1dotk2) * (1.0f - nu * k1dotk2)));

  float tmp0 = fabs(1.0f - qk1dotk2) < EPS ? 0.f : (1.0f - qk1dotk2);
  Ap[0][1] = qb1 / qb2 * 0.5f * sqrtf(tmp0) * (nu * (2.0f + nu) + eps2 * (1.0f - qnu * qk1dotk2));

  float tmp1 = 0.5f * (Ap[0][0] + Ap[1][1]);
  float tmp3 = Ap[0][0] - Ap[1][1];
  float tmp2 = 0.25f * tmp3 * tmp3 + Ap[0][1] * Ap[0][1];
  tmp3 = sqrtf(tmp2);
  lambdaplus = tmp1 + tmp3;
  lambdaminus = tmp1 - tmp3;
  bp2 = 1.0f / sqrtf(lambdaplus);
  ap2 = 1.0f / sqrtf(lambdaminus);

  // original: if (fabs(k1dotk2) == 1.0) {
  if (fabs(k1dotk2) > 1.0f - EPS) {
    if (Ap[0][0] > Ap[1][1])
      cosphi = qb1 / qa1 * qk1dotd / (1.0f - eps1 * qk1dotd);
    else
      cosphi = (1.0f - qk1dotd) / (1.0f - eps1 * qk1dotd);
  } else {
    float Tmp1, Tmp2, Tmp3, Tmp4, Tmp5, Tmp6, Tmp7, Tmp8;
    Tmp1 = 2.0f * (Ap[0][1] * Ap[0][1] + (lambdaplus - Ap[0][0]) * (lambdaplus - Ap[0][0]));
    Tmp2 = (1.0f - eps1 * qk1dotd);
    Tmp8 = Tmp1 * Tmp2;

    Tmp3 = k2dotd + (b1 / a1 - 1.0f) * k1dotd * k1dotk2;
    Tmp4 = b1 / a1 * k1dotd;
    Tmp5 = Tmp4 + Tmp3;
    Tmp6 = Tmp4 - Tmp3;
    Tmp7 = Ap[0][1] / sqrtf(1.0f + k1dotk2) * (Tmp5) +
           (lambdaplus - Ap[0][0]) / sqrtf(1.0f - k1dotk2) * (Tmp6);
    cosphi = 1.0f / Tmp8 * (Tmp7 * Tmp7);
  }
  float qap2 = ap2 * ap2;
  float qbp2 = bp2 * bp2;
  delta = qap2 / qbp2 - 1.0f;
  if (delta == 0.0f || cosphi == 0.0f)
    dp = 1.0f + ap2;
  else {
    float Tmp, Tmp1;
    tanphi2 = 1.0f / (cosphi)-1.0f;
    Tmp = 1.0f + tanphi2;
    Tmp1 = 1.0f + delta;
    A = -(Tmp) / qbp2;
    B = -2.0f * (Tmp + delta) / bp2;
    C = -tanphi2 - Tmp1 * Tmp1 + (1.0f + Tmp1 * tanphi2) / qbp2;
    D = 2.0f * Tmp * Tmp1 / bp2;
    E = (Tmp + delta) * Tmp1;
    std::complex<float> qA = A * A;
    std::complex<float> qB = B * B;
    alpha = -3.0f * qB / (8.0f * qA) + C / A;
    beta = qB * B / (8.0f * qA * A) - B * C / (2.0f * qA) + D / A;
    gamma = -3.0f * qB * qB / (256.0f * qA * qA) + C * qB / (16.0f * qA * A) - B * D / (4.0f * qA) +
            E / A;
    std::complex<float> qalpha = alpha * alpha;
    if (beta == 0.0f) {
      qu = -B / (4.0f * A) + sqrt(0.5f * (-alpha + sqrt(qalpha - 4.0f * gamma)));
    } else {
      P = -qalpha / 12.0f - gamma;
      Q = -qalpha * alpha / 108.0f + alpha * gamma / 3.0f - beta * beta / 8.0f;
      U = c_cbrt(-Q * 0.5f + sqrt(Q * Q * 0.25f + P * P * P / 27.0f));
      if (U == 0.0f)
        y = -5.0f * alpha / 6.0f - c_cbrt(Q);
      else
        y = -5.0f * alpha / 6.0f + U - P / (3.0f * U);
      std::complex<float> tsqrt = sqrt(alpha + 2.0f * y);

      qu =
          -B / (4.0f * A) + 0.5f * (tsqrt + sqrt(-(3.0f * alpha + 2.0f * y + 2.0f * beta / tsqrt)));
    }
    std::complex<float> Tmp2 =
        (qu * qu - 1.0f) / delta * (1.0f + bp2 * (1.0f + delta) / qu) *
            (1.0f + bp2 * (1.0f + delta) / qu) +
        (1.0f - (qu * qu - 1.0f) / delta) * (1.0f + bp2 / qu) * (1.0f + bp2 / qu);
    dp = real(sqrt(Tmp2));
  }
  float result = dp * b1 / sqrtf(1.0f - eps1 * qk1dotd);
  if (result != result || result < 0.f) {
    // Testing for NaN and negative values
    return 0.f;
  }
  return result;
}

////////////////////////////////////////////////////////////////

float Ellipse::distanceOfClosestApproach(const Obstacle* line) const {
  Vector2 nearPt;
  float distSq;
  if (line->distanceSqToPoint(_center, nearPt, distSq) != Obstacle::MIDDLE) {
    return 0.f;
  }
  Vector2 p0(toEllipseSpace(line->getP0()));
  Vector2 p1(toEllipseSpace(line->getP1()));
  Vector2 lineDir = norm(p0 - p1);
  Vector2 N(-lineDir.y(), lineDir.x());
  float C = -(N * p0);
  if (C < 0) {
    lineDir.negate();
    N.negate();
  }
  Vector2 T(norm(Vector2(lineDir.y() / _minorAxis, -lineDir.x() / _majorAxis)));
  Vector2 R(_majorAxis * T.x(), _minorAxis * T.y());
  float d = N * R + C;

  return C - d;
}

////////////////////////////////////////////////////////////////

Vector2 Ellipse::closestPoint(const Vector2& pt) const {
  // Code stolen from: http://www.iquilezles.org/blog/?p=1310
  //	Assumes pt is in the first quadrant in ellipse space
  const float x = fabs(pt.x());
  const float y = fabs(pt.y());
  float ea = _majorAxis;
  float eb = _minorAxis;

  const float k = eb * eb - ea * ea;
  const float a = ea * x / k;
  const float a2 = a * a;
  const float b = eb * y / k;
  const float b2 = b * b;
  const float c = (a2 + b2 - 1.0f) / 3.0f;
  const float c3 = c * c * c;

  const float q = c3 + b2 * a2 * 2.0f;
  const float d = c3 + b2 * a2;
  const float g = a + a * b2;

  float co;

  if (d < 0.0f) {
    const float p = acosf(q / c3) / 3.0f;
    const float m = cosf(p);
    const float n = sinf(p) * sqrtf(3.0f);
    const float rx = sqrtf(-c * (m + n + 2.0f) + a2);
    const float ry = sqrtf(-c * (m - n + 2.0f) + a2);
    const float s = rx + g / (rx * ry);
    co = (ry + (k > 0.0f ? rx : -rx) + fabsf(g) / (rx * ry) - a) / 2.0f;
  } else {
    const float h = 2.0f * a * b * sqrtf(d);
    const float s = powf(q + h, 1.0f / 3.0f);
    const float u = powf(q - h, 1.0f / 3.0f);
    const float rx = -s - u - c * 4.0f + 2.0f * a2;
    const float ry = (s - u) * sqrtf(3.0f);
    const float rm = sqrtf(rx * rx + ry * ry);
    const float p = ry / sqrtf(rm - rx);
    co = (p + 2.0f * g / rm - a) / 2.0f;
  }

  if (co > 1.0f) co = 1.0f;
  const float si = sqrtf(1.0f - co * co);

  float px = ea * co;
  float py = eb * si;
  if (pt.x() < 0.f) px = -px;
  if (pt.y() < 0.f) py = -py;

  return Vector2(px, py);
}

////////////////////////////////////////////////////////////////

float Ellipse::minimumDistance(const Obstacle* line, Vector2& dir) const {
  Vector2 p0(toEllipseSpace(line->getP0()));
  Vector2 p1(toEllipseSpace(line->getP1()));
  Vector2 O(p1);
  Vector2 E(p0);
  Vector2 lineDir = norm(p0 - p1);
  Vector2 N(-lineDir.y(), lineDir.x());
  float C = -(N * p0);
  if (C < 0) {
    lineDir.negate();
    N.negate();
    O.set(p0);
    E.set(p1);
  }
  Vector2 T(norm(Vector2(lineDir.y() / _minorAxis, -lineDir.x() / _majorAxis)));
  Vector2 R(_majorAxis * T.x(), _minorAxis * T.y());
  // R is the nearest point to the LINE - does it project to the segment?
  float t = (R - O) * lineDir;
  // TODO: confirm that this assumption is correct.
  //	This is true because the distance field to the ellipse is convex
  //	Given a line through the field, if I find the minimum distance on the line,
  //	the closer a point is to the minimum the closer to the ellipse.
  //	So, if the closest point is beyond the end point, the end point must be the closest point
  //		if it is before the start point, the start point must be closest.
  if (t > line->length()) {
    // nearest to end
    R.set(closestPoint(E));
    dir.set(E - R);
    float dist = abs(dir);
    dir /= dist;
    return dist;
  } else if (t < 0) {
    // nearest to start
    R.set(closestPoint(O));
    dir.set(O - R);
    float dist = abs(dir);
    dir /= dist;
    return dist;
  } else {
    // closest point projects to the segment, so its just the distance to the line
    dir.set(N);
    return N * R + C;
  }
}

////////////////////////////////////////////////////////////////

Vector2 Ellipse::toEllipseSpace(const Vector2& pt) const {
  Vector2 p = pt - _center;
  float x = p.x() * _cosPhi + p.y() * _sinPhi;
  float y = p.y() * _cosPhi - p.x() * _sinPhi;
  return Vector2(x, y);
}

////////////////////////////////////////////////////////////////

Vector2 Ellipse::fromEllipseSpace(const Vector2& pt) const {
  float x = pt.x() * _cosPhi - pt.y() * _sinPhi;
  float y = pt.y() * _cosPhi + pt.x() * _sinPhi;
  return Vector2(x, y) + _center;
}

////////////////////////////////////////////////////////////////

float Ellipse::approximateMinimumDistance(const Vector2& pt) const {
  Vector2 pSpace = toEllipseSpace(pt);
  float dist = abs(pSpace);
  Vector2 dir = pSpace / dist;
  Vector2 boundPoint(_majorAxis * dir.x(), _minorAxis * dir.y());
  float radius = abs(boundPoint);
  // Note: if the point lies INSIDE the ellipse, this value will be
  //		negative.
  return dist - radius;
}

////////////////////////////////////////////////////////////////

float Ellipse::radiusInPointDirection(const Vector2& pt) const {
  Vector2 p = toEllipseSpace(pt);
  Vector2 dir = norm(p);
  Vector2 boundPoint(_majorAxis * dir.x(), _minorAxis * dir.y());
  return abs(boundPoint);
}

////////////////////////////////////////////////////////////////

float Ellipse::radiusInDirection(const Vector2& dir) const {
  // rotate the direction
  float x = dir.x() * _cosPhi + dir.y() * _sinPhi;
  float y = dir.y() * _cosPhi - dir.x() * _sinPhi;

  Vector2 boundPoint(_majorAxis * x, _minorAxis * y);
  return abs(boundPoint);
}
}  // namespace GCF
