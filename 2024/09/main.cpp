#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <vector>

struct Block {
    enum Type { empty, file };
    int fileID;
    Type type;
    int size;
    bool valid;
};

struct Input {
    std::list<Block> blocks;
    std::vector<int> memory;
};

Input parseInput(std::string fileName);

std::vector<int> compactInput(std::vector<int> input);
std::list<Block> compactInputByBlock(std::list<Block> input);

size_t grayStar(Input input);
size_t goldStar(Input input);

int main(int, char*[]) {
    auto input = parseInput("input");
    std::cout << "res gray star : " << grayStar(input) << std::endl;
    std::cout << "res gold star : " << goldStar(input) << std::endl;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input res;
    std::string line;
    bool flag = true;
    int idFile = 0;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            for (int i = 0; i < line.size(); ++i) {
                if (flag) {
                    res.blocks.push_back(Block{idFile, Block::file, line[i] - '0', true});
                    for (int j = 0; j < line[i] - '0'; ++j) {
                        res.memory.push_back(idFile);
                    }
                    idFile++;
                } else {
                    res.blocks.push_back(Block{idFile, Block::empty, line[i] - '0', true});
                    for (int j = 0; j < line[i] - '0'; ++j) {
                        res.memory.push_back(-1);
                    }
                }
                flag = !flag;
            }
        }
    } else {
        std::cout << "fichier non trouvé";
    }
    return res;
}

std::vector<int> compactInput(Input input) {
    size_t emptySpaces = 0;
    size_t targetSize = 0;
    for (auto elem : input.memory) {
        if (elem == -1) emptySpaces++;
    }
    targetSize = input.memory.size() - emptySpaces;
    for (int i = 0; i < targetSize; ++i) {
        if (input.memory[i] == -1) {
            while (input.memory.back() == -1) {
                input.memory.pop_back();
            }
            input.memory[i] = input.memory.back();
            input.memory.pop_back();
        }
    }
    return input.memory;
}

std::list<Block> compactInputByBlock(std::list<Block> input) {
    for (auto itr = input.rbegin(); itr != input.rend(); ++itr) {
        if (itr->type == Block::file) {
            for (auto it = input.begin(); it != itr.base(); ++it) {
                if (it->type == Block::empty) {
                    if (it->size == itr->size) {
                        input.insert(it, *itr);
                        itr->valid = false;
                        input.erase(it);
                        break;
                    } else if (it->size > itr->size) {
                        input.insert(it, *itr);
                        it->size -= itr->size;
                        itr->valid = false;
                        break;
                    }
                }
            }
        }
    }
    return input;
}

size_t grayStar(Input input) {
    size_t res = 0;
    auto compacted = compactInput(input);
    for (int i = 0; i < compacted.size(); ++i) {
        res += i * compacted[i];
    }
    return res;
}

size_t goldStar(Input input) {
    size_t res = 0;
    size_t count = 0;
    auto compacted = compactInputByBlock(input.blocks);
    for (auto it = compacted.begin(); it != compacted.end(); ++it) {
        if (it->type == Block::empty || !it->valid) {
            count += it->size;
        } else {
            for (int i = 0; i < it->size; ++i) {
                res += count++ * it->fileID;
            }
        }
    }
    return res;
}
