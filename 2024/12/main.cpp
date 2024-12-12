#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

struct Pos2D {
    int x;
    int y;

    Pos2D operator+(Pos2D &other) { return Pos2D{x + other.x, y + other.y}; }
    bool operator==(const Pos2D &other) { return x == other.x && y == other.y; }

    bool inbound(int maxWidth, int maxHeight) {
        return x >= 0 && x < maxWidth && y >= 0 && y < maxHeight;
    }
};

struct Border {
    static inline std::vector<Pos2D> dirs{Pos2D{0, 1}, Pos2D{1, 0}, Pos2D{0, -1}, Pos2D{-1, 0}};
    std::vector<Pos2D> plots;
    Pos2D dir;
};

std::ostream &operator<<(std::ostream &os, Pos2D pos) {
    os << "[" << pos.x << "," << pos.y << "]";
    return os;
}

template <class T>
bool contains(std::vector<T> vect, T value) {
    return (std::find(vect.begin(), vect.end(), value) != vect.end());
}

typedef std::vector<std::string> Input;

Input parseInput(std::string fileName);
std::vector<std::vector<Pos2D>> getRegions(Input input);
int getPerimeter(std::vector<Pos2D> region);
std::vector<Border> getBorders(std::vector<Pos2D> region, Pos2D size);

size_t grayStar(Input input);
size_t goldStar(Input input);

int main(int, char *[]) {
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
    std::string line;
    Input res;

    std::cout << "Parsing input" << std::endl;

    if (file.is_open()) {
        while (file.good()) {
            std::vector<int> row;
            std::getline(file, line);
            if (!line.empty()) {
                res.push_back(line);
            }
        }
    } else {
        std::cout << "fichier non trouvé";
    }
    return res;
}

std::vector<std::vector<Pos2D>> getRegions(Input input) {
    std::vector<Pos2D> dirs{Pos2D{0, 1}, Pos2D{1, 0}, Pos2D{0, -1}, Pos2D{-1, 0}};
    std::vector<std::vector<Pos2D>> res;
    std::list<Pos2D> posStack;
    std::vector<std::vector<bool>> processedPos(input.size(),
                                                std::vector<bool>(input[0].size(), false));
    for (int i = 0; i < input.size(); ++i) {
        for (int j = 0; j < input[i].size(); ++j) {
            if (!processedPos[i][j]) {
                std::vector<Pos2D> currentRegion;
                char currentRegionChar = input[i][j];
                posStack.push_back(Pos2D{j, i});
                currentRegion.push_back(Pos2D{j, i});
                processedPos[i][j] = true;
                while (!posStack.empty()) {
                    Pos2D currentPos = posStack.back();
                    posStack.pop_back();

                    for (auto &dir : dirs) {
                        Pos2D newPos = currentPos + dir;
                        if (newPos.inbound(input[0].size(), input.size()) &&
                            processedPos[newPos.y][newPos.x] == false &&
                            input[newPos.y][newPos.x] == currentRegionChar) {
                            currentRegion.push_back(newPos);
                            posStack.push_back(newPos);
                            processedPos[newPos.y][newPos.x] = true;
                        }
                    }
                }
                res.push_back(currentRegion);
            }
        }
    }
    return res;
}

int getPerimeter(std::vector<Pos2D> region) {
    int res = 0;
    std::vector<Pos2D> dirs{Pos2D{0, 1}, Pos2D{1, 0}, Pos2D{0, -1}, Pos2D{-1, 0}};
    for (auto &plot : region) {
        for (auto &dir : dirs) {
            Pos2D newPos = plot + dir;
            if (std::find(region.begin(), region.end(), newPos) == region.end()) {
                res++;
            }
        }
    }
    return res;
}

std::vector<Border> getBorders(std::vector<Pos2D> region, Pos2D size) {
    std::vector<Border> res;
    std::list<Pos2D> posStack;
    std::vector<std::vector<std::vector<Pos2D>>> processedPos(
        size.y, std::vector<std::vector<Pos2D>>(size.x, std::vector<Pos2D>()));

    Pos2D current;
    for (auto &plot : region) {
        for (auto &dir : Border::dirs) {
            Pos2D newPos = plot + dir;
            if (!contains(region, newPos) && !contains(processedPos[plot.y][plot.x], dir)) {
                Border newBorder;
                newBorder.dir = dir;
                posStack.push_back(plot);
                while (!posStack.empty()) {
                    current = posStack.back();
                    posStack.pop_back();
                    if (contains(processedPos[current.y][current.x], dir)) {
                        continue;
                    }
                    if (!contains(region, current + dir)) {
                        Pos2D normal1{-dir.y, dir.x};
                        Pos2D normal2{dir.y, -dir.x};
                        if (contains(region, current + normal1)) {
                            posStack.push_back(current + normal1);
                        }
                        if (contains(region, current + normal2)) {
                            posStack.push_back(current + normal2);
                        }
                        newBorder.plots.push_back(current);
                    }
                    processedPos[current.y][current.x].push_back(dir);
                }
                res.push_back(newBorder);
            }
        }
    }
    return res;
}

size_t grayStar(Input input) {
    int res = 0;
    auto regions = getRegions(input);
    for (auto &region : regions) {
        res += region.size() * getPerimeter(region);
    }
    return res;
}

size_t goldStar(Input input) {
    int res = 0;
    auto regions = getRegions(input);
    for (auto &region : regions) {
        auto borders = getBorders(region, Pos2D{int(input[0].size()), int(input.size())});
        res += region.size() * borders.size();
    }
    return res;
}
