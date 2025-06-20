#pragma once

#include "Building.hpp"

class Dormitory : public Building {
public:
    Dormitory(unsigned maxCapacity);

    Dormitory* clone() const override;

    void serialize(std::ostream& out) const override;
};