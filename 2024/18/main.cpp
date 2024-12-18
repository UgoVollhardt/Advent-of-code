#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <string>
#include <vector>

#define WIDTH 71
#define HEIGHT 71
#define threshold 1024

// #define WIDTH 7
// #define HEIGHT 7
// #define threshold 12

struct Pos2D {
    int x;
    int y;

    Pos2D operator+(const Pos2D &other) { return Pos2D{x + other.x, y + other.y}; }
    Pos2D operator-(const Pos2D &other) { return Pos2D{x - other.x, y - other.y}; }
    bool operator==(const Pos2D &other) const { return x == other.x && y == other.y; }

    bool inbound(int maxWidth, int maxHeight, int minWidth = 0, int minHeight = 0) const {
        return x >= minWidth && x < maxWidth && y >= minHeight && y < maxHeight;
    }

    bool orthogonal(const Pos2D &other) const {
        return (x == other.y || x == -other.y) && (y == other.x || y == -other.x);
    }
};

std::ostream &operator<<(std::ostream &os, Pos2D pos) {
    os << "[" << pos.x << "," << pos.y << "]";
    return os;
}

struct Pos2DCompare {
    bool operator()(const Pos2D &lhs, const Pos2D &rhs) const {
        return lhs.x * 1000 + lhs.y < rhs.x * 1000 + rhs.y;
    }
};

struct GraphNode {
    Pos2D pos;
    int cost;
    Pos2D previous;

    bool operator<(GraphNode &other) const { return cost < other.cost; }
};

typedef std::map<Pos2D, GraphNode, Pos2DCompare> GraphMap;

struct Input {
    enum cell { wall, empty };
    std::vector<std::vector<cell>> map;
    std::vector<Pos2D> fallingPixels;
};

Input parseInput(std::string fileName);
std::vector<Pos2D> getPath(Input input);
std::vector<Pos2D> constructPath(GraphMap graph);

size_t grayStar(Input input);
Pos2D goldStar(Input input);

int main(int, char *[]) {
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
    Input res{std::vector<std::vector<Input::cell>>(HEIGHT,
                                                    std::vector<Input::cell>(WIDTH, Input::empty))};
    std::string line;
    int count = 0;
    int x, y;

    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            if (!line.empty()) {
                std::stringstream ss(line);
                std::string value;

                std::getline(ss, value, ',');
                x = std::stoi(value);
                std::getline(ss, value, ',');
                y = std::stoi(value);
                if (count < threshold) {
                    res.map[y][x] = Input::wall;
                } else {
                    res.fallingPixels.push_back(Pos2D{x, y});
                }
                count++;
            }
        }
    } else {
        std::cout << "fichier non trouvé";
    }
    return res;
}

std::vector<Pos2D> getPath(Input input) {
    Pos2D target{WIDTH - 1, HEIGHT - 1};
    std::list<GraphNode> stack;
    std::vector<std::vector<bool>> processedCells(HEIGHT, std::vector<bool>(WIDTH, false));
    std::vector<Pos2D> dirs{Pos2D{0, 1}, Pos2D{1, 0}, Pos2D{0, -1}, Pos2D{-1, 0}};
    GraphMap cellTravelled;

    stack.push_back(GraphNode{Pos2D{0, 0}, 0, -1});
    while (!stack.empty()) {
        GraphNode current = stack.front();
        stack.pop_front();
        cellTravelled.insert(std::make_pair(current.pos, current));
        if (current.pos == target) {
            return constructPath(cellTravelled);
        }
        for (auto dir : dirs) {
            Pos2D newPos = current.pos + dir;
            if (newPos.inbound(WIDTH, HEIGHT) && !processedCells[newPos.y][newPos.x] &&
                input.map[newPos.y][newPos.x] == Input::empty) {
                stack.push_back(GraphNode{newPos, current.cost + 1, current.pos});
                processedCells[newPos.y][newPos.x] = true;
            }
        }
        stack.sort();
    }

    return std::vector<Pos2D>();
}

std::vector<Pos2D> constructPath(GraphMap graph) {
    std::vector<Pos2D> res;
    if (!graph.contains(Pos2D{WIDTH - 1, HEIGHT - 1})) {
        return res;
    }
    auto current = graph[Pos2D{WIDTH - 1, HEIGHT - 1}];
    while (current.cost != 0) {
        res.push_back(current.pos);
        current = graph[current.previous];
    }
    return res;
}

size_t grayStar(Input input) {
    auto path = getPath(input);
    return path.size();
}

Pos2D goldStar(Input input) {
    auto path = getPath(input);
    for (auto pos : input.fallingPixels) {
        input.map[pos.y][pos.x] = Input::wall;
        if (std::find(path.begin(), path.end(), pos) != path.end()) {
            path = getPath(input);
            if (!path.size()) return pos;
        }
    }
    return Pos2D{0, 0};
}