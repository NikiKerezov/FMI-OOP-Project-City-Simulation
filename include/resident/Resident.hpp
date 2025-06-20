#pragma once

#include <iostream>
#include <string>
#include <vector>

class Resident {
public:
    Resident(const std::string& name,
             unsigned happiness,
             unsigned money,
             unsigned lifePoints);
    virtual ~Resident() noexcept = default;

    virtual unsigned getMonthlySalary() const = 0;
    virtual void applyMonthlyEffect() = 0;

    virtual Resident* clone() const = 0;

    virtual void serialize(std::ostream& out) const = 0;
    static Resident* deserialize(std::istream& in);

    void liveOneDay(bool firstDayOfMonth, double rentDue);

    const std::string& getName()       const noexcept;
    unsigned           getHappiness()  const noexcept;
    unsigned           getMoney()      const noexcept;
    unsigned           getLifePoints() const noexcept;
    const std::vector<std::string>& getHistory() const noexcept;

protected:
    void recordHistory(const std::string& entry);
    std::string name;
    unsigned    happiness;
    unsigned    money;
    unsigned    lifePoints;
    std::vector<std::string> history;
};
