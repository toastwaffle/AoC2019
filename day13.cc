#include <map>
#include <utility>
#include <iostream>

#include "intcode.h"

int main() {
  std::map<std::pair<int64_t, int64_t>, int64_t> drawn;

  intcode::IntCode arcade("arcade", "/home/samuel/aoc/day13.txt");

  int64_t outputs[3];
  int idx = 0;
  arcade.SetOutputter([&drawn, &outputs, &idx](int64_t output) {
    outputs[idx] = output;
    idx++;
    if (idx == 3) {
      drawn[std::pair<int,int>(outputs[0], outputs[1])] = outputs[2];
      idx = 0;
    }
  });

  arcade.Run();

  int count = 0;
  for (const auto [unused_key, value] : drawn) {
    if (value == 2) count++;
  }

  std::cout << count << std::endl;
}
