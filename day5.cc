#include <iostream>

#include "intcode.h"

int main() {
  std::string filename = "/home/samuel/aoc/day5.txt";
  int result = intcode::RunProgram(filename);
  std::cout << "HALT: " << result << std::endl;
}
