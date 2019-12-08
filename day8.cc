#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

std::string ReadFile() {
  std::ifstream file("/home/samuel/aoc/day8.txt");
  std::stringstream buffer;
  buffer << file.rdbuf();
  auto str = buffer.str();
  // remove trailing newline
  return str.substr(0, str.size()-1);
}

std::vector<std::string> SplitLayers(const std::string& content) {
  std::vector<std::string> layers;
  for (uint i = 0; i < content.size(); i += 150) {
    layers.push_back(content.substr(i, 150));
  }
  return layers;
}

int CountDigit(const std::string& layer, const std::string digit) {
  auto pos = layer.find(digit);
  int count = 0;
  while (pos != std::string::npos) {
    count++;
    pos = layer.find(digit, pos+1);
  }
  return count;
}

std::string LayerWithFewestZeroes(const std::vector<std::string>& layers) {
  std::string layerWithFewestZeroes;
  int zeroesCount = std::numeric_limits<int>::max();
  for (const auto& layer : layers) {
    int count = CountDigit(layer, "0");
    if (count < zeroesCount) {
      zeroesCount = count;
      layerWithFewestZeroes = layer;
    }
  }
  return layerWithFewestZeroes;
}

std::string BuildImage(const std::vector<std::string>& layers) {
  std::stringstream ss;
  for (int y = 0; y < 6; y++) {
    for (int x = 0; x < 25; x++) {
      int i = y*25+x;
      bool found = false;
      for (const auto& layer : layers) {
        switch (layer.at(i)) {
          case '0':
            ss << " ";
            break;
          case '1':
            ss << "+";
            break;
          case '2':
            continue;
        }
        found = true;
        break;
      }
      if (!found) {
        ss << " ";
      }
    }
    ss << "\n";
  }
  return ss.str();
}

int main() {
  auto layers = SplitLayers(ReadFile());
  auto layer = LayerWithFewestZeroes(layers);
  std::cout << CountDigit(layer, "1")*CountDigit(layer, "2") << std::endl;
  std::cout << BuildImage(layers);
}
