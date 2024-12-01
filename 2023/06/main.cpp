#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

typedef struct {
    long time;
    long distance;
} Race;

typedef std::vector<Race> Input;

int getRecordWaysProduct(std::string fileName, bool flag = false);
Input parseInput(std::string fileName, bool flag = false);
void removeFrontSpaces(std::string &line);
void removeAllSpaces(std::string &line);
std::vector<long> getValues(std::string line, bool flag = false);
int getNumberOfWays(Race race, bool flag = false);

int main() {
    std::cout << getRecordWaysProduct("input") << std::endl;
    std::cout << getRecordWaysProduct("input", true) << std::endl;
}

int getRecordWaysProduct(std::string fileName, bool flag) {
    int prod = 1;
    Input input = parseInput(fileName, flag);
    for (int i = 0; i < input.size(); ++i) {
        prod *= getNumberOfWays(input[i], flag);
    }
    return prod;
}

Input parseInput(std::string fileName, bool flag) {
    std::ifstream file(fileName);
    std::string line;
    Input res;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            if (line.find("Time") != std::string::npos) {
                auto values = getValues(line, flag);
                for (int i = 0; i < values.size(); ++i) {
                    res.emplace_back(Race{values[i], 0});
                }
            } else if (line.find("Distance") != std::string::npos) {
                auto values = getValues(line, flag);
                for (int i = 0; i < values.size(); ++i) {
                    res[i].distance = values[i];
                }
            }
        }
    }
    return res;
}

void removeFrontSpaces(std::string &line) {
    while (line[0] == ' ') line.erase(0, 1);
}

void removeAllSpaces(std::string &line) {
    auto index = line.find(" ", 0);
    while (index != std::string::npos) {
        line.erase(index, 1);
        index = line.find(" ", index);
    }
}

std::vector<long> getValues(std::string line, bool flag) {
    std::string buffer;
    std::vector<long> output;
    std::stringstream stream(line);
    if (flag) {
        removeAllSpaces(line);
        stream = std::stringstream(line);
        getline(stream, buffer, ':');
        getline(stream, buffer);
        output.emplace_back(stol(buffer));
    } else {
        getline(stream, buffer, ' ');
        while (getline(stream, buffer, ' ')) {
            if (!buffer.empty()) {
                output.emplace_back(stol(buffer));
            }
        }
    }

    return output;
}

int getNumberOfWays(Race race, bool flag) {
    int sum = 0;
    for (int i = 0; i < race.time; ++i) {
        if (i * (race.time - i) > race.distance) sum++;
    }
    return sum;
}