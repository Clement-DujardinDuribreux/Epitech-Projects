#include "Server.hpp"

#include <csignal>
#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "USAGE: ./myteams_server port\n";
        return 84;
    }

    signal(SIGINT, Server::onSigint);

    try {
        Server server(std::atoi(argv[1]));
        server.run();
    } catch (const std::exception &e) {
        std::cout << "error: " << e.what() << '\n';
        return 84;
    }
    return 0;
}
