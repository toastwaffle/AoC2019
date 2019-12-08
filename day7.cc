#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

#include "intcode.h"

#define LOG(message) std::cerr << message << std::endl;

int Amplify(const std::vector<int>& phases) {
  std::string filename = "/home/samuel/aoc/day7.txt";
  intcode::IntCode amplifierA("A", filename);
  intcode::IntCode amplifierB("B", filename);
  intcode::IntCode amplifierC("C", filename);
  intcode::IntCode amplifierD("D", filename);
  intcode::IntCode amplifierE("E", filename);

  // Prime
  LOG("starting E");
  amplifierE.Run();
  amplifierE.SendInput(phases[4]);
  LOG("starting D");
  amplifierD.Run();
  amplifierD.SendInput(phases[3]);
  LOG("starting C");
  amplifierC.Run();
  amplifierC.SendInput(phases[2]);
  LOG("starting B");
  amplifierB.Run();
  amplifierB.SendInput(phases[1]);
  LOG("starting A");
  amplifierA.Run();
  amplifierA.SendInput(phases[0]);

  // Set up feedback loop
  LOG("Setting up feedback loop");
  amplifierA.SetOutputter(std::bind(&intcode::IntCode::SendInput, &amplifierB, std::placeholders::_1));
  amplifierB.SetOutputter(std::bind(&intcode::IntCode::SendInput, &amplifierC, std::placeholders::_1));
  amplifierC.SetOutputter(std::bind(&intcode::IntCode::SendInput, &amplifierD, std::placeholders::_1));
  amplifierD.SetOutputter(std::bind(&intcode::IntCode::SendInput, &amplifierE, std::placeholders::_1));

  std::vector<int> outputs;
  amplifierE.SetOutputter([&outputs, &amplifierA](int output) {
    LOG("output from E: " << output);
    outputs.push_back(output);
    amplifierA.SendInput(output);
  });

  // Go
  LOG("Starting")
  amplifierA.SendInput(0);

  if (
    !amplifierA.IsHalted() &&
    !amplifierB.IsHalted() &&
    !amplifierC.IsHalted() &&
    !amplifierD.IsHalted() &&
    !amplifierE.IsHalted()
  ) {
    std::cerr << "Amplifiers not halted" << std::endl;
    exit(1);
  }

  return outputs[outputs.size()-1];
}

int main() {
  std::vector<int> phases = {5,6,7,8,9};
  int max = std::numeric_limits<int>::min();
  while (std::next_permutation(phases.begin(), phases.end())) {
    int output = Amplify(phases);
    if (output > max) {
      max = output;
    }
  }
  std::cout << max << std::endl;
}
