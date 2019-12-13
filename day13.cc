#include <map>
#include <utility>
#include <iostream>

#include "intcode.h"

int main() {
  std::map<std::pair<int64_t, int64_t>, int64_t> drawn;

  intcode::IntCode arcade("arcade", "/home/samuel/aoc/day13.txt");

  int64_t outputs[3];
  int idx = 0;
  int64_t score;
  arcade.SetOutputter([&drawn, &outputs, &idx, &score](int64_t output) {
    outputs[idx] = output;
    idx++;
    if (idx == 3) {
      if (outputs[0] == -1 && outputs[1] == 0) {
        score = outputs[2];
      } else {
        drawn[std::pair<int64_t,int64_t>(outputs[0], outputs[1])] = outputs[2];
      }
      idx = 0;
    }
  });

  arcade.Run();

  int count = 0;
  int64_t min_x = std::numeric_limits<int64_t>::max();
  int64_t max_x = std::numeric_limits<int64_t>::min();
  int64_t min_y = std::numeric_limits<int64_t>::max();
  int64_t max_y = std::numeric_limits<int64_t>::min();

  for (const auto [position, value] : drawn) {
    min_x = std::min(min_x, position.first);
    max_x = std::max(max_x, position.first);
    min_y = std::min(min_y, position.second);
    max_y = std::max(max_y, position.second);
    if (value == 2) count++;
  }

  std::cout << count << std::endl;
  std::cout << min_x << std::endl;
  std::cout << max_x << std::endl;
  std::cout << min_y << std::endl;
  std::cout << max_y << std::endl;

  int64_t ball_x = -1;
  int64_t paddle_x = -1;
  while (!arcade.IsHalted()) {
    for (int64_t y = min_y; y <= max_y; y++) {
      for (int64_t x = min_x; x <= max_x; x++) {
        switch (drawn[std::pair<int64_t, int64_t>(x, y)]) {
          case 1:
            std::cout << "█";
            break;
          case 2:
            std::cout << "▒";
            break;
          case 3:
            std::cout << "━";
            paddle_x = x;
            break;
          case 4:
            std::cout << "o";
            ball_x = x;
            break;
          default:
            std::cout << " ";
            break;
        }
      }
      std::cout << "\n";
    }
    std::cout << paddle_x << ", " << ball_x << std::endl;
    if (paddle_x < ball_x) {
      arcade.SendInput(1);
    } else if (ball_x < paddle_x) {
      arcade.SendInput(-1);
    } else {
      arcade.SendInput(0);
    }
  }

  std::cout << score << std::endl;
}
