#include "../include/SimulationEngine.hpp"
#include "../include/constants.hpp"

#include <fstream>
#include <stdexcept>
#include <cmath>
#include <utility>
#include <limits>

SimulationEngine::SimulationEngine(City initialCity)
    : city(std::move(initialCity))
    , last{0,0,0}
{
    history.push_back({ city, clock, last });
}

std::vector<std::string> SimulationEngine::step(SimulationClock::DayCount days) {
    std::vector<std::string> purged;

    if (days > 0) {
        for (SimulationClock::DayCount d = 0; d < days; ++d) {
            City             cityBak  = city;
            SimulationClock  clkBak   = clock;
            ZeroCounters     lastBak  = last;

            try {
                bool firstOfMonth = clock.isFirstOfMonth();
                unsigned R = city.getRows();
                unsigned C = city.getCols();
                double centerR = (R - 1) / 2.0;
                double centerC = (C - 1) / 2.0;
                double minDim = std::min(R, C);
                double centralTh   = minDim / 8.0;
                double outskirtsTh = 6.0 * minDim / 8.0;

                for (unsigned r = 0; r < R; ++r) {
                    for (unsigned c = 0; c < C; ++c) {
                        Building& b = city.at(r, c);
                        double rent = b.getCurrentRent();
                        double dr = r - centerR;
                        double dc = c - centerC;
                        double dist = std::sqrt(dr*dr + dc*dc);
                        if (dist <= centralTh)       rent *= constants::centralMultiplier;
                        else if (dist > outskirtsTh) rent *= constants::outskirtsDiscount;

                        std::vector<Resident*>& occ = b.getOccupantsMutable();
                        for (std::vector<Resident*>::iterator itRes = occ.begin(); itRes != occ.end(); ++itRes) {
                            Resident* res = *itRes;
                            try {
                                res->liveOneDay(firstOfMonth, rent);
                            } catch(...) {
                                // ignore individual errors
                            }
                        }
                    }
                }

                purgeZeroResidents(purged);

                clock.advance(1);
                history.push_back({ city, clock, last });
            }
            catch(...) {
                city  = std::move(cityBak);
                clock = clkBak;
                last  = lastBak;
                throw;
            }
        }
    }
    else if (days < 0) {
        for (SimulationClock::DayCount i = 0; i < -days; ++i) {
            if (history.size() <= 1) break;
            history.pop_back();
            // restore from last snapshot
            const Snapshot& snap = history.back();
            city  = snap.city;
            clock = snap.clock;
            last  = snap.counters;
        }
    }

    return purged;
}


void SimulationEngine::purgeZeroResidents(std::vector<std::string>& purged) {
    last.happyZero = 0;
    last.lifeZero  = 0;
    last.moneyZero = 0;
    lastRemovedNames.clear();

    unsigned R = city.getRows();
    unsigned C = city.getCols();
    for (unsigned r = 0; r < R; ++r) {
        for (unsigned c = 0; c < C; ++c) {
            Building& b = city.at(r, c);
            std::vector<Resident*>& occ = b.getOccupantsMutable();
            for (std::vector<Resident*>::iterator it = occ.begin(); it != occ.end();) {
                Resident* res = *it;
                bool remove = false;
                if (res->getHappiness() == 0) { ++last.happyZero; remove = true; }
                if (res->getLifePoints() == 0) { ++last.lifeZero;   remove = true; }
                if (res->getMoney() == 0)       { ++last.moneyZero;  remove = true; }

                if (remove) {
                    purged.push_back(res->getName());
                    lastRemovedNames.push_back(res->getName());
                    delete res;
                    it = occ.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }
}

void SimulationEngine::addResident(unsigned r, unsigned c, Resident* resident) {
    Building& b = city.at(r, c);
    if (!b.addResident(resident)) {
        delete resident;
        throw std::logic_error("Building at capacity or addition failed");
    }
    history.emplace_back(city, clock, last);
}

void SimulationEngine::removeResident(unsigned r, unsigned c, const std::string& name) {
    Building& b = city.at(r, c);
    if (!b.removeResident(name)) {
        throw std::runtime_error("Resident '" + name + "' not found");
    }
    history.emplace_back(city, clock, last);
}

SimulationEngine::ZeroCounters SimulationEngine::lastCounters() const noexcept {
    return last;
}

void SimulationEngine::save(const std::string& filename) const {
    if (filename.empty())
        throw std::invalid_argument("Filename cannot be empty");

    std::ofstream ofs(filename, std::ios::trunc);
    if (!ofs)
        throw std::runtime_error("Cannot open file for writing: " + filename);

    ofs << history.size() << "\n";

    for (const Snapshot& entry : history) {
        const City&           histCity     = entry.city;
        const SimulationClock histClock    = entry.clock;
        const ZeroCounters    histCounters = entry.counters;

        ofs << histClock.dateString() << "\n";
        histCity.serialize(ofs);
        ofs << histCounters.happyZero << " "
            << histCounters.lifeZero  << " "
            << histCounters.moneyZero << "\n";
    }

    if (!ofs)
        throw std::runtime_error("Error writing to file: " + filename);
}

void SimulationEngine::load(const std::string& filename) {
    if (filename.empty())
        throw std::invalid_argument("Filename cannot be empty");

    std::ifstream ifs(filename);
    if (!ifs)
        throw std::runtime_error("Cannot open file for reading: " + filename);

    size_t histSize;
    if (!(ifs >> histSize))
        throw std::runtime_error("Failed to read history size from file: " + filename);
    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    history.clear();
    for (size_t i = 0; i < histSize; ++i) {
        std::string dateLine;
        if (!std::getline(ifs, dateLine))
            throw std::runtime_error("Failed to read date for snapshot " + std::to_string(i));
        SimulationClock clk = SimulationClock::fromDateString(dateLine);

        City c = City::deserialize(ifs);

        unsigned hz, lz, mz;
        if (!(ifs >> hz >> lz >> mz))
            throw std::runtime_error("Failed to read purge counters for snapshot " + std::to_string(i));
        ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        history.push_back({ std::move(c), clk, ZeroCounters{hz, lz, mz} });
    }

    if (history.empty())
        throw std::runtime_error("Loaded history is empty");

    const Snapshot& lastEntry = history.back();
    city  = lastEntry.city;
    clock = lastEntry.clock;
    last  = lastEntry.counters;

    lastRemovedNames.clear();
}


const City& SimulationEngine::getCity() const noexcept
{
    return city;
}

SimulationClock& SimulationEngine::getClock() noexcept
{
    return clock;
}

const std::vector<std::string>& SimulationEngine::getLastRemovedNames() const noexcept
{
    static const std::vector<std::string> empty;
    return empty;
}
