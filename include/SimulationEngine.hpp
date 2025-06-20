#pragma once

#include <string>
#include <vector>
#include "City.hpp"
#include "SimulationClock.hpp"
#include "resident/Resident.hpp"

class SimulationEngine {
public:
    explicit SimulationEngine(City city);

    std::vector<std::string> step(SimulationClock::DayCount days = 1);

    void addResident(unsigned r, unsigned c, Resident* resident);
    void removeResident(unsigned r, unsigned c, const std::string& name);

    struct ZeroCounters { unsigned happyZero, lifeZero, moneyZero; };
    ZeroCounters lastCounters() const noexcept;

    void save(const std::string& filename) const;
    void load(const std::string& filename);

    const City&      getCity()  const noexcept;
    SimulationClock& getClock()       noexcept;

    const std::vector<std::string>& getLastRemovedNames() const noexcept;

    struct Snapshot {
        City city;
        SimulationClock clock;
        ZeroCounters counters;
    };

private:
    void purgeZeroResidents(std::vector<std::string>& purged);

    City city;
    SimulationClock clock;
    ZeroCounters last;
    std::vector<Snapshot> history;

    std::vector<std::string> lastRemovedNames;
};
