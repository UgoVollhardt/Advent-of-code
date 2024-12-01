#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

typedef std::vector<std::string> Input;
typedef std::vector<Input> History;

long getResPart1(std::string fileName);
long getResPart2(std::string fileName);
Input parseInput(std::string fileName);
void tiltPlateformNorth(Input &plateforme);
void tiltPlateformWest(Input &plateforme);
void tiltPlateformSouth(Input &plateforme);
void tiltPlateformEast(Input &plateforme);
long getLoad(Input plateforme);
long isStateInHistory(History &history, Input plateforme);

int main() {
    std::cout << "--Part 1--" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << getResPart1("input") << std::endl;
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time : "
              << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "µs"
              << std::endl;
    std::cout << "--Part 2--" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    std::cout << getResPart2("input") << std::endl;
    stop = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time : "
              << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "µs"
              << std::endl;
}

long getResPart1(std::string fileName) {
    auto input = parseInput(fileName);
    tiltPlateformNorth(input);
    return getLoad(input);
}

long getResPart2(std::string fileName) {
    History history;
    auto input = parseInput(fileName);
    long last = -1;
    do {
        tiltPlateformNorth(input);
        tiltPlateformWest(input);
        tiltPlateformSouth(input);
        tiltPlateformEast(input);
        last = isStateInHistory(history, input);
        history.push_back(input);
    } while (last == -1);
    input = history[last + (1000000000 - history.size()) % (history.size() - last - 1)];
    return getLoad(input);
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::string line;
    Input res;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            res.emplace_back(line);
        }
    }
    return res;
}

void tiltPlateformNorth(Input &plateforme) {
    for (int i = 1; i < plateforme.size(); ++i) {
        for (int j = 0; j < plateforme[i].size(); ++j) {
            if (plateforme[i][j] == 'O') {
                int shift = 1;
                while (i - shift >= 0 && plateforme[i - shift][j] == '.') {
                    shift++;
                }
                if (shift != 0) {
                    plateforme[i][j] = '.';
                    plateforme[i - shift + 1][j] = 'O';
                }
            }
        }
    }
}

void tiltPlateformWest(Input &plateforme) {
    for (int i = 0; i < plateforme.size(); ++i) {
        for (int j = 0; j < plateforme[i].size(); ++j) {
            if (plateforme[i][j] == 'O') {
                int shift = 1;
                while (j - shift >= 0 && plateforme[i][j - shift] == '.') {
                    shift++;
                }
                if (shift != 0) {
                    plateforme[i][j] = '.';
                    plateforme[i][j - shift + 1] = 'O';
                }
            }
        }
    }
}
void tiltPlateformSouth(Input &plateforme) {
    for (int i = plateforme.size() - 2; i >= 0; --i) {
        for (int j = 0; j < plateforme[i].size(); ++j) {
            if (plateforme[i][j] == 'O') {
                int shift = 1;
                while (i + shift < plateforme.size() && plateforme[i + shift][j] == '.') {
                    shift++;
                }
                if (shift != 0) {
                    plateforme[i][j] = '.';
                    plateforme[i + shift - 1][j] = 'O';
                }
            }
        }
    }
}
void tiltPlateformEast(Input &plateforme) {
    for (int i = 0; i < plateforme.size(); ++i) {
        for (int j = plateforme[i].size() - 1; j >= 0; --j) {
            if (plateforme[i][j] == 'O') {
                int shift = 1;
                while (j + shift < plateforme[i].size() && plateforme[i][j + shift] == '.') {
                    shift++;
                }
                if (shift != 0) {
                    plateforme[i][j] = '.';
                    plateforme[i][j + shift - 1] = 'O';
                }
            }
        }
    }
}

long getLoad(Input plateforme) {
    long sum = 0;
    for (int i = 0; i < plateforme.size(); ++i) {
        for (int j = 0; j < plateforme[i].size(); ++j) {
            if (plateforme[i][j] == 'O') sum += plateforme.size() - i;
        }
    }
    return sum;
}

long isStateInHistory(History &history, Input plateforme) {
    auto it = std::find(history.begin(), history.end(), plateforme);
    if (it != history.end()) {
        return it - history.begin();
    }
    return -1;
}