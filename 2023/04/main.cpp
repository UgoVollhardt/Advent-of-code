#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>

typedef struct {
    int index;
    std::vector<int> winningsNumbers;
    std::vector<int> obtainedNumbers;
    int match;
} ScratchCard;

std::vector<int> extractNumbers(std::string line);
void removeFirstSpace(std::string &string);
int totalOfPoints(std::string fileName);
int getLinePoints(std::string line);
std::map<int,ScratchCard> parseInput(std::string fileName);
int totalOfCards(std::string fileName);
int getCardIndex(std::string cardInfos);


int main() {
    std::cout << totalOfPoints("input") << std::endl;
    std::cout << totalOfCards("input") << std::endl;
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

std::map<int,ScratchCard> parseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::string buffer;
    std::stringstream sstream;
    std::map<int,ScratchCard> output;
    int matches, index;
    if (file.is_open()) {
        while(file.good()) {
            matches = 0;
            std::getline(file,buffer);
            sstream = std::stringstream(buffer);
            std::getline(sstream, buffer, ':');
            index = getCardIndex(buffer);
            std::getline(sstream, buffer, '|');
            std::vector<int> winningNumbers = extractNumbers(buffer);
            std::getline(sstream, buffer);
            std::vector<int> obtainedNumber = extractNumbers(buffer);
            for (int i = 0; i < obtainedNumber.size(); ++i) {
                if (std::find(winningNumbers.begin(), winningNumbers.end(),obtainedNumber[i]) != winningNumbers.end()) {
                    matches++;
                }
            }
            output[index] = ScratchCard{index,winningNumbers, obtainedNumber, matches};
        }
    }
    return output;
}

int totalOfCards(std::string fileName) {
    std::map<int,ScratchCard> cards = parseInput(fileName);
    std::list<ScratchCard> queue;
    ScratchCard current;
    int count;

    for (auto card : cards) {
        queue.push_back(card.second);
    }

    while (!queue.empty()) {
        current = queue.front();
        queue.pop_front();
        count++;
        for (int i = 1; i <= std::min<int>(current.match, cards.size()); ++i) {
            queue.push_back(cards[current.index + i]);
        }
    }
    return count;
}

int getCardIndex(std::string cardInfos) {
    while(!isdigit(cardInfos[0])) cardInfos.erase(0,1);
    return stoi(cardInfos);
}