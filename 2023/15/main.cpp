#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

typedef std::vector<std::string> Input;
typedef std::vector<Input> History;
typedef struct {
    std::string label = "";
    int targetBox = 0;
    char operation = 0;
    int focalLength = 0;
} SequenceStep;

long getResPart1(std::string fileName);
long getResPart2(std::string fileName);
int hash(std::string instruct);
SequenceStep getStep(std::string instruction);
void insertInBoxes(SequenceStep step, std::map<int, std::list<SequenceStep>>& boxes);
long computeFocusPower(std::map<int, std::list<SequenceStep>>& boxes);

inline bool operator==(const SequenceStep& lhs, const SequenceStep& rhs) {
    return lhs.label == rhs.label;
}

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
    long sum = 0;
    std::ifstream file(fileName);
    std::string line, buffer;
    std::stringstream stream;
    Input res;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            stream = std::stringstream(line);
            while (stream.good()) {
                getline(stream, buffer, ',');
                sum += hash(buffer);
            }
        }
    }

    return sum;
}

long getResPart2(std::string fileName) {
    long sum = 0;
    std::ifstream file(fileName);
    std::string line, buffer;
    std::stringstream stream;
    SequenceStep currentStep;
    std::map<int, std::list<SequenceStep>> boxes;
    Input res;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            stream = std::stringstream(line);
            while (stream.good()) {
                getline(stream, buffer, ',');
                currentStep = getStep(buffer);
                if (currentStep.operation == '=') {
                    insertInBoxes(currentStep, boxes);
                } else {
                    boxes[currentStep.targetBox].remove(currentStep);
                }
            }
        }
    }

    return computeFocusPower(boxes);
}

int hash(std::string instruct) {
    int current = 0;
    for (auto letter : instruct) {
        current += letter;
        current *= 17;
        current %= 256;
    }
    return current;
}

SequenceStep getStep(std::string instruction) {
    SequenceStep ouput;
    for (auto letter : instruction) {
        if (isalpha(letter)) {
            ouput.targetBox += letter;
            ouput.targetBox *= 17;
            ouput.targetBox %= 256;
            ouput.label += letter;
        } else if (isdigit(letter)) {
            ouput.focalLength = letter - '0';
        } else {
            ouput.operation = letter;
        }
    }
    return ouput;
}

void insertInBoxes(SequenceStep step, std::map<int, std::list<SequenceStep>>& boxes) {
    auto it = boxes.find(step.targetBox);
    if (it == boxes.end()) {
        boxes[step.targetBox].emplace_back(step);
    } else {
        auto focalIt = std::find(it->second.begin(), it->second.end(), step);
        if (focalIt != it->second.end()) {
            focalIt->focalLength = step.focalLength;
        } else {
            it->second.emplace_back(step);
        }
    }
}

long computeFocusPower(std::map<int, std::list<SequenceStep>>& boxes) {
    long sum = 0;
    for (auto elem : boxes) {
        for (auto it = elem.second.begin(); it != elem.second.end(); ++it) {
            sum +=
                (elem.first + 1) * (std::distance(elem.second.begin(), it) + 1) * it->focalLength;
        }
    }
    return sum;
}