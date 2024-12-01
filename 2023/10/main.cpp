#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

enum Direction { kFromUp, kFromLeft, kFromDown, kFromRight };

typedef std::pair<int, int> Point;

typedef struct {
    Point startingPoint;
    std::vector<std::string> map;
} Input;

typedef struct {
    Point current;
    Direction source;
} Mvt;

int getfarthestPoint(std::string fileName);
int getInsideArea(std::string fileName);
Input parseInput(std::string fileName);
std::vector<Point> getLoop(Input input);
Mvt getFirstSteps(Point startingPoint, Input input);
int shoelaceAlgo(std::vector<Point> loop);

int main() {
    std::cout << "--Part 1--" << std::endl;
    std::cout << getfarthestPoint("input") << std::endl;
    std::cout << "--Part 2--" << std::endl;
    std::cout << getInsideArea("input") << std::endl;
}

int getfarthestPoint(std::string fileName) {
    auto input = parseInput(fileName);
    auto loop = getLoop(input);

    return loop.size() / 2;
}
int getInsideArea(std::string fileName) {
    auto input = parseInput(fileName);
    auto loop = getLoop(input);

    return shoelaceAlgo(loop);
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::string line, buffer;
    Input res;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            int pos = line.find('S', 0);
            if (pos != std::string::npos) {
                res.startingPoint = std::make_pair(res.map.size(), pos);
            }
            res.map.emplace_back(line);
        }
    }
    return res;
}

std::vector<Point> getLoop(Input input) {
    std::vector<Point> ouput;
    int x, y;
    ouput.push_back(input.startingPoint);
    auto currPos = getFirstSteps(input.startingPoint, input);
    while (currPos.current != input.startingPoint) {
        ouput.push_back(currPos.current);
        x = currPos.current.second;
        y = currPos.current.first;
        switch (input.map[y][x]) {
            case '|':
                currPos.current =
                    currPos.source == kFromUp ? std::make_pair(y + 1, x) : std::make_pair(y - 1, x);
                currPos.source = currPos.source;
                break;
            case '-':
                currPos.current = currPos.source == kFromLeft ? std::make_pair(y, x + 1)
                                                              : std::make_pair(y, x - 1);
                currPos.source = currPos.source;
                break;
            case 'L':
                if (currPos.source == kFromUp) {
                    currPos.current = std::make_pair(y, x + 1);
                    currPos.source = kFromLeft;
                } else {
                    currPos.current = std::make_pair(y - 1, x);
                    currPos.source = kFromDown;
                }
                break;
            case 'J':
                if (currPos.source == kFromUp) {
                    currPos.current = std::make_pair(y, x - 1);
                    currPos.source = kFromRight;
                } else {
                    currPos.current = std::make_pair(y - 1, x);
                    currPos.source = kFromDown;
                }
                break;
            case '7':
                if (currPos.source == kFromDown) {
                    currPos.current = std::make_pair(y, x - 1);
                    currPos.source = kFromRight;
                } else {
                    currPos.current = std::make_pair(y + 1, x);
                    currPos.source = kFromUp;
                }
                break;
            case 'F':
                if (currPos.source == kFromDown) {
                    currPos.current = std::make_pair(y, x + 1);
                    currPos.source = kFromLeft;
                } else {
                    currPos.current = std::make_pair(y + 1, x);
                    currPos.source = kFromUp;
                }
                break;

            default:
                break;
        }
    }
    return ouput;
}

Mvt getFirstSteps(Point startingPoint, Input input) {
    char current = input.map[startingPoint.first - 1][startingPoint.second];
    if (startingPoint.first != 0 && (current == '|' || current == '7' || current == 'F')) {
        return Mvt{Point(startingPoint.first - 1, startingPoint.second), kFromDown};
    }
    current = input.map[startingPoint.first][startingPoint.second + 1];
    if (startingPoint.first != 0 && (current == '-' || current == '7' || current == 'J')) {
        return Mvt{Point(startingPoint.first, startingPoint.second + 1), kFromLeft};
    }
    current = input.map[startingPoint.first + 1][startingPoint.second];
    if (startingPoint.first != 0 && (current == '|' || current == 'L' || current == 'J')) {
        return Mvt{Point(startingPoint.first + 1, startingPoint.second), kFromUp};
    }
    current = input.map[startingPoint.first][startingPoint.second - 1];
    if (startingPoint.first != 0 && (current == '-' || current == 'L' || current == 'F')) {
        return Mvt{Point(startingPoint.first, startingPoint.second - 1), kFromRight};
    }
    return Mvt();
}

int shoelaceAlgo(std::vector<Point> loop) {
    int sum = 0;
    for (int i = 0; i < loop.size() - 1; ++i) {
        sum += loop[i].first * loop[i + 1].second - loop[i].second * loop[i + 1].first;
    }
    sum += loop.back().first * loop.front().second - loop.back().second * loop.front().first;
    sum = std::abs(sum);
    sum -= loop.size();
    sum /= 2;
    return sum + 1;  // Don't know why but there is always an offset of 1 with the answer
}
