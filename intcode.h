#pragma once

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

class IntCode {
private:
  std::string instance;
  std::vector<int> memory;
  uint ip;
  bool halted;
  std::function<void(int)> outputter;
  Instruction currentInstruction;

  int GetArg(int parameterIndex);
  void NextInstruction();

public:
  IntCode(const std::string instance, const std::string& filename);
  void SetOutputter(std::function<void(int)> outputter);
  void Run();
  void SendInput(int input);
  bool IsHalted();
};

} // namespace intcode
