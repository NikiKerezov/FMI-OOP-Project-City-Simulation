#include <iostream>
#include <string>
#include <ctime>

#include "include/CommandRouter.hpp"
#include "include/SimulationEngine.hpp"
#include "include/City.hpp"

int main(int argc, char* argv[]) {
    try {
        std::srand(static_cast<unsigned>(std::time(nullptr)));

        std::cout << "City Simulation v1.0\n";
        std::cout << "====================\n\n";
        std::cout << "Tip: Use commands 'help' in the CLI to get started.\n";
        std::cout << "Starting interactive CLI mode...\n\n";

        City initialCity(2, 2);
        SimulationEngine engine(std::move(initialCity));

        CommandRouter router(engine);
        router.run();

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
