#pragma once

#include <chrono>
#include <string>

class SimulationClock {
public:
    using DayCount = long long;
    SimulationClock();
    void advance(DayCount days);
    bool isFirstOfMonth() const noexcept;
    std::string dateString() const;
    static SimulationClock fromDateString(const std::string& s);

private:
    std::chrono::sys_days current;
};
