#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

typedef struct {
    std::vector<std::pair<int, int>> rules;
    std::vector<std::vector<int>> updates;
} Input;

Input parseInput(std::string fileName);
std::pair<int, int> parseRule(std::string line);
std::vector<int> parseUpdate(std::string line);

size_t checkRules(std::vector<std::pair<int, int>> rules, std::vector<int> update);
std::vector<int> reorder(std::vector<std::pair<int, int>> rules, std::vector<int> update);

size_t grayStar(Input input);
size_t goldStar(Input input);

int main(int, char *[]) {
    auto input = parseInput("input");
    std::cout << "res gray star : " << grayStar(input) << std::endl;
    std::cout << "res gold star : " << goldStar(input) << std::endl;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input res;
    std::string line;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            if (!line.empty()) {
                if (line.find("|", 0) != std::string::npos)
                    res.rules.emplace_back(parseRule(line));
                else if (line.find(",", 0) != std::string::npos)
                    res.updates.emplace_back(parseUpdate(line));
            }
        }
    } else {
        std::cout << "fichier non trouvé";
    }
    return res;
}

std::pair<int, int> parseRule(std::string line) {
    std::stringstream ss(line);
    std::string value;
    std::pair<int, int> res;

    getline(ss, value, '|');
    res.first = std::stoi(value);
    getline(ss, value, '\n');
    res.second = std::stoi(value);

    return res;
}

std::vector<int> parseUpdate(std::string line) {
    std::stringstream ss(line);
    std::string value;
    std::vector<int> res;

    while (getline(ss, value, ',')) {
        res.emplace_back(std::stoi(value));
    }

    return res;
}

size_t checkRules(std::vector<std::pair<int, int>> rules, std::vector<int> update) {
    std::map<int, int> pagePosition;
    for (size_t i = 0; i < 100; ++i) {
        pagePosition.insert(std::make_pair(i, -1));
    }
    for (size_t i = 0; i < update.size(); ++i) {
        pagePosition[update[i]] = i;
    }

    for (size_t i = 0; i < rules.size(); ++i) {
        if (pagePosition[rules[i].first] != -1 && pagePosition[rules[i].second] != -1 &&
            pagePosition[rules[i].first] > pagePosition[rules[i].second]) {
            return pagePosition[rules[i].first];
        }
    }
    return update.size();
}

size_t grayStar(Input input) {
    size_t res = 0;
    for (auto update : input.updates) {
        if (checkRules(input.rules, update) == update.size()) {
            res += update[update.size() / 2];
        }
    }
    return res;
}

std::vector<int> reorder(std::vector<std::pair<int, int>> rules, std::vector<int> update) {
    size_t index = checkRules(rules, update);
    int value = update[index];
    std::vector<std::vector<int>::iterator> itMajor;

    update.erase(update.begin() + index);

    for (int i = 0; i < rules.size(); ++i) {
        auto it = std::find(update.begin(), update.end(), rules[i].second);
        if (it != update.end()) {
            itMajor.push_back(it);
        }
    }
    std::sort(itMajor.begin(), itMajor.end());

    update.insert(itMajor.front(), value);

    if (checkRules(rules, update) != update.size()) {
        update = reorder(rules, update);
    }
    return update;
}

size_t goldStar(Input input) {
    size_t res = 0;
    for (auto update : input.updates) {
        if (checkRules(input.rules, update) != update.size()) {
            auto reorderedUpdate = reorder(input.rules, update);
            res += reorderedUpdate[reorderedUpdate.size() / 2];
        }
    }
    return res;
}