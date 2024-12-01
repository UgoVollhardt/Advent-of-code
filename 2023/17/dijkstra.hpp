#include <algorithm>
#include <array>
#include <queue>
#include <unordered_set>
#include <vector>

struct Pose {
    int x, y;

    Pose operator-() { return Pose{-x, -y}; }
    bool operator==(const Pose& rhs) { return (rhs.x == x && rhs.y == y); }
    bool operator!=(const Pose& rhs) { return (rhs.x != x || rhs.y != y); }
};

namespace std {
/* implement hash function so we can put Pose into an unordered_set */
template <>
struct hash<Pose> {
    std::size_t operator()(const Pose& id) const noexcept {
        return std::hash<int>()(id.x ^ (id.y << 16));
    }
};
}  // namespace std

struct SquareGrid {
    static std::array<Pose, 4> DIRS;

    Pose lastMvt;
    int nbForward;
    int width, height;

    SquareGrid(int width_, int height_)
        : width(width_), height(height_), nbForward(0), lastMvt{0, 0} {}

    bool in_bounds(Pose id) const {
        return 0 <= id.x && id.x < width && 0 <= id.y && id.y < height;
    }

    bool passable(Pose id) const { return true; }

    std::vector<Pose> neighbors(Pose id) const {
        std::vector<Pose> results;

        for (Pose dir : DIRS) {
            Pose next{id.x + dir.x, id.y + dir.y};
            if (in_bounds(next) && passable(next)) {
                results.push_back(next);
            }
        }

        if ((id.x + id.y) % 2 == 0) {
            std::reverse(results.begin(), results.end());
        }

        return results;
    }
};

std::array<Pose, 4> SquareGrid::DIRS = {
    /* East, West, North, South */
    Pose{1, 0}, Pose{-1, 0}, Pose{0, -1}, Pose{0, 1}};

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