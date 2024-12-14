#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#define MAX_WIDTH 101
#define MAX_HEIGHT 103

struct Pos2D {
    int x;
    int y;

    Pos2D operator+(Pos2D &other) { return Pos2D{x + other.x, y + other.y}; }
    bool operator==(const Pos2D &other) { return x == other.x && y == other.y; }

    bool inbound(int maxWidth, int maxHeight, int minWidth = 0, int minHeight = 0) {
        return x >= minWidth && x < maxWidth && y >= minHeight && y < maxHeight;
    }
};

std::ostream &operator<<(std::ostream &os, Pos2D pos) {
    os << "[" << pos.x << "," << pos.y << "]";
    return os;
}

struct Robot {
    Pos2D pos;
    Pos2D vel;
};

typedef std::vector<Robot> Input;

Input parseInput(std::string fileName);
void move(Robot *robot, int steps);
bool bigBlob(std::vector<std::vector<bool>> map);
void printMap(std::vector<std::vector<bool>> &map);
void clearMap(std::vector<std::vector<bool>> &map);

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
    std::string line, elem;
    Input res;

    std::cout << "Parsing input" << std::endl;

    if (file.is_open()) {
        while (file.good()) {
            std::vector<int> row;
            std::getline(file, line);
            if (!line.empty()) {
                Robot current;

                std::stringstream ss(line);
                std::getline(ss, elem, '=');
                std::getline(ss, elem, ',');
                current.pos.x = std::stoi(elem);
                std::getline(ss, elem, ' ');
                current.pos.y = std::stoi(elem);
                std::getline(ss, elem, '=');
                std::getline(ss, elem, ',');
                current.vel.x = std::stoi(elem);
                std::getline(ss, elem, '=');
                current.vel.y = std::stoi(elem);
                res.push_back(current);
            }
        }
    } else {
        std::cout << "fichier non trouvé";
    }
    return res;
}

void move(Robot *robot, int steps) {
    robot->pos.x = (robot->pos.x + robot->vel.x * steps) % MAX_WIDTH;
    robot->pos.y = (robot->pos.y + robot->vel.y * steps) % MAX_HEIGHT;

    if (robot->pos.x < 0) robot->pos.x += MAX_WIDTH;
    if (robot->pos.y < 0) robot->pos.y += MAX_HEIGHT;
}

bool bigBlob(std::vector<std::vector<bool>> map) {
    std::vector<Pos2D> dirs{Pos2D{0, 1}, Pos2D{1, 0}, Pos2D{0, -1}, Pos2D{-1, 0}};
    std::list<Pos2D> posStack;

    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            if (map[i][j]) {
                posStack.push_back(Pos2D{j, i});
                size_t blobSize = 0;
                while (!posStack.empty()) {
                    Pos2D current = posStack.back();
                    posStack.pop_back();
                    blobSize++;
                    for (auto &dir : dirs) {
                        Pos2D newPos = current + dir;
                        if (newPos.inbound(MAX_WIDTH, MAX_HEIGHT) &&
                            map[newPos.y][newPos.x] == true) {
                            posStack.push_back(newPos);
                            map[newPos.y][newPos.x] = false;
                        }
                    }
                }
                if (blobSize >= 100) {
                    return true;
                }
            }
        }
    }
    return false;
}

size_t grayStar(Input input) {
    size_t quad1 = 0, quad2 = 0, quad3 = 0, quad4 = 0;
    for (auto robot : input) {
        move(&robot, 100);
        if (robot.pos.x < MAX_WIDTH / 2 && robot.pos.y < MAX_HEIGHT / 2) {
            quad1++;
        } else if (robot.pos.x > MAX_WIDTH / 2 && robot.pos.y < MAX_HEIGHT / 2) {
            quad2++;
        } else if (robot.pos.x < MAX_WIDTH / 2 && robot.pos.y > MAX_HEIGHT / 2) {
            quad3++;
        } else if (robot.pos.x > MAX_WIDTH / 2 && robot.pos.y > MAX_HEIGHT / 2) {
            quad4++;
        }
    }
    return quad1 * quad2 * quad3 * quad4;
}

size_t goldStar(Input input) {
    std::vector<std::vector<bool>> map(MAX_HEIGHT, std::vector<bool>(MAX_WIDTH, false));
    Input robots = input;
    int iterations = 10000;
    for (int i = 0; i < iterations; ++i) {
        for (int j = 0; j < robots.size(); ++j) {
            move(&robots[j], 1);
            map[robots[j].pos.y][robots[j].pos.x] = true;
        }

        if (bigBlob(map)) {
            return i + 1;
        }
        for (int j = 0; j < map.size(); ++j) {
            for (int k = 0; k < map[j].size(); ++k) {
                map[j][k] = false;
            }
        }
    }
    return 0;
}
