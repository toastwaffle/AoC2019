#pragma once

namespace intcode {

int RunProgram(const std::string& filename, std::vector<int>& inputs, std::vector<int>& outputs);
int RunProgram(const std::string& filename, int noun, int verb, std::vector<int>& inputs, std::vector<int>& outputs);

} // namespace intcode
