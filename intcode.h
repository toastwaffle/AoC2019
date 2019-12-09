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
  std::vector<long int> parameters;
  std::vector<ParameterMode> parameterModes;
};

class IntCode {
private:
  std::string instance;
  std::vector<long int> memory;
  uint ip;
  bool halted;
  std::function<void(long int)> outputter;
  Instruction currentInstruction;
  int relativeBase;

  uint GetAddress(uint index);
  long int GetArg(uint parameterIndex);
  void NextInstruction();
  void WriteMem(uint address, long int value);
  long int ReadMem(uint address);
  void EnsureMem(uint address);

public:
  IntCode(const std::string instance, const std::string& filename);
  void SetOutputter(std::function<void(long int)> outputter);
  void Run();
  void SendInput(long int input);
  bool IsHalted();
};

} // namespace intcode
