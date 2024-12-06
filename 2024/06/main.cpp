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
} Vect;

typedef struct input {
    typedef enum { empty, obstacle } mapElement;
    typedef enum { left, right, up, down } dir;

    static inline std::map<dir, vect> dirToVec = {
        {left, {-1, 0}}, {right, {1, 0}}, {up, {0, -1}}, {down, {0, 1}}};

    std::vector<std::vector<mapElement>> map;
    vect guardPos;
    dir guardDir;
} Input;

Input parseInput(std::string fileName);
bool iterateOver(Input *input, std::vector<std::vector<bool>> *traversed);

size_t grayStar(Input input);
size_t goldStar(Input input);

int main(int, char *[]) {
    auto input = parseInput("input");
    std::cout << "res gray star : " << grayStar(input) << std::endl;
    // std::cout << "res gold star : " << goldStar(input) << std::endl;
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
    std::vector<std::vector<bool>> mapTraversed;
    size_t res = 0;

    for (int i = 0; i < currentInput.map.size(); ++i) {
        mapTraversed.emplace_back(std::vector<bool>(currentInput.map[i].size(), false));
    }

    mapTraversed[input.guardPos.y][input.guardPos.x] = true;
    while (iterateOver(&input, &mapTraversed)) {
    }

    for (int i = 0; i < mapTraversed.size(); ++i) {
        for (int j = 0; j < mapTraversed[i].size(); ++j) {
            if (mapTraversed[i][j]) {
                res++;
            }
        }
    }

    return res;
}

bool iterateOver(Input *input, std::vector<std::vector<bool>> *traversed) {
    Vect newPos = input->guardPos + Input::dirToVec[input->guardDir];
    if (newPos.y >= input->map.size() || newPos.y < 0 || newPos.x >= input->map[0].size() ||
        newPos.y < 0) {
        return false;
    } else if (input->map[newPos.y][newPos.x] == Input::empty) {
        input->guardPos = newPos;
        traversed->at(newPos.y)[newPos.x] = true;
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