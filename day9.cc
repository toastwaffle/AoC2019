#include <iostream>

#include "intcode.h"

int main() {
  intcode::IntCode ic("day9", "/home/samuel/aoc/day9.txt");
  ic.SetOutputter([](long int output) {
    std::cout << "Output: " << output << std::endl;
  });
  ic.Run();
  ic.SendInput(2);
}
