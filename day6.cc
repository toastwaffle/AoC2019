#include <map>
#include <iostream>
#include <fstream>
#include <string>

int CountOrbits(const std::map<std::string, std::string>& orbits, std::string planet) {
  int count = 0;
  while (orbits.find(planet) != orbits.end()) {
    planet = orbits.at(planet);
    count++;
  }
  return count;
}

std::map<std::string, std::string> ReadInput() {
  std::ifstream file("/home/samuel/aoc/day6.txt");
  if (!file.is_open()) {
    std::cerr << "Failed to open file" << std::endl;
    exit(1);
  }
  std::string line;
  std::map<std::string, std::string> orbits;
  while (std::getline(file, line)) {
    orbits[line.substr(4)] = line.substr(0, 3);
  }
  return orbits;
}

std::map<std::string, int> Depths(const std::map<std::string, std::string>& orbits) {
  std::map<std::string, int> depths;
  for (const auto& [planet, unused] : orbits) {
    depths[planet] = CountOrbits(orbits, planet);
  }
  return depths;
}

int TotalOrbits(const std::map<std::string, int>& depths) {
  int total = 0;
  for (const auto& [unused, depth] : depths) {
    total += depth;
  }
  return total;
}

int TransferCount(const std::map<std::string, std::string>& orbits, const std::map<std::string, int>& depths) {
  int youDepth = depths.at("YOU");
  int santaDepth = depths.at("SAN");
  std::string you = orbits.at("YOU");
  std::string santa = orbits.at("SAN");
  int count = 0;
  if (youDepth > santaDepth) {
    count = youDepth-santaDepth;
    for (int i = 0; i < count; i++) {
      you = orbits.at(you);
    }
  } else if (santaDepth > youDepth) {
    count = santaDepth-youDepth;
    for (int i = 0; i < count; i++) {
      santa = orbits.at(santa);
    }
  }
  while (you != santa) {
    you = orbits.at(you);
    santa = orbits.at(santa);
    count += 2;
  }
  return count;
}

int main() {
  auto orbits = ReadInput();
  auto depths = Depths(orbits);
  std::cout << TotalOrbits(depths) << std::endl;
  std::cout << TransferCount(orbits, depths) << std::endl;
}
