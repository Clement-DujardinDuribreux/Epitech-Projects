/*
** EPITECH PROJECT, 2026
** ServerArgs.cpp
** File description:
** ServerArgs class definition
*/

#include "utils/ServerArgs.hpp"
#include <cstdlib>
#include <set>

ServerArgs::ServerArgs(int argc, char **argv) : _argc(argc), _argv(argv) {
    opterr = 0;
    optind = 1;
    int opt = getopt(_argc, _argv, "p:x:y:c:f:n:");

    while (opt != -1) {
        switch (opt){
            case 'p':
                if (std::string(optarg).find_first_not_of("0123456789") != std::string::npos)
                    throw ServerArgsException("Port must be a number");
                _port = atoi(optarg);
                break;
            case 'x':
                if (std::string(optarg).find_first_not_of("0123456789") != std::string::npos)
                    throw ServerArgsException("Width must be a number");
                _width = atoi(optarg);
                break;
            case 'y':
                if (std::string(optarg).find_first_not_of("0123456789") != std::string::npos)
                    throw ServerArgsException("Height must be a number");
                _height = atoi(optarg);
                break;
            case 'c':
                if (std::string(optarg).find_first_not_of("0123456789") != std::string::npos)
                    throw ServerArgsException("Client number must be a number");
                _clientNumber = atoi(optarg);
                break;
            case 'f':
                if (std::string(optarg).find_first_not_of("0123456789") != std::string::npos)
                    throw ServerArgsException("Frequency must be a number");
                _frequency = atoi(optarg);
                break;
            case 'n': 
                _teamsName.push_back(optarg);
                while (optind < _argc && _argv[optind][0] != '-') {
                    _teamsName.push_back(_argv[optind]);
                    optind++;
                }
                break;
            case '?':
                throw ServerArgsException("Usage: ./zappy_server -p <port> -x <width> -y <height> -c <client number> -f <frequency> -n <team name 1> <team name 2> ...");
            default:
                throw ServerArgsException("Usage: ./zappy_server -p <port> -x <width> -y <height> -c <client number> -f <frequency> -n <team name 1> <team name 2> ...");
        }

        opt = getopt(_argc, _argv, "p:x:y:c:f:n:");
    }
    
    if (!_port || !_width || !_height || !_clientNumber || !_teamsName.size())
        throw ServerArgsException("Usage: ./zappy_server -p <port> -x <width> -y <height> -c <client number> -f <frequency> -n <team name 1> <team name 2> ...");
    if (_port < MIN_PORT || _port > MAX_PORT)
        throw ServerArgsException("Port must be between 1 and 65535");
    if (_width < MIN_MAP_SIZE || _width > MAX_MAP_SIZE)
        throw ServerArgsException("Width must be between 10 and 42");
    if (_height < MIN_MAP_SIZE || _height > MAX_MAP_SIZE)
        throw ServerArgsException("Height must be between 10 and 42");
    if (_clientNumber < 1)
        throw ServerArgsException("Client number must be greater than 0");
    if (_frequency < MIN_FREQUENCY || _frequency > MAX_FREQUENCY)
        throw ServerArgsException("Frequency must be between 1 and 1000");

    std::set<std::string> seenTeams;
    for (const std::string &teamName : _teamsName) {
        if (teamName.empty() || teamName[0] == '-')
            throw ServerArgsException("Team name cannot be empty or start with '-'");
        if (teamName == "GRAPHIC")
            throw ServerArgsException("GRAPHIC is a reserved team name");
        if (!seenTeams.insert(teamName).second)
            throw ServerArgsException("Team names must be unique");
    }
}

int ServerArgs::getPort() const {
    return _port;
}

int ServerArgs::getFrequency() const {
    return _frequency;
}

int ServerArgs::getHeight() const {
    return _height;
}

int ServerArgs::getWidth() const {
    return _width;
}

int ServerArgs::getClientNumber() const {
    return _clientNumber;
}

std::vector<std::string> ServerArgs::getTeamsName() const {
    return _teamsName;
}

ServerArgs::ServerArgsException::ServerArgsException(const std::string &msg) : _msg(msg) {}

const char *ServerArgs::ServerArgsException::what() const noexcept {
    return _msg.c_str();
}
