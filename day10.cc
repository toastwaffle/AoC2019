#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>

#include "xy.h"

std::vector<xy::XY> ReadInput() {
  std::ifstream file("/home/samuel/aoc/day10.txt");
  std::string line;
  std::vector<xy::XY> asteroids;
  int y = 0;
  while (std::getline(file, line)) {
    int x = 0;
    for (const auto& c : line) {
      if (c == '#') {
        asteroids.push_back(xy::XY(x, y));
      }
      x++;
    }
    y++;
  }
  return asteroids;
}

std::vector<std::pair<xy::XY,xy::XY>> Visible(const xy::XY asteroid, const std::vector<xy::XY>& asteroids) {
  std::vector<xy::XY> visibleDirections;
  std::vector<std::pair<xy::XY,xy::XY>> visible;
  for (const auto& other : asteroids) {
    if (other == asteroid) {
      continue;
    }
    xy::XY delta = (other - asteroid);
    xy::XY direction = delta.Normalise();
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
      visible.push_back(std::pair<xy::XY,xy::XY>(delta, other));
    }
  }
  return visible;
}

int main() {
  auto asteroids = ReadInput();
  std::vector<std::pair<xy::XY,xy::XY>> best;
  for (const xy::XY& asteroid : asteroids) {
    auto visible = Visible(asteroid, asteroids);
    if (visible.size() > best.size()) {
      best = visible;
    }
  }
  std::cout << best.size() << std::endl;
  std::sort(best.begin(), best.end(), [](const std::pair<xy::XY,xy::XY>& left, const std::pair<xy::XY,xy::XY>& right) -> bool {
    return left.first.Angle() < right.first.Angle();
  });
  auto a = best[199];
  std::cout << a.first.x << "," << a.first.y << "," << a.second.x << "," << a.second.y << std::endl;
}
