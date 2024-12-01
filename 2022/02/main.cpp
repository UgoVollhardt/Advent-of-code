#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int scoreWithGivenChoice(std::string file);
int scoreWithGivenOutcome(std::string fileName);

int main(int argc, char* argv[]) {
  std::cout << scoreWithGivenChoice("input") << std::endl;
  std::cout << scoreWithGivenOutcome("input") << std::endl;
};

int scoreWithGivenChoice(std::string fileName) {
  std::ifstream file(fileName);
  std::string line;
  int choiceAdv, choiceOwn, gain;
  int score = 0;
  int outcome = 0;
  if (file.is_open()) {
    while (file.good()) {
      std::getline(file, line);
      if (!line.empty()) {
        choiceAdv = line[0] - 'A';
        choiceOwn = line[2] - 'X';
        gain = choiceOwn + 1;
        outcome = choiceOwn - choiceAdv + 1;
        outcome = outcome >= 0 ? outcome % 3 : outcome + 3;
        gain += outcome * 3;
        score += gain;
      }
    }
    return score;
  } else {
    std::cout << "fichier non trouvé";
    return 0;
  }
}

int scoreWithGivenOutcome(std::string fileName) {
  std::ifstream file(fileName);
  std::string line;
  int choiceAdv, choiceOwn, gain;
  int score = 0;
  int outcome = 0;
  if (file.is_open()) {
    while (file.good()) {
      std::getline(file, line);
      if (!line.empty()) {
        choiceAdv = line[0] - 'A';
        outcome = line[2] - 'X';
        gain = outcome * 3;
        choiceOwn = choiceAdv + (outcome - 1);
        choiceOwn = choiceOwn > -1 ? choiceOwn % 3 : choiceOwn + 3;
        gain += choiceOwn + 1;
        score += gain;
      }
    }
    return score;
  } else {
    std::cout << "fichier non trouvé";
    return 0;
  }
}