#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

#include "intcode.h"

int Amplify(const std::vector<int>& phases) {
  std::vector<int> inputs = {phases[0], 0};
  std::vector<int> outputs;
  std::string filename = "/home/samuel/aoc/day7.txt";
  intcode::RunProgram(filename, inputs, outputs);
  inputs[0] = phases[1];
  inputs[1] = outputs[0];
  intcode::RunProgram(filename, inputs, outputs);
  inputs[0] = phases[2];
  inputs[1] = outputs[1];
  intcode::RunProgram(filename, inputs, outputs);
  inputs[0] = phases[3];
  inputs[1] = outputs[2];
  intcode::RunProgram(filename, inputs, outputs);
  inputs[0] = phases[4];
  inputs[1] = outputs[3];
  intcode::RunProgram(filename, inputs, outputs);
  return outputs[4];
}

int main() {
  std::vector<int> phases = {0, 1, 2, 3, 4};
  int max = std::numeric_limits<int>::min();
  while (std::next_permutation(phases.begin(), phases.end())) {
    int output = Amplify(phases);
    if (output > max) {
      max = output;
    }
  }
  std::cout << max << std::endl;
}
