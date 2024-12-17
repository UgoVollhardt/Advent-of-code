#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <string>
#include <vector>

struct Command {
    enum Instruct { adv, bxl, bst, jnz, bxc, out, bdv, cdv };
    static inline std::vector<Instruct> opcodeToInstruct{adv, bxl, bst, jnz, bxc, out, bdv, cdv};
    Instruct instruction;
    int operand;
};

struct Input {
    std::vector<Command> commands;
    std::vector<int> rawCommands;
    unsigned long A;
    unsigned long B;
    unsigned long C;
};

Input parseInput(std::string fileName);
std::string runCommands(Input input);
std::vector<int> runShortCmd(Input input, ulong value = 0);

ulong reverseCmds(Input input, ulong value, int index);

std::string grayStar(Input input);
size_t goldStar(Input input);

void printVector(std::vector<int> vect) {
    for (auto elem : vect) {
        std::cout << elem << ",";
    }
    std::cout << std::endl;
}

int main(int, char *[]) {
    auto input = parseInput("input");
    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "res gray star : " << grayStar(input) << std::endl;
    std::cout << "res gold star : " << goldStar(input) << std::endl;

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time : "
              << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << "µs"
              << std::endl;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    Input res;
    std::string line;
    int status = 0;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            if (!line.empty()) {
                std::stringstream ss(line);
                std::string value;
                Command cmd;
                int count = 0;
                switch (status++) {
                    case 0:
                        std::getline(ss, value, ' ');
                        std::getline(ss, value, ' ');
                        std::getline(ss, value, ' ');
                        res.A = std::stoul(value);
                        break;
                    case 1:
                        std::getline(ss, value, ' ');
                        std::getline(ss, value, ' ');
                        std::getline(ss, value, ' ');
                        res.B = std::stoul(value);
                        break;
                    case 2:
                        std::getline(ss, value, ' ');
                        std::getline(ss, value, ' ');
                        std::getline(ss, value, ' ');
                        res.C = std::stoul(value);
                        break;
                    case 3:
                        std::getline(ss, value, ' ');
                        while (std::getline(ss, value, ',')) {
                            res.rawCommands.push_back(std::stoi(value));
                            if (!(count++ % 2)) {
                                cmd.instruction = Command::opcodeToInstruct[std::stoi(value)];
                            } else {
                                cmd.operand = std::stoi(value);
                                res.commands.push_back(cmd);
                            }
                        }
                        break;

                    default:
                        break;
                }
            }
        }
    } else {
        std::cout << "fichier non trouvé";
    }
    return res;
}

std::string runCommands(Input input) {
    int pointer = 0;
    int operand;
    int combo = 0;
    std::string res;
    while (pointer != input.commands.size()) {
        operand = input.commands[pointer].operand;
        if (operand == 4) {
            combo = input.A;
        } else if (operand == 5) {
            combo = input.B;
        } else if (operand == 6) {
            combo = input.C;
        } else {
            combo = operand;
        }
        switch (input.commands[pointer].instruction) {
            case Command::adv:
                input.A = input.A >> combo;
                pointer++;
                break;
            case Command::bxl:
                input.B = input.B ^ operand;
                pointer++;
                break;
            case Command::bst:
                input.B = combo % 8;
                pointer++;
                break;
            case Command::jnz:
                if (input.A != 0) {
                    pointer = operand;
                } else {
                    pointer++;
                }
                break;
            case Command::bxc:
                input.B = input.B ^ input.C;
                pointer++;
                break;
            case Command::out:
                res += std::to_string(combo % ulong(8)) + ",";
                pointer++;
                break;
            case Command::bdv:
                input.B = input.A >> combo;
                pointer++;
                break;
            case Command::cdv:
                input.C = input.A >> combo;
                pointer++;
                break;

            default:
                break;
        }
    }
    res.pop_back();
    return res;
}

std::vector<int> runShortCmd(Input input, ulong value) {
    ulong A = value;
    ulong B = input.B;
    ulong C = input.C;
    std::vector<int> res;
    do {
        B = (((A % 8) ^ 5) ^ 6) ^ (A >> ((A % 8) ^ 5));
        C = A >> ((A % 8) ^ 5);
        A = A >> 3;
        res.push_back(B % 8);
    } while (A != 0);
    return res;
}

ulong reverseCmds(Input input, ulong value, int index) {
    std::vector<int> expectedOutput(input.rawCommands.begin() + index, input.rawCommands.end());
    for (int i = 0; i < 8; ++i) {
        auto resIter = runShortCmd(input, value * 8 + i);
        if (resIter == expectedOutput) {
            if (index == 0) {
                return value * 8 + i;
            }
            ulong res = reverseCmds(input, value * 8 + i, index - 1);
            if (res != -1) {
                return res;
            }
        }
    }
    return -1;
}

std::string grayStar(Input input) {
    std::string res;
    auto resVect = runShortCmd(input, input.A);
    for (int i = 0; i < resVect.size() - 1; ++i) {
        res += std::to_string(resVect[i]) + ",";
    }
    res += std::to_string(resVect.back());
    return res;
}

size_t goldStar(Input input) { return reverseCmds(input, 0, input.rawCommands.size() - 1); }