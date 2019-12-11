#pragma once

#include <vector>
#include <functional>

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
  ADJUST_RELATIVE_BASE = 9,
  HALT = 99,
};

enum ParameterMode {
  POSITION = 0,
  IMMEDIATE = 1,
  RELATIVE = 2,
};

struct Instruction {
  Operation operation;
  std::vector<int64_t> parameters;
  std::vector<ParameterMode> parameter_modes;
};

class IntCode {
private:
  const std::string instance_;

  std::vector<int64_t> memory_;
  Instruction current_instruction_;

  int64_t ip_ = 0;
  bool halted_ = false;
  std::function<void(int64_t)> outputter_ = nullptr;
  int relative_base_ = 0;

  int64_t GetAddress(int64_t index) const;
  int64_t GetArg(int64_t parameter_index);
  void NextInstruction();
  void WriteMem(int64_t address, int64_t value);
  int64_t ReadMem(int64_t address);
  void EnsureMem(int64_t address);

public:
  IntCode(const std::string instance, const std::string& filename);
  void SetOutputter(std::function<void(int64_t)> outputter);
  void Run();
  void SendInput(int64_t input);
  bool IsHalted() const;
};

} // namespace intcode
