#include "xy.h"

#include <cmath>

namespace xy {
namespace {

constexpr double PI = std::acos(-1);

int GreatestCommonDivisor(int a, int b) {
  // Everything divides 0
  if (a == 0) return b;
  if (b == 0) return a;

  // base case
  if (a == b) return a;

  // a is greater
  if (a > b) {
    return GreatestCommonDivisor(a-b, b);
  } else {
    return GreatestCommonDivisor(a, b-a);
  }
}

} // namespace

XY::XY(int x, int y) : x(x), y(y) {}

XY XY::operator-(const XY& other) const {
  return XY(x - other.x, y - other.y);
}

XY XY::Normalise() const {
  int gcd = GreatestCommonDivisor(std::abs(x), std::abs(y));
  return XY(x / gcd, y / gcd);
}

bool XY::operator==(const XY& other) const {
  return x == other.x && y == other.y;
}

double XY::Magnitude() const {
  return std::sqrt(x * x + y * y);
}

double XY::Angle() const {
  // atan2 returns counterclockwise angle from x-axis in range [-pi, +pi]
  // Also, our y-axis is upside down
  double angle = std::atan2(-y, x);
  // Make clockwise
  angle *= -1;
  // Make relative to y-axis
  angle += 0.5 * PI;
  // Pin to range [0, 2*pi]
  if (angle < 0) {
    angle += 2 * PI;
  }
  if (angle > 2 * PI) {
    angle -= 2 * PI;
  }
  return angle;
}

} // namespace xy
