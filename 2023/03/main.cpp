#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

typedef struct {
    int value;
    size_t index_b;
    size_t index_e;
    size_t line = 0;
} Number;

int sumOfNumberNearSymbol(std::string name);
std::vector<std::string> parseInput(std::string fileName);
int getPartNumberSum(std::vector<std::string> table);
std::vector<Number> getLineNumbers(std::string line, size_t index = 0);
int isPartNumber(Number num, int line, std::vector<std::string> table);
int sumOfGearRatios(std::string name);
std::map<std::pair<int,int>,std::vector<int>> getStarsGears(std::vector<std::string> table);
std::vector<Number> getNumbers(std::vector<std::string> table);
std::vector<std::pair<int,int>> hasStars(Number number, std::vector<std::string> table);


int main() {
    std::cout << sumOfNumberNearSymbol("input") << std::endl;
    std::cout << sumOfGearRatios("input") << std::endl;
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

std::vector<Number> getLineNumbers(std::string line, size_t index) {
    std::vector<Number> ouput;
    std::string current_number;
    for (int i = 0; i < line.size(); ++i) {
        if (isdigit(line[i])) {
            current_number += line[i];
        } else if (!current_number.empty()) {
            Number val = {stoi(current_number),size_t(i)-current_number.size(),size_t(i)-1,index};
            ouput.emplace_back(val);
            current_number.clear();
        }
    }
    if (!current_number.empty()) {
        Number val = {stoi(current_number),line.size()-current_number.size(),line.size()-1,index};
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

int sumOfGearRatios(std::string name) {
    std::vector<std::string> table = parseInput(name);
    std::map<std::pair<int,int>,std::vector<int>> starsGears = getStarsGears(table);
    int sum = 0;
    for (auto elem : starsGears) {
        if (elem.second.size() == 2) {
            sum += elem.second[0]*elem.second[1];
        }
    }
    return sum;
}

std::map<std::pair<int,int>,std::vector<int>> getStarsGears(std::vector<std::string> table) {
    std::map<std::pair<int,int>,std::vector<int>> output;
    auto numbers = getNumbers(table);
    for (int i = 0; i < numbers.size(); ++i) {
        auto stars = hasStars(numbers[i], table);
        for (int j = 0; j < stars.size(); ++j) {
            output[stars[j]].push_back(numbers[i].value);
        }
    }
    return output;
}

std::vector<Number> getNumbers(std::vector<std::string> table) {
    std::vector<Number> output;
    for (int i = 0; i < table.size(); ++i) {
        auto val = getLineNumbers(table[i],i);
        output.insert(output.end(), val.begin(), val.end());
    }
    return output;
}

std::vector<std::pair<int,int>> hasStars(Number number, std::vector<std::string> table) {
    std::vector<std::pair<int,int>> res;
    int i_min = std::max<int>(number.line - 1, 0);
    int i_max = std::min<int>(number.line + 1, table.size()-1) + 1;
    int j_min = std::max<int>(number.index_b - 1, 0);
    int j_max = std::min<int>(number.index_e + 1, table[0].size()-1) + 1;
    for (int i = i_min; i < i_max; ++i ) {
        for (int j = j_min; j < j_max; ++j) {
            if (table[i][j] == '*') {
                res.push_back(std::make_pair(i,j));
            }
        }
    }
    return res;
}

