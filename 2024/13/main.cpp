#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <tuple>
#include <unordered_map>
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

std::ostream &operator<<(std::ostream &os, Pos2D pos) {
    os << "[" << pos.x << "," << pos.y << "]";
    return os;
}

bool isEqual(double a, double b) { return std::fabs(a - b) < 0.000001; }

struct Machine {
    Pos2D A;
    Pos2D B;
    Pos2D prize;
};

typedef std::vector<Machine> Input;

Input parseInput(std::string fileName);
size_t computeCost(Machine machine);

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

    // for (auto machine : input) {
    //     // std::cout << "Button A : " << machine.A << std::endl;
    //     // std::cout << "Button B : " << machine.B << std::endl;
    //     // std::cout << "Prize : " << machine.prize << std::endl;
    //     computeCost(machine);
    // }
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::string line, elem;
    Input res;
    Pos2D A, B, prize;
    int status = 0;

    std::cout << "Parsing input" << std::endl;

    if (file.is_open()) {
        while (file.good()) {
            std::vector<int> row;
            std::getline(file, line);
            if (!line.empty()) {
                std::stringstream ss(line);
                if (status == 0) {
                    std::getline(ss, elem, '+');
                    std::getline(ss, elem, ',');
                    A.x = std::stoi(elem);
                    std::getline(ss, elem, '+');
                    std::getline(ss, elem, ',');
                    A.y = std::stoi(elem);
                    status = 1;
                } else if (status == 1) {
                    std::getline(ss, elem, '+');
                    std::getline(ss, elem, ',');
                    B.x = std::stoi(elem);
                    std::getline(ss, elem, '+');
                    std::getline(ss, elem, ',');
                    B.y = std::stoi(elem);
                    status = 2;
                } else if (status == 2) {
                    std::getline(ss, elem, '=');
                    std::getline(ss, elem, ',');
                    prize.x = std::stoi(elem);
                    std::getline(ss, elem, '=');
                    std::getline(ss, elem, ',');
                    prize.y = std::stoi(elem);
                    status = 0;
                    res.push_back(Machine{A, B, prize});
                }
            }
        }
    } else {
        std::cout << "fichier non trouvé";
    }
    return res;
}

size_t computeCost(Machine machine) {
    double x, y;
    // int X, Y;
    size_t res = 0;
    x = (double(machine.prize.y) / double(machine.B.y) - double(machine.prize.x) / (machine.B.x)) /
        (double(machine.A.y) / double(machine.B.y) - double(machine.A.x) / (machine.B.x));
    y = (double(machine.prize.y) / double(machine.A.y) - double(machine.prize.x) / (machine.A.x)) /
        (double(machine.B.y) / double(machine.A.y) - double(machine.B.x) / (machine.A.x));
    std::cout << "A: " << x << "|B: " << y << std::endl;
    // X = int(machine.A.x * x) + machine.B.x * y;
    if (isEqual(x, std::round(x)) && isEqual(y, std::round(y)) && x <= 100.0 && y <= 100.0) {
        std::cout << "return " << x << "|" << y << std::endl;
        return x * 3 + y;
    }
    return 0;
}

size_t grayStar(Input input) {
    size_t res = 0;
    for (auto machine : input) {
        res += computeCost(machine);
    }
    return res;
}
