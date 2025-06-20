#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../resident/Resident.hpp"

class Building {
public:
    virtual ~Building() noexcept;

    virtual Building* clone() const = 0;

    bool addResident(Resident* resident);
    bool removeResident(const std::string& name);

    double getCurrentRent() const noexcept;
    unsigned getCapacity() const noexcept;
    const std::vector<Resident*>& getOccupants() const noexcept;
    std::vector<Resident*>&       getOccupantsMutable() noexcept;

    virtual void serialize(std::ostream& out) const = 0;
    static Building* deserialize(std::istream& in) noexcept(false);

protected:
    Building(unsigned maxCapacity, double baseRent);

    unsigned                   maxCapacity;
    double                     baseRent;
    std::vector<Resident*>     residents;
};