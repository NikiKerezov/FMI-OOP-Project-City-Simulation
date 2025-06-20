#include "../include/Generator.hpp"
#include "../include/resident/Teacher.hpp"
#include "../include/resident/Programmer.hpp"
#include "../include/resident/Miner.hpp"
#include "../include/resident/Unemployed.hpp"
#include "../include/building/ModernBlock.hpp"
#include "../include/building/PanelBlock.hpp"
#include "../include/building/Dormitory.hpp"

#include <random>
#include <stdexcept>
#include <chrono>

Generator::Generator() {
    unsigned seed = static_cast<unsigned>(
        std::chrono::system_clock::now().time_since_epoch().count()
    );
    rng.seed(seed);
    initializeNames();
}

void Generator::initializeNames() {
    names = {
        "Ivan", "Maria", "Georgi", "Elena", "Dimitar", "Ana",
        "Stoyan", "Petya", "Nikola", "Gergana", "Stefan", "Radka",
        "Plamen", "Teodora", "Krasimir", "Silviya", "Boyan", "Mariya",
        "Veselin", "Desislava", "Hristo", "Violeta", "Petar", "Yordanka",
        "Stanimir", "Tanya", "Zdravko", "Nadezhda", "Rumen",
        "Ekaterina", "Valentin", "Milena", "Angel", "Daniela", "Todor",
        "Svetlana"
    };
    if (names.empty()) {
        throw std::runtime_error("Name list initialization failed");
    }
}

std::string Generator::getRandomName() {
    if (names.empty()) {
        throw std::runtime_error("Name list is empty");
    }
    unsigned idx;
    try {
        std::uniform_int_distribution<unsigned> dist(0, static_cast<unsigned>(names.size() - 1));
        idx = dist(rng);
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Random index generation failed: ") + e.what());
    }
    try {
        return names.at(idx);
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to retrieve random name: ") + e.what());
    }
}

unsigned Generator::getRandomStat(unsigned min, unsigned max) {
    if (min > max) {
        throw std::invalid_argument("Invalid stat range: min > max");
    }
    try {
        std::uniform_int_distribution<unsigned> dist(min, max);
        return dist(rng);
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Random stat generation failed: ") + e.what());
    }
}

Resident* Generator::generateResident() {
    std::string name    = getRandomName();
    unsigned happiness  = getRandomStat(20, 100);
    unsigned money      = getRandomStat(500, 5000);
    unsigned lifePoints = getRandomStat(50, 100);
    unsigned type       = getRandomStat(0, 3);

    switch (type) {
        case 0: return new Teacher(name, happiness, money, lifePoints);
        case 1: return new Programmer(name, happiness, money, lifePoints);
        case 2: return new Miner(name, happiness, money, lifePoints);
        default: return new Unemployed(name, happiness, money, lifePoints);
    }
}

Building* Generator::generateBuilding() {
    unsigned capacity = getRandomStat(1, 10);
    unsigned type     = getRandomStat(0, 2);

    switch (type) {
        case 0: return new ModernBlock(capacity);
        case 1: return new PanelBlock(capacity);
        default: return new Dormitory(capacity);
    }
}

City Generator::generateCity(unsigned rowsCount, unsigned colsCount) {
    if (rowsCount == 0 || colsCount == 0) {
        throw std::invalid_argument("City dimensions must be positive");
    }
    City temp(rowsCount, colsCount);
    for (unsigned r = 0; r < rowsCount; ++r) {
        for (unsigned c = 0; c < colsCount; ++c) {
            Building* b = generateBuilding();
            temp.set(r, c, b);
            unsigned count = 0;
            try {
                count = getRandomStat(0, b->getCapacity());
            } catch (...) {
                count = 0;
            }
            for (unsigned i = 0; i < count; ++i) {
                Resident* res = nullptr;
                try {
                    res = generateResident();
                } catch (...) {
                    break;
                }
                if (!b->addResident(res)) {
                    delete res;
                    break;
                }
            }
        }
    }
    return temp;
}
