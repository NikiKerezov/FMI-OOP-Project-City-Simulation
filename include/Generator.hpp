#pragma once

#include <vector>
#include <string>
#include <random>
#include "City.hpp"
#include "resident/Resident.hpp"
#include "building/Building.hpp"

class Generator {
public:
    Generator();
    City      generateCity(unsigned rowsCount, unsigned colsCount);
    Resident* generateResident();
    Building* generateBuilding();

private:
    std::mt19937 rng;
    std::vector<std::string> names;
    void initializeNames();
    std::string getRandomName();
    unsigned getRandomStat(unsigned min, unsigned max);
};