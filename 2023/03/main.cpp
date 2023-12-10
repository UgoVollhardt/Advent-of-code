#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

typedef struct {
    int value;
    size_t index_b;
    size_t index_e;
} Number;


int sumOfNumberNearSymbol(std::string name);
std::vector<std::string> parseInput(std::string fileName);
int getPartNumberSum(std::vector<std::string> table);
std::vector<Number> getLineNumbers(std::string line);
int isPartNumber(Number num, int line, std::vector<std::string> table);


int main() {
    std::cout << sumOfNumberNearSymbol("input") << std::endl;
}

int sumOfNumberNearSymbol(std::string name) {
    std::vector<std::string> table = parseInput(name);
    return getPartNumberSum(table);
}

std::vector<std::string> parseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::string line;
    std::vector<std::string> ouput;
    if (file.is_open()) {
        while(file.good()) {
            std::getline(file,line);
            ouput.push_back(line);
        }
    }
    return ouput;
}

int getPartNumberSum(std::vector<std::string> table) {
    int sum = 0;
    std::vector<Number> currentLineNumbers;
    for (int i = 0; i < table.size(); ++i) {
        currentLineNumbers = getLineNumbers(table[i]);
        for (int j = 0; j < currentLineNumbers.size(); ++j) {
            sum += isPartNumber(currentLineNumbers[j],i,table);
        }
    } 
    return sum;
}

std::vector<Number> getLineNumbers(std::string line) {
    std::vector<Number> ouput;
    std::string current_number;
    for (int i = 0; i < line.size(); ++i) {
        if (isdigit(line[i])) {
            current_number += line[i];
        } else if (!current_number.empty()) {
            Number val = {stoi(current_number),size_t(i)-current_number.size(),size_t(i)-1};
            ouput.emplace_back(val);
            current_number.clear();
        }
    }
    if (!current_number.empty()) {
        Number val = {stoi(current_number),line.size()-current_number.size(),line.size()-1};
        ouput.emplace_back(val);
    }
    return ouput;
}

int isPartNumber(Number num, int line, std::vector<std::string> table) {
    bool flag = false;
    int i_min = std::max<int>(line - 1, 0);
    int i_max = std::min<int>(line + 1, table.size()-1) + 1;
    int j_min = std::max<int>(num.index_b - 1, 0);
    int j_max = std::min<int>(num.index_e + 1, table[0].size()-1) + 1;
    for (int i = i_min; i < i_max; ++i ) {
        for (int j = j_min; j < j_max; ++j) {
            flag = flag || (table[i][j] != '.' && !isdigit(table[i][j]));
        }
    }
    return flag ? num.value : 0;
}