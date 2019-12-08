#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <functional>

#include "intcode.h"

namespace intcode {

IntCode::IntCode(const std::string instance, const std::string& filename) {
  this->instance = instance;
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << this->instance << ": Failed to open file" << std::endl;
    exit(1);
  }
  std::string line;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, ',')) {
      if (token == "") continue;
      this->memory.push_back(std::stoi(token));
    }
  }
  this->ip = 0;
  this->halted = false;
  this->outputter = nullptr;
  NextInstruction();
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

void IntCode::SetOutputter(std::function<void(int)> outputter) {
  this->outputter = outputter;
}

void IntCode::NextInstruction() {
  if (this->halted) return;
  if (this->ip >= this->memory.size()) {
    std::cerr << this->instance << ": Ran out of instructions" << std::endl;
    exit(1);
  }
  int opcode = memory[this->ip];
  Instruction instruction;
  instruction.operation = static_cast<Operation>(opcode%100);
  opcode /= 100;
  for (int i = 0; i < ParameterCount(instruction.operation); i++) {
    this->ip++;
    instruction.parameters.push_back(memory[this->ip]);
    instruction.parameterModes.push_back(static_cast<ParameterMode>(opcode%10));
    opcode /= 10;
  }
  this->ip++;
  this->currentInstruction = instruction;
}

int IntCode::GetArg(int index) {
  int param = this->currentInstruction.parameters[index];
  if (this->currentInstruction.parameterModes[index] == IMMEDIATE) return param;
  return this->memory[param];
}

void IntCode::Run() {
  while (!this->halted) {
    switch (this->currentInstruction.operation) {
    case ADD:
      this->memory[this->currentInstruction.parameters[2]] = this->GetArg(0) + this->GetArg(1);
      break;
    case MULTIPLY:
      this->memory[this->currentInstruction.parameters[2]] = this->GetArg(0) * this->GetArg(1);
      break;
    case INPUT:
      return;
    case OUTPUT:
    {
      if (this->outputter == nullptr) {
        std::cerr << this->instance << ": No outputter set for OUTPUT instruction." << std::endl;
        exit(1);
      }
      // If the output causes Input or Run to be called, we need the instruction to be advanced.
      int output = this->GetArg(0);
      this->NextInstruction();
      this->outputter(output);
      continue;
    }
    case JUMP_IF_TRUE:
      if (this->GetArg(0) != 0) {
        ip = this->GetArg(1);
      }
      break;
    case JUMP_IF_FALSE:
      if (this->GetArg(0) == 0) {
        ip = this->GetArg(1);
      }
      break;
    case LESS_THAN:
      if (this->GetArg(0) < this->GetArg(1)) {
        this->memory[this->currentInstruction.parameters[2]] = 1;
      } else {
        this->memory[this->currentInstruction.parameters[2]] = 0;
      }
      break;
    case EQUALS:
      if (this->GetArg(0) == this->GetArg(1)) {
        this->memory[this->currentInstruction.parameters[2]] = 1;
      } else {
        this->memory[this->currentInstruction.parameters[2]] = 0;
      }
      break;
    case HALT:
      this->halted = true;
      return;
    default:
      std::cerr << this->instance << ": Invalid opcode " << this->currentInstruction.operation << std::endl;
      exit(1);
    }
    this->NextInstruction();
  }
}

void IntCode::SendInput(int input) {
  if (this->currentInstruction.operation != INPUT) {
    this->Run();
    if (this->halted) {
      std::cerr << this->instance << ": Cannot send input; program has halted" << std::endl;
      // exit(1);
    } else if (this->currentInstruction.operation != INPUT) {
      std::cerr << this->instance << ": Cannot send input; current opcode is " << this->currentInstruction.operation << std::endl;
      exit(1);
    }
  }
  this->memory[this->currentInstruction.parameters[0]] = input;
  this->NextInstruction();
  this->Run();
}

bool IntCode::IsHalted() {
  return this->halted;
}

} // namespace intcode
