#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

enum Direction { kFromLeft, kFromRight, kFromUp, kFromDown };

typedef struct {
    int x;
    int y;
} Pose;

typedef struct {
    std::vector<std::string> map;
    std::vector<std::vector<std::vector<Direction>>> energized;
} Input;

inline Pose &operator+(Pose &lhs, const Direction &rhs) {
    switch (rhs) {
        case kFromLeft:
            lhs.x++;
            break;
        case kFromRight:
            lhs.x--;
            break;
        case kFromUp:
            lhs.y++;
            break;
        case kFromDown:
            lhs.y--;
            break;

        default:
            break;
    }
    return lhs;
}

inline Pose &operator+=(Pose &lhs, const Direction &rhs) { return lhs + rhs; }
inline std::ostream &operator<<(std::ostream &os, const Direction &rhs) {
    if (rhs == kFromDown) os << "↑";
    if (rhs == kFromLeft) os << "→";
    if (rhs == kFromRight) os << "←";
    if (rhs == kFromUp) os << "↓";
    return os;
}

int getResPart1(std::string fileName);
int getResPart2(std::string fileName);
Input parseInput(std::string fileName);
void followRay(Pose space, Direction direct, Input &layout);
bool rayStops(Pose space, Input &layout);
bool addDirectionEnergized(Pose space, Direction direct, Input &layout);
void resetEnergized(Input &layout);
int getSum(Input &layout);

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

int getResPart1(std::string fileName) {
    int sum = 0;
    auto layout = parseInput(fileName);
    followRay(Pose{0, 0}, kFromLeft, layout);

    for (int i = 0; i < layout.energized.size(); ++i) {
        for (int j = 0; j < layout.energized[i].size(); ++j) {
            if (layout.energized[i][j].size()) {
                sum++;
            }
        }
    }
    return sum;
}

int getResPart2(std::string fileName) {
    int sum = 0;
    int mostEnergized = 0;
    auto layout = parseInput(fileName);
    for (int i = 0; i < layout.map.size(); ++i) {
        sum = 0;
        resetEnergized(layout);
        followRay(Pose{0, i}, kFromLeft, layout);
        mostEnergized = std::max(getSum(layout), mostEnergized);
    }
    for (int i = 0; i < layout.map.size(); ++i) {
        sum = 0;
        resetEnergized(layout);
        followRay(Pose{int(layout.map.front().size() - 1), i}, kFromRight, layout);
        mostEnergized = std::max(getSum(layout), mostEnergized);
    }
    resetEnergized(layout);
    for (int i = 0; i < layout.map.front().size(); ++i) {
        sum = 0;
        resetEnergized(layout);
        followRay(Pose{i, 0}, kFromUp, layout);
        mostEnergized = std::max(getSum(layout), mostEnergized);
    }
    resetEnergized(layout);
    for (int i = 0; i < layout.map.front().size(); ++i) {
        sum = 0;
        resetEnergized(layout);
        followRay(Pose{i, int(layout.map.size() - 1)}, kFromDown, layout);
        mostEnergized = std::max(getSum(layout), mostEnergized);
    }
    return mostEnergized;
}

Input parseInput(std::string fileName) {
    Input ouput;
    std::ifstream file(fileName);
    std::string line;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            ouput.map.emplace_back(line);
        }
    }
    resetEnergized(ouput);
    return ouput;
}

void followRay(Pose tile, Direction direct, Input &layout) {
    Pose currentTile = tile;
    Direction currentDirect = direct;
    while (!rayStops(currentTile, layout) &&
           addDirectionEnergized(currentTile, currentDirect, layout)) {
        switch (layout.map[currentTile.y][currentTile.x]) {
            case '.':
                break;
            case '-':
                if (currentDirect == kFromDown || currentDirect == kFromUp) {
                    followRay(currentTile + kFromLeft, kFromLeft, layout);
                    currentDirect = kFromRight;
                }
                break;
            case '|':
                if (currentDirect == kFromLeft || currentDirect == kFromRight) {
                    followRay(currentTile + kFromDown, kFromDown, layout);
                    currentDirect = kFromUp;
                }
                break;
            case '\\':
                if (currentDirect == kFromDown) {
                    currentDirect = kFromRight;
                } else if (currentDirect == kFromLeft) {
                    currentDirect = kFromUp;
                } else if (currentDirect == kFromRight) {
                    currentDirect = kFromDown;
                } else if (currentDirect == kFromUp) {
                    currentDirect = kFromLeft;
                }
                break;
            case '/':
                if (currentDirect == kFromDown) {
                    currentDirect = kFromLeft;
                } else if (currentDirect == kFromLeft) {
                    currentDirect = kFromDown;
                } else if (currentDirect == kFromRight) {
                    currentDirect = kFromUp;
                } else if (currentDirect == kFromUp) {
                    currentDirect = kFromRight;
                }
                break;

            default:
                break;
        }
        currentTile += currentDirect;
    }
}

bool rayStops(Pose tile, Input &layout) {
    if (tile.x < 0 || tile.x >= layout.map[tile.y].size()) {
        return true;
    }
    if (tile.y < 0 || tile.y >= layout.map.size()) {
        return true;
    }
    return false;
}

bool addDirectionEnergized(Pose space, Direction direct, Input &layout) {
    auto vect = layout.energized[space.y][space.x];
    auto it = std::find(vect.begin(), vect.end(), direct);
    if (it != vect.end()) {
        return false;
    }
    layout.energized[space.y][space.x].emplace_back(direct);
    return true;
}

void resetEnergized(Input &layout) {
    layout.energized = std::vector<std::vector<std::vector<Direction>>>(
        layout.map.size(), std::vector(layout.map.front().size(), std::vector<Direction>()));
}

int getSum(Input &layout) {
    int sum = 0;
    for (int i = 0; i < layout.energized.size(); ++i) {
        for (int j = 0; j < layout.energized[i].size(); ++j) {
            if (layout.energized[i][j].size()) {
                sum++;
            }
        }
    }
    return sum;
}