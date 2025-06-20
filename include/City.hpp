#pragma once

#include <iostream>
#include <vector>
#include "building/Building.hpp"

class City {
public:
    City(unsigned rowsCount, unsigned colsCount);
    City(const City& other);
    City& operator=(const City& other);
    City(City&& other) noexcept;
    City& operator=(City&& other) noexcept;
    ~City();

    unsigned getRows() const noexcept;
    unsigned getCols() const noexcept;

    Building&       at(unsigned r, unsigned c);
    const Building& at(unsigned r, unsigned c) const;
    void set(unsigned r, unsigned c, Building* building);

    template<typename Fn> void forEachBuilding(Fn&& fn) const;
    template<typename Fn> void forEachBuilding(Fn&& fn);

    void serialize(std::ostream& out) const;
    static City deserialize(std::istream& in);

private:
    unsigned rowsCount;
    unsigned colsCount;
    std::vector<std::vector<Building*>> grid;
};

template<typename Fn>
void City::forEachBuilding(Fn&& fn) const {
    for (unsigned r = 0; r < rowsCount; ++r)
        for (unsigned c = 0; c < colsCount; ++c)
            if (grid[r][c]) fn(*grid[r][c], r, c);
}

template<typename Fn>
void City::forEachBuilding(Fn&& fn) {
    for (unsigned r = 0; r < rowsCount; ++r)
        for (unsigned c = 0; c < colsCount; ++c)
            if (grid[r][c]) fn(*grid[r][c], r, c);
}


