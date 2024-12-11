#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

typedef std::vector<size_t> Input;

Input parseInput(std::string fileName);
Input mutate(Input input);
std::string trimLeadingZeros(std::string str);

size_t grayStar(Input input);
size_t goldStar(Input input);

int main(int, char *[]) {
    auto input = parseInput("input");
    std::cout << "res gray star : " << grayStar(input) << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "res gold star : " << goldStar(input) << std::endl;
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms"
              << std::endl;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::string line;
    Input res;

    std::cout << "Parsing input" << std::endl;

    if (file.is_open()) {
        while (file.good()) {
            std::vector<int> row;
            std::getline(file, line);
            std::stringstream ss(line);
            std::string number;
            while (std::getline(ss, number, ' ')) {
                if (!number.empty()) {
                    res.push_back(std::stoi(number));
                }
            }
        }
    } else {
        std::cout << "fichier non trouvé";
    }
    return res;
}

Input mutate(Input input) {
    Input res;
    for (auto elem : input) {
        std::string elemStr = std::to_string(elem);
        if (elem == 0) {
            res.push_back(1);
        } else if (elemStr.size() % 2 == 0) {
            std::string first = elemStr.substr(0, elemStr.size() / 2);
            std::string second =
                trimLeadingZeros(elemStr.substr(elemStr.size() / 2, elemStr.size() / 2));
            res.push_back(std::stoul(first));
            res.push_back(std::stoul(second));
        } else {
            res.push_back(elem * 2024);
        }
    }
    return res;
}

std::string trimLeadingZeros(std::string str) {
    while (str[0] == '0' && str.size() > 1) {
        str.erase(str.begin());
    }
    return str;
}

size_t grayStar(Input input) {
    for (int i = 0; i < 25; ++i) {
        input = mutate(input);
    }
    return input.size();
}

size_t goldStar(Input input) {
    std::unordered_map<size_t, std::vector<size_t>> numberMap25;
    size_t res = 0;
    Input tmp, tmp2, tmp3;
    for (auto elem : input) {
        tmp = Input{elem};
        for (int i = 0; i < 25; ++i) {
            tmp = mutate(tmp);
        }
        numberMap25.insert(std::make_pair(elem, tmp));

        for (auto second : tmp) {
            tmp2 = Input{second};
            if (!numberMap25.contains(second)) {
                for (int i = 0; i < 25; ++i) {
                    tmp2 = mutate(tmp2);
                }
                numberMap25.insert(std::make_pair(second, tmp2));
            } else {
                tmp2 = numberMap25[second];
            }
            for (auto third : tmp2) {
                tmp3 = Input{third};
                if (!numberMap25.contains(third)) {
                    for (int i = 0; i < 25; ++i) {
                        tmp3 = mutate(tmp3);
                    }
                    res += tmp3.size();
                    numberMap25.insert(std::make_pair(third, tmp3));
                } else {
                    res += numberMap25[third].size();
                }
            }
        }
    }

    return res;
}
