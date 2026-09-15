/*
** EPITECH PROJECT, 2026
** Poll.cpp
** File description:
** Poll class definition
*/

#include <utils/Poll.hpp>

Poll::Poll() {}

Poll::~Poll() {
    for (pollfd &fd : _fds) {
        close(fd.fd);
    }
}

std::vector<pollfd>& Poll::getFds() {
    return _fds;
}

void Poll::addFd(int fd) {
    pollfd newFd = {};
    newFd.events = POLLIN;
    newFd.fd = fd;
    _fds.push_back(newFd);
}

void Poll::removeFd(int fd) {
    for (std::vector<pollfd>::iterator iterator = _fds.begin(); iterator != _fds.end(); ++iterator) {
        if (iterator->fd == fd) {
            _fds.erase(iterator);
            return;
        }
    }
}

int Poll::wait(int timeout) {
    return poll(_fds.data(), _fds.size(), timeout);
}