#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

typedef std::vector<std::vector<long>> Measure;

typedef std::vector<Measure> Input;

int getSumNextMeasure(std::string fileName);
Input parseInput(std::string fileName);
std::vector<long> getData(std::string line);
bool isVectOfZero(std::vector<long> vect);
void generateDiffs(Input &input);
void computeNextStep(Input &input);
void computePreviousStep(Input &input);
int getSumPreviousMeasure(std::string fileName);

int main() {
    std::cout << "--Part 1--" << std::endl;
    std::cout << getSumNextMeasure("input") << std::endl;
    std::cout << "--Part 2--" << std::endl;
    std::cout << getSumPreviousMeasure("input") << std::endl;
}

int getSumNextMeasure(std::string fileName) {
    long sum = 0;
    auto input = parseInput(fileName);
    generateDiffs(input);
    computeNextStep(input);

    for (auto measure : input) {
        sum += measure[0].back();
    }
    return sum;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::string line, buffer;
    Input res;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            res.emplace_back(Measure{getData(line)});
        }
    }
    return res;
}

std::vector<long> getData(std::string line) {
    std::string buffer;
    std::vector<long> output;
    std::stringstream stream(line);
    while (getline(stream, buffer, ' ')) {
        if (!buffer.empty()) output.emplace_back(stol(buffer));
    }
    return output;
}

bool isVectOfZero(std::vector<long> vect) {
    if (vect.empty()) return false;
    for (auto elem : vect) {
        if (elem != 0) return false;
    }
    return true;
}

void generateDiffs(Input &input) {
    for (int i = 0; i < input.size(); ++i) {
        while (!isVectOfZero(input[i].back())) {
            std::vector<long> diff;
            for (int j = 0; j < input[i].back().size() - 1; ++j) {
                diff.push_back(input[i].back()[j + 1] - input[i].back()[j]);
            }
            input[i].emplace_back(diff);
        }
    }
}

void computeNextStep(Input &input) {
    for (int i = 0; i < input.size(); ++i) {
        for (int j = input[i].size() - 2; j >= 0; --j) {
            input[i][j].emplace_back(input[i][j].back() + input[i][j + 1].back());
        }
    }
}

void computePreviousStep(Input &input) {
    for (int i = 0; i < input.size(); ++i) {
        for (int j = input[i].size() - 2; j >= 0; --j) {
            input[i][j].insert(input[i][j].begin(), input[i][j].front() - input[i][j + 1].front());
        }
    }
}

int getSumPreviousMeasure(std::string fileName) {
    long sum = 0;
    auto input = parseInput(fileName);
    generateDiffs(input);
    computePreviousStep(input);

    for (auto measure : input) {
        sum += measure[0].front();
    }
    return sum;
}
