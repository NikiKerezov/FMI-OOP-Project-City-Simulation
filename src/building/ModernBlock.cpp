#include "../../include/building/ModernBlock.hpp"
#include "../../include/constants.hpp"

#include <stdexcept>
#include <ostream>

ModernBlock::ModernBlock(unsigned maxCapacity)
    : Building(maxCapacity, constants::baseRentModern) {}

ModernBlock* ModernBlock::clone() const {
    ModernBlock* copy = new ModernBlock(getCapacity());
    for (Resident* r : getOccupants()) {
        copy->addResident(r->clone());
    }
    return copy;
}

void ModernBlock::serialize(std::ostream& out) const {
    if (!out) throw std::invalid_argument("Invalid stream for ModernBlock::serialize");
    out << "ModernBlock\n"
        << getCapacity() << '\n'
        << getOccupants().size() << '\n';
    for (Resident* r : getOccupants()) {
        r->serialize(out);
    }
}
