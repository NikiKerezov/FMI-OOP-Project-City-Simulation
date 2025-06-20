#include "../../include/resident/Teacher.hpp"
#include "../../include/constants.hpp"

#include <stdexcept>
#include <ostream>
#include <string>
#include <cstdlib>

Teacher::Teacher(const std::string& name,
                 unsigned happiness,
                 unsigned money,
                 unsigned lifePoints)
    : Resident(name, happiness, money, lifePoints) {}

Teacher *Teacher::clone() const {
    return new Teacher(*this);
}

unsigned Teacher::getMonthlySalary() const {
    return 1200u + (std::rand() % 101u);
}

void Teacher::applyMonthlyEffect() {
    if (happiness < 100) {
        happiness += 1;
    }
}

void Teacher::serialize(std::ostream& out) const {
    if (!out) throw std::invalid_argument("Invalid stream for Teacher::serialize");

    out << "Teacher\n"
        << getName() << '\n'
        << getHappiness() << '\n'
        << getMoney() << '\n'
        << getLifePoints() << '\n'
        << getHistory().size() << '\n';

    const std::vector<std::string>& hist = getHistory();
    for (unsigned i = 0; i < hist.size(); ++i) {
        out << hist[i] << '\n';
    }
}
