#include "Client.hpp"

#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cout << "USAGE: ./myteams_cli ip port\n";
        return 84;
    }

    try {
        Client client(argv[1], std::atoi(argv[2]));
        client.run();
    } catch (const std::exception &e) {
        std::cout << "error: " << e.what() << '\n';
        return 84;
    }
    return 0;
}
