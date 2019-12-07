#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

namespace intcode {

enum Operation {
  UNKNoWN = -1,
  ADD = 1,
  MULTIPLY = 2,
  INPUT = 3,
  OUTPUT = 4,
  JUMP_IF_TRUE = 5,
  JUMP_IF_FALSE = 6,
  LESS_THAN = 7,
  EQUALS = 8,
  HALT = 99,
};

enum ParameterMode {
  POSITION = 0,
  IMMEDIATE = 1,
};

struct Instruction {
  Operation operation;
  std::vector<int> parameters;
  std::vector<ParameterMode> parameterModes;
};

std::vector<int> ReadInput(const std::string& filename) {
  std::ifstream file(filename);
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
  return result;
}

int ParameterCount(const Operation& op) {
  switch (op) {
    case ADD:
    case MULTIPLY:
    case LESS_THAN:
    case EQUALS:
      return 3;
    case JUMP_IF_TRUE:
    case JUMP_IF_FALSE:
      return 2;
    case INPUT:
    case OUTPUT:
      return 1;
    case HALT:
      return 0;
    default:
      std::cerr << "Unknown operation: " << op << std::endl;
      exit(1);
  }
}

Instruction NextInstruction(const std::vector<int>& memory, int* ip) {
  int opcode = memory[*ip];
  Instruction instruction;
  instruction.operation = static_cast<Operation>(opcode%100);
  opcode /= 100;
  for (int i = 0; i < ParameterCount(instruction.operation); i++) {
    (*ip)++;
    instruction.parameters.push_back(memory[*ip]);
    instruction.parameterModes.push_back(static_cast<ParameterMode>(opcode%10));
    opcode /= 10;
  }
  (*ip)++;
  return instruction;
}

int GetArg(const std::vector<int>& memory, const Instruction& instr, int index) {
  int param = instr.parameters[index];
  if (instr.parameterModes[index] == IMMEDIATE) return param;
  return memory[param];
}

int Run(std::vector<int> memory, std::vector<int>& inputs, std::vector<int>& outputs) {
  int ip = 0;
  int inputIndex = 0;
  while (ip < memory.size()) {
    auto instr = NextInstruction(memory, &ip);
    switch (instr.operation) {
    case ADD:
      memory[instr.parameters[2]] = GetArg(memory, instr, 0) + GetArg(memory, instr, 1);
      break;
    case MULTIPLY:
      memory[instr.parameters[2]] = GetArg(memory, instr, 0) * GetArg(memory, instr, 1);
      break;
    case INPUT:
    {
      int input = inputs[inputIndex];
      inputIndex++;
      memory[instr.parameters[0]] = input;
      break;
    }
    case OUTPUT:
      outputs.push_back(GetArg(memory, instr, 0));
      break;
    case JUMP_IF_TRUE:
      if (GetArg(memory, instr, 0) != 0) {
        ip = GetArg(memory, instr, 1);
      }
      break;
    case JUMP_IF_FALSE:
      if (GetArg(memory, instr, 0) == 0) {
        ip = GetArg(memory, instr, 1);
      }
      break;
    case LESS_THAN:
      if (GetArg(memory, instr, 0) < GetArg(memory, instr, 1)) {
        memory[instr.parameters[2]] = 1;
      } else {
        memory[instr.parameters[2]] = 0;
      }
      break;
    case EQUALS:
      if (GetArg(memory, instr, 0) == GetArg(memory, instr, 1)) {
        memory[instr.parameters[2]] = 1;
      } else {
        memory[instr.parameters[2]] = 0;
      }
      break;
    case HALT:
      return memory[0];
    default:
      std::cerr << "Invalid opcode " << instr.operation << std::endl;
      exit(1);
    }
  }
  std::cerr << "Ran out of instructions" << std::endl;
  exit(1);
}

int RunProgram(const std::string& filename, std::vector<int>& inputs, std::vector<int>& outputs) {
  return Run(ReadInput(filename), inputs, outputs);
}

int RunProgram(const std::string& filename, int noun, int verb, std::vector<int>& inputs, std::vector<int>& outputs) {
  std::vector<int> memory = ReadInput(filename);

  memory[1] = noun;
  memory[2] = verb;

  return Run(memory, inputs, outputs);
}

} // namespace intcode
