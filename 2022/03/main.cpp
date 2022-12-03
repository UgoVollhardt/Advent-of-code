#include <fstream>
#include <iostream>
#include <string>

int findCostBetweenCompatments(std::string name);
int findCostTeams(std::string name);

int main() {
    std::cout << findCostBetweenCompatments("input") << std::endl;
    std::cout << findCostTeams("input") << std::endl;
}

int findCostBetweenCompatments(std::string name) {
    std::ifstream file(name);
    std::string line, subLeft, subRight;
    int sum = 0;
    if (file.is_open()) {
        while(file.good()) {
            std::getline(file,line);
            int stringSize = line.size();
            subLeft = line.substr(0,stringSize/2);
            subRight = line.substr(stringSize/2, stringSize/2);
            for (int i = 0; i < subLeft.size(); ++i) {
                if (subRight.find(subLeft[i]) != std::string::npos) {
                    if (subLeft[i] >= 'a' && subLeft[i] <= 'z') {
                        sum += subLeft[i] - 'a' + 1;
                    } else if (subLeft[i] >= 'A' && subLeft[i] <= 'Z') {
                        sum += subLeft[i] - 'A' + 27;
                    }
                    break;
                }
            }
        }
    }
    return sum;
}

int findCostTeams(std::string name) {
    std::ifstream file(name);
    std::string line1, line2, line3;
    int sum = 0;
    if (file.is_open()) {
        while(file.good()) {
            std::getline(file,line1);
            std::getline(file,line2);
            std::getline(file,line3);
            std::cout << line1 << std::endl << line2 << std::endl << line3 << std::endl;
            for (int i = 0; i < line1.size(); ++i) {
                if (line2.find(line1[i]) != std::string::npos && line3.find(line1[i]) != std::string::npos) {
                    std::cout << line1[i] << std::endl;
                    if (line1[i] >= 'a' && line1[i] <= 'z') {
                        sum += line1[i] - 'a' + 1;
                    } else if (line1[i] >= 'A' && line1[i] <= 'Z') {
                        sum += line1[i] - 'A' + 27;
                    }
                    break;
                }
            }
        }
    }
    return sum;
}