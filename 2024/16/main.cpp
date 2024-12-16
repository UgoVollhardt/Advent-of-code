#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <string>
#include <vector>

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

typedef Pos2D Dir;

struct Pos2DCompare {
    bool operator()(const Pos2D &lhs, const Pos2D &rhs) const {
        return lhs.x * 1000 + lhs.y < rhs.x * 1000 + rhs.y;
    }
};

struct Config {
    Pos2D pos;
    Dir dir;
};

struct Reindeer {
    Pos2D pos;
    Pos2D orient;
};

std::ostream &operator<<(std::ostream &os, Pos2D pos) {
    os << "[" << pos.x << "," << pos.y << "]";
    return os;
}

struct GraphNode {
    std::map<Dir, int, Pos2DCompare> costs;
    std::map<Dir, Pos2D, Pos2DCompare> previous;

    std::vector<Dir> costMinDir(const Dir src = Dir{0, 0}) {
        int costRes = std::numeric_limits<int>::max();
        std::vector<Dir> dirs;
        for (auto cost : costs) {
            int actualCost = 0;
            if (src != Dir{0, 0}) {
                if (cost.first == src)
                    actualCost = cost.second;
                else if (cost.first.orthogonal(src))
                    actualCost = cost.second + 1000;
                else
                    actualCost = cost.second + 2000;
            } else {
                actualCost = cost.second;
            }

            if (actualCost < costRes) {
                costRes = actualCost;
                dirs.clear();
                dirs.push_back(cost.first);
            } else if (actualCost == costRes) {
                dirs.push_back(cost.first);
            }
        }
        return dirs;
    }
};

struct Input {
    enum cell { wall, empty };
    std::vector<std::vector<cell>> map;
    Pos2D exit;
    Reindeer reindeer;
};

Input parseInput(std::string fileName);
std::vector<std::vector<GraphNode>> constructGraph(Input input);
std::vector<std::string> drawGraph(std::vector<std::vector<GraphNode>> graph, Pos2D exit,
                                   Pos2D start);

size_t grayStar(std::vector<std::vector<GraphNode>> graph, Pos2D exit);
size_t goldStar(std::vector<std::string> imageGraph);

int main(int, char *[]) {
    auto start = std::chrono::high_resolution_clock::now();
    auto input = parseInput("input");
    auto graph = constructGraph(input);
    auto graphImage = drawGraph(graph, input.exit, input.reindeer.pos);

    std::cout << "res gray star : " << grayStar(graph, input.exit) << std::endl;
    std::cout << "res gold star : " << goldStar(graphImage) << std::endl;

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms"
              << std::endl;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input res;
    std::string line;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            std::vector<Input::cell> mapLine;
            if (!line.empty()) {
                for (int i = 0; i < line.size(); ++i) {
                    if (line[i] == '#') {
                        mapLine.push_back(Input::wall);
                    } else if (line[i] == '.') {
                        mapLine.push_back(Input::empty);
                    } else if (line[i] == 'E') {
                        mapLine.push_back(Input::empty);
                        res.exit = Pos2D{i, int(res.map.size())};
                    } else if (line[i] == 'S') {
                        mapLine.push_back(Input::empty);
                        res.reindeer.pos = Pos2D{i, int(res.map.size())};
                        res.reindeer.orient = Pos2D{1, 0};
                    }
                }
            }
            res.map.push_back(mapLine);
        }
    } else {
        std::cout << "fichier non trouvé";
    }
    return res;
}

std::vector<std::vector<GraphNode>> constructGraph(Input input) {
    std::vector<Pos2D> dirs{Pos2D{0, 1}, Pos2D{1, 0}, Pos2D{0, -1}, Pos2D{-1, 0}};
    std::vector<std::vector<GraphNode>> processedPos(
        input.map.size(), std::vector<GraphNode>(input.map[0].size(), GraphNode()));

    int width = int(input.map[0].size());
    int height = int(input.map.size());
    Pos2D newPos;

    std::list<Config> stack;

    stack.push_back({input.reindeer.pos, input.reindeer.orient});

    while (!stack.empty()) {
        Config current = stack.back();
        stack.pop_back();

        if (current.pos == input.exit) {
            continue;
        }

        for (auto dir : dirs) {
            newPos = current.pos + dir;
            if (newPos.inbound(width, height) && input.map[newPos.y][newPos.x] == Input::empty) {
                int newCost = processedPos[current.pos.y][current.pos.x].costs[current.dir];
                if (dir == current.dir) {
                    newCost += 1;
                } else if (current.dir.orthogonal(dir)) {
                    newCost += 1001;
                } else {
                    newCost += 2001;
                }
                if (processedPos[newPos.y][newPos.x].costs.contains(dir)) {
                    if (processedPos[newPos.y][newPos.x].costs[dir] == newCost) {
                        processedPos[newPos.y][newPos.x].previous[dir] = current.pos;
                    } else if (processedPos[newPos.y][newPos.x].costs[dir] > newCost) {
                        processedPos[newPos.y][newPos.x].costs[dir] = newCost;
                        processedPos[newPos.y][newPos.x].previous[dir] = current.pos;
                        stack.push_back({newPos, dir});
                    }
                } else {
                    processedPos[newPos.y][newPos.x].costs[dir] = newCost;
                    processedPos[newPos.y][newPos.x].previous[dir] = current.pos;
                    stack.push_back({newPos, dir});
                }
            }
        }
    }
    return processedPos;
}

std::vector<std::string> drawGraph(std::vector<std::vector<GraphNode>> graph, Pos2D exit,
                                   Pos2D start) {
    std::vector<std::string> graphimage(graph.size(), std::string(graph[0].size(), '-'));
    std::list<Config> stack;
    Config current;
    std::vector<Dir> dirs = graph[exit.y][exit.x].costMinDir();

    for (auto dir : dirs) {
        stack.push_back(Config{exit, dir});
        while (!stack.empty()) {
            current = stack.back();
            stack.pop_back();
            graphimage[current.pos.y][current.pos.x] = '0';
            auto costminDirs = graph[current.pos.y][current.pos.x].costMinDir(current.dir);
            for (auto dir : costminDirs) {
                stack.push_back(Config(current.pos - dir, dir));
            }
        }
    }
    return graphimage;
}

size_t grayStar(std::vector<std::vector<GraphNode>> graph, Pos2D exit) {
    auto graphNodeExit = graph[exit.y][exit.x];

    return graphNodeExit.costs[graphNodeExit.costMinDir()[0]];
}

size_t goldStar(std::vector<std::string> imageGraph) {
    size_t res = 0;

    for (auto line : imageGraph) {
        for (auto charact : line) {
            if (charact == '0') {
                res++;
            }
        }
    }
    return res - 1;
}