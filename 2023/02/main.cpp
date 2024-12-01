#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

int sumOfPossibleGamesIndexes(std::string name);
int isPossibleGame(std::string line);
int getGameIndex(std::string gameInfo);
void removeFirstSpace(std::string &string);
int sumOfPowerMinAmountCubes(std::string name);
int getLinePower(std::string line);

int main() {
    std::cout << sumOfPossibleGamesIndexes("input") << std::endl;
    std::cout << sumOfPowerMinAmountCubes("input") << std::endl;
}

int sumOfPossibleGamesIndexes(std::string name) {
    std::ifstream file(name);
    std::string line;
    int number, sum = 0;
    if (file.is_open()) {
        while(file.good()) {
            std::getline(file,line);
            sum += isPossibleGame(line);
        }
    }
    return sum;
}

int isPossibleGame(std::string line) {
    std::stringstream sstream1(line), sstream2;
    std::string gameInfoStr, drawsStr, draw, colorCubes, value, color;
    std::getline(sstream1, gameInfoStr, ':');
    std::getline(sstream1, drawsStr);

    int index = getGameIndex(gameInfoStr);
    
    sstream1 = std::stringstream(drawsStr);
    while(std::getline(sstream1, draw, ';')) {
        removeFirstSpace(draw);
        sstream2 = std::stringstream(draw);
        while (std::getline(sstream2, colorCubes, ',')) {
            removeFirstSpace(colorCubes);
            size_t pos = colorCubes.find(" ");
            value = colorCubes.substr(0,pos);
            color = colorCubes.substr(pos+1);
            if (!color.compare("red") && stoi(value) > MAX_RED) return 0;
            if (!color.compare("green") && stoi(value) > MAX_GREEN) return 0;
            if (!color.compare("blue") && stoi(value) > MAX_BLUE) return 0;
        }
    }
    return index;
}

int getGameIndex(std::string gameInfo) {
    size_t pos = gameInfo.find(" ");
    gameInfo.erase(0,pos);
    return stoi(gameInfo);
}

void removeFirstSpace(std::string &string) {
    if (string[0] == ' ') string.erase(0,1);
}

int sumOfPowerMinAmountCubes(std::string name) {
    std::ifstream file(name);
    std::string line;
    int number, sum = 0;
    if (file.is_open()) {
        while(file.good()) {
            std::getline(file,line);
            sum += getLinePower(line);
        }
    }
    return sum;
}

int getLinePower(std::string line) {
    size_t red = 0, blue = 0, green = 0;
    std::stringstream sstream1(line), sstream2;
    std::string gameInfoStr, drawsStr, draw, colorCubes, value, color;
    std::getline(sstream1, gameInfoStr, ':');
    std::getline(sstream1, drawsStr);
    
    sstream1 = std::stringstream(drawsStr);
    while(std::getline(sstream1, draw, ';')) {
        removeFirstSpace(draw);
        sstream2 = std::stringstream(draw);
        while (std::getline(sstream2, colorCubes, ',')) {
            removeFirstSpace(colorCubes);
            size_t pos = colorCubes.find(" ");
            value = colorCubes.substr(0,pos);
            color = colorCubes.substr(pos+1);
            if (!color.compare("red") && stoi(value) > red) red = stoi(value);
            if (!color.compare("green") && stoi(value) > green) green = stoi(value);
            if (!color.compare("blue") && stoi(value) > blue) blue = stoi(value);
        }
    }
    return red * green * blue;
}
