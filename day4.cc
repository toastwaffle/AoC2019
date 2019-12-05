#include <string>
#include <cmath>
#include <iostream>

bool IsValid(const int& password) {
  std::string as_string = std::to_string(password);

  if (
    as_string.find("00") == std::string::npos &&
    as_string.find("11") == std::string::npos &&
    as_string.find("22") == std::string::npos &&
    as_string.find("33") == std::string::npos &&
    as_string.find("44") == std::string::npos &&
    as_string.find("55") == std::string::npos &&
    as_string.find("66") == std::string::npos &&
    as_string.find("77") == std::string::npos &&
    as_string.find("88") == std::string::npos &&
    as_string.find("99") == std::string::npos
  ) {
    return false;
  }

  for (int i = 5; i > 0; i--) {
    int digit1 = (password / (int)pow(10, i)) % 10;
    int digit2 = (password / (int)pow(10, i-1)) % 10;
    if (digit1 > digit2) {
      return false;
    }
  }

  return true;
}

int main() {
  int count = 0;
  for (int password = 193651; password <= 649729; password++) {
    if (IsValid(password)) {
      count++;
    }
  }
  std::cout << count << std::endl;
}
