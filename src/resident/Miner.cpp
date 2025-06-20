#include "../../include/resident/Miner.hpp"
#include "../../include/constants.hpp"

#include <stdexcept>
#include <ostream>
#include <string>
#include <vector>
#include <cstdlib>

Miner::Miner(const std::string& name,
             unsigned happiness,
             unsigned money,
             unsigned lifePoints)
    : Resident(name, happiness, money, lifePoints) {}

Miner *Miner::clone() const {
    return new Miner(*this);
}


unsigned Miner::getMonthlySalary() const {
    return 1000u + (std::rand() % 2001u);
}

void Miner::applyMonthlyEffect() {
    if (lifePoints > 0u) {
        lifePoints -= 1u;
    }
}

void Miner::serialize(std::ostream& out) const {
    if (!out) throw std::invalid_argument("Invalid stream for Miner::serialize");

    out << "Miner\n"
        << getName()       << '\n'
        << getHappiness()  << '\n'
        << getMoney()      << '\n'
        << getLifePoints() << '\n'
        << getHistory().size() << '\n';

    const std::vector<std::string>& hist = getHistory();
    for (unsigned i = 0u; i < hist.size(); ++i) {
        out << hist[i] << '\n';
    }
}