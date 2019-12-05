#include <utility>
#include <string>
#include <vector>
#include <iostream>
#include <optional>
#include <fstream>
#include <sstream>

struct Point {
  int x;
  int y;
};

enum Direction {
  LR,
  UD,
};

struct Wire {
  Point start;
  Point end;
  Direction direction;
  // Cumulative signal delay to start.
  int delay;
};

std::optional<Point> Intersection(const Wire& first, const Wire& second) {
  if (first.direction == second.direction) {
    return std::nullopt;
  }

  Wire updown, leftright;

  if (first.direction == LR) {
    leftright = first;
    updown = second;
  } else {
    leftright = second;
    updown = first;
  }

  int bottom = std::min(updown.start.y, updown.end.y);
  int top = std::max(updown.start.y, updown.end.y);
  int left = std::min(leftright.start.x, leftright.end.x);
  int right = std::max(leftright.start.x, leftright.end.x);

  if (
    bottom <= leftright.start.y &&
    leftright.start.y <= top &&
    left <= updown.start.x &&
    updown.start.x <= right
  ) {
    return std::optional<Point>({updown.start.x, leftright.start.y});
  }

  return std::nullopt;
}

std::vector<Wire> MakeWires(const std::vector<std::string>& moves) {
  Point current = {0, 0};
  int delay = 0;
  std::vector<Wire> wires;
  for (const auto& move : moves) {
    Point next = {current.x, current.y};
    int distance = std::stoi(move.substr(1));
    Direction direction = UD;
    switch (move.front()) {
      case 'R':
        next.x += distance;
        direction = LR;
        break;
      case 'L':
        next.x -= distance;
        direction = LR;
        break;
      case 'U':
        next.y += distance;
        break;
      case 'D':
        next.y -= distance;
        break;
    }
    wires.push_back(Wire{current, next, direction, delay});
    current = next;
    delay += distance;
  }
  return wires;
}

std::vector<std::string> ReadLine(const std::string& line) {
  std::vector<std::string> result;
  std::stringstream ss(line);
  std::string token;
  while (std::getline(ss, token, ',')) {
    if (token == "") continue;
    result.push_back(token);
  }
  return result;
}

std::pair<std::vector<std::string>, std::vector<std::string>> ReadInput() {
  std::ifstream file("/home/samuel/aoc/day3.txt");
  if (!file.is_open()) {
    std::cerr << "Failed to open file" << std::endl;
    exit(1);
  }
  std::string line;
  std::getline(file, line);
  auto first = ReadLine(line);
  std::getline(file, line);
  auto second = ReadLine(line);
  file.close();
  return std::pair<std::vector<std::string>, std::vector<std::string>>(first, second);
}

int SignalDelay(const Wire& wire, const Point& intersection) {
  if (wire.direction == LR) {
    return std::abs(intersection.x - wire.start.x) + wire.delay;
  }
  return std::abs(intersection.y - wire.start.y) + wire.delay;
}

int ClosestIntersection(const std::pair<std::vector<std::string>, std::vector<std::string>>& moves) {
  int closest = std::numeric_limits<int>::max();
  for (const auto& first : MakeWires(moves.first)) {
    for (const auto& second : MakeWires(moves.second)) {
      auto intersection = Intersection(first, second);
      if (intersection.has_value()) {
        int distance = SignalDelay(first, *intersection) + SignalDelay(second, *intersection);
        if (distance < closest) {
          closest = distance;
        }
      }
    }
  }
  return closest;
}

int main() {
  std::cout << ClosestIntersection(ReadInput()) << std::endl;
}
