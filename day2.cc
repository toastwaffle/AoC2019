#include <iostream>

#include "intcode.h"

int main() {
  std::string filename = "/home/samuel/aoc/day2.txt";
  for (int noun = 0; noun < 100; noun++) {
    for (int verb = 0; verb < 100; verb++) {
      if (intcode::RunProgram(filename, noun, verb) == 19690720) {
        std::cout << 100*noun+verb << std::endl;
        return 0;
      }
    }
  }
}
