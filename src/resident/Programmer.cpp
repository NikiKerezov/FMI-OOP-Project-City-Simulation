#include "../../include/resident/Programmer.hpp"
#include "../../include/constants.hpp"


#include <stdexcept>
#include <ostream>
#include <string>
#include <cstdlib>

Programmer::Programmer(const std::string& name,
                       unsigned happiness,
                       unsigned money,
                       unsigned lifePoints)
    : Resident(name, happiness, money, lifePoints) {}

Programmer *Programmer::clone() const {
    return new Programmer(*this);
}


unsigned Programmer::getMonthlySalary() const {
    return 2000u + (std::rand() % 3001u);
}

void Programmer::applyMonthlyEffect() {
    if (happiness > 0u) {
        happiness -= 1u;
    }
}

void Programmer::serialize(std::ostream& out) const {
    if (!out) throw std::invalid_argument("Invalid stream for Programmer::serialize");

    out << "Programmer\n"
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