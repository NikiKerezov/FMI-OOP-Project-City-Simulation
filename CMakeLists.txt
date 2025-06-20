cmake_minimum_required(VERSION 3.31)
project(city_simulation)

set(CMAKE_CXX_STANDARD 20)

add_executable(city_simulation main.cpp
        include/resident/Resident.hpp
        include/resident/Teacher.hpp
        include/resident/Programmer.hpp
        include/resident/Miner.hpp
        include/resident/Unemployed.hpp
        include/building/Building.hpp
        include/building/ModernBlock.hpp
        include/building/PanelBlock.hpp
        include/building/Dormitory.hpp
        include/Generator.hpp
        include/SimulationClock.hpp
        include/SimulationEngine.hpp
        src/building/Building.cpp
        src/building/ModernBlock.cpp
        src/building/PanelBlock.cpp
        src/building/Dormitory.cpp
        src/City.cpp
        src/CommandRouter.cpp
        src/Generator.cpp
        src/resident/Resident.cpp
        src/resident/Teacher.cpp
        src/resident/Programmer.cpp
        src/resident/Miner.cpp
        src/resident/Unemployed.cpp
        src/SimulationClock.cpp
        src/SimulationEngine.cpp
)
