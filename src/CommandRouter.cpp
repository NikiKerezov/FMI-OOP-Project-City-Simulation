#include "../include/CommandRouter.hpp"
#include "../include/SimulationEngine.hpp"
#include "../include/Generator.hpp"
#include "../include/resident/Teacher.hpp"
#include "../include/resident/Programmer.hpp"
#include "../include/resident/Miner.hpp"
#include "../include/resident/Unemployed.hpp"
#include "../include/building/ModernBlock.hpp"
#include "../include/building/PanelBlock.hpp"
#include "../include/building/Dormitory.hpp"
#include "../include/constants.hpp"

#include <sstream>
#include <stdexcept>
#include <cstdio>
#include <cmath>
#include <vector>

CommandRouter::CommandRouter(SimulationEngine& sim,
                             std::istream& in,
                             std::ostream& out)
    : sim(sim), in(in), out(out) {}

void CommandRouter::run() {
    std::string line;
    out << "City Simulation v1.0\n";
    out << "Type 'help' for available commands.\n";

    while (true) {
        out << ">> ";
        out.flush();
        if (!std::getline(in, line)) break;
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string command;
        iss >> command;

        try {
            if (command == "exit" || command == "quit") {
                break;
            } else if (command == "help") {
                printHelp();
            } else if (command == "generate") {
                cmdGenerate(iss);
            } else if (command == "add") {
                cmdAdd(iss);
            } else if (command == "remove") {
                cmdRemove(iss);
            } else if (command == "step") {
                cmdStep(iss);
            } else if (command == "info") {
                cmdInfo(iss);
            } else if (command == "save") {
                cmdSave(iss);
            } else if (command == "load") {
                cmdLoad(iss);
            } else {
                out << "Unknown command: " << command << ". Type 'help' for available commands.\n";
            }
        } catch (const std::exception& e) {
            out << "Error: " << e.what() << "\n";
        }
    }
}

void CommandRouter::printHelp() const {
    out << "Available commands:\n"
        << "  generate <n> <m>                     - Create city with random data\n"
        << "  add <r> <c> <name> <job> <h> <m> <l> - Add resident\n"
        << "  remove <r> <c> <name>                - Remove resident\n"
        << "  step [n]                             - Simulate n days\n"
        << "  info [r c [name]]                    - Show info\n"
        << "  save <filename>                      - Save simulation\n"
        << "  load <filename>                      - Load simulation\n"
        << "  help                                 - Show this help\n"
        << "  exit/quit                            - Exit simulation\n";
}

void CommandRouter::cmdGenerate(std::istream& args) {
    unsigned n, m;
    if (!(args >> n >> m)) {
        out << "Error: Usage: generate <n> <m>\n";
        return;
    }
    if (n == 0 || m == 0) {
        out << "Error: City dimensions must be positive\n";
        return;
    }

    try {
        Generator gen;
        City city = gen.generateCity(n, m);
        SimulationEngine newEngine(std::move(city));
        sim = std::move(newEngine);
        out << sim.getClock().dateString() << "\n";
    } catch (const std::exception& e) {
        out << "Failed to generate city: " << e.what() << "\n";
    }
}

void CommandRouter::cmdAdd(std::istream& args) {
    unsigned r, c;
    std::string name, job;
    unsigned h, money, life;

    if (!(args >> r >> c >> name >> job >> h >> money >> life)) {
        throw std::invalid_argument("Usage: add <r> <c> <name> <job> <h> <m> <l>");
    }

    try {
        sim.getCity().at(r, c);
    } catch (const std::exception& e) {
        out << "Error: cannot add resident to (" << r << "," << c << "): "
            << e.what() << "\n";
        return;
    }

    Resident* residentPtr = nullptr;
    try {
        if (job == "Teacher") {
            residentPtr = new Teacher(name, h, money, life);
        } else if (job == "Programmer") {
            residentPtr = new Programmer(name, h, money, life);
        } else if (job == "Miner") {
            residentPtr = new Miner(name, h, money, life);
        } else if (job == "Unemployed") {
            residentPtr = new Unemployed(name, h, money, life);
        } else {
            throw std::invalid_argument(
                "Invalid job: must be Teacher, Programmer, Miner, or Unemployed");
        }

        sim.addResident(r, c, residentPtr);
        out << "Success: Resident added.\n";

    } catch (const std::exception& e) {
        out << "Failed to add resident: " << e.what() << "\n";
    }
}


void CommandRouter::cmdRemove(std::istream& args) {
    unsigned r, c;
    std::string name;
    if (!(args >> r >> c >> name)) {
        throw std::invalid_argument("Usage: remove <r> <c> <name>");
    }
    sim.removeResident(r, c, name);
    out << "Success: Resident removed.\n";
}

void CommandRouter::cmdStep(std::istream& args) {
    long long days = 1;
    if (!(args >> days)) {
        out << "Usage: step [n]\n";
        return;
    }

    try {
        std::vector<std::string> purged = sim.step(days);
        if (purged.empty()) {
            out << "No residents were removed.\n";
        } else {
            out << "Purged residents:\n";
            for (const std::string& name : purged) {
                out << "  " << name << "\n";
            }
        }
    } catch (const std::exception& e) {
        out << "Error: " << e.what() << "\n";
    }
}

void CommandRouter::cmdInfo(std::istream& args) {
    std::vector<std::string> parts;
    std::string token;
    while (args >> token) {
        parts.push_back(token);
    }

    const City& city = sim.getCity();
    unsigned rows = city.getRows();
    unsigned cols = city.getCols();

    if (parts.size() == 0) {
        for (unsigned r = 0; r < rows; ++r) {
            for (unsigned c = 0; c < cols; ++c) {
                out << "Location " << r << " " << c << ":\n";
                const Building& b = city.at(r, c);

                if (dynamic_cast<const ModernBlock*>(&b))       out << "    ModernBlock:\n";
                else if (dynamic_cast<const PanelBlock*>(&b))   out << "    PanelBlock:\n";
                else if (dynamic_cast<const Dormitory*>(&b))    out << "    Dormitory:\n";
                else                                            out << "    UnknownBuilding:\n";

                const std::vector<Resident*>& occ = b.getOccupants();
                for (std::vector<Resident*>::const_iterator it = occ.begin(); it != occ.end(); ++it) {
                    Resident* resident = *it;
                    out << "        " << resident->getName() << "\n";
                    out << "            Happiness: " << resident->getHappiness() << "\n";
                    out << "            Money:     " << resident->getMoney()     << "\n";
                    out << "            Life:      " << resident->getLifePoints()<< "\n";
                    out << "            History:\n";
                    const std::vector<std::string>& history = resident->getHistory();
                    for (std::vector<std::string>::const_iterator hit = history.begin(); hit != history.end(); ++hit) {
                        out << "                " << *hit << "\n";
                    }
                }
            }
        }
        return;
    }

    // info r c
    if (parts.size() == 2) {
        unsigned r, c;
        {
            std::istringstream iss(parts[0] + " " + parts[1]);
            if (!(iss >> r >> c)) {
                out << "Usage: info [r c [name]]\n";
                return;
            }
        }
        try {
            const Building& b = city.at(r, c);
            out << "Building at (" << r << "," << c << "):\n";

            if      (dynamic_cast<const ModernBlock*>(&b))     out << "  Type: ModernBlock\n";
            else if (dynamic_cast<const PanelBlock*>(&b))      out << "  Type: PanelBlock\n";
            else if (dynamic_cast<const Dormitory*>(&b))       out << "  Type: Dormitory\n";
            else                                               out << "  Type: UnknownBuilding\n";

            out << "  Capacity:     " << b.getCapacity() << "\n";

            unsigned R = city.getRows();
            unsigned C = city.getCols();
            double centerR   = (R - 1) / 2.0;
            double centerC   = (C - 1) / 2.0;
            double minDim    = std::min(R, C);
            double centralTh = minDim / 8.0;
            double outskirtsTh = 6.0 * minDim / 8.0;

            double rent = b.getCurrentRent();
            double dr = r - centerR, dc = c - centerC;
            double dist = std::sqrt(dr*dr + dc*dc);
            if      (dist <= centralTh)    rent *= constants::centralMultiplier;
            else if (dist > outskirtsTh)   rent *= constants::outskirtsDiscount;

            out << "  Current rent: " << rent << "\n";

            const std::vector<Resident*>& occ = b.getOccupants();
            out << "  Occupants (" << occ.size() << "):\n";
            for (Resident* resident : occ) {
                out << "    " << resident->getName() << "\n";
            }
        } catch (const std::exception& e) {
            out << "Error: " << e.what() << "\n";
        }
        return;
    }


    // info r c name
    if (parts.size() == 3) {
        unsigned r, c;
        const std::string& name = parts[2];
        {
            std::istringstream iss(parts[0] + " " + parts[1]);
            if (!(iss >> r >> c)) {
                out << "Usage: info [r c [name]]\n";
                return;
            }
        }
        try {
            const Building& b = city.at(r, c);

            Resident* target = nullptr;
            const std::vector<Resident*>& occ = b.getOccupants();
            for (std::vector<Resident*>::const_iterator it = occ.begin(); it != occ.end(); ++it) {
                Resident* res = *it;
                if (res->getName() == name) {
                    target = res;
                    break;
                }
            }
            if (!target) {
                out << "Error: no resident named '" << name
                    << "' at (" << r << "," << c << ")\n";
                return;
            }

            out << "Resident '" << name << "' at (" << r << "," << c << "):\n";
            out << "  Profession:   ";
            if      (dynamic_cast<Teacher*>(target))    out << "Teacher\n";
            else if (dynamic_cast<Programmer*>(target)) out << "Programmer\n";
            else if (dynamic_cast<Miner*>(target))      out << "Miner\n";
            else if (dynamic_cast<Unemployed*>(target)) out << "Unemployed\n";
            else                                        out << "Unknown\n";

            out << "  Happiness:    " << target->getHappiness()  << "\n";
            out << "  Money:        " << target->getMoney()      << "\n";
            out << "  LifePoints:   " << target->getLifePoints() << "\n";
            out << "  History:\n";
            const std::vector<std::string>& history = target->getHistory();
            for (std::vector<std::string>::const_iterator hit = history.begin(); hit != history.end(); ++hit) {
                out << "    " << *hit << "\n";
            }
        } catch (const std::exception& e) {
            out << "Error: " << e.what() << "\n";
        }
        return;
    }

    // too many args
    out << "Usage: info [r c [name]]\n";
}



void CommandRouter::cmdSave(std::istream& args) {
    std::string filename;
    if (!(args >> filename)) {
        out << "Error: Usage: save <filename>\n";
        return;
    }
    try {
        sim.save(filename);
        out << "Saved to " << filename << "\n";
    } catch (const std::exception& e) {
        out << "Save failed: " << e.what() << "\n";
    }
}

void CommandRouter::cmdLoad(std::istream& args) {
    std::string filename;
    if (!(args >> filename)) {
        out << "Error: Usage: load <filename>\n";
        return;
    }
    try {
        sim.load(filename);
        out << "Loaded from " << filename
            << "  (simulation date: " << sim.getClock().dateString() << ")\n";
    } catch (const std::exception& e) {
        out << "Load failed: " << e.what() << "\n";
    }
}
