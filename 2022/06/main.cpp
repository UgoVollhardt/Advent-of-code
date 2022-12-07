#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <vector>

int getFirst4diff(std::string fileName, int size);

int main() {
  std::cout << getFirst4diff("input-test1", 4) << std::endl;
  std::cout << getFirst4diff("input-test2", 4) << std::endl;
  std::cout << getFirst4diff("input-test3", 4) << std::endl;
  std::cout << getFirst4diff("input-test4", 4) << std::endl;
  std::cout << getFirst4diff("input", 4) << std::endl;
  std::cout << getFirst4diff("input", 14) << std::endl;
}

int getFirst4diff(std::string name, int size) {
  std::ifstream file(name);
  std::string line;

  std::set<char> subString;
  bool flag = true;

  if (file.is_open()) {
    while (file.good()) {
      std::getline(file, line);
      if (line.size() <= size) return 0;

      for (size_t i = size; i < line.size(); i++) {
        subString = std::set<char>(line.begin() + i - size, line.begin() + i);
        for (auto &&i : subString) {
          // std::cout << i;
        }
        // std::cout << std::endl;

        if (subString.size() == size) {
          return i;
        }
      }
    }
  }
  return 0;
}
