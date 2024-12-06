#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

typedef struct vect {
    int x;
    int y;
    vect operator+(vect &other) { return vect{x + other.x, y + other.y}; }
    bool operator!=(vect &&other) { return x != other.x || y != other.y; }
} Vect;

typedef struct input {
    typedef enum { empty, obstacle } mapElement;
    typedef enum { left, right, up, down, none } dir;

    static inline std::map<dir, vect> dirToVec = {
        {left, {-1, 0}}, {right, {1, 0}}, {up, {0, -1}}, {down, {0, 1}}};

    std::vector<std::vector<mapElement>> map;
    vect guardPos;
    dir guardDir;

    bool inBoundaries() {
        return guardPos.x < map[0].size() && guardPos.x >= 0 && guardPos.y < map.size() &&
               guardPos.y >= 0;
    }
    bool inBoundaries(vect pos) {
        return pos.x < map[0].size() && pos.x >= 0 && pos.y < map.size() && pos.y >= 0;
    }
} Input;

Input parseInput(std::string fileName);
bool iterateOver(Input *input, std::vector<std::vector<Input::dir>> *traversed);
bool isLooping(Input input, vect newObstacle);

size_t grayStar(Input input);
size_t goldStar(Input input);

int main(int, char *[]) {
    auto input = parseInput("input");
    std::cout << "res gray star : " << grayStar(input) << std::endl;
    std::cout << "res gold star : " << goldStar(input) << std::endl;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input res;
    std::string line;
    size_t lineId = 0;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            std::vector<Input::mapElement> currentLine;
            if (!line.empty()) {
                for (auto elem : line) {
                    if (elem == '.') {
                        currentLine.push_back(Input::empty);
                    } else if (elem == '#') {
                        currentLine.push_back(Input::obstacle);
                    } else {
                        res.guardPos = {int(currentLine.size()), int(lineId)};
                        currentLine.push_back(Input::empty);
                        if (elem == '<') {
                            res.guardDir = Input::left;
                        } else if (elem == '^') {
                            res.guardDir = Input::up;
                        } else if (elem == '>') {
                            res.guardDir = Input::right;
                        } else if (elem == 'v') {
                            res.guardDir = Input::down;
                        }
                    }
                }
            }
            res.map.emplace_back(currentLine);
            lineId++;
        }
    } else {
        std::cout << "fichier non trouvé";
    }
    return res;
}

size_t grayStar(Input input) {
    Input currentInput = input;
    std::vector<std::vector<Input::dir>> mapTraversed;
    size_t res = 0;

    for (int i = 0; i < currentInput.map.size(); ++i) {
        mapTraversed.emplace_back(std::vector<Input::dir>(currentInput.map[i].size(), Input::none));
    }

    mapTraversed[input.guardPos.y][input.guardPos.x] = input.guardDir;
    while (iterateOver(&input, &mapTraversed)) {
    }

    for (int i = 0; i < mapTraversed.size(); ++i) {
        for (int j = 0; j < mapTraversed[i].size(); ++j) {
            if (mapTraversed[i][j] != Input::none) {
                res++;
            }
        }
    }

    return res;
}

bool iterateOver(Input *input, std::vector<std::vector<Input::dir>> *traversed) {
    Vect newPos = input->guardPos + Input::dirToVec[input->guardDir];
    if (newPos.y >= input->map.size() || newPos.y < 0 || newPos.x >= input->map[0].size() ||
        newPos.y < 0) {
        return false;
    } else if (input->map[newPos.y][newPos.x] == Input::empty) {
        input->guardPos = newPos;
        traversed->at(newPos.y)[newPos.x] = input->guardDir;
        return true;
    } else if (input->map[newPos.y][newPos.x] == Input::obstacle) {
        switch (input->guardDir) {
            case Input::left:
                input->guardDir = Input::up;
                break;
            case Input::up:
                input->guardDir = Input::right;
                break;
            case Input::right:
                input->guardDir = Input::down;
                break;
            case Input::down:
                input->guardDir = Input::left;
                break;

            default:
                break;
        }
        return true;
    }
    return true;
}

bool isLooping(Input input, vect newObstacle) {
    Input currentInput = input;
    std::vector<std::vector<Input::dir>> mapTraversed;

    for (int i = 0; i < currentInput.map.size(); ++i) {
        mapTraversed.emplace_back(std::vector<Input::dir>(currentInput.map[i].size(), Input::none));
    }
    currentInput.map[newObstacle.y][newObstacle.x] = Input::obstacle;

    while (currentInput.inBoundaries()) {
        Vect newPos = currentInput.guardPos + Input::dirToVec[currentInput.guardDir];
        if (!currentInput.inBoundaries(newPos)) {
            return false;
        } else if (currentInput.map[newPos.y][newPos.x] == Input::empty) {
            currentInput.guardPos = newPos;
            if (mapTraversed[newPos.y][newPos.x] == currentInput.guardDir) {
                return true;
            }
            mapTraversed[newPos.y][newPos.x] = currentInput.guardDir;
        } else if (currentInput.map[newPos.y][newPos.x] == Input::obstacle) {
            switch (currentInput.guardDir) {
                case Input::left:
                    currentInput.guardDir = Input::up;
                    break;
                case Input::up:
                    currentInput.guardDir = Input::right;
                    break;
                case Input::right:
                    currentInput.guardDir = Input::down;
                    break;
                case Input::down:
                    currentInput.guardDir = Input::left;
                    break;

                default:
                    break;
            }
        }
    }
    return false;
}

size_t goldStar(Input input) {
    Input currentInput = input;
    std::vector<std::vector<Input::dir>> mapTraversed;
    std::vector<vect> validPos;
    size_t res = 0;

    for (int i = 0; i < currentInput.map.size(); ++i) {
        mapTraversed.emplace_back(std::vector<Input::dir>(currentInput.map[i].size(), Input::none));
    }

    mapTraversed[input.guardPos.y][input.guardPos.x] = input.guardDir;
    while (iterateOver(&currentInput, &mapTraversed)) {
    }

    for (int i = 0; i < mapTraversed.size(); ++i) {
        for (int j = 0; j < mapTraversed[i].size(); ++j) {
            if (mapTraversed[i][j] != Input::none) {
                if (input.guardPos != vect{j, i}) {
                    validPos.push_back({j, i});
                }
            }
        }
    }

    for (auto pos : validPos) {
        auto flag = isLooping(input, pos);
        if (flag) {
            res++;
        }
    }

    return res;
}
