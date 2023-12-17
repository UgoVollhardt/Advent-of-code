#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

enum spring { kOperational, kDamaged, kUnknown };

typedef struct {
    std::vector<spring> springs;
    std::vector<int> contiguous;
    std::vector<int> contiguousInit;
} Row;

typedef std::vector<Row> Input;

std::map<spring, char> stc = {{kOperational, '.'}, {kDamaged, '#'}, {kUnknown, '?'}};

std::ostream& operator<<(std::ostream& os, const Row& dt) {
    std::string ouput;
    for (auto spring : dt.springs) {
        if (spring == kOperational)
            ouput += '.';
        else if (spring == kDamaged)
            ouput += '#';
        else
            ouput += '?';
    }
    ouput += " | ";
    for (auto contig : dt.contiguous) {
        ouput += std::to_string(contig) + ",";
    }
    ouput.pop_back();
    return os << ouput;
}

long getSumArrangements(std::string fileName);
Input parseInput(std::string fileName);
Row parseLine(std::string line);
int getArrangements(Row row, int posMin, int posMax);
bool isArrangementValid(Row initialRow, int pos);
int getMinSpaceLasts(Row row);
Row createArrangement(Row initialRow, int pos);
int countContigous(Row row);
Row unfold(Row row);

int main() {
    std::cout << "--Part 1--" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << getSumArrangements("input") << std::endl;
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms"
              << std::endl;
    std::cout << "--Part 2--" << std::endl;
}

long getSumArrangements(std::string fileName) {
    long sum = 0;
    auto input = parseInput(fileName);
    for (int i = 0; i < input.size(); ++i) {
        int ars =
            getArrangements(input[i], 0, input[i].springs.size() - getMinSpaceLasts(input[i]));
        sum += ars;
    }
    return sum;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::string line;
    Input res;
    if (file.is_open()) {
        while (file.good()) {
            std::getline(file, line);
            res.emplace_back(parseLine(line));
        }
    }
    return res;
}

Row parseLine(std::string line) {
    Row ouput;
    std::string buffer;
    std::stringstream stream(line);
    getline(stream, buffer, ' ');
    for (auto elem : buffer) {
        if (elem == '.')
            ouput.springs.push_back(kOperational);
        else if (elem == '#')
            ouput.springs.push_back(kDamaged);
        else
            ouput.springs.push_back(kUnknown);
    }
    while (getline(stream, buffer, ',')) {
        if (!buffer.empty()) ouput.contiguous.push_back(stoi(buffer));
    }
    ouput.contiguousInit = ouput.contiguous;
    return ouput;
}

int getArrangements(Row row, int posMin, int posMax) {
    long sum = 0;
    for (int i = posMin; i < posMax; ++i) {
        if (isArrangementValid(row, i)) {
            auto newRow = createArrangement(row, i);
            if (row.contiguous.size() > 1) {
                sum += getArrangements(createArrangement(row, i), i + row.contiguous.front() + 1,
                                       row.springs.size() - getMinSpaceLasts(newRow));
            } else {
                if (row.contiguousInit.size() == countContigous(newRow)) {
                    sum++;
                }
            }
        }
    }
    return sum;
}

bool isArrangementValid(Row row, int pos) {
    if (pos + row.contiguous.front() - 1 >= row.springs.size()) return false;
    for (int i = 0; i < row.contiguous.front(); ++i) {
        if (row.springs[i + pos] == kOperational) {
            return false;
        }
    }
    if (pos > 0 && row.springs[pos - 1] == kDamaged) {
        return false;
    }
    if (pos < row.springs.size() - 1 && (pos + row.contiguous.front() != row.springs.size()) &&
        row.springs[pos + row.contiguous.front()] == kDamaged) {
        return false;
    }

    return true;
}

int getMinSpaceLasts(Row row) {
    if (row.contiguous.size() <= 1) return 0;
    int sum = 0;
    for (int i = 1; i < row.contiguous.size(); ++i) {
        sum += row.contiguous[i] + 1;
    }
    return sum;
}

Row createArrangement(Row initialRow, int pos) {
    Row output = initialRow;
    for (int i = 0; i < initialRow.contiguous.front(); ++i) {
        output.springs[i + pos] = kDamaged;
    }
    output.contiguous.erase(output.contiguous.begin());
    return output;
}

int countContigous(Row row) {
    int sum = 0;
    spring last = row.springs.front();
    for (int i = 0; i < row.springs.size(); ++i) {
        if (row.springs[i] != last && last == kDamaged) {
            sum += 1;
        }
        last = row.springs[i];
    }
    if (row.springs.back() == kDamaged) {
        sum += 1;
    }
    return sum;
}

Row unfold(Row row) {
    Row ouput = row;
    for (int i = 0; i < 4; ++i) {
        ouput.springs.push_back(kUnknown);
        ouput.springs.insert(ouput.springs.end(), row.springs.begin(), row.springs.end());
        ouput.contiguous.insert(ouput.contiguous.end(), row.contiguous.begin(),
                                row.contiguous.end());
        ouput.contiguousInit.insert(ouput.contiguousInit.end(), row.contiguousInit.begin(),
                                    row.contiguousInit.end());
    }
    return ouput;
}