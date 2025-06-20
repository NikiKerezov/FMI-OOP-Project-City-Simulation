#include "../../include/resident/Unemployed.hpp"

#include <stdexcept>
#include <ostream>
#include <string>
#include <vector>

Unemployed::Unemployed(const std::string& name,
                       unsigned happiness,
                       unsigned money,
                       unsigned lifePoints)
    : Resident(name, happiness, money, lifePoints) {}

Unemployed *Unemployed::clone() const {
    return new Unemployed(*this);
}


unsigned Unemployed::getMonthlySalary() const {
    return 0u;
}

void Unemployed::applyMonthlyEffect() {
    if (lifePoints > 0u) {
        lifePoints -= 1u;
    }
}

void Unemployed::serialize(std::ostream& out) const {
    if (!out) throw std::invalid_argument("Invalid stream for Unemployed::serialize");

    out << "Unemployed\n"
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
