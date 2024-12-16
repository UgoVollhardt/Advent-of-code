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
    bool operator==(const Pos2D &other) const { return x == other.x && y == other.y; }

    bool inbound(int maxWidth, int maxHeight, int minWidth = 0, int minHeight = 0) {
        return x >= minWidth && x < maxWidth && y >= minHeight && y < maxHeight;
    }

    bool orthogonal(const Pos2D &other) const {
        return (x == other.y || x == -other.y) && (y == other.x || y == -other.x);
    }
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
    Pos2D pos;
    Pos2D orient;
    int cost;
    int indexPrevious;

    bool operator>(const GraphNode &other) const { return cost > other.cost; }
};

struct comp {
    bool operator()(const GraphNode &a, const GraphNode &b) const { return a.cost < b.cost; }
};

struct Input {
    enum cell { wall, empty };
    std::vector<std::vector<cell>> map;
    Pos2D exit;
    Reindeer reindeer;
};

Input parseInput(std::string fileName);
GraphNode constructGraph(Input input);

size_t grayStar(Input input);
size_t goldStar(Input input);

int main(int, char *[]) {
    auto input = parseInput("input");
    std::cout << "res gray star : " << grayStar(input) << std::endl;
    // auto start = std::chrono::high_resolution_clock::now();
    // std::cout << "res gold star : " << goldStar(input) << std::endl;
    // auto stop = std::chrono::high_resolution_clock::now();
    // std::cout << "Elapsed time : "
    //           << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() <<
    //           "ms"
    //           << std::endl;

    // for (int i = 0; i < input.map.size(); ++i) {
    //     for (int j = 0; j < input.map[j].size(); ++j) {
    //         Pos2D pos = Pos2D{j, i};
    //         if (pos == input.exit) {
    //             std::cout << "E";
    //         } else if (pos == input.reindeer.pos) {
    //             std::cout << "S";
    //         } else if (input.map[i][j] == Input::empty) {
    //             std::cout << ".";
    //         } else {
    //             std::cout << "#";
    //         }
    //     }
    //     std::cout << std::endl;
    // }
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

GraphNode constructGraph(Input input) {
    std::vector<Pos2D> dirs{Pos2D{0, 1}, Pos2D{1, 0}, Pos2D{0, -1}, Pos2D{-1, 0}};
    std::vector<std::vector<bool>> processedPos(input.map.size(),
                                                std::vector<bool>(input.map[0].size(), false));
    std::vector<GraphNode> graph;
    int width = int(input.map[0].size());
    int height = int(input.map.size());
    std::list<GraphNode> stack;
    stack.push_back(GraphNode{input.reindeer.pos, input.reindeer.orient, 0, -1});

    while (!stack.empty()) {
        graph.push_back(stack.front());
        stack.pop_front();
        GraphNode &current = graph.back();
        int indexCurrent = int(graph.size() - 1);

        if (current.pos == input.exit) {
            return current;
        }

        for (auto dir : dirs) {
            Pos2D newPos = current.pos + dir;
            if (newPos.inbound(width, height) && !processedPos[newPos.y][newPos.x] &&
                input.map[newPos.y][newPos.x] == Input::empty) {
                if (dir == current.orient) {
                    stack.push_back(GraphNode{newPos, dir, current.cost + 1, indexCurrent});
                } else if (current.orient.orthogonal(dir)) {
                    stack.push_back(GraphNode{newPos, dir, current.cost + 1001, indexCurrent});
                } else {
                    stack.push_back(GraphNode{newPos, dir, current.cost + 2001, indexCurrent});
                }
                processedPos[newPos.y][newPos.x] = true;
            }
        }

        stack.sort([](const GraphNode &a, const GraphNode &b) { return a.cost < b.cost; });
    }
    return GraphNode{Pos2D{0, 0}, Pos2D{0, 0}, -1, -1};
}

size_t grayStar(Input input) {
    auto graphNode = constructGraph(input);
    return graphNode.cost;
}