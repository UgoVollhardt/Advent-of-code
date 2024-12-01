#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

typedef std::vector<std::string> Pattern;
typedef std::vector<Pattern> Input;

long getResPart1(std::string fileName);
long getResPart2(std::string fileName);
Input parseInput(std::string fileName);
int getMirrorAxeH(Pattern pattern);
int getMirrorAxeV(Pattern pattern);
int getMirrorAxeHWSmudge(Pattern pattern);
int getMirrorAxeVWSmudge(Pattern pattern);

int main() {
    std::cout << "--Part 1--" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << getResPart1("input") << std::endl;
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time : "
              << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "µs"
              << std::endl;
    std::cout << "--Part 2--" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    std::cout << getResPart2("input") << std::endl;
    stop = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time : "
              << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "µs"
              << std::endl;
}

long getResPart1(std::string fileName) {
    auto input = parseInput(fileName);
    long sum = 0;
    int index = 0;
    for (auto elem : input) {
        index = getMirrorAxeV(elem);
        if (index != -1) {
            sum += index;
        } else {
            sum += getMirrorAxeH(elem) * 100;
        }
    }
    return sum;
}

long getResPart2(std::string fileName) {
    long sum = 0;
    auto input = parseInput(fileName);
    int index = 0;
    for (auto elem : input) {
        index = getMirrorAxeVWSmudge(elem);
        if (index != -1) {
            sum += index;
        } else {
            sum += getMirrorAxeHWSmudge(elem) * 100;
        }
    }
    return sum;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::string line;
    Input res;
    Pattern current;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            if (!line.empty()) {
                current.emplace_back(line);
            } else {
                res.push_back(current);
                current.clear();
            }
        }
        res.push_back(current);
    }
    return res;
}

int getMirrorAxeH(Pattern pattern) {
    bool flag = true;
    for (int i = 1; i < pattern.size(); ++i) {
        for (int j = 0; j < pattern[i].size(); ++j) {
            for (int k = 0; i + k < pattern.size() && i - (k + 1) >= 0; ++k) {
                if (pattern[i + k][j] != pattern[i - (k + 1)][j]) {
                    flag = false;
                    break;
                }
            }
            if (!flag) break;
        }
        if (!flag) {
            flag = true;
        } else {
            return i;
        }
    }
    return -1;
}

int getMirrorAxeV(Pattern pattern) {
    bool flag = true;
    for (int j = 1; j < pattern[0].size(); ++j) {
        for (int i = 0; i < pattern.size(); ++i) {
            for (int k = 0; j - (k + 1) >= 0 && j + k < pattern[i].size(); ++k) {
                if (pattern[i][j - (k + 1)] != pattern[i][j + k]) {
                    flag = false;
                    break;
                }
            }
            if (!flag) break;
        }
        if (!flag) {
            flag = true;
        } else {
            return j;
        }
    }
    return -1;
}

int getMirrorAxeHWSmudge(Pattern pattern) {
    bool flag = true;
    std::vector<int> smudges(pattern.size(), 0);
    for (int i = 1; i < pattern.size(); ++i) {
        for (int j = 0; j < pattern[i].size(); ++j) {
            for (int k = 0; i + k < pattern.size() && i - (k + 1) >= 0; ++k) {
                if (pattern[i + k][j] != pattern[i - (k + 1)][j]) {
                    smudges[i]++;
                }
            }
        }
    }
    for (int i = 0; i < smudges.size(); ++i) {
        // std::cout << smudges[i] << " ";
        if (smudges[i] == 1) {
            return i;
        }
    }
    return -1;
}

int getMirrorAxeVWSmudge(Pattern pattern) {
    bool flag = true;
    std::vector<int> smudges(pattern.front().size(), 0);
    for (int j = 1; j < pattern[0].size(); ++j) {
        for (int i = 0; i < pattern.size(); ++i) {
            for (int k = 0; j - (k + 1) >= 0 && j + k < pattern[i].size(); ++k) {
                if (pattern[i][j - (k + 1)] != pattern[i][j + k]) {
                    smudges[j]++;
                }
            }
        }
    }
    for (int i = 0; i < smudges.size(); ++i) {
        if (smudges[i] == 1) {
            return i;
        }
    }
    return -1;
}