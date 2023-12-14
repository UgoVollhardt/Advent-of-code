#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

typedef struct {
    int col;
    int row;
} Position;

typedef struct {
    std::vector<int> emptyRows;
    std::vector<int> emptyColumns;
    std::vector<std::string> map;
} Input;

long getSumDistanceAfterExtension(std::string fileName, int extentionFactor);
Input parseInput(std::string fileName);
void updatetEmptyHorizontalLines(Input &input);
void updatetEmptyVerticalLines(Input &input);
void expand(Input &input);
std::vector<Position> getStarsPositions(Input input);
long getNumberOfEmptyLineInPath(Position p1, Position p2, Input input);

int main() {
    std::cout << "--Part 1--" << std::endl;
    std::cout << getSumDistanceAfterExtension("input", 2) << std::endl;
    std::cout << "--Part 2--" << std::endl;
    std::cout << getSumDistanceAfterExtension("input", 1000000) << std::endl;
}

long getSumDistanceAfterExtension(std::string fileName, int extentionFactor) {
    long sum = 0;
    auto input = parseInput(fileName);
    updatetEmptyHorizontalLines(input);
    updatetEmptyVerticalLines(input);

    auto stars = getStarsPositions(input);

    for (int i = 0; i < stars.size(); ++i) {
        for (int j = 0; j < stars.size(); ++j) {
            if (i != j) {
                sum +=
                    std::abs(stars[i].col - stars[j].col) + std::abs(stars[i].row - stars[j].row) +
                    getNumberOfEmptyLineInPath(stars[i], stars[j], input) * (extentionFactor - 1);
                ;
            }
        }
    }
    return sum / 2;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::string line, buffer;
    Input res;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            res.map.emplace_back(line);
        }
    }
    return res;
}

void updatetEmptyHorizontalLines(Input &input) {
    std::vector<bool> notEmptyLines(input.map.size(), false);
    for (int i = 0; i < input.map.size(); ++i) {
        std::string line = input.map[i];
        for (int j = 0; j < line.size(); ++j) {
            if (line[j] != '.') {
                notEmptyLines[i] = true;
                break;
            }
        }
    }
    for (int i = 0; i < notEmptyLines.size(); ++i) {
        if (!notEmptyLines[i]) input.emptyRows.push_back(i);
    }
}

void updatetEmptyVerticalLines(Input &input) {
    std::vector<bool> notEmptyLines(input.map.size(), false);
    for (int i = 0; i < input.map.front().size(); ++i) {
        for (int j = 0; j < input.map.size(); ++j) {
            if (input.map[j][i] != '.') {
                notEmptyLines[i] = true;
                break;
            }
        }
    }

    for (int i = 0; i < notEmptyLines.size(); ++i) {
        if (!notEmptyLines[i]) input.emptyColumns.push_back(i);
    }
}

std::vector<Position> getStarsPositions(Input input) {
    std::vector<Position> output;
    for (int i = 0; i < input.map.size(); ++i) {
        for (int j = 0; j < input.map[i].size(); ++j) {
            if (input.map[i][j] == '#') output.emplace_back(Position{j, i});
        }
    }
    return output;
}

long getNumberOfEmptyLineInPath(Position p1, Position p2, Input input) {
    long sum = 0;
    int buffer;
    int minCol = std::min(p1.col, p2.col);
    int maxCol = std::max(p1.col, p2.col);
    int minRow = std::min(p1.row, p2.row);
    int maxRow = std::max(p1.row, p2.row);
    for (int i = 0; i < input.emptyColumns.size(); ++i) {
        if (input.emptyColumns[i] < maxCol && input.emptyColumns[i] > minCol) {
            sum++;
        }
    }

    for (int i = 0; i < input.emptyRows.size(); ++i) {
        if (input.emptyRows[i] < maxRow && input.emptyRows[i] > minRow) {
            sum++;
        }
    }

    return sum;
}