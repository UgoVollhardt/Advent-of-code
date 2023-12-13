#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

typedef struct {
    std::string name;
    std::vector<std::string> desti;
} Node;

typedef struct {
    std::map<std::string, Node> Nodes;
    std::vector<int> instructions;
    std::vector<std::string> startingPoints;
} Input;

int getNbSteps(std::string fileName);
long getNbStepsAsGhosts(std::string fileName);
Input parseInput(std::string fileName);
std::pair<std::string, Node> getNode(std::string line);
std::vector<int> getInstructions(std::string line);
bool isAtEndingPoint(std::vector<std::string> currentPoints);
long lcm(std::vector<int> values);

int main() {
    std::cout << "--Part 1--" << std::endl;
    std::cout << getNbSteps("input") << std::endl;
    std::cout << "--Part 2--" << std::endl;
    std::cout << getNbStepsAsGhosts("input") << std::endl;
}

int getNbSteps(std::string fileName) {
    auto input = parseInput(fileName);
    std::string currentNode = "AAA";
    int currentInstruction = 0;
    int steps = 0;
    while (currentNode != "ZZZ") {
        int dir = input.instructions[currentInstruction++];
        currentNode = input.Nodes[currentNode].desti[dir];
        if (currentInstruction == input.instructions.size()) currentInstruction = 0;
        steps++;
    }
    return steps;
}

long getNbStepsAsGhosts(std::string fileName) {
    auto input = parseInput(fileName);
    std::vector<int> loopSizeNodes;
    int currentInstruction = 0;
    long steps = 0;
    for (auto currentStartingNode : input.startingPoints) {
        std::string currentNode = currentStartingNode;
        int loopSize = 0;
        currentInstruction = 0;
        while (currentNode.back() != 'Z') {
            int dir = input.instructions[currentInstruction++];
            currentNode = input.Nodes[currentNode].desti[dir];
            if (currentInstruction == input.instructions.size()) currentInstruction = 0;
            loopSize++;
        }
        loopSizeNodes.push_back(loopSize);
    }
    return lcm(loopSizeNodes);
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::string line, buffer;
    Input res;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            if (line.empty()) continue;
            if (line.find('=', 0) != std::string::npos) {
                auto node = getNode(line);
                res.Nodes.insert(node);
                if (node.first.back() == 'A') {
                    res.startingPoints.push_back(node.first);
                }
            } else {
                res.instructions = getInstructions(line);
            }
        }
    }
    return res;
}

std::pair<std::string, Node> getNode(std::string line) {
    std::string buffer;
    std::stringstream stream(line);
    Node output;
    getline(stream, buffer, ' ');
    output.name = buffer;
    getline(stream, buffer, '(');
    getline(stream, buffer, ',');
    output.desti.push_back(buffer);
    getline(stream, buffer, ' ');
    getline(stream, buffer, ')');
    output.desti.push_back(buffer);

    return std::make_pair(output.name, output);
}

std::vector<int> getInstructions(std::string line) {
    std::vector<int> ouput;
    for (auto elem : line) {
        ouput.push_back(elem == 'L' ? 0 : 1);
    }
    return ouput;
}

bool isAtEndingPoint(std::vector<std::string> currentPoints) {
    for (auto elem : currentPoints) {
        if (elem.back() != 'Z') return false;
    }
    return true;
}

long lcm(std::vector<int> values) {
    long output = values.front();
    for (auto elem : values) {
        output = elem * output / std::gcd(elem, output);
    }
    return output;
}
