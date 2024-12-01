#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int intersectAllCount(std::string name);
int intersectPartialCount(std::string name);

int main() {
    std::cout << intersectAllCount("input") << std::endl;
    std::cout << intersectPartialCount("input") << std::endl;
}

int intersectAllCount(std::string name) {
    std::ifstream file(name);
    std::string line, subLeft, subRight, begin, end;
    std::vector<int> left, right;
    int sum = 0;
    if (file.is_open()) {
        while(file.good()) {
            left.clear();
            right.clear();
            std::getline(file,subLeft,',');
            std::getline(file,subRight);

            std::istringstream str(subLeft);
            std::getline(str,begin,'-');
            std::getline(str,end);
            for (int i = std::stoi(begin); i <= std::stoi(end); ++i) {
                left.push_back(i);
            }

            str = std::istringstream(subRight);
            std::getline(str,begin,'-');
            std::getline(str,end);
            for (int i = std::stoi(begin); i <= std::stoi(end); ++i) {
                right.push_back(i);
            }

            if (std::search(left.begin(),left.end(),right.begin(),right.end()) != left.end() 
                || std::search(right.begin(),right.end(),left.begin(),left.end()) != right.end() ) {
                    sum += 1;
                }

        }
    }
    return sum;
}

int intersectPartialCount(std::string name) {
        std::ifstream file(name);
    std::string line, subLeft, subRight, begin, end;
    std::vector<int> left, right, intersect;
    int sum = 0;
    if (file.is_open()) {
        while(file.good()) {
            left.clear();
            right.clear();
            begin.clear();
            end.clear();
            std::getline(file,subLeft,',');
            std::getline(file,subRight);

            std::istringstream str(subLeft);
            std::getline(str,begin,'-');
            std::getline(str,end);
            for (int i = std::stoi(begin); i <= std::stoi(end); ++i) {
                left.push_back(i);
            }

            begin.clear();
            end.clear();

            str = std::istringstream(subRight);
            std::getline(str,begin,'-');
            std::getline(str,end);
            for (int i = std::stoi(begin); i <= std::stoi(end); ++i) {
                right.push_back(i);
            }

            for (size_t i = 0; i < left.size(); i++) {
                std::cout << left[i] << " ";
            }
            std::cout << ", ";
            for (size_t i = 0; i < right.size(); i++) {
                std::cout << right[i] << " ";
            }
            intersect.clear();
            std::set_intersection(left.begin(), left.end(),
                          right.begin(), right.end(),
                          std::back_inserter(intersect));
            std::cout << "intersect : ";
            for (size_t i = 0; i < intersect.size(); i++)
            {
                std::cout << intersect[i] << " ";
            }
            

            if (intersect.size()) {
                    std::cout << " founded";
                    sum += 1;
                }

            std::cout << std::endl;
        }
    }
    return sum;
}