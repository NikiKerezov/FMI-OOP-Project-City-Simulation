#include "../../include/resident/Resident.hpp"
#include "../../include/resident/Teacher.hpp"
#include "../../include/resident/Programmer.hpp"
#include "../../include/resident/Miner.hpp"
#include "../../include/resident/Unemployed.hpp"
#include "../../include/constants.hpp"

#include <stdexcept>
#include <string>
#include <iostream>

static void validateName(const std::string& name) {
    if (name.empty()) {
        throw std::invalid_argument("Name cannot be empty");
    }
    if (name.size() > 32) {
        throw std::invalid_argument("Name cannot exceed 32 characters");
    }
    for (unsigned char c : name) {
        if (c < 32 || c > 126) {
            throw std::invalid_argument("Name must contain only printable ASCII characters");
        }
    }
}

Resident::Resident(const std::string& name,
                   unsigned happiness,
                   unsigned money,
                   unsigned lifePoints)
    : name(name), happiness(happiness), money(money), lifePoints(lifePoints) {

    validateName(name);
    recordHistory("Created: h=" + std::to_string(happiness)
                  + ", m=" + std::to_string(money)
                  + ", l=" + std::to_string(lifePoints));
}

void Resident::liveOneDay(bool firstDayOfMonth, double rentDue) {
    unsigned dayIndex = static_cast<unsigned>(history.size());

    if (money >= constants::foodCost) {
        money -= constants::foodCost;
        recordHistory("Day " + std::to_string(dayIndex) +
                      ": food h=" + std::to_string(happiness) +
                      ", m=" + std::to_string(money) +
                      ", l=" + std::to_string(lifePoints));
    } else {
        recordHistory("Day " + std::to_string(dayIndex) +
                      ": no food h=" + std::to_string(happiness) +
                      ", m=" + std::to_string(money) +
                      ", l=" + std::to_string(lifePoints));
    }

    if (firstDayOfMonth) {
        if (money >= static_cast<unsigned>(rentDue)) {
            money -= static_cast<unsigned>(rentDue);
            recordHistory("Day " + std::to_string(dayIndex) +
                          ": paid rent h=" + std::to_string(happiness) +
                          ", m=" + std::to_string(money) +
                          ", l=" + std::to_string(lifePoints));
        } else {
            recordHistory("Day " + std::to_string(dayIndex) +
                          ": failed rent h=" + std::to_string(happiness) +
                          ", m=" + std::to_string(money) +
                          ", l=" + std::to_string(lifePoints));
            lifePoints = (lifePoints > 1 ? lifePoints - 1 : 0);
        }

        unsigned salary = getMonthlySalary();
        money += salary;
        recordHistory("Day " + std::to_string(dayIndex) +
                      ": salary h=" + std::to_string(happiness) +
                      ", m=" + std::to_string(money) +
                      ", l=" + std::to_string(lifePoints));
    }

    unsigned beforeLife = lifePoints;
    applyMonthlyEffect();
    if (lifePoints != beforeLife) {
        recordHistory("Day " + std::to_string(dayIndex) +
                      ": profession effect h=" + std::to_string(happiness) +
                      ", m=" + std::to_string(money) +
                      ", l=" + std::to_string(lifePoints));
    }
}

const std::string& Resident::getName() const noexcept { return name; }
unsigned Resident::getHappiness()   const noexcept { return happiness; }
unsigned Resident::getMoney()       const noexcept { return money; }
unsigned Resident::getLifePoints()  const noexcept { return lifePoints; }
const std::vector<std::string>& Resident::getHistory() const noexcept { return history; }

void Resident::recordHistory(const std::string& entry) {
    try {
        history.push_back(entry);
    } catch (...) {
        // ignore
    }
}

Resident* Resident::deserialize(std::istream& in) {
    if (!in) throw std::invalid_argument("Invalid stream for Resident::deserialize");

    std::string type;
    if (!std::getline(in, type) || type.empty()) {
        throw std::runtime_error("Failed to read resident type");
    }

    std::string name;
    if (!std::getline(in, name)) {
        throw std::runtime_error("Failed to read resident name");
    }

    unsigned h, m, l, histCount;
    if (!(in >> h >> m >> l >> histCount)) {
        throw std::runtime_error("Failed to read resident stats");
    }
    in.ignore();

    Resident* r = nullptr;
    try {
        if      (type == "Teacher")    r = new Teacher(name, h, m, l);
        else if (type == "Programmer") r = new Programmer(name, h, m, l);
        else if (type == "Miner")      r = new Miner(name, h, m, l);
        else if (type == "Unemployed") r = new Unemployed(name, h, m, l);
        else                            throw std::runtime_error("Unknown resident type: " + type);

        for (unsigned i = 0; i < histCount; ++i) {
            std::string entry;
            if (!std::getline(in, entry)) {
                throw std::runtime_error("Failed to read history entry");
            }
            r->recordHistory(entry);
        }
        return r;
        // ReSharper disable once CppDFAUnreachableCode
    } catch (...) {
        delete r;
        throw;
    }
}
