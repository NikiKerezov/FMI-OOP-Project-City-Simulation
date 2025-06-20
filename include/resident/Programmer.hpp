#pragma once

#include "Resident.hpp"

class Programmer : public Resident {
public:
    Programmer(const std::string& name,
               unsigned happiness,
               unsigned money,
               unsigned lifePoints);
    ~Programmer() noexcept override = default;

    unsigned getMonthlySalary() const override;
    void applyMonthlyEffect() override;
    Programmer* clone() const override;
    void serialize(std::ostream& out) const override;
};
