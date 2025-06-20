#pragma once

#include "Resident.hpp"

class Miner : public Resident {
public:
    Miner(const std::string& name,
          unsigned happiness,
          unsigned money,
          unsigned lifePoints);
    ~Miner() noexcept override = default;

    unsigned getMonthlySalary() const override;
    void applyMonthlyEffect() override;
    Miner* clone() const override;
    void serialize(std::ostream& out) const override;
};
