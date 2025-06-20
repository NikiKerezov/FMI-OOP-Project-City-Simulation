#pragma once

#include "Building.hpp"

class ModernBlock : public Building {
public:
    ModernBlock(unsigned maxCapacity);

    ModernBlock* clone() const override;

    void serialize(std::ostream& out) const override;
};