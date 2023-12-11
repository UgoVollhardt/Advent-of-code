#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>

typedef struct {
    long beginSource;
    long beginDestination;
    long sizeSection;
} MapSection;

typedef std::vector<MapSection> Map;

typedef struct {
    std::vector<long> seeds;
    Map seedToSoil;
    Map soilToFertilizer;
    Map fertilizerToWater;
    Map waterToLight;
    Map lightToTemperature;
    Map temperatureToHumidity;
    Map humidityToLocation;
} Input;

long getLowestLocationNumber(std::string fileName);
Input parseInput(std::string fileName);
std::vector<long> getListOfSeeds(std::string line);
void feedMap(Map *map, std::string line);
long getLocation(long seed, Input &maps);
long getMappedItem(long key, Map map);
long getLowestLocationNumberRange(std::string fileName);

int main() {
    std::cout << getLowestLocationNumber("input") << std::endl;
    std::cout << getLowestLocationNumberRange("input") << std::endl;
}

long getLowestLocationNumber(std::string fileName) {
    auto input = parseInput(fileName);
    long lowestLocation = std::numeric_limits<long>::max();
    long correpondingLocation;
    for (long i = 0; i < input.seeds.size(); ++i) {
        correpondingLocation = getLocation(input.seeds[i], input);
        if (correpondingLocation < lowestLocation) lowestLocation = correpondingLocation;
    }
    return lowestLocation;
}

long getLowestLocationNumberRange(std::string fileName) {
    auto input = parseInput(fileName);
    long lowestLocation = std::numeric_limits<long>::max();
    long correpondingLocation;
    for (long i = 0; i < input.seeds.size(); i = i+2) {
        for (long j = 0; j < input.seeds[i+1]; ++j) {
            correpondingLocation = getLocation(input.seeds[i] + j, input);
            if (correpondingLocation < lowestLocation) lowestLocation = correpondingLocation;
        }
    }
    return lowestLocation;
}

Input parseInput(std::string fileName) {
    std::ifstream file(fileName);
    std::string line;
    Input maps;
    long sum = 0;
    if (file.is_open()) {
        while(file.good()) {
            std::getline(file,line);
            if (line.find("seeds", 0) != std::string::npos) {
                maps.seeds = getListOfSeeds(line);
            } else if (!line.empty()) {
                Map* refMap = nullptr;
                if (line.find("seed-to-soil", 0) != std::string::npos) refMap = &maps.seedToSoil;
                if (line.find("soil-to-fertilizer", 0) != std::string::npos) refMap = &maps.soilToFertilizer;
                if (line.find("fertilizer-to-water", 0) != std::string::npos) refMap = &maps.fertilizerToWater;
                if (line.find("water-to-light", 0) != std::string::npos) refMap = &maps.waterToLight;
                if (line.find("light-to-temperature", 0) != std::string::npos) refMap = &maps.lightToTemperature;
                if (line.find("temperature-to-humidity", 0) != std::string::npos) refMap = &maps.temperatureToHumidity;
                if (line.find("humidity-to-location", 0) != std::string::npos) refMap = &maps.humidityToLocation;
                while(!line.empty() && file.good()) {
                    std::getline(file,line);
                    feedMap(refMap, line);
                }
            }
        }
    }
    return maps;
}

std::vector<long> getListOfSeeds(std::string line) {
    std::vector<long> output;
    std::string buffer;
    std::stringstream stream(line);
    std::getline(stream,buffer,' ');

    while(std::getline(stream,buffer,' ')) {
        output.emplace_back(stol(buffer));
    }
    return output;
}

void feedMap(Map *map, std::string line) {
    if (line.empty()) return;
    std::string buffer;
    std::stringstream stream(line);
    std::vector<long> datas;
    while(getline(stream,buffer,' ')) {
        datas.push_back(stol(buffer));
    }
    map->push_back(MapSection{datas[1], datas[0], datas[2]});
}

long getLocation(long seed, Input &map) {
    long soil, fertilizer, water, light, temperature, humidity, location;
    soil = getMappedItem(seed, map.seedToSoil);
    fertilizer = getMappedItem(soil, map.soilToFertilizer);
    water = getMappedItem(fertilizer, map.fertilizerToWater);
    light = getMappedItem(water, map.waterToLight);
    temperature = getMappedItem(light, map.lightToTemperature);
    humidity = getMappedItem(temperature, map.temperatureToHumidity);
    return getMappedItem(humidity, map.humidityToLocation);
}

long getMappedItem(long key, Map map) {
    for (int i = 0; i < map.size(); ++i) {
        if (key >= map[i].beginSource && key < map[i].beginSource + map[i].sizeSection) {
            return key - map[i].beginSource + map[i].beginDestination;
        }
    }
    return key;
}

