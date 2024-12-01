#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

typedef struct {
    std::vector<int> firstList;
    std::vector<int> secondList;
} Input;

std::pair<int, int> parseLine(std::string inputLine);
Input parseInput(std::string fileName);
size_t greyStar(Input input);
size_t goldStar(Input input);

int main(int, char *[]) {
    auto input = parseInput("input");
    std::cout << "res gray star : " << greyStar(input) << std::endl;
    std::cout << "res gold star : " << goldStar(input) << std::endl;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input res;
    std::string line;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            if (!line.empty()) {
                auto pair = parseLine(line);
                res.firstList.push_back(pair.first);
                res.secondList.push_back(pair.second);
            }
        }
    } else {
        std::cout << "fichier non trouvé";
    }
    return res;
}

std::pair<int, int> parseLine(std::string inputLine) {
    std::stringstream ss(inputLine);
    std::string value;
    std::vector<int> container;
    while (getline(ss, value, ' ')) {
        if (!value.empty()) {
            container.push_back(std::stoi(value));
        }
    }
    if (container.size() == 2) {
        return std::make_pair(container[0], container[1]);
    }
    std::cerr << "issue in parsing ! Wrong size parsed : " << container.size() << std::endl;
    return std::pair<int, int>();
}

size_t greyStar(Input input) {
    size_t res = 0;
    std::sort(input.firstList.begin(), input.firstList.end());
    std::sort(input.secondList.begin(), input.secondList.end());

    if (input.firstList.size() == input.secondList.size()) {
        for (size_t i = 0; i < input.firstList.size(); ++i) {
            res += std::abs(input.firstList[i] - input.secondList[i]);
        }
    }
    return res;
}

size_t goldStar(Input input) {
    size_t res = 0;
    std::unordered_map<int, int> rightMap;

    for (auto val : input.secondList) {
        if (rightMap.contains(val)) {
            rightMap[val]++;
        } else {
            rightMap[val] = 1;
        }
    }

    for (auto val : input.firstList) {
        res += val * rightMap[val];
    }
    return res;
}