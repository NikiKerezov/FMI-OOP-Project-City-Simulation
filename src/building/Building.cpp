#include "../../include/building/Building.hpp"
#include "../../include/building/ModernBlock.hpp"
#include "../../include/building/PanelBlock.hpp"
#include "../../include/building/Dormitory.hpp"

#include "../../include/resident/Resident.hpp"

#include <stdexcept>
#include <string>

template<typename Iterator, typename Predicate>
Iterator find_if_manual(Iterator first, Iterator last, Predicate pred) {
    for (; first != last; ++first) {
        if (pred(*first)) return first;
    }
    return last;
}

Building::Building(unsigned maxCapacity, double baseRent)
    : maxCapacity(maxCapacity), baseRent(baseRent) {}

Building::~Building() noexcept {
    for (Resident* r : residents) {
        delete r;
    }
}

bool Building::addResident(Resident* resident) {
    if (residents.size() >= maxCapacity) {
        return false;
    }
    residents.push_back(resident);
    return true;
}

bool Building::removeResident(const std::string& name) {
    std::vector<Resident*>::iterator it = find_if_manual(residents.begin(), residents.end(),
        [&name](Resident* r) { return r->getName() == name; });
    if (it == residents.end()) {
        return false;
    }
    Resident* target = *it;
    residents.erase(it);
    delete target;
    return true;
}

double Building::getCurrentRent() const noexcept {
    return baseRent;
}

unsigned Building::getCapacity() const noexcept {
    return maxCapacity;
}

const std::vector<Resident*>& Building::getOccupants() const noexcept {
    return residents;
}

std::vector<Resident*>& Building::getOccupantsMutable() noexcept {
    return residents;
}

Building* Building::deserialize(std::istream& in) {
    if (!in) throw std::runtime_error("Invalid input stream for Building::deserialize");

    std::string type;
    unsigned cap;
    if (!std::getline(in, type) || !(in >> cap)) {
        throw std::runtime_error("Failed to read building type or capacity");
    }
    in.ignore();

    Building* b = nullptr;
    try {
        if      (type == "ModernBlock")  b = new ModernBlock(cap);
        else if (type == "PanelBlock")   b = new PanelBlock(cap);
        else if (type == "Dormitory")    b = new Dormitory(cap);
        else throw std::runtime_error("Unknown building type: " + type);

        unsigned count;
        if (!(in >> count)) {
            throw std::runtime_error("Failed to read resident count");
        }
        in.ignore();

        for (unsigned i = 0; i < count; ++i) {
            Resident* r = Resident::deserialize(in);
            if (!b->addResident(r)) {
                delete r;
                break;
            }
        }
        return b;
        // ReSharper disable once CppDFAUnreachableCode
    } catch (...) {
        delete b;
        throw;
    }
}

