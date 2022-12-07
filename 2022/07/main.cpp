#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

#include "elemfilesystem.h"

void tokenize(std::string const &str, const char delim,
              std::vector<std::string> &out);
std::shared_ptr<ElemFilesystem> parseInput(std::string name);
int sumFolderLessThan100K(std::shared_ptr<ElemFilesystem> root);
int findClosestTo30MSize(std::shared_ptr<ElemFilesystem> root);

int main() {
  auto root = parseInput("input");
  std::cout << "exo 1 : " << sumFolderLessThan100K(root) << std::endl;
  std::cout << "exo 2 : " << findClosestTo30MSize(root) << std::endl;
}

std::shared_ptr<ElemFilesystem> parseInput(std::string name) {
  std::ifstream file(name);
  std::string line;
  std::vector<std::string> tokenizedLine;
  std::shared_ptr<ElemFilesystem> root = std::shared_ptr<ElemFilesystem>(
      new ElemFilesystem("/", ElemFilesystem::ElemType::kDir));
  std::shared_ptr<ElemFilesystem> currentDir = root;
  const char delim = ' ';
  if (file.is_open()) {
    while (file.good()) {
      std::getline(file, line);
      tokenize(line, delim, tokenizedLine);
      if (tokenizedLine[0] == "$") {
        if (tokenizedLine[1] == "cd") {
          if (tokenizedLine[2] == "..") {
            auto parent = currentDir.get()->getParent();
            if (parent != nullptr) {
              currentDir = parent;
            }
          } else if (tokenizedLine[2] == "/") {
            currentDir = root;
          } else {
            auto child = currentDir.get()->getChild(tokenizedLine[2]);
            if (child != nullptr) {
              currentDir = child;
            }
          }
        }
      } else {
        if (tokenizedLine[0] == "dir") {
          auto newChild = std::shared_ptr<ElemFilesystem>(new ElemFilesystem(
              tokenizedLine[1], ElemFilesystem::ElemType::kDir, currentDir));
          currentDir->addChild(newChild);
        } else {
          auto newChild = std::shared_ptr<ElemFilesystem>(new ElemFilesystem(
              tokenizedLine[1], ElemFilesystem::ElemType::kFile, currentDir,
              std::stoi(tokenizedLine[0])));
          currentDir->addChild(newChild);
        }
      }
      tokenizedLine.clear();
    }
  }
  return root;
}

void tokenize(std::string const &str, const char delim,
              std::vector<std::string> &out) {
  std::stringstream ss(str);

  std::string s;
  while (std::getline(ss, s, delim)) {
    out.push_back(s);
  }
};

int sumFolderLessThan100K(std::shared_ptr<ElemFilesystem> root) {
  auto allRootFolder = root->getFolders(true);
  int sum = 0;
  for (size_t i = 0; i < allRootFolder.size(); i++) {
    int sizeFolder = allRootFolder[i]->getSize();
    if (sizeFolder <= 100000) {
      sum += sizeFolder;
    }
  }
  return sum;
}

int findClosestTo30MSize(std::shared_ptr<ElemFilesystem> root) {
  auto allRootFolder = root->getFolders(true);
  int spaceToFree = 30000000 - (70000000 - root->getSize());
  int minDistance = std::numeric_limits<int>::max();
  int sizeFolderToDelete = 0;
  for (size_t i = 0; i < allRootFolder.size(); i++) {
    int sizeFolder = allRootFolder[i]->getSize();
    if (sizeFolder >= spaceToFree) {
      if (std::abs(sizeFolder - spaceToFree) < minDistance) {
        sizeFolderToDelete = sizeFolder;
        minDistance = sizeFolderToDelete - spaceToFree;
      }
    }
  }
  return sizeFolderToDelete;
}