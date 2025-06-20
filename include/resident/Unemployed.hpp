#pragma once

#include "Resident.hpp"

class Unemployed : public Resident {
public:
    Unemployed(const std::string& name,
               unsigned happiness,
               unsigned money,
               unsigned lifePoints);
    ~Unemployed() noexcept override = default;

    unsigned getMonthlySalary() const override;
    void applyMonthlyEffect() override;
    Unemployed* clone() const override;
    void serialize(std::ostream& out) const override;
};
