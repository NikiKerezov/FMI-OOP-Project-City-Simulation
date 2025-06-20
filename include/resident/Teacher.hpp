#pragma once

#include "Resident.hpp"

class Teacher : public Resident {
public:
    Teacher(const std::string& name,
            unsigned happiness,
            unsigned money,
            unsigned lifePoints);
    ~Teacher() noexcept override = default;

    unsigned getMonthlySalary() const override;
    void applyMonthlyEffect() override;
    Teacher* clone() const override;
    void serialize(std::ostream& out) const override;
};
