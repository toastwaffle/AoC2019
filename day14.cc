#include <utility>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>

#include "absl/strings/str_split.h"

typedef std::pair<std::string, int64_t> Requirement;

struct Ingredient {
  std::string name;

  // How many recipes this ingredient is used in;
  int64_t required_for = 0;
  // How many of the above recipes we've processed; if equal to the number above
  // we can start processing this ingredient's recipe.
  int64_t recipes_processed = 0;
  // How many units of this ingredient are required for the recipes above;
  int64_t count_required = 0;

  // How many units of this ingredient are produced by the recipe below;
  int64_t recipe_produces;

  std::vector<Requirement> recipe;
};

Requirement ParseRequirement(const std::string& input) {
  std::vector<std::string> parts = absl::StrSplit(input, " ");
  int64_t count = std::stol(parts[0]);
  return Requirement(parts[1], count);
}

std::map<std::string, Ingredient*> ReadInput() {
  std::ifstream file("/home/samuel/aoc/day14.txt");
  std::string line;
  std::map<std::string, Ingredient*> ingredients;
  while (std::getline(file, line)) {
    std::vector<std::string> parts = absl::StrSplit(line, " => ");
    auto product = ParseRequirement(parts[1]);
    Ingredient* ingredient = new Ingredient;
    ingredient->name = product.first;
    ingredient->recipe_produces = product.second;
    std::vector<std::string> requirement_parts = absl::StrSplit(parts[0], ", ");
    for (const auto& requirement : requirement_parts) {
      ingredient->recipe.push_back(ParseRequirement(requirement));
    }
    ingredients[ingredient->name] = ingredient;
  }
  for (auto [name, ingredient] : ingredients) {
    for (const auto& requirement : ingredient->recipe) {
      if (requirement.first != "ORE") ingredients[requirement.first]->required_for++;
    }
  }
  return ingredients;
}

int64_t OreRequired(const std::map<std::string, Ingredient*>& ingredients, int64_t fuel_required) {
  int64_t ore_required = 0;
  std::queue<Ingredient*> queue;
  ingredients.at("FUEL")->count_required = fuel_required;
  queue.push(ingredients.at("FUEL"));
  while (!queue.empty()) {
    auto ingredient = queue.front();
    queue.pop();
    int64_t multiplier = ingredient->count_required / ingredient->recipe_produces;
    if (ingredient->count_required % ingredient->recipe_produces != 0) {
      multiplier++;
    }
    for (const auto& req : ingredient->recipe) {
      if (req.first == "ORE") {
        ore_required += req.second * multiplier;
      } else {
        auto ing = ingredients.at(req.first);
        ing->recipes_processed++;
        ing->count_required += req.second * multiplier;

        if (ing->recipes_processed == ing->required_for) {
          queue.push(ing);
        }
      }
    }
  }
  return ore_required;
}

int main() {
  int64_t fuel_required;
  while (true) {
    std::cin >> fuel_required;
    auto recipes = ReadInput();
    std::cout << OreRequired(recipes, fuel_required) << std::endl;
  }
}
