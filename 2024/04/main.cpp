#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

typedef std::vector<std::string> Input;

class XMASFinder {
   private:
    char current;

   public:
    XMASFinder() : current(0) {}
    ~XMASFinder() {}

    void reset() { current = 0; }
    bool check(char letter) {
        if (letter == 'X') {
            current = 'X';
        } else if (current == 'X' && letter == 'M') {
            current = 'M';
        } else if (current == 'M' && letter == 'A') {
            current = 'A';
        } else if (current == 'A' && letter == 'S') {
            current = 0;
            return true;
            // std::cout << "XMAS = [" << i << "," << j << "]" << std::endl;
        } else {
            current = 0;
        }
        return false;
    }
};

Input parseInput(std::string fileName);
int horizontalSearch(Input grid);
int verticalSearch(Input grid);
int mainDiagonalSearch(Input grid);
int secondaryDiagonalSearch(Input grid);

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
                res.emplace_back(line);
            }
        }
    } else {
        std::cout << "fichier non trouvé";
    }
    return res;
}

int horizontalSearch(Input grid) {
    int res = 0;
    XMASFinder finder;

    // left -> rigth
    for (int i = 0; i < grid.size(); ++i) {
        finder.reset();
        for (int j = 0; j < grid[i].size(); ++j) {
            res += int(finder.check(grid[i][j]));
        }
    }

    // right -> left
    for (int i = 0; i < grid.size(); ++i) {
        finder.reset();
        for (int j = grid[i].size() - 1; j >= 0; --j) {
            res += int(finder.check(grid[i][j]));
        }
    }
    return res;
}

int verticalSearch(Input grid) {
    int res = 0;
    XMASFinder finder;

    // top -> down
    for (int i = 0; i < grid[0].size(); ++i) {
        finder.reset();
        for (int j = 0; j < grid.size(); ++j) {
            res += int(finder.check(grid[j][i]));
        }
    }

    // down -> top
    for (int i = 0; i < grid[0].size(); ++i) {
        finder.reset();
        for (int j = grid.size() - 1; j >= 0; --j) {
            res += int(finder.check(grid[j][i]));
        }
    }
    return res;
}

int mainDiagonalSearch(Input grid) {
    int res = 0;
    XMASFinder finder;
    size_t idl, idc;
    size_t halfPerimeter = grid.size() + grid[0].size();

    // left -> right
    for (int i = 0; i < halfPerimeter; ++i) {
        if (i == grid.size()) {
            continue;
        }
        idl = i < grid.size() ? i : 0;
        idc = i >= grid.size() ? i - grid.size() : 0;
        finder.reset();
        while (idl < grid.size() && idc < grid[0].size()) {
            res += int(finder.check(grid[idl][idc]));
            idl++;
            idc++;
        }
    }

    // left -> right
    for (int i = 0; i < halfPerimeter - 1; ++i) {
        idl = i < grid.size() ? i : grid.size() - 1;
        idc = i >= grid.size() ? i - grid.size() : grid[0].size() - 1;
        finder.reset();
        while (idl < grid.size() && idc < grid[0].size()) {
            res += int(finder.check(grid[idl][idc]));
            idl--;
            idc--;
        }
    }

    return res;
}
int secondaryDiagonalSearch(Input grid) {
    int res = 0;
    XMASFinder finder;
    size_t idl, idc;
    size_t halfPerimeter = grid.size() + grid[0].size();

    // left -> right
    for (int i = 0; i < halfPerimeter; ++i) {
        if (i == grid.size()) {
            continue;
        }
        idl = i < grid.size() ? i : grid.size() - 1;
        idc = i >= grid.size() ? i - grid.size() : 0;
        finder.reset();
        while (idl < grid.size() && idc < grid[0].size()) {
            res += int(finder.check(grid[idl][idc]));
            idl--;
            idc++;
        }
    }

    // left -> right
    for (int i = 0; i < halfPerimeter - 1; ++i) {
        idl = i < grid.size() ? i : 0;
        idc = i >= grid.size() ? i - grid.size() : grid[0].size() - 1;
        finder.reset();
        while (idl < grid.size() && idc < grid[0].size()) {
            res += int(finder.check(grid[idl][idc]));
            idl++;
            idc--;
        }
    }
    return res;
}

size_t grayStar(Input input) {
    size_t res = 0;
    res += horizontalSearch(input);
    res += verticalSearch(input);
    res += mainDiagonalSearch(input);
    res += secondaryDiagonalSearch(input);
    return res;
}

size_t goldStar(Input grid) {
    int res = 0;
    bool main, secondary;
    for (int i = 1; i < grid.size() - 1; ++i) {
        for (int j = 1; j < grid[i].size() - 1; ++j) {
            main = false;
            secondary = false;
            if (grid[i][j] == 'A') {
                if ((grid[i + 1][j + 1] == 'M' && grid[i - 1][j - 1] == 'S') ||
                    (grid[i + 1][j + 1] == 'S' && grid[i - 1][j - 1] == 'M')) {
                    main = true;
                }
                if ((grid[i - 1][j + 1] == 'M' && grid[i + 1][j - 1] == 'S') ||
                    (grid[i - 1][j + 1] == 'S' && grid[i + 1][j - 1] == 'M')) {
                    secondary = true;
                }
                if (secondary && main) res++;
            }
        }
    }
    return res;
}