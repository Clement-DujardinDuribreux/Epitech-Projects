/*
** EPITECH PROJECT, 2026
** AGraphical.hpp
** File description:
** AGraphical class definition
*/

#include <Server.hpp>

int main(int argc, char **argv) {
    try {
        ServerArgs args(argc, argv);
        Server server(args);
        server.startUp();
        return 0;
    } catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    
}