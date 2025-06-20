#include "../include/City.hpp"
#include "../include/building/Building.hpp"
#include <stdexcept>
#include <vector>

City::City(unsigned rowsCount, unsigned colsCount)
    : rowsCount(rowsCount), colsCount(colsCount) {
    if (rowsCount == 0 || colsCount == 0) {
        throw std::invalid_argument("City dimensions must be positive");
    }
    grid.resize(rowsCount);
    for (unsigned r = 0; r < rowsCount; ++r) {
        grid[r].resize(colsCount, nullptr);
    }
}

City::~City() {
    for (unsigned r = 0; r < rowsCount; ++r) {
        for (unsigned c = 0; c < colsCount; ++c) {
            delete grid[r][c];
        }
    }
}

City::City(const City& other)
  : rowsCount(other.rowsCount), colsCount(other.colsCount)
{
    grid.resize(rowsCount);
    for (unsigned r = 0; r < rowsCount; ++r) {
        grid[r].resize(colsCount, nullptr);
    }

    try {
        for (unsigned r = 0; r < rowsCount; ++r) {
            for (unsigned c = 0; c < colsCount; ++c) {
                Building* src = other.grid[r][c];
                if (src) {
                    // may throw
                    grid[r][c] = src->clone();
                }
            }
        }
    } catch (...) {
        for (unsigned r = 0; r < rowsCount; ++r) {
            for (unsigned c = 0; c < colsCount; ++c) {
                delete grid[r][c];
                grid[r][c] = nullptr;
            }
        }
        throw;
    }
}

City& City::operator=(const City& other) {
    if (this != &other) {
        City tmp(other);
        std::swap(rowsCount, tmp.rowsCount);
        std::swap(colsCount, tmp.colsCount);
        grid.swap(tmp.grid);
    }
    return *this;
}

City::City(City&& other) noexcept
    : rowsCount(other.rowsCount)
    , colsCount(other.colsCount)
    , grid(std::move(other.grid)) {
    other.rowsCount = 0;
    other.colsCount = 0;
}

City& City::operator=(City&& other) noexcept {
    if (this != &other) {
        for (unsigned r = 0; r < rowsCount; ++r) {
            for (unsigned c = 0; c < colsCount; ++c) {
                delete grid[r][c];
            }
        }
        rowsCount = other.rowsCount;
        colsCount = other.colsCount;
        grid = std::move(other.grid);
        other.rowsCount = 0;
        other.colsCount = 0;
    }
    return *this;
}

unsigned City::getRows() const noexcept { return rowsCount; }
unsigned City::getCols() const noexcept { return colsCount; }

Building& City::at(unsigned r, unsigned c) {
    if (r >= rowsCount || c >= colsCount) {
        throw std::out_of_range("City coordinates out of bounds");
    }
    Building* b = grid[r][c];
    if (!b) {
        throw std::runtime_error("No building at specified coordinates");
    }
    return *b;
}

const Building& City::at(unsigned r, unsigned c) const {
    if (r >= rowsCount || c >= colsCount) {
        throw std::out_of_range("City coordinates out of bounds");
    }
    Building* b = grid[r][c];
    if (!b) {
        throw std::runtime_error("No building at specified coordinates");
    }
    return *b;
}

void City::set(unsigned r, unsigned c, Building* building) {
    if (r >= rowsCount || c >= colsCount) {
        delete building;
        throw std::out_of_range("City coordinates out of bounds");
    }
    delete grid[r][c];
    grid[r][c] = building;
}

void City::serialize(std::ostream& out) const {
    if (!out) throw std::invalid_argument("Invalid stream for City::serialize");
    out << rowsCount << '\n' << colsCount << '\n';
    for (unsigned r = 0; r < rowsCount; ++r) {
        for (unsigned c = 0; c < colsCount; ++c) {
            if (grid[r][c]) {
                out << '1' << '\n';
                grid[r][c]->serialize(out);
            } else {
                out << '0' << '\n';
            }
        }
    }
}

City City::deserialize(std::istream& in) {
    if (!in) throw std::invalid_argument("Invalid stream for City::deserialize");

    unsigned n, m;
    if (!(in >> n >> m)) {
        throw std::runtime_error("Failed to read city dimensions");
    }
    in.ignore();

    City city(n, m);
    try {
        for (unsigned r = 0; r < n; ++r) {
            for (unsigned c = 0; c < m; ++c) {
                char marker;
                in.get(marker);
                in.ignore();
                if (marker == '1') {
                    city.grid[r][c] = Building::deserialize(in);
                } else if (marker != '0') {
                    throw std::runtime_error("Invalid building marker");
                }
            }
        }
        return city;
    } catch (...) {
        for (unsigned r = 0; r < city.rowsCount; ++r) {
            // ReSharper disable once CppDFAUnreachableCode
            for (unsigned c = 0; c < city.colsCount; ++c) {
                delete city.grid[r][c];
                city.grid[r][c] = nullptr;
            }
        }
        // ReSharper disable once CppDFAUnreachableCode
        throw;
    }
}
