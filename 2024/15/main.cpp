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

    Pos2D operator+(const Pos2D &other) { return Pos2D{x + other.x, y + other.y}; }
    bool operator==(const Pos2D &other) const { return x == other.x && y == other.y; }

    bool inbound(int maxWidth, int maxHeight, int minWidth = 0, int minHeight = 0) {
        return x >= minWidth && x < maxWidth && y >= minHeight && y < maxHeight;
    }
};

std::ostream &operator<<(std::ostream &os, Pos2D pos) {
    os << "[" << pos.x << "," << pos.y << "]";
    return os;
}

typedef struct input {
    enum MapType { wall, box, empty };
    std::vector<std::vector<MapType>> map;
    std::vector<Pos2D> cmds;
    Pos2D robot;

} Input;

typedef struct inputBig {
    enum MapType { wall, boxLeft, boxRight, empty };
    std::vector<std::vector<MapType>> map;
    std::vector<Pos2D> cmds;
    Pos2D robot;

} InputBig;

Input parseInput(std::string fileName);
InputBig parseInputBig(std::string fileName);
void applyCmd(Input *input, Pos2D dir);
void applyCmd(InputBig *input, Pos2D dir);
bool moveBox(Input *input, Pos2D pos, Pos2D dir);
bool moveBox(InputBig *input, Pos2D pos, Pos2D dir);
bool canMoveBox(InputBig *input, Pos2D pos, Pos2D dir);
void moveBoxAll(InputBig *input, Pos2D pos, Pos2D dir);

size_t grayStar(Input input);
size_t goldStar(InputBig input);

int main(int, char *[]) {
    std::string file("input");
    auto input = parseInput(file);
    auto inputBig = parseInputBig(file);

    std::cout << "res gray star : " << grayStar(input) << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "res gold star : " << goldStar(inputBig) << std::endl;
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms"
              << std::endl;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::string line, elem;
    Input res;
    int status = 0;

    std::cout << "Parsing input" << std::endl;

    if (file.is_open()) {
        while (file.good()) {
            std::vector<int> row;
            std::getline(file, line);

            if (!line.empty()) {
                if (status == 0) {
                    std::vector<Input::MapType> mapLine;
                    for (int i = 0; i < line.size(); ++i) {
                        switch (line[i]) {
                            case '#':
                                mapLine.push_back(Input::wall);
                                break;
                            case 'O':
                                mapLine.push_back(Input::box);
                                break;
                            case '.':
                                mapLine.push_back(Input::empty);
                                break;
                            case '@':
                                mapLine.push_back(Input::empty);
                                res.robot = Pos2D{i, int(res.map.size())};
                                break;

                            default:
                                break;
                        }
                    }
                    res.map.push_back(mapLine);
                } else {
                    for (int i = 0; i < line.size(); ++i) {
                        switch (line[i]) {
                            case '^':
                                res.cmds.push_back(Pos2D{0, -1});
                                break;
                            case '>':
                                res.cmds.push_back(Pos2D{1, 0});
                                break;
                            case 'v':
                                res.cmds.push_back(Pos2D{0, 1});
                                break;
                            case '<':
                                res.cmds.push_back(Pos2D{-1, 0});
                                break;

                            default:
                                break;
                        }
                    }
                }
            } else {
                status = 1;
            }
        }
    } else {
        std::cout << "fichier non trouvé";
    }
    return res;
}

InputBig parseInputBig(std::string fileName) {
    std::ifstream file(fileName);
    std::string line, elem;
    InputBig res;
    int status = 0;

    std::cout << "Parsing input" << std::endl;

    if (file.is_open()) {
        while (file.good()) {
            std::vector<int> row;
            std::getline(file, line);

            if (!line.empty()) {
                if (status == 0) {
                    std::vector<InputBig::MapType> mapLine;
                    for (int i = 0; i < line.size(); ++i) {
                        switch (line[i]) {
                            case '#':
                                mapLine.push_back(InputBig::wall);
                                mapLine.push_back(InputBig::wall);
                                break;
                            case 'O':
                                mapLine.push_back(InputBig::boxLeft);
                                mapLine.push_back(InputBig::boxRight);
                                break;
                            case '.':
                                mapLine.push_back(InputBig::empty);
                                mapLine.push_back(InputBig::empty);
                                break;
                            case '@':
                                mapLine.push_back(InputBig::empty);
                                mapLine.push_back(InputBig::empty);
                                res.robot = Pos2D{i * 2, int(res.map.size())};
                                break;

                            default:
                                break;
                        }
                    }
                    res.map.push_back(mapLine);
                } else {
                    for (int i = 0; i < line.size(); ++i) {
                        switch (line[i]) {
                            case '^':
                                res.cmds.push_back(Pos2D{0, -1});
                                break;
                            case '>':
                                res.cmds.push_back(Pos2D{1, 0});
                                break;
                            case 'v':
                                res.cmds.push_back(Pos2D{0, 1});
                                break;
                            case '<':
                                res.cmds.push_back(Pos2D{-1, 0});
                                break;

                            default:
                                break;
                        }
                    }
                }
            } else {
                status = 1;
            }
        }
    } else {
        std::cout << "fichier non trouvé";
    }
    return res;
}

void applyCmd(Input *input, Pos2D dir) {
    Pos2D newPos = input->robot + dir;
    if (!newPos.inbound(input->map[0].size(), input->map.size())) {
        std::cout << "error in robot position" << std::endl;
        return;
    }
    switch (input->map[newPos.y][newPos.x]) {
        case Input::wall:
            break;
        case Input::box:
            if (moveBox(input, newPos, dir)) {
                input->robot = newPos;
                input->map[input->robot.y][input->robot.x] = Input::empty;
            }
            break;
        case Input::empty:
            input->robot = newPos;
            input->map[input->robot.y][input->robot.x] = Input::empty;
            break;

        default:
            break;
    }
}

void applyCmd(InputBig *input, Pos2D dir) {
    Pos2D newPos = input->robot + dir;
    if (!newPos.inbound(input->map[0].size(), input->map.size())) {
        std::cout << "error in robot position" << std::endl;
        return;
    }
    switch (input->map[newPos.y][newPos.x]) {
        case InputBig::wall:
            break;
        case InputBig::boxLeft:
        case InputBig::boxRight:

            if (dir == Pos2D{1, 0} || dir == Pos2D{-1, 0}) {
                if (canMoveBox(input, newPos, dir)) {
                    moveBoxAll(input, newPos, dir);
                    input->robot = newPos;
                    input->map[input->robot.y][input->robot.x] = InputBig::empty;
                }
            } else {
                if (input->map[newPos.y][newPos.x] == InputBig::boxLeft &&
                    canMoveBox(input, newPos, dir) &&
                    canMoveBox(input, newPos + Pos2D{1, 0}, dir)) {
                    moveBoxAll(input, newPos, dir);
                    moveBoxAll(input, newPos + Pos2D{1, 0}, dir);
                    input->robot = newPos;
                    input->map[input->robot.y][input->robot.x] = InputBig::empty;
                } else if (input->map[newPos.y][newPos.x] == InputBig::boxRight &&
                           canMoveBox(input, newPos, dir) &&
                           canMoveBox(input, newPos + Pos2D{-1, 0}, dir)) {
                    moveBoxAll(input, newPos, dir);
                    moveBoxAll(input, newPos + Pos2D{-1, 0}, dir);
                    input->robot = newPos;
                    input->map[input->robot.y][input->robot.x] = InputBig::empty;
                }
            }
            break;
        case InputBig::empty:
            input->robot = newPos;
            input->map[input->robot.y][input->robot.x] = InputBig::empty;
            break;

        default:
            break;
    }
}

bool moveBox(Input *input, Pos2D pos, Pos2D dir) {
    Pos2D newPos = pos + dir;
    if (!newPos.inbound(input->map[0].size(), input->map.size())) {
        return false;
    }
    switch (input->map[newPos.y][newPos.x]) {
        case Input::wall:
            return false;
            break;
        case Input::box:
            return moveBox(input, newPos, dir);
            break;
        case Input::empty:
            input->map[newPos.y][newPos.x] = Input::box;
            return true;
            break;

        default:
            break;
    }
    return false;
}

bool canMoveBox(InputBig *input, Pos2D pos, Pos2D dir) {
    Pos2D newPos = pos + dir;
    if (!newPos.inbound(input->map[0].size(), input->map.size())) {
        return false;
    }
    if (dir == Pos2D{1, 0} || dir == Pos2D{-1, 0}) {
        switch (input->map[newPos.y][newPos.x]) {
            case InputBig::wall:
                return false;

            case InputBig::boxLeft:
            case InputBig::boxRight:
                return canMoveBox(input, newPos, dir);

            case InputBig::empty:
                return true;

            default:
                break;
        }
    } else {
        switch (input->map[newPos.y][newPos.x]) {
            case InputBig::wall:
                return false;

            case InputBig::boxLeft:
                return canMoveBox(input, newPos, dir) &&
                       canMoveBox(input, newPos + Pos2D{1, 0}, dir);

            case InputBig::boxRight:
                return canMoveBox(input, newPos, dir) &&
                       canMoveBox(input, newPos + Pos2D{-1, 0}, dir);

            case InputBig::empty:
                return true;

            default:
                break;
        }
    }
    return false;
}
void moveBoxAll(InputBig *input, Pos2D pos, Pos2D dir) {
    Pos2D newPos = pos + dir;
    if (!newPos.inbound(input->map[0].size(), input->map.size())) {
        return;
    }
    if (dir == Pos2D{1, 0} || dir == Pos2D{-1, 0}) {
        if (input->map[newPos.y][newPos.x] != InputBig::empty) {
            moveBoxAll(input, newPos, dir);
        }
        input->map[newPos.y][newPos.x] = input->map[pos.y][pos.x];
        input->map[pos.y][pos.x] = InputBig::empty;
        return;
    } else {
        if (input->map[newPos.y][newPos.x] == InputBig::boxLeft) {
            moveBoxAll(input, newPos, dir);
            moveBoxAll(input, newPos + Pos2D{1, 0}, dir);
        } else if (input->map[newPos.y][newPos.x] == InputBig::boxRight) {
            moveBoxAll(input, newPos, dir);
            moveBoxAll(input, newPos + Pos2D{-1, 0}, dir);
        }

        input->map[newPos.y][newPos.x] = input->map[pos.y][pos.x];
        input->map[pos.y][pos.x] = InputBig::empty;

        return;
    }
}

size_t grayStar(Input input) {
    size_t res = 0;
    Input inputCopy = input;
    for (int i = 0; i < inputCopy.cmds.size(); ++i) {
        applyCmd(&inputCopy, inputCopy.cmds[i]);
    }

    for (int i = 0; i < inputCopy.map.size(); ++i) {
        for (int j = 0; j < inputCopy.map[i].size(); ++j) {
            if (inputCopy.map[i][j] == Input::box) {
                res += 100 * i + j;
            }
        }
    }
    return res;
}

size_t goldStar(InputBig input) {
    size_t res = 0;
    InputBig inputCopy = input;

    for (int i = 0; i < inputCopy.cmds.size(); ++i) {
        applyCmd(&inputCopy, inputCopy.cmds[i]);
    }

    for (int i = 0; i < inputCopy.map.size(); ++i) {
        for (int j = 0; j < inputCopy.map[i].size(); ++j) {
            if (inputCopy.map[i][j] == InputBig::boxLeft) {
                res += 100 * i + j;
            }
        }
    }
    return res;
}