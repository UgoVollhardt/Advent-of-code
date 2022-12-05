#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
  std::ifstream file("input");
  std::vector<int> caloriesCarriedByElf;
  std::string line;
  int currentAccVal = 0;
  int currentVal = 0;
  if (file.is_open()) {
    while (file.good()) {
      std::getline(file, line);
      if (line.empty()) {
        caloriesCarriedByElf.push_back(currentAccVal);
        currentAccVal = 0;
      } else {
        currentVal = std::stoi(line);
        currentAccVal += currentVal;
      }
    }
  } else {
    std::cout << "fichier non trouvé";
  }
  std::sort(caloriesCarriedByElf.begin(), caloriesCarriedByElf.end(),
            std::greater<int>());
  int value = 0;
  if (caloriesCarriedByElf.size() > 3) {
    for (int i = 0; i < 3; ++i) {
      value += caloriesCarriedByElf[i];
    }
  }
  std::cout << value << std::endl;
}