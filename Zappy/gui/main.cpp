#include <iostream>
#include "src/GUI.hpp"
#include "src/ArgvParser.hpp"

/**
 * @brief Starts the Zappy graphical client.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Zero on success, 84 on startup error.
 */
int main([[maybe_unused]] int argc, char *argv[]) {
    try {
        GUI::checkDisplay();
        AssetsManager::loadGameAssets();
        ArgvParser parser(argv);
        int port = parser.getArgFromFlag<int>("-p", [](const std::string &s) { return std::stoi(s); });
        std::string hostname = parser.getArgFromFlag<std::string>("-h", [](const std::string &s) { return s; });
        GUI gui(port, hostname);
        gui.run();
    } catch (const ArgvParser::ArgvParserException &e) {
        std::cout << "\nUSAGE:\n  ./zappy_gui -p <port> -h <machine>\n\n  - [<port>]: Port to connect on server\n  - [<machine>]: Hostname of the server\n" << std::endl;
        return 84;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
