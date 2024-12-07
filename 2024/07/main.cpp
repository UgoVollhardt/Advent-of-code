#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

typedef struct line {
    enum oper { mul, add, conc };

    size_t total;
    std::vector<int> numbers;
} Line;

typedef std::vector<Line> Input;

Input parseInput(std::string fileName);
std::unordered_map<int, std::vector<std::vector<Line::oper>>> combosOfN(
    const std::vector<Line::oper>& a, int n);

int getMaxSize(Input input);

size_t grayStar(Input input);
size_t goldStar(Input input);

int main(int, char*[]) {
    auto input = parseInput("input");
    std::cout << "res gray star : " << grayStar(input) << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
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
    size_t lineId = 0;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            if (!line.empty()) {
                std::string value;
                std::stringstream ss(line);
                Line lineElem;
                std::getline(ss, value, ':');
                lineElem.total = std::stoul(value);
                while (std::getline(ss, value, ' ')) {
                    if (!value.empty()) {
                        lineElem.numbers.emplace_back(std::stoi(value));
                    }
                }
                res.emplace_back(lineElem);
            }
        }
    } else {
        std::cout << "fichier non trouvé";
    }
    return res;
}

std::unordered_map<int, std::vector<std::vector<Line::oper>>> combosOfN(
    const std::vector<Line::oper>& a, int n) {
    std::unordered_map<int, std::vector<std::vector<Line::oper>>> res;
    for (int i = 1; i <= n; i++) {
        if (res.find(i - 1) != res.end()) {
            std::vector<std::vector<Line::oper>> temp;
            for (const auto& e : res[i - 1]) {
                for (const auto& n : a) {
                    std::vector<Line::oper> new_combination = e;
                    new_combination.push_back(n);
                    temp.push_back(new_combination);
                }
            }
            res[i] = temp;
        } else {
            for (const auto& e : a) {
                res[i].push_back({e});
            }
        }
    }
    return res;
}

int getMaxSize(Input input) {
    int res = 0;
    for (auto elem : input) {
        if (elem.numbers.size() > res) {
            res = elem.numbers.size();
        }
    }
    return res;
}

size_t grayStar(Input input) {
    size_t res = 0;
    size_t total = 0;
    bool flag = false;
    std::vector<line::oper> operVect = {line::add, line::mul};
    auto mapCombos = combosOfN(operVect, getMaxSize(input) - 1);
    for (auto line : input) {
        auto arr = mapCombos.find(line.numbers.size() - 1);
        if (arr != mapCombos.end()) {
            flag = false;
            for (auto&& it : arr->second) {
                total = line.numbers[0];
                for (int i = 0; i < it.size(); ++i) {
                    switch (it[i]) {
                        case Line::mul:
                            total *= line.numbers[i + 1];
                            break;
                        case Line::add:
                            total += line.numbers[i + 1];
                            break;
                        default:
                            break;
                    }
                }
                if (total == line.total) {
                    flag = true;
                    break;
                }
            }
            if (flag) {
                res += line.total;
            }
        }
    }
    return res;
}

size_t goldStar(Input input) {
    size_t res = 0;
    size_t total = 0;
    bool flag = false;
    std::vector<line::oper> operVect = {line::add, line::mul, line::conc};
    auto mapCombos = combosOfN(operVect, getMaxSize(input) - 1);
    for (auto line : input) {
        auto arr = mapCombos.find(line.numbers.size() - 1);
        if (arr != mapCombos.end()) {
            flag = false;
            for (auto&& it : arr->second) {
                total = line.numbers[0];
                for (int i = 0; i < it.size(); ++i) {
                    switch (it[i]) {
                        case Line::mul:
                            total *= line.numbers[i + 1];
                            break;
                        case Line::add:
                            total += line.numbers[i + 1];
                            break;
                        case Line::conc:
                            total *= std::pow(10, std::floor(std::log10(line.numbers[i + 1]) + 1));
                            total += line.numbers[i + 1];
                            break;
                        default:
                            break;
                    }
                }
                if (total == line.total) {
                    flag = true;
                    break;
                }
            }
            if (flag) {
                res += line.total;
            }
        }
    }
    return res;
}