#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

enum spring { kOperational, kDamaged, kUnknown };

typedef struct {
    std::vector<spring> springs;
    std::vector<int> pattern;
} Row;

typedef std::vector<Row> Input;

std::map<spring, char> stc = {{kOperational, '.'}, {kDamaged, '#'}, {kUnknown, '?'}};

long getSumArrangements(std::string fileName, bool unfolded);
Input parseInput(std::string fileName);
Row parseLine(std::string line);
bool doesMatch(Row row, int pos, int patternIndex, int boundary);
bool doesMatchFirst(Row row, int pos, int patternIndex, int boundary);
bool doesMatchLast(Row row, int pos, int patternIndex, int boundary);
int getRightBoundary(std::vector<int> pattern, int pos);
long sumPreviousRow(std::vector<long> patternRow, Row row, int springIndex, int patternIndex);
int getFirstOfLine(std::vector<long> line);
Row unfold(Row row);

int main() {
    std::cout << "--Part 1--" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << getSumArrangements("input", false) << std::endl;
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms"
              << std::endl;
    std::cout << "--Part 2--" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    std::cout << getSumArrangements("input", true) << std::endl;
    stop = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms"
              << std::endl;
}

long getSumArrangements(std::string fileName, bool unfolded) {
    long sum = 0;
    auto input = parseInput(fileName);
    for (auto row : input) {
        if (unfolded) row = unfold(row);
        row.springs.push_back(
            kOperational);  // add a trailing '.' to facilitate later computation with boundaries
        std::vector<std::vector<long>> dp(row.pattern.size(),
                                          std::vector<long>(row.springs.size(), 0));
        for (int i = 0; i < row.pattern.size(); ++i) {
            int leftBondary = 0;
            int rightBondary = row.springs.size() - getRightBoundary(row.pattern, i);
            if (i != 0) {
                leftBondary = getFirstOfLine(dp[i - 1]) + row.pattern[i - 1] + 1;
            }
            for (int j = leftBondary; j < rightBondary; ++j) {
                if (i == 0) {
                    dp[i][j] = doesMatchFirst(row, j, i, rightBondary);
                } else if (i == row.pattern.size() - 1) {
                    dp[i][j] = int(doesMatchLast(row, j, i, rightBondary)) *
                               sumPreviousRow(dp[i - 1], row, j, i);
                } else {
                    dp[i][j] = int(doesMatch(row, j, i, rightBondary)) *
                               sumPreviousRow(dp[i - 1], row, j, i);
                }
            }
        }

        for (auto weight : dp.back()) {
            sum += weight;
        }
    }
    return sum;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::string line;
    Input res;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            res.emplace_back(parseLine(line));
        }
    }
    return res;
}

Row parseLine(std::string line) {
    Row ouput;
    std::string buffer;
    std::stringstream stream(line);
    getline(stream, buffer, ' ');
    for (auto elem : buffer) {
        if (elem == '.')
            ouput.springs.push_back(kOperational);
        else if (elem == '#')
            ouput.springs.push_back(kDamaged);
        else
            ouput.springs.push_back(kUnknown);
    }
    while (getline(stream, buffer, ',')) {
        if (!buffer.empty()) ouput.pattern.push_back(stoi(buffer));
    }
    return ouput;
}

bool doesMatch(Row row, int pos, int patternIndex, int boundary) {
    auto springs = row.springs;
    auto size = row.pattern[patternIndex];
    if (pos + size >= boundary) return false;
    if (pos > 0 && springs[pos - 1] == kDamaged) return false;
    if (springs[pos + size] == kDamaged) return false;
    for (int i = pos; i < pos + size; ++i) {
        if (springs[i] == kOperational) return false;
    }
    return true;
}

bool doesMatchFirst(Row row, int pos, int patternIndex, int boundary) {
    for (int i = 0; i < pos; ++i) {
        if (row.springs[i] == kDamaged) return false;
    }
    return doesMatch(row, pos, patternIndex, boundary);
}

bool doesMatchLast(Row row, int pos, int patternIndex, int boundary) {
    auto springs = row.springs;
    auto size = row.pattern[patternIndex];
    for (int i = pos + size; i < springs.size(); ++i) {
        if (springs[i] == kDamaged) return false;
    }
    return doesMatch(row, pos, patternIndex, boundary);
}

int getRightBoundary(std::vector<int> pattern, int pos) {
    int sum = 0;
    for (int i = pos + 1; i < pattern.size(); ++i) {
        sum += pattern[i] + 1;
    }
    return sum;
}

long sumPreviousRow(std::vector<long> patternRow, Row row, int springIndex, int patternIndex) {
    long sum = 0;
    bool damaged = false;
    int count = 0;
    int boundary = springIndex - row.pattern[patternIndex - 1];

    for (int i = 0; i < boundary; ++i) {
        bool flag = true;
        if (patternRow[i] == 0) continue;
        for (int j = i + row.pattern[patternIndex - 1]; j < springIndex - 1; ++j) {
            if (row.springs[j] == kDamaged) flag = false;
        }
        sum += flag ? patternRow[i] : 0;
    }
    return sum;
}

int getFirstOfLine(std::vector<long> line) {
    for (int i = 0; i < line.size(); ++i) {
        if (line[i] != 0) return i;
    }
    return -1;
}

Row unfold(Row row) {
    Row ouput = row;
    for (int i = 0; i < 4; ++i) {
        ouput.springs.push_back(kUnknown);
        ouput.springs.insert(ouput.springs.end(), row.springs.begin(), row.springs.end());
        ouput.pattern.insert(ouput.pattern.end(), row.pattern.begin(), row.pattern.end());
    }
    return ouput;
}