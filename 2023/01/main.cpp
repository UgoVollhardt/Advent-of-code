#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

std::string replaceDigitNameByDigit(std::string line);
int getFirstAndLast(std::string line);
int findSumFirstAndLastDigits_part1(std::string name);
int findSumFirstAndLastDigits_part2(std::string name);

int main() {
    std::cout << findSumFirstAndLastDigits_part1("input") << std::endl;
    std::cout << findSumFirstAndLastDigits_part2("input") << std::endl;
}

int findSumFirstAndLastDigits_part1(std::string name) {
    std::ifstream file(name);
    std::string line;
    int number, sum = 0;
    if (file.is_open()) {
        while(file.good()) {
            std::getline(file,line);
            number = getFirstAndLast(line);
            sum += number;
        }
    }
    return sum;
}

std::string replaceDigitNameByDigit(std::string line) {
    std::map<std::string, std::string> map = {{"one","one1one"}, 
            {"two","two2two"}, {"three","three3three"}, 
            {"four","four4four"}, {"five","five5five"}, 
            {"six","six6six"}, {"seven","seven7seven"}, 
            {"eight","eight8eight"}, {"nine","nine9nine"}};
    std::string line_cp = line;
    for (auto it = map.begin(); it != map.end(); ++it) {
        std::size_t pos = line_cp.find(it->first);
        if (pos != std::string::npos) {
            line_cp.replace(pos, it->first.length(), it->second);
        }

        pos = line_cp.rfind(it->first);
        if (pos != std::string::npos) {
            line_cp.replace(pos, it->first.length(), it->second);
        }
    }
    return line_cp;
}

int findSumFirstAndLastDigits_part2(std::string name) {
    std::ifstream file(name);
    std::string line, lineRec, LineLin;
    int number;
    int sum = 0;
    if (file.is_open()) {
        while(file.good()) {
            std::getline(file,line);
            line = replaceDigitNameByDigit(line);
            number = getFirstAndLast(line);
            sum += number;
        }
    }
    return sum;
}

int getFirstAndLast(std::string line) {
    int first = -1, last = -1;
    for (int i = 0; i < line.size(); ++i) {
        if (std::isdigit(line[i])) {
            if (first == -1) {
                first = line[i] - '0';
            } 
            last = line[i] - '0';
        }
    }
    return first*10 + last;
}