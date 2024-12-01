#include <algorithm>
#include <array>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

enum class Direction { kUp, kRight, kDown, kLeft, kNone };

class Pose {
   public:
    int x, y;
    Direction lastDir = Direction::kNone;
    int dirCount = 0;

    bool operator<(const Pose b) const { return std::tie(x, y) < std::tie(b.x, b.y); }

    Pose operator-(Pose& rhs) { return Pose{x - rhs.x, y - rhs.y}; }
    Pose operator+(Direction& rhs) {
        if (rhs == Direction::kDown) return Pose{x, y + 1};
        if (rhs == Direction::kLeft) return Pose{x - 1, y};
        if (rhs == Direction::kRight) return Pose{x + 1, y};
        if (rhs == Direction::kUp) return Pose{x, y - 1};
        return *this;
    }
    Direction getDirectionFrom(Pose from) {
        auto diff = from - *this;
        if (diff.x < 0 && diff.y == 0) return Direction::kRight;
        if (diff.x > 0 && diff.y == 0) return Direction::kLeft;
        if (diff.y < 0 && diff.x == 0) return Direction::kUp;
        if (diff.y > 0 && diff.x == 0) return Direction::kDown;
        return Direction::kNone;
    }
};

inline bool operator==(const Pose& a, const Pose& b) { return a.x == b.x && a.y == b.y; }
inline bool operator!=(const Pose& a, const Pose& b) { return !(a == b); }

inline Direction operator-(Direction& rhs) {
    if (rhs == Direction::kDown) return Direction::kUp;
    if (rhs == Direction::kLeft) return Direction::kRight;
    if (rhs == Direction::kRight) return Direction::kLeft;
    if (rhs == Direction::kUp) return Direction::kDown;
    return Direction::kNone;
}

std::basic_iostream<char>::basic_ostream& operator<<(std::basic_iostream<char>::basic_ostream& out,
                                                     const Pose& loc) {
    out << '(' << loc.x << ',' << loc.y << ')';
    return out;
}

std::basic_iostream<char>::basic_ostream& operator<<(std::basic_iostream<char>::basic_ostream& out,
                                                     const Direction& dir) {
    // out << '(' << loc.x << ',' << loc.y << ')';
    if (dir == Direction::kDown) out << "↑";
    if (dir == Direction::kLeft) out << "←";
    if (dir == Direction::kRight) out << "→";
    if (dir == Direction::kUp) out << "↓";
    if (dir == Direction::kNone) out << "↻";

    return out;
}

namespace std {
/* implement hash function so we can put Pose into an unordered_set */
template <>
struct hash<Pose> {
    std::size_t operator()(const Pose& id) const noexcept {
        // NOTE: better to use something like boost hash_combine
        return std::hash<int>()(id.x ^ (id.y << 16));
    }
};
}  // namespace std

struct SquareGrid {
    static std::array<Direction, 4> DIRS;

    int width, height;
    std::unordered_set<Pose> walls;

    SquareGrid(int width_, int height_) : width(width_), height(height_) {}

    bool in_bounds(Pose id) const {
        return 0 <= id.x && id.x < width && 0 <= id.y && id.y < height;
    }

    // bool passable(Pose id) const { return walls.find(id) == walls.end(); }

    std::vector<Pose> neighbors(Pose id) const {
        std::vector<Pose> results;

        id.dirCount++;
        std::cout << "Count :" << id.dirCount << std::endl;

        for (Direction dir : DIRS) {
            // std::cout << dir << " " << -dir << std::endl;
            //  Pose next{id.x + dir.x, id.y + dir.y};
            std::cout << id.lastDir << " " << -dir << " " << (id.lastDir != -dir) << std::endl;
            if (id.lastDir == dir && id.dirCount > 3) continue;
            if (id.lastDir != -dir) {
                Pose next = id + dir;
                next.lastDir = dir;
                if (dir == id.lastDir) {
                    next.dirCount = id.dirCount;
                } else {
                    next.dirCount = 1;
                }
                if (in_bounds(next) /*&& passable(next)*/) {
                    results.push_back(next);
                }
                std::cout << "From " << id << " Next : " << next << " count : " << next.dirCount
                          << std::endl;
            }
        }

        if ((id.x + id.y) % 2 == 0) {
            // see "Ugly paths" section for an explanation:
            std::reverse(results.begin(), results.end());
        }

        return results;
    }
};

std::array<Direction, 4> SquareGrid::DIRS = {Direction::kRight, Direction::kLeft, Direction::kUp,
                                             Direction::kDown};

template <class Graph>
void draw_grid(const Graph& graph, std::unordered_map<Pose, double>* distances = nullptr,
               std::unordered_map<Pose, Pose>* point_to = nullptr,
               std::vector<Pose>* path = nullptr, Pose* start = nullptr, Pose* goal = nullptr) {
    const int field_width = 3;
    std::cout << std::string(field_width * graph.width, '_') << '\n';
    for (int y = 0; y != graph.height; ++y) {
        for (int x = 0; x != graph.width; ++x) {
            Pose id{x, y};
            if (graph.walls.find(id) != graph.walls.end()) {
                std::cout << std::string(field_width, '#');
            } else if (start && id == *start) {
                std::cout << " A ";
            } else if (goal && id == *goal) {
                std::cout << " Z ";
            } else if (path != nullptr && find(path->begin(), path->end(), id) != path->end()) {
                std::cout << " @ ";
            } else if (point_to != nullptr && point_to->count(id)) {
                Pose next = (*point_to)[id];
                if (next.x == x + 1) {
                    std::cout << " > ";
                } else if (next.x == x - 1) {
                    std::cout << " < ";
                } else if (next.y == y + 1) {
                    std::cout << " v ";
                } else if (next.y == y - 1) {
                    std::cout << " ^ ";
                } else {
                    std::cout << " * ";
                }
            } else if (distances != nullptr && distances->count(id)) {
                std::cout << ' ' << std::left << std::setw(field_width - 1) << (*distances)[id];
            } else {
                std::cout << " . ";
            }
        }
        std::cout << '\n';
    }
    std::cout << std::string(field_width * graph.width, '~') << '\n';
}

struct GridWithWeights : SquareGrid {
    std::vector<std::vector<int>> weigthMap;

    GridWithWeights(std::vector<std::vector<int>> weigthMap_)
        : SquareGrid(weigthMap_.front().size(), weigthMap_.size()), weigthMap(weigthMap_) {}

    double cost(Pose from_node, Pose to_node) const {
        if (in_bounds(to_node)) return weigthMap[to_node.y][to_node.x];
        return 1;
    }
};

template <typename T, typename priority_t>
struct PriorityQueue {
    typedef std::pair<priority_t, T> PQElement;
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> elements;

    inline bool empty() const { return elements.empty(); }

    inline void put(T item, priority_t priority) { elements.emplace(priority, item); }

    T get() {
        T best_item = elements.top().second;
        elements.pop();
        return best_item;
    }
};

template <typename Location, typename Graph>
void dijkstra_search(Graph graph, Location start, Location goal,
                     std::unordered_map<Location, Location>& came_from,
                     std::unordered_map<Location, double>& cost_so_far) {
    PriorityQueue<Location, double> frontier;
    frontier.put(start, 0);

    came_from[start] = start;
    cost_so_far[start] = 0;

    while (!frontier.empty()) {
        Location current = frontier.get();

        if (current == goal) {
            break;
        }

        for (Location next : graph.neighbors(current)) {
            std::cout << next << std::endl;
            double new_cost = cost_so_far[current] + graph.cost(current, next);
            if (cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next]) {
                cost_so_far[next] = new_cost;
                came_from[next] = current;
                frontier.put(next, new_cost);
            }
        }
    }
    std::cout << "Came_from size " << came_from.size() << " cost_so_dar size " << cost_so_far.size()
              << std::endl;
}

template <typename Location>
std::vector<Location> reconstruct_path(Location start, Location goal,
                                       std::unordered_map<Location, Location> came_from) {
    std::vector<Location> path;
    Location current = goal;
    std::cout << goal << std::endl;
    if (came_from.find(goal) == came_from.end()) {
        std::cout << "goal not founded" << std::endl;
        return path;  // no path can be found
    }
    while (current != start) {
        path.push_back(current);
        current = came_from[current];
    }
    path.push_back(start);  // optional
    std::reverse(path.begin(), path.end());
    return path;
}

struct Input {
    std::vector<std::vector<int>> map;
    GridWithWeights graph;
};

int getResPart1(std::string fileName);
int getResPart2(std::string fileName);
Input parseInput(std::string fileName);
void graphUpdate(Input& input, Pose start);
std::vector<int> parseLine(std::string line);

int main() {
    std::cout << "--Part 1--" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << getResPart1("input-test") << std::endl;
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time : "
              << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "µs"
              << std::endl;
    /*std::cout << "--Part 2--" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    std::cout << getResPart2("input") << std::endl;
    stop = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time : "
              << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "µs"
              << std::endl;*/
}

int getResPart1(std::string fileName) {
    int sum = 0;
    auto layout = parseInput(fileName);
    std::unordered_map<Pose, Pose> came_from;
    std::unordered_map<Pose, double> cost_so_far;
    dijkstra_search<Pose, GridWithWeights>(layout.graph, Pose{0, 0},
                                           Pose{layout.graph.width - 1, layout.graph.height - 1},
                                           came_from, cost_so_far);
    auto path = reconstruct_path(Pose{0, 0}, Pose{layout.graph.width - 1, layout.graph.height - 1},
                                 came_from);
    draw_grid(layout.graph, nullptr, &came_from /*nullptr, /*&path*/);
    // std::cout << path.size() << std::endl;
    for (auto pose : path) {
        sum += layout.map[pose.y][pose.x];
    }
    return sum;
}

int getResPart2(std::string fileName) {
    int sum = 0;

    return 0;
}

Input parseInput(std::string fileName) {
    std::vector<std::vector<int>> map;
    std::ifstream file(fileName);
    std::string line;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            map.emplace_back(parseLine(line));
        }
    }
    Input ouput{map, GridWithWeights(map)};
    return ouput;
}

/*void graphUpdate(Input& input, Pose start) {
    std::queue<Pose> frontier;
    frontier.push(start);

    std::unordered_set<Pose> reached;
    reached.insert(start);

    while (!frontier.empty()) {
        Pose current = frontier.front();
        frontier.pop();

        // std::cout << "  Visiting " << current << '\n';
        for (Pose next : input.neighbors(current)) {
            if (reached.find(next) == reached.end()) {
                frontier.push(next);
                reached.insert(next);
            }
        }
    }
}*/

std::vector<int> parseLine(std::string line) {
    std::vector<int> output;
    for (auto num : line) {
        output.emplace_back(num - '0');
    }
    return output;
}