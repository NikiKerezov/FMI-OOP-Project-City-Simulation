# 🏙️ City Simulation v1.0

This is an object-oriented C++ simulation of a virtual city populated by residents living in various building types. Residents go through daily routines, pay rent, and may be purged from the city if their stats (happiness, money, life points) drop to zero.

---

## 📚 Project Overview

This project was developed as a university assignment in object-oriented programming. It demonstrates modular architecture, data encapsulation, memory management, and simulation logic.

**Key mechanics include:**

* Daily resident routines
* Rent adjustments based on building location
* Snapshot-based rewind system
* Persistent save/load format with full state and history

---

## ⚙️ Build Instructions

Make sure you have a C++ compiler and CMake installed.

```bash
mkdir build
cd build
cmake ..
make
```

This will generate an executable (e.g., `city_simulation` or `city_simulation.exe` on Windows).

---

## ▶️ How to Run

From the `build` folder:

```bash
./city_simulation
```

You will enter an interactive command-line interface (CLI).

---

## 🧪 Example Commands

```text
help
# Lists all available commands.

generate 3 3
# Generates a 3×3 city with random buildings.

add 0 0 krum Unemployed 10 10 4
# Adds a resident named 'krum' to the building at (0,0).

info 0 0
# Displays info for the building at (0,0).

info 0 0 krum
# Displays stats for resident 'krum' in building (0,0).

step 5
# Advances the simulation by 5 days.

save mysave
# Saves the simulation to file 'mysave'.

load mysave
# Loads the simulation from file 'mysave', restoring rewind history.

step -3
# Rewinds the simulation by 3 days.

remove 0 0 krum
# Removes 'krum' from the building at (0,0).

exit
# Exits the simulation safely.
```

---

## 🗃️ File Format

Saved files contain:

1. Simulation clock date (ISO format)
2. City structure and all residents
3. Last purge counters (happy, life, money)
4. Full rewind history (snapshots of city + clock)

This enables restoring the entire state and applying backward steps even after reloading from disk.

---

## 🧠 Architecture Summary

### Key Classes

#### `SimulationEngine`

* Manages simulation logic, state history, saving, loading, and stepping forward/backward.
* **Members:**

  * `City city`
  * `SimulationClock clock`
  * `ZeroCounters last` — last purge stats
  * `std::vector<Snapshot> history` — full history for rewind
  * `std::vector<std::string> lastRemovedNames`

#### `City`

* Represents a 2D grid (`rowsCount × colsCount`) of `Building*`. Handles bounds checking and serialization.

#### `Building` (abstract)

* Base class for `ModernBlock`, `PanelBlock`, `Dormitory`.
* **Members:**

  * `unsigned maxCapacity`, `double baseRent`, `std::vector<Resident*> residents`.
* **Methods:** `clone()`, `addResident()`, `removeResident()`, `getCurrentRent()`, `serialize()`.

#### `Resident` (abstract)

* Base for `Teacher`, `Programmer`, `Miner`, `Unemployed`.
* **Members:** `std::string name`, `unsigned happiness, money, lifePoints`, `std::vector<std::string> history`.
* **Methods:** `liveOneDay()`, `getMonthlySalary()`, `applyMonthlyEffect()`, `serialize()`, `clone()`.

#### `SimulationClock`

* Manages the current date with `std::chrono::sys_days` and `year_month_day`.
* Supports `advance(days)`, `isFirstOfMonth()`, `dateString()`, `fromDateString()`.

---

## 🛠️ Example Test Workflow

```text
./city_simulation

>> generate 2 2
>> add 0 0 krum Unemployed 10 10 4
>> info 0 0 krum
>> step 4
>> save testfile
*RESTART*
>> load testfile
>> step -3
>> info 0 0 krum
>> exit
```
