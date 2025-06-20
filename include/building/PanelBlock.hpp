#pragma once

#include "Building.hpp"

class PanelBlock : public Building {
public:
    PanelBlock(unsigned maxCapacity);

    PanelBlock* clone() const override;

    void serialize(std::ostream& out) const override;
};