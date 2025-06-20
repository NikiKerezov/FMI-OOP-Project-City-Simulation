#include "../../include/building/Dormitory.hpp"
#include "../../include/constants.hpp"

#include <stdexcept>
#include <ostream>

Dormitory::Dormitory(unsigned maxCapacity)
    : Building(maxCapacity, constants::baseRentDorm) {}

Dormitory* Dormitory::clone() const {
    Dormitory* copy = new Dormitory(getCapacity());
    for (Resident* r : getOccupants()) {
        copy->addResident(r->clone());
    }
    return copy;
}

void Dormitory::serialize(std::ostream& out) const {
    if (!out) throw std::invalid_argument("Invalid stream for Dormitory::serialize");

    out << "Dormitory\n"
        << getCapacity() << '\n'
        << getOccupants().size() << '\n';

    for (Resident* r : getOccupants()) {
        r->serialize(out);
    }
}
