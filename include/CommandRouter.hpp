#pragma once

#include <iostream>
class SimulationEngine;

class CommandRouter {
public:
    explicit CommandRouter(SimulationEngine& sim,
                           std::istream& in  = std::cin,
                           std::ostream& out = std::cout);
    void run();

private:
    void cmdGenerate(std::istream& in);
    void cmdAdd     (std::istream& in);
    void cmdRemove  (std::istream& in);
    void cmdStep    (std::istream& in);
    void cmdInfo    (std::istream& in);
    void cmdSave    (std::istream& in);
    void cmdLoad    (std::istream& in);
    void printHelp() const;

    SimulationEngine& sim;
    std::istream&     in;
    std::ostream&     out;
};
