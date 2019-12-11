#include <map>
#include <iostream>

#include "xy.h"
#include "intcode.h"

enum Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

enum Turn {
  LEFT_TURN = 0,
  RIGHT_TURN = 1
};

Direction DoTurn(const Direction current, const Turn turn) {
  if (turn == LEFT_TURN) {
    switch (current) {
      case UP:
        return LEFT;
      case DOWN:
        return RIGHT;
      case LEFT:
        return DOWN;
      case RIGHT:
        return UP;
    }
  } else {
    switch (current) {
      case UP:
        return RIGHT;
      case DOWN:
        return LEFT;
      case LEFT:
        return UP;
      case RIGHT:
        return DOWN;
    }
  }
}

xy::XY Move(const Direction direction) {
  switch (direction) {
    case UP:
      return xy::XY(0, 1);
    case DOWN:
      return xy::XY(0, -1);
    case LEFT:
      return xy::XY(-1, 0);
    case RIGHT:
      return xy::XY(1, 0);
  }
}

int main() {
  std::map<std::pair<int, int>, int64_t> hull_colour;
  xy::XY position(0, 0);
  Direction direction = UP;
  intcode::IntCode robot("robot", "/home/samuel/aoc/day11.txt");

  hull_colour[std::pair<int, int>(0, 0)] = 1;

  std::vector<int64_t> outputs;
  robot.SetOutputter([&outputs](int64_t output) {
    outputs.push_back(output);
  });

  int min_x = 0;
  int max_x = 0;
  int min_y = 0;
  int max_y = 0;

  robot.Run();
  while (!robot.IsHalted()) {
    min_x = std::min(min_x, position.x);
    max_x = std::max(max_x, position.x);
    min_y = std::min(min_y, position.y);
    max_y = std::max(max_y, position.y);

    auto pp = position.AsPair();
    robot.SendInput(hull_colour[pp]);
    hull_colour[pp] = outputs[outputs.size() - 2];
    direction = DoTurn(direction, static_cast<Turn>(outputs[outputs.size() - 1]));
    position += Move(direction);
  }
  for (int y = 0; y < 6; ++y) {
    for (int x = 0; x < 43; ++x) {
      if (hull_colour[std::pair<int,int>(x, -y)] == 1) {
        std::cout << "X";
      } else {
        std::cout << " ";
      }
    }
    std::cout << std::endl;
  }
  std::cout << min_x << std::endl;
  std::cout << max_x << std::endl;
  std::cout << min_y << std::endl;
  std::cout << max_y << std::endl;
}
