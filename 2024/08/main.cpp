#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

typedef struct vect {
    int x;
    int y;
    vect(int _x = -1, int _y = -1) : x(_x), y(_y) {}
    vect operator+(vect& other) { return vect{x + other.x, y + other.y}; }
    vect operator-(vect& other) { return vect{x - other.x, y - other.y}; }
    bool operator!=(const vect& other) { return x != other.x || y != other.y; }
    bool inbound(const vect size) { return x >= 0 && x < size.x && y >= 0 && y < size.y; }
} Vect;

typedef struct {
    std::unordered_map<char, std::vector<Vect>> antennas;
    vect size;
} Input;

typedef std::vector<std::vector<std::vector<char>>> NodeMap;

Input parseInput(std::string fileName);
NodeMap getNodeMap(Input input);
NodeMap getNodeMapResonant(Input input);

size_t grayStar(Input input);
size_t goldStar(Input input);

int main(int, char*[]) {
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
            res.size.x = line.size();
            if (!line.empty()) {
                for (int i = 0; i < line.size(); ++i) {
                    if (line[i] != '.') {
                        if (res.antennas.contains(line[i])) {
                            res.antennas[line[i]].push_back(Vect{i, int(lineId)});
                        } else {
                            res.antennas.insert(
                                std::make_pair(line[i], std::vector<Vect>{Vect{i, int(lineId)}}));
                        }
                    }
                }
            }
            lineId++;
        }
        res.size.y = lineId;
    } else {
        std::cout << "fichier non trouvé";
    }
    return res;
}

NodeMap getNodeMap(Input input) {
    NodeMap res;
    for (int i = 0; i < input.size.y; ++i) {
        std::vector<std::vector<char>> line;
        for (int j = 0; j < input.size.x; ++j) {
            line.push_back(std::vector<char>{});
        }
        res.push_back(line);
    }

    for (auto antenna : input.antennas) {
        for (int i = 0; i < antenna.second.size() - 1; ++i) {
            for (int j = i + 1; j < antenna.second.size(); ++j) {
                Vect diff = antenna.second[i] - antenna.second[j];
                Vect antinode = antenna.second[i] + diff;
                if (antinode.inbound(input.size)) {
                    res[antinode.y][antinode.x].push_back(antenna.first);
                }
                antinode = antenna.second[j] - diff;
                if (antinode.inbound(input.size)) {
                    res[antinode.y][antinode.x].push_back(antenna.first);
                }
            }
        }
    }
    return res;
}

NodeMap getNodeMapResonant(Input input) {
    NodeMap res;
    for (int i = 0; i < input.size.y; ++i) {
        std::vector<std::vector<char>> line;
        for (int j = 0; j < input.size.x; ++j) {
            line.push_back(std::vector<char>{});
        }
        res.push_back(line);
    }

    for (auto antenna : input.antennas) {
        for (int i = 0; i < antenna.second.size() - 1; ++i) {
            for (int j = i + 1; j < antenna.second.size(); ++j) {
                Vect diff = antenna.second[i] - antenna.second[j];
                Vect antinode = antenna.second[i];
                while (antinode.inbound(input.size)) {
                    res[antinode.y][antinode.x].push_back(antenna.first);
                    antinode = antinode + diff;
                }
                antinode = antenna.second[j];
                while (antinode.inbound(input.size)) {
                    res[antinode.y][antinode.x].push_back(antenna.first);
                    antinode = antinode - diff;
                }
            }
        }
    }
    return res;
}

size_t grayStar(Input input) {
    size_t res = 0;
    auto map = getNodeMap(input);
    for (auto line : map) {
        for (auto pos : line) {
            res += pos.size() != 0 ? 1 : 0;
        }
    }
    return res;
}

size_t goldStar(Input input) {
    size_t res = 0;
    auto map = getNodeMapResonant(input);
    for (auto line : map) {
        for (auto pos : line) {
            res += pos.size() != 0 ? 1 : 0;
        }
    }
    return res;
}
