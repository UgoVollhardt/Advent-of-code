/*
 Sample code from https://www.redblobgames.com/pathfinding/a-star/
 Copyright 2014 Red Blob Games <redblobgames@gmail.com>

 Feel free to use this code in your own projects, including commercial projects
 License: Apache v2.0 <http://www.apache.org/licenses/LICENSE-2.0.html>
*/

#include <algorithm>
#include <array>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

enum class Direction { kUp, kRight, kDown, kLeft, kNone };

struct GridLocation {
    int x, y;
    Direction lastDir = Direction::kNone;
    int dirCount = 0;

    bool operator==(GridLocation& b) { return x == b.x && y == b.y; }
    bool operator!=(GridLocation& b) { return !(*this == b); }
    bool operator<(GridLocation& b) { return std::tie(x, y) < std::tie(b.x, b.y); }

    GridLocation operator-(GridLocation& rhs) { return GridLocation{x - rhs.x, y - rhs.y}; }
    GridLocation operator+(Direction& rhs) {
        if (rhs == Direction::kDown) return GridLocation{x, y + 1};
        if (rhs == Direction::kLeft) return GridLocation{x - 1, y};
        if (rhs == Direction::kRight) return GridLocation{x + 1, y};
        if (rhs == Direction::kUp) return GridLocation{x, y - 1};
        return *this;
    }
    Direction getDirectionFrom(GridLocation from) {
        auto diff = from - *this;
        if (diff.x < 0 && diff.y == 0) return Direction::kRight;
        if (diff.x > 0 && diff.y == 0) return Direction::kLeft;
        if (diff.y < 0 && diff.x == 0) return Direction::kUp;
        if (diff.y > 0 && diff.x == 0) return Direction::kDown;
        return Direction::kNone;
    }
};

inline Direction operator-(Direction& rhs) {
    if (rhs == Direction::kDown) return Direction::kUp;
    if (rhs == Direction::kLeft) return Direction::kRight;
    if (rhs == Direction::kRight) return Direction::kLeft;
    if (rhs == Direction::kUp) return Direction::kUp;
    return Direction::kNone;
}

std::basic_iostream<char>::basic_ostream& operator<<(std::basic_iostream<char>::basic_ostream& out,
                                                     const GridLocation& loc) {
    out << '(' << loc.x << ',' << loc.y << ')';
    return out;
}

std::array<Direction, 4> SquareGrid::DIRS = {
    /* East, West, North, South */ Direction::kRight, Direction::kLeft, Direction::kUp,
    Direction::kDown};

namespace std {
/* implement hash function so we can put GridLocation into an unordered_set */
template <>
struct hash<GridLocation> {
    std::size_t operator()(const GridLocation& id) const noexcept {
        // NOTE: better to use something like boost hash_combine
        return std::hash<int>()(id.x ^ (id.y << 16));
    }
};
}  // namespace std

struct SquareGrid {
    static std::array<Direction, 4> DIRS;

    int width, height;
    std::unordered_set<GridLocation> walls;

    SquareGrid(int width_, int height_) : width(width_), height(height_) {}

    bool in_bounds(GridLocation id) const {
        return 0 <= id.x && id.x < width && 0 <= id.y && id.y < height;
    }

    bool passable(GridLocation id) const { return walls.find(id) == walls.end(); }

    std::vector<GridLocation> neighbors(GridLocation id) const {
        std::vector<GridLocation> results;

        int count = id.dirCount++;

        for (Direction dir : DIRS) {
            // GridLocation next{id.x + dir.x, id.y + dir.y};
            if (id.lastDir != -dir && !(id.lastDir == dir && count > 3)) {
                GridLocation next = id + dir;
                if (in_bounds(next) && passable(next)) {
                    results.push_back(next);
                }
            }
        }

        if ((id.x + id.y) % 2 == 0) {
            // see "Ugly paths" section for an explanation:
            std::reverse(results.begin(), results.end());
        }

        return results;
    }
};

template <class Graph>
void draw_grid(const Graph& graph, std::unordered_map<GridLocation, double>* distances = nullptr,
               std::unordered_map<GridLocation, GridLocation>* point_to = nullptr,
               std::vector<GridLocation>* path = nullptr, GridLocation* start = nullptr,
               GridLocation* goal = nullptr) {
    const int field_width = 3;
    std::cout << std::string(field_width * graph.width, '_') << '\n';
    for (int y = 0; y != graph.height; ++y) {
        for (int x = 0; x != graph.width; ++x) {
            GridLocation id{x, y};
            if (graph.walls.find(id) != graph.walls.end()) {
                std::cout << std::string(field_width, '#');
            } else if (start && id == *start) {
                std::cout << " A ";
            } else if (goal && id == *goal) {
                std::cout << " Z ";
            } else if (path != nullptr && find(path->begin(), path->end(), id) != path->end()) {
                std::cout << " @ ";
            } else if (point_to != nullptr && point_to->count(id)) {
                GridLocation next = (*point_to)[id];
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

    GridWithWeights(int w, int h, std::vector<std::vector<int>> weigthMap_)
        : SquareGrid(w, h), weigthMap(weigthMap_) {}

    double cost(GridLocation from_node, GridLocation to_node) const {
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
            double new_cost = cost_so_far[current] + graph.cost(current, next);
            if (cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next]) {
                cost_so_far[next] = new_cost;
                came_from[next] = current;
                frontier.put(next, new_cost);
            }
        }
    }
}

template <typename Location>
std::vector<Location> reconstruct_path(Location start, Location goal,
                                       std::unordered_map<Location, Location> came_from) {
    std::vector<Location> path;
    Location current = goal;
    if (came_from.find(goal) == came_from.end()) {
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