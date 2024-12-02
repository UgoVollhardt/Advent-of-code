#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

typedef std::vector<std::vector<int>> Input;

std::vector<int> parseLine(std::string inputLine);
Input parseInput(std::string fileName);
size_t grayStar(Input input);
size_t goldStar(Input input);

bool isSafe(std::vector<int> report);
bool isSafeMargin(std::vector<int> report, bool firstAnalysis);
std::pair<int, size_t> countIrregularities(std::vector<bool> reportAnalysis);

int main(int, char *[]) {
    auto input = parseInput("input");
    std::cout << "res gray star : " << grayStar(input) << std::endl;
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
                res.emplace_back(parseLine(line));
            }
        }
    } else {
        std::cout << "fichier non trouvé";
    }
    return res;
}

std::vector<int> parseLine(std::string inputLine) {
    std::stringstream ss(inputLine);
    std::string value;
    std::vector<int> container;
    while (getline(ss, value, ' ')) {
        if (!value.empty()) {
            container.push_back(std::stoi(value));
        }
    }
    return container;
}

size_t grayStar(Input input) {
    size_t res = 0;
    for (auto report : input) {
        res += size_t(isSafe(report));
    }
    return res;
}

bool isSafe(std::vector<int> report) {
    enum Direction { undef, inc, dec };
    Direction current = Direction::undef;
    for (size_t i = 0; i < report.size(); ++i) {
        switch (current) {
            case Direction::undef:
                if (i + 1 < report.size()) {
                    int diff = std::abs(report[i + 1] - report[i]);
                    if (diff > 3 || diff < 1) {
                        return false;
                    }
                    current = report[i] < report[i + 1] ? Direction::inc : Direction::dec;
                }
                break;
            case Direction::inc:
                if (i + 1 < report.size()) {
                    int diff = std::abs(report[i + 1] - report[i]);
                    if (diff > 3 || diff < 1) {
                        return false;
                    }
                    if (report[i] > report[i + 1]) {
                        return false;
                    }
                }
                break;
            case Direction::dec:
                if (i + 1 < report.size()) {
                    int diff = std::abs(report[i + 1] - report[i]);
                    if (diff > 3 || diff < 1) {
                        return false;
                    }
                    if (report[i] < report[i + 1]) {
                        return false;
                    }
                }
                break;

            default:
                break;
        }
    }
    return true;
}

size_t goldStar(Input input) {
    size_t res = 0;
    for (auto report : input) {
        auto flag = isSafeMargin(report, true);
        res += size_t(flag);
    }
    return res;
}

bool isSafeMargin(std::vector<int> report, bool firstAnalysis) {
    enum Direction { undef, inc, dec };
    Direction current = Direction::undef;
    std::vector<bool> reportAnalysis;
    bool flag = false;
    for (size_t i = 0; i < report.size(); ++i) {
        switch (current) {
            case Direction::undef:
                if (i + 1 < report.size()) {
                    int diff = std::abs(report[i + 1] - report[i]);
                    if (diff > 3 || diff < 1) {
                        reportAnalysis.push_back(false);
                    } else {
                        reportAnalysis.push_back(true);
                    }

                    if (report[i] != report[i + 1]) {
                        current = report[i] < report[i + 1] ? Direction::inc : Direction::dec;
                    }
                }
                break;
            case Direction::inc:
                if (i + 1 < report.size()) {
                    int diff = std::abs(report[i + 1] - report[i]);
                    if (diff > 3 || diff < 1) {
                        reportAnalysis.push_back(false);
                    } else if (report[i] > report[i + 1]) {
                        reportAnalysis.push_back(false);
                    } else {
                        reportAnalysis.push_back(true);
                    }
                }
                break;
            case Direction::dec:
                if (i + 1 < report.size()) {
                    int diff = std::abs(report[i + 1] - report[i]);
                    if (diff > 3 || diff < 1) {
                        reportAnalysis.push_back(false);
                    } else if (report[i] < report[i + 1]) {
                        reportAnalysis.push_back(false);
                    } else {
                        reportAnalysis.push_back(true);
                    }
                }
                break;

            default:
                break;
        }
    }
    auto irregularities = countIrregularities(reportAnalysis);

    if (irregularities.first == 0) {
        flag = true;
    } else if (firstAnalysis) {
        for (auto offset : {-1, 0, 1}) {
            auto reportTruncate = report;
            int index = irregularities.second + offset;
            if (index >= 0 && index <= report.size()) {
                reportTruncate.erase(reportTruncate.begin() + index);
                flag = flag || isSafeMargin(reportTruncate, false);
            }
        }
    }
    return flag;
}

std::pair<int, size_t> countIrregularities(std::vector<bool> reportAnalysis) {
    int res = 0;
    size_t index = reportAnalysis.size();
    for (size_t i = 0; i < reportAnalysis.size(); ++i) {
        res += int(!reportAnalysis[i]);
        if (index == reportAnalysis.size() && !reportAnalysis[i]) {
            index = i;
        }
    }
    return std::make_pair(res, index);
}