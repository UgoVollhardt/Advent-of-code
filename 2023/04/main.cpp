#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

std::vector<int> extractNumbers(std::string line);
void removeFirstSpace(std::string &string);
int totalOfPoints(std::string fileName);
int getLinePoints(std::string line);

int main() {
    std::cout << totalOfPoints("input") << std::endl;
}

int totalOfPoints(std::string fileName) {
    std::ifstream file(fileName);
    std::string line;
    int sum = 0;
    if (file.is_open()) {
        while(file.good()) {
            std::getline(file,line);
            sum += getLinePoints(line);
        }
    }
    return sum;
}

std::vector<int> extractNumbers(std::string line) {
    std::vector<int> output;
    std::stringstream sstream;
    std::string buffer;
    removeFirstSpace(line);
    sstream = std::stringstream(line);
    while(std::getline(sstream,buffer, ' ')) {
        if (!buffer.empty()) output.push_back(stoi(buffer));
    }
    return output;
}

int getLinePoints(std::string line) {
    std::stringstream sstream(line);
    std::string buffer;
    int points = 0;
    std::getline(sstream, buffer, ':');
    std::getline(sstream, buffer, '|');
    std::vector<int> winningNumbers = extractNumbers(buffer);
    std::getline(sstream, buffer);
    std::vector<int> obtainedNumber = extractNumbers(buffer);
    for (int i = 0; i < obtainedNumber.size(); ++i) {
        if (std::find(winningNumbers.begin(), winningNumbers.end(),obtainedNumber[i]) != winningNumbers.end()) {
            points = points ? points*2 : 1;
        }
    }
    return points;
}

void removeFirstSpace(std::string &string) {
    while(string[0] == ' ') string.erase(0,1);
}