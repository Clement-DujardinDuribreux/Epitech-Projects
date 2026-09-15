/*
** EPITECH PROJECT, 2026
** Socket.cpp
** File description:
** Socket class definition
*/

#include "utils/Socket.hpp"

Socket::Socket(int port) {
    _fd = socket(AF_INET, SOCK_STREAM, 0); 
    int opt = 1;   
    if (_fd == -1)
        throw SocketException("Socket can't be init");

    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Échec de setsockopt (SO_REUSEADDR)");
        throw SocketException("Can't reused port");
    }

    sockaddr_in addr = {};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (bind(_fd, (struct sockaddr*)&addr, sizeof(addr)) != 0)
        throw SocketException("can't bind the socket");

    if (listen(_fd, 10) < 0)
        throw SocketException("can't initialized listening");
}

Socket::Socket(std::string ip, int port) {
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0)
        throw SocketException("Socket can't be init");

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0)
        throw SocketException("can't convert ip");

    if (connect(_fd, (sockaddr *)&addr, sizeof(addr)) < 0)
        throw SocketException("can't initialized connexion");
}

Socket::~Socket() {
    if (_fd != -1)
        close(_fd);
}

int Socket::getFd() const {
    return _fd;
}

Socket::SocketException::SocketException(const std::string &msg) : _msg(msg) {}

const char *Socket::SocketException::what() const noexcept {
    return _msg.c_str();
}