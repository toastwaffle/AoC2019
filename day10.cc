#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <utility>

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

class XY {
public:
  int x;
  int y;

  XY(int x, int y) : x(x), y(y) {}

  XY operator-(const XY& other) const {
    return XY(x - other.x, y - other.y);
  }

  XY Normalise() const {
    int gcd = GreatestCommonDivisor(std::abs(x), std::abs(y));
    return XY(x / gcd, y / gcd);
  }

  bool operator==(const XY& other) const {
    return x == other.x && y == other.y;
  }

  double Magnitude() const {
    return std::sqrt(x * x + y * y);
  }

  double Angle() const {
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
};

std::vector<XY> ReadInput() {
  std::ifstream file("/home/samuel/aoc/day10.txt");
  std::string line;
  std::vector<XY> asteroids;
  int y = 0;
  while (std::getline(file, line)) {
    int x = 0;
    for (const auto& c : line) {
      if (c == '#') {
        asteroids.push_back(XY(x, y));
      }
      x++;
    }
    y++;
  }
  return asteroids;
}

std::vector<std::pair<XY,XY>> Visible(const XY asteroid, const std::vector<XY>& asteroids) {
  std::vector<XY> visibleDirections;
  std::vector<std::pair<XY,XY>> visible;
  for (const auto& other : asteroids) {
    if (other == asteroid) {
      continue;
    }
    XY delta = (other - asteroid);
    XY direction = delta.Normalise();
    bool aligned = false;
    for (unsigned int i = 0; i < visibleDirections.size(); i++) {
      if (direction == visibleDirections[i]) {
        aligned = true;
        if (delta.Magnitude() < visible[i].first.Magnitude()) {
          visible[i].second = other;
        }
        break;
      }
    }
    if (!aligned) {
      visibleDirections.push_back(direction);
      visible.push_back(std::pair<XY,XY>(delta, other));
    }
  }
  return visible;
}

int main() {
  auto asteroids = ReadInput();
  std::vector<std::pair<XY,XY>> best;
  for (const XY& asteroid : asteroids) {
    auto visible = Visible(asteroid, asteroids);
    if (visible.size() > best.size()) {
      best = visible;
    }
  }
  std::cout << best.size() << std::endl;
  std::sort(best.begin(), best.end(), [](const std::pair<XY,XY>& left, const std::pair<XY,XY>& right) -> bool {
    return left.first.Angle() < right.first.Angle();
  });
  auto a = best[199];
  std::cout << a.first.x << "," << a.first.y << "," << a.second.x << "," << a.second.y << std::endl;
}
