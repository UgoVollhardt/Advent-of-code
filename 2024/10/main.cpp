#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <tuple>
#include <vector>

struct Pos2D {
    int x;
    int y;

    Pos2D operator+(Pos2D &other) { return Pos2D{x + other.x, y + other.y}; }
    bool operator==(const Pos2D &other) { return x == other.x && y == other.y; }
    bool inbound(int maxWidth, int maxHeight) {
        return x >= 0 && x < maxWidth && y >= 0 && y < maxHeight;
    }
};

struct TrailStep {
    Pos2D pos;
    int height;

    TrailStep(Pos2D _pos = Pos2D{-1, -1}, int _height = 0)
        : pos(_pos), height(_height) /*, id(0)*/ {}
    bool operator==(const TrailStep &other) { return pos == other.pos && height == other.height; }
};

struct Trail {
    TrailStep head;
    std::vector<TrailStep> tails;
    std::vector<TrailStep> steps;
};

typedef std::vector<std::vector<int>> Input;

Input parseInput(std::string fileName);
std::vector<Trail> getTrailTracks(Input input);
std::tuple<std::vector<TrailStep>, std::vector<TrailStep>> constructSteps(Input input, Pos2D head,
                                                                          bool DuplicateSteps);

size_t grayStar(Input input);
size_t goldStar(Input input);

int main(int, char *[]) {
    auto input = parseInput("input-test");
    std::cout << "res gray star : " << grayStar(input) << std::endl;
    std::cout << "res gold star : " << goldStar(input) << std::endl;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::string line;
    Input res;

    std::cout << "Parsing input" << std::endl;

    if (file.is_open()) {
        while (file.good()) {
            std::vector<int> row;
            std::getline(file, line);
            for (auto num : line) {
                row.push_back(num - '0');
            }
            res.push_back(row);
        }
    } else {
        std::cout << "fichier non trouvé";
    }
    return res;
}

std::vector<Trail> getTrailTracks(Input input, bool DuplicateSteps) {
    std::vector<Trail> res;
    for (int i = 0; i < input.size(); ++i) {
        for (int j = 0; j < input[i].size(); ++j) {
            if (input[i][j] == 0) {
                auto [steps, tails] = constructSteps(input, Pos2D{j, i}, DuplicateSteps);
                res.push_back(Trail{steps[0], tails, steps});
            }
        }
    }

    return res;
}

std::tuple<std::vector<TrailStep>, std::vector<TrailStep>> constructSteps(Input input, Pos2D head,
                                                                          bool DuplicateSteps) {
    std::vector<TrailStep> steps;
    std::vector<TrailStep> tails;
    std::list<TrailStep> trailStack;
    TrailStep current;
    std::vector<Pos2D> dirs{Pos2D{0, 1}, Pos2D{1, 0}, Pos2D{0, -1}, Pos2D{-1, 0}};
    trailStack.emplace_back(TrailStep(head, 0));

    while (!trailStack.empty()) {
        current = trailStack.back();
        trailStack.pop_back();
        steps.push_back(current);

        for (auto dir : dirs) {
            Pos2D newPos = current.pos + dir;
            if (newPos.inbound(input[0].size(), input.size())) {
                int height = input[newPos.y][newPos.x];
                if (height == current.height + 1) {
                    TrailStep newStep(newPos, height);
                    auto itStep = std::find(steps.begin(), steps.end(), newStep);
                    if (!DuplicateSteps || itStep == steps.end()) {
                        trailStack.emplace_back(newStep);
                        if (newStep.height == 9) {
                            tails.push_back(newStep);
                        }
                    }
                }
            }
        }
    }
    return std::make_tuple(steps, tails);
}

size_t grayStar(Input input) {
    auto trails = getTrailTracks(input, true);
    size_t res = 0;
    for (auto trail : trails) {
        res += trail.tails.size();
    }
    return res;
}

size_t goldStar(Input input) {
    auto trails = getTrailTracks(input, false);
    size_t res = 0;
    for (auto trail : trails) {
        res += trail.tails.size();
    }
    return res;
}