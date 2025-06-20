#include "../include/SimulationClock.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdexcept>

using namespace std::chrono;

SimulationClock::SimulationClock()
    : current(floor<days>(system_clock::now()))
{}

void SimulationClock::advance(DayCount delta) {
    current += days{delta};
}

bool SimulationClock::isFirstOfMonth() const noexcept {
    year_month_day ymd{ current };
    return unsigned(ymd.day()) == 1;
}

std::string SimulationClock::dateString() const {
    year_month_day ymd{ current };
    std::ostringstream oss;
    oss << int(ymd.year()) << '-'
        << std::setfill('0') << std::setw(2) << unsigned(ymd.month()) << '-'
        << std::setfill('0') << std::setw(2) << unsigned(ymd.day());
    return oss.str();
}

SimulationClock SimulationClock::fromDateString(const std::string& s) {
    std::istringstream iss(s);
    int y, m, d;
    char dash1, dash2;
    if (!(iss >> y >> dash1 >> m >> dash2 >> d)
        || dash1 != '-' || dash2 != '-')
    {
        throw std::invalid_argument("Bad date format: " + s);
    }

    year_month_day ymd{ year{y}, month{unsigned(m)}, day{unsigned(d)} };
    if (!ymd.ok()) {
        throw std::out_of_range("Invalid date: " + s);
    }

    SimulationClock clk;
    clk.current = sys_days{ymd};
    return clk;
}
