#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

int intersectAllCount(std::string name);
int intersectPartialCount(std::string name);
std::vector<std::vector<char>> parseInitialSetup(std::string name);

int main() {
  // std::cout << intersectAllCount("input") << std::endl;
  // std::cout << intersectPartialCount("input") << std::endl;
  parseInitialSetup("input");
}

std::vector<std::vector<char>> parseInitialSetup(std::string name) {
  std::vector<std::vector<char>> out;
  /*out.push_back({'Z', 'N'});
  out.push_back({'M', 'C', 'D'});
  out.push_back({'P'});*/
  out.push_back({'F', 'D', 'B', 'Z', 'T', 'J', 'R', 'N'});
  out.push_back({'R', 'S', 'N', 'J', 'H'});
  out.push_back({'C', 'R', 'N', 'J', 'G', 'Z', 'F', 'Q'});
  out.push_back({'F', 'V', 'N', 'G', 'R', 'T', 'Q'});
  out.push_back({'L', 'T', 'Q', 'F'});
  out.push_back({'Q', 'C', 'W', 'Z', 'B', 'R', 'G', 'N'});
  out.push_back({'F', 'C', 'L', 'S', 'N', 'H', 'M'});
  out.push_back({'D', 'N', 'Q', 'M', 'T', 'J'});
  out.push_back({'P', 'G', 'S'});

  std::ifstream file(name);
  std::string line;
  std::regex itemRegex("[0-9]+");
  std::vector<int> res;
  bool flag = true;

  if (file.is_open()) {
    while (file.good()) {
      std::getline(file, line);
      flag = !line.empty();
      res.clear();
      std::sregex_token_iterator rend;
      std::sregex_token_iterator iter(line.begin(), line.end(), itemRegex);
      while (iter != rend) {
        int num = std::stoi(*iter++);
        // std::cout << num << ",";
        res.push_back(num);
      }
      /*std::cout << std::endl;
      std::cout << res[0] << res[1] << res[2] << std::endl;*/
      std::cout << line << std::endl;
      std::vector toMove(
          out[res[1] - 1].begin() + out[res[1] - 1].size() - res[0],
          out[res[1] - 1].end());
      for (size_t i = 0; i < out[res[1] - 1].size(); i++) {
        std::cout << out[res[1] - 1][i] << " ";
      }
      std::cout << std::endl;
      for (size_t i = 0; i < toMove.size(); i++) {
        std::cout << toMove[i] << " ";
      }
      std::cout << std::endl;

      out[res[2] - 1].insert(out[res[2] - 1].end(), toMove.begin(),
                             toMove.end());
      for (size_t i = 0; i < res[0]; ++i) {
        // int num = out[res[1] - 1].back();
        out[res[1] - 1].pop_back();
        // out[res[2] - 1].push_back(num);
        /*std::cout << num << " moved from " << res[1] << " to " << res[2]
                  << std::endl;
        for (size_t i = 0; i < out.size(); i++) {
          for (size_t j = 0; j < out[i].size(); j++) {
            std::cout << out[i][j] << " ";
          }
          std::cout << std::endl;
        }*/
      }
      /*for (size_t i = 0; i < out.size(); i++) {
        for (size_t j = 0; j < out[i].size(); j++) {
          std::cout << out[i][j] << " ";
        }
        std::cout << std::endl;
      }*/

      // std::cout << std::endl;
      /*std::copy(
          std::sregex_token_iterator(line.begin(), line.end(), itemRegex, -1),
          std::sregex_token_iterator(),
          std::ostream_iterator<std::string>(std::cout, "\n"));*/

      /*while (std::regex_search(line, itemMatchs, itemRegex)) {
        std::cout << "matches for '" << line << "'\n";
        std::cout << "Prefix: '" << itemMatchs.prefix() << "'\n";
        for (size_t i = 0; i < itemMatchs.size(); ++i)
          std::cout << i << ": " << itemMatchs[i] << '\n';
        std::cout << "Suffix: '" << itemMatchs.suffix() << "\'\n\n";
      }*/
    }
  }
  for (size_t i = 0; i < out.size(); i++) {
    std::cout << out[i].back();
  }
  std::cout << std::endl;
  return out;
}

int intersectAllCount(std::string name) {
  std::ifstream file(name);
  std::string line, subLeft, subRight, begin, end;
  std::vector<int> left, right;
  int sum = 0;
  if (file.is_open()) {
    while (file.good()) {
      left.clear();
      right.clear();
      std::getline(file, subLeft, ',');
      std::getline(file, subRight);

      std::istringstream str(subLeft);
      std::getline(str, begin, '-');
      std::getline(str, end);
      for (int i = std::stoi(begin); i <= std::stoi(end); ++i) {
        left.push_back(i);
      }

      str = std::istringstream(subRight);
      std::getline(str, begin, '-');
      std::getline(str, end);
      for (int i = std::stoi(begin); i <= std::stoi(end); ++i) {
        right.push_back(i);
      }

      if (std::search(left.begin(), left.end(), right.begin(), right.end()) !=
              left.end() ||
          std::search(right.begin(), right.end(), left.begin(), left.end()) !=
              right.end()) {
        sum += 1;
      }
    }
  }
  return sum;
}

int intersectPartialCount(std::string name) {
  std::ifstream file(name);
  std::string line, subLeft, subRight, begin, end;
  std::vector<int> left, right, intersect;
  int sum = 0;
  if (file.is_open()) {
    while (file.good()) {
      left.clear();
      right.clear();
      begin.clear();
      end.clear();
      std::getline(file, subLeft, ',');
      std::getline(file, subRight);

      std::istringstream str(subLeft);
      std::getline(str, begin, '-');
      std::getline(str, end);
      for (int i = std::stoi(begin); i <= std::stoi(end); ++i) {
        left.push_back(i);
      }

      begin.clear();
      end.clear();

      str = std::istringstream(subRight);
      std::getline(str, begin, '-');
      std::getline(str, end);
      for (int i = std::stoi(begin); i <= std::stoi(end); ++i) {
        right.push_back(i);
      }

      for (size_t i = 0; i < left.size(); i++) {
        std::cout << left[i] << " ";
      }
      std::cout << ", ";
      for (size_t i = 0; i < right.size(); i++) {
        std::cout << right[i] << " ";
      }
      intersect.clear();
      std::set_intersection(left.begin(), left.end(), right.begin(),
                            right.end(), std::back_inserter(intersect));
      std::cout << "intersect : ";
      for (size_t i = 0; i < intersect.size(); i++) {
        std::cout << intersect[i] << " ";
      }

      if (intersect.size()) {
        std::cout << " founded";
        sum += 1;
      }

      std::cout << std::endl;
    }
  }
  return sum;
}