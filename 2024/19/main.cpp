#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Input {
    std::vector<std::string> pattern;
    std::vector<std::string> design;
    size_t designMaxSize;
};

Input parseInput(std::string fileName);
std::unordered_map<int, std::unordered_set<std::string>> permWithLimit(
    const std::vector<std::string>& a, int limit);
bool fitIn(std::string design, std::vector<std::string> parts);
size_t fitInMemory(std::string design, std::vector<std::string> parts,
                   std::unordered_map<std::string, size_t>& memory);

size_t grayStar(Input input);
size_t goldStar(Input input);

int main(int, char*[]) {
    auto input = parseInput("input");
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "res gray star : " << grayStar(input) << std::endl;
    std::cout << "res gold star : " << goldStar(input) << std::endl;

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms"
              << std::endl;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input res;
    std::string line;
    int status = 0;
    res.designMaxSize = 0;

    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            if (!line.empty()) {
                std::stringstream ss(line);
                std::string value;
                if (status == 0) {
                    while (std::getline(ss, value, ',')) {
                        res.pattern.push_back(value);
                        std::getline(ss, value, ' ');
                    }
                } else {
                    res.design.push_back(line);
                    if (line.size() > res.designMaxSize) {
                        res.designMaxSize = line.size();
                    }
                }
            } else {
                status = 1;
            }
        }
    } else {
        std::cout << "fichier non trouvé";
    }
    return res;
}

bool fitIn(std::string design, std::vector<std::string> parts) {
    if (design.size() == 0) {
        return true;
    }
    for (int i = 0; i < parts.size(); ++i) {
        if (parts[i].size() <= design.size() && parts[i] == design.substr(0, parts[i].size())) {
            if (fitIn(design.substr(parts[i].size()), parts)) {
                return true;
            }
        }
    }
    return false;
}

size_t fitInMemory(std::string design, std::vector<std::string> parts,
                   std::unordered_map<std::string, size_t>* memory) {
    size_t count = 0;
    if (design.size() == 0) {
        return 1;
    } else if (memory->contains(design)) {
        return memory->at(design);
    }
    for (int i = 0; i < parts.size(); ++i) {
        if (parts[i].size() <= design.size() && parts[i] == design.substr(0, parts[i].size())) {
            size_t subCount = fitInMemory(design.substr(parts[i].size()), parts, memory);
            memory->insert(std::make_pair(design.substr(parts[i].size()), subCount));
            count += subCount;
        }
    }
    return count;
}

size_t grayStar(Input input) {
    size_t res = 0;
    for (int i = 0; i < input.design.size(); ++i) {
        bool flag = fitIn(input.design[i], input.pattern);
        res += flag;
    }
    return res;
}

size_t goldStar(Input input) {
    size_t res = 0;
    std::unordered_map<std::string, size_t> memory;
    for (int i = 0; i < input.design.size(); ++i) {
        size_t count = fitInMemory(input.design[i], input.pattern, &memory);
        res += count;
    }
    return res;
}