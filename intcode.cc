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
namespace {

int ParameterCount(const Operation op) {
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
    case ADJUST_RELATIVE_BASE:
      return 1;
    case HALT:
      return 0;
    default:
      std::cerr << "Unknown operation: " << op << std::endl;
      exit(1);
  }
}

} // namespace

IntCode::IntCode(const std::string instance, const std::string& filename) : instance_(instance) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << instance_ << ": Failed to open file" << std::endl;
    exit(1);
  }
  std::string line;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, ',')) {
      if (token == "") continue;
      memory_.push_back(std::stoi(token));
    }
  }
  NextInstruction();
}

void IntCode::SetOutputter(std::function<void(int64_t)> outputter) {
  outputter_ = outputter;
}

void IntCode::WriteMem(int64_t address, int64_t value) {
  EnsureMem(address);
  memory_[address] = value;
}

int64_t IntCode::ReadMem(int64_t address) {
  EnsureMem(address);
  return memory_[address];
}

void IntCode::EnsureMem(int64_t address) {
  if (address >= memory_.size()) {
    memory_.resize(address * 2);
  }
}

void IntCode::NextInstruction() {
  if (halted_) return;
  if (ip_ >= memory_.size()) {
    std::cerr << instance_ << ": Ran out of instructions" << std::endl;
    exit(1);
  }
  int opcode = ReadMem(ip_);
  Instruction instruction;
  instruction.operation = static_cast<Operation>(opcode%100);
  opcode /= 100;
  for (int i = 0; i < ParameterCount(instruction.operation); i++) {
    ip_++;
    instruction.parameters.push_back(ReadMem(ip_));
    instruction.parameter_modes.push_back(static_cast<ParameterMode>(opcode%10));
    opcode /= 10;
  }
  ip_++;
  current_instruction_ = instruction;
}

int64_t IntCode::GetAddress(int64_t index) const {
  int64_t param = current_instruction_.parameters[index];
  switch (current_instruction_.parameter_modes[index]) {
    case POSITION:
      return param;
    case RELATIVE:
      return relative_base_+param;
  }
  std::cerr << instance_ << ": Cannot use IMMEDIATE mode for write address" << std::endl;
  exit(1);
}

int64_t IntCode::GetArg(int64_t index) {
  int64_t param = current_instruction_.parameters[index];
  if (current_instruction_.parameter_modes[index] == IMMEDIATE) {
    return param;
  }
  return ReadMem(GetAddress(index));
}

void IntCode::Run() {
  while (!halted_) {
    switch (current_instruction_.operation) {
    case ADD:
      WriteMem(GetAddress(2), GetArg(0) + GetArg(1));
      break;
    case MULTIPLY:
      WriteMem(GetAddress(2), GetArg(0) * GetArg(1));
      break;
    case INPUT:
      return;
    case OUTPUT:
    {
      if (outputter_ == nullptr) {
        std::cerr << instance_ << ": No outputter set for OUTPUT instruction." << std::endl;
        exit(1);
      }
      // If the output causes Input or Run to be called, we need the instruction
      // to be advanced else it will repeatedly output the same thing.
      int64_t output = GetArg(0);
      NextInstruction();
      outputter_(output);
      // We also need to not advance the instruction (at the end of the switch),
      // so we continue the while loop
      continue;
    }
    case JUMP_IF_TRUE:
      if (GetArg(0) != 0) {
        ip_ = GetArg(1);
      }
      break;
    case JUMP_IF_FALSE:
      if (GetArg(0) == 0) {
        ip_ = GetArg(1);
      }
      break;
    case LESS_THAN:
      WriteMem(GetAddress(2), GetArg(0) < GetArg(1));
      break;
    case EQUALS:
      WriteMem(GetAddress(2), GetArg(0) == GetArg(1));
      break;
    case ADJUST_RELATIVE_BASE:
      relative_base_ += GetArg(0);
      break;
    case HALT:
      halted_ = true;
      return;
    default:
      std::cerr << instance_ << ": Invalid opcode " << current_instruction_.operation << std::endl;
      exit(1);
    }
    NextInstruction();
  }
}

void IntCode::SendInput(int64_t input) {
  if (current_instruction_.operation != INPUT) {
    Run();
    if (halted_) {
      std::cerr << instance_ << ": Cannot send input; program has halted" << std::endl;
      // exit(1);
    } else if (current_instruction_.operation != INPUT) {
      std::cerr << instance_ << ": Cannot send input; current opcode is " << current_instruction_.operation << std::endl;
      exit(1);
    }
  }
  WriteMem(GetAddress(0), input);
  NextInstruction();
  Run();
}

bool IntCode::IsHalted() const {
  return halted_;
}

} // namespace intcode
