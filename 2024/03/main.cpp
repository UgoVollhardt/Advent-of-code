#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

typedef std::vector<std::string> Input;
typedef struct {
    std::string type;
    int first;
    int second;
} Operation;

Input parseInput(std::string fileName);
std::vector<Operation> parseOperation(std::string line);
std::vector<int> extractValues(std::string operation_str, std::string type);
size_t applyOperation(Operation op);
size_t grayStar(Input input);

int main(int, char *[]) {
    auto input = parseInput("input");
    std::cout << "res gray star : " << grayStar(input) << std::endl;
    // std::cout << "res gold star : " << goldStar(input) << std::endl;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input res;
    std::string line;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            if (!line.empty()) {
                res.emplace_back(line);
            }
        }
    } else {
        std::cout << "fichier non trouvé";
    }
    return res;
}

std::vector<Operation> parseOperation(std::string line) {
    std::vector<Operation> res;
    std::regex mul_regex("mul\\((\\d{1,3}),(\\d{1,3})\\)");
    auto words_begin = std::sregex_iterator(line.begin(), line.end(), mul_regex);
    auto words_end = std::sregex_iterator();
    std::cout << "Found " << std::distance(words_begin, words_end) << " matchs" << std::endl;

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        std::cout << match_str << " ";
        auto values = extractValues(match_str, "mul");
        res.push_back({"mul", values[0], values[1]});
    }
    std::cout << std::endl;

    for (auto elem : res) {
        std::cout << elem.type << " " << elem.first << " " << elem.second << std::endl;
    }
    std::cout << std::endl;

    return res;
}

std::vector<int> extractValues(std::string operation_str, std::string type) {
    std::stringstream ss(operation_str);
    std::string value;
    std::vector<int> res;
    if (!getline(ss, value, '(') || value != type) {
        return res;
    }
    getline(ss, value, ',');
    res.push_back(std::stoi(value));
    getline(ss, value, ')');
    res.push_back(std::stoi(value));

    return res;
}

size_t applyOperation(Operation op) {
    if (op.type == "mul") {
        return op.first * op.second;
    }
    return 0;
}

size_t grayStar(Input input) {
    size_t res = 0;
    for (auto elem : input) {
        auto operations = parseOperation(elem);
        for (auto op : operations) {
            res += applyOperation(op);
        }
    }
    return res;
}