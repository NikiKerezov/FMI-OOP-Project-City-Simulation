#include "../../include/building/PanelBlock.hpp"
#include "../../include/constants.hpp"

#include <stdexcept>
#include <ostream>

PanelBlock::PanelBlock(unsigned maxCapacity)
    : Building(maxCapacity, constants::baseRentPanel) {}

PanelBlock* PanelBlock::clone() const {
    PanelBlock* copy = new PanelBlock(getCapacity());
    for (Resident* r : getOccupants()) {
        copy->addResident(r->clone());
    }
    return copy;
}

void PanelBlock::serialize(std::ostream& out) const {
    if (!out) throw std::invalid_argument("Invalid stream for PanelBlock::serialize");

    out << "PanelBlock\n"
        << getCapacity() << '\n'
        << getOccupants().size() << '\n';

    for (Resident* r : getOccupants()) {
        r->serialize(out);
    }
}
