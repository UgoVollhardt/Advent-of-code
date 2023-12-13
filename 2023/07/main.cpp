#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

enum Type { kFive, kFour, kFull, kThree, kTwoP, kOneP, kHigh, kNone };

typedef struct {
    std::string initial;
    Type type;
    int bid;
    int nbJockers = 0;
} Hand;

typedef std::vector<Hand> Input;

Input parseInput(std::string fileName, bool jocker = false);
Type getType(std::string hand, bool jocker = false);
std::string squach(std::string hand);
void orderHands(Input &hands, bool jocker = false);
int getTotalWinning(std::string fileName, bool jocker = false);
int getNbJocker(std::string line);

int main() {
    std::cout << "--Part 1--" << std::endl;
    std::cout << getTotalWinning("input") << std::endl;
    std::cout << "--Part 2--" << std::endl;
    std::cout << getTotalWinning("input", true) << std::endl;
}

Input parseInput(std::string fileName, bool jocker) {
    std::ifstream file(fileName);
    std::string line, buffer;
    Input res;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            std::stringstream stream(line);
            std::getline(stream, buffer, ' ');
            res.emplace_back(Hand{buffer, getType(buffer, jocker), 0, getNbJocker(buffer)});
            getline(stream, buffer);
            res.back().bid = stoi(buffer);
        }
    }
    return res;
}

std::string squach(std::string hand) {
    std::string output;
    char last = ' ';
    std::vector<char> hand_v(hand.begin(), hand.end());
    std::sort(hand_v.begin(), hand_v.end());
    for (auto elem : hand_v) {
        if (elem != last) {
            output.push_back('1');
            last = elem;
        } else {
            output.back() += 1;
        }
    }
    std::sort(output.begin(), output.end());
    return output;
}

Type getType(std::string hand, bool jocker) {
    int nbJocker = jocker ? getNbJocker(hand) : 0;
    std::string str = squach(hand);
    if (str == "5") {
        return kFive;
    } else if (str == "14") {
        if (nbJocker == 1 || nbJocker == 4) {
            return kFive;
        }
        return kFour;
    } else if (str == "23") {
        if (nbJocker == 2 || nbJocker == 3) {
            return kFive;
        }
        return kFull;
    } else if (str == "113") {
        if (nbJocker == 1 || nbJocker == 3) {
            return kFour;
        }
        return kThree;
    } else if (str == "122") {
        if (nbJocker == 1) {
            return kFull;
        } else if (nbJocker == 2) {
            return kFour;
        }
        return kTwoP;
    } else if (str == "1112") {
        if (nbJocker == 1 || nbJocker == 2) {
            return kThree;
        }
        return kOneP;
    } else if (str == "11111") {
        if (nbJocker == 1) {
            return kOneP;
        }
        return kHigh;
    }
    return kNone;
}

void orderHands(Input &hands, bool jocker) {
    std::map<char, int> headToStrength = {
        {'2', 2},  {'3', 3},  {'4', 4}, {'5', 5},  {'6', 6},
        {'7', 7},  {'8', 8},  {'9', 9}, {'T', 10}, {'J', jocker ? 0 : 11},
        {'Q', 12}, {'K', 13}, {'A', 14}};
    auto comp = [&headToStrength](Hand &first, Hand &second) {
        if (first.type == second.type) {
            for (int i = 0; i < first.initial.size(); ++i) {
                if (first.initial[i] == second.initial[i]) continue;
                return headToStrength[first.initial[i]] < headToStrength[second.initial[i]];
            }
        }
        return (first.type > second.type);
    };
    std::sort(hands.begin(), hands.end(), comp);
}

int getTotalWinning(std::string fileName, bool jocker) {
    auto input = parseInput(fileName, jocker);
    orderHands(input, jocker);
    int sum = 0;
    for (int i = 0; i < input.size(); ++i) {
        sum += (i + 1) * input[i].bid;
    }
    return sum;
}

int getNbJocker(std::string line) { return std::count(line.begin(), line.end(), 'J'); }