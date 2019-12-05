#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

std::vector<int> ReadInput() {
  std::ifstream file("/home/samuel/aoc/day2.txt");
  if (!file.is_open()) {
    std::cerr << "Failed to open file" << std::endl;
    exit(1);
  }
  std::string line;
  std::vector<int> result;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, ',')) {
      if (token == "") continue;
      result.push_back(std::stoi(token));
    }
  }
  file.close();
  // std::cout << "Program size: " << result.size() << std::endl;
  return result;
}

int RunProgram(int noun, int verb) {
  std::vector<int> program = ReadInput();

  program[1] = noun;
  program[2] = verb;

  for (auto i = 0; i < program.size(); i += 4) {
    // std::cout << "#" << i << ": ";
    switch (program[i]) {
    case 1:
    {
      int l = program[i+1];
      int r = program[i+2];
      int o = program[i+3];
      int v = program[l] + program[r];
      // std::cout << "$" << l << " + $" << r << " = " << program[l] << " + " << program[r] << " = " << v << " -> $" << o << std::endl;
      program[o] = v;
      break;
    }
    case 2:
    {
      int l = program[i+1];
      int r = program[i+2];
      int o = program[i+3];
      int v = program[l] * program[r];
      // std::cout << "$" << l << " + $" << r << " = " << program[l] << " * " << program[r] << " = " << v << " -> $" << o << std::endl;
      program[o] = v;
      break;
    }
    case 99:
      return program[0];
    default:
      std::cerr << "Invalid opcode " << program[i] << std::endl;
      exit(1);
    }
  }
  std::cerr << "Ran out of instructions" << std::endl;
  exit(1);
}

int main() {
  for (int noun = 0; noun < 100; noun++) {
    for (int verb = 0; verb < 100; verb++) {
      if (RunProgram(noun, verb) == 19690720) {
        std::cout << 100*noun+verb << std::endl;
        return 0;
      }
    }
  }
}
