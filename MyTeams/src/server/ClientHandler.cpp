#include "ClientHandler.hpp"

#include <sys/socket.h>
#include <unistd.h>

ClientHandler::ClientHandler(int fd) : _fd(fd), _dead(false), _currentTeam(""), _currentChannel(""), _currentThread("") {}

ClientHandler::~ClientHandler()
{
    close(_fd);
}

void ClientHandler::appendToWriteBuffer(const std::string &data)
{
    _writeBuffer += data + "\n";
}

std::string ClientHandler::readLine()
{
    char buf[1024];
    ssize_t n = recv(_fd, buf, sizeof(buf), MSG_DONTWAIT);

    if (n <= 0) {
        markDead();
        return "";
    }

    _readBuffer.append(buf, n);

    auto pos = _readBuffer.find('\n');
    if (pos == std::string::npos)
        return "";

    std::string line = _readBuffer.substr(0, pos);
    _readBuffer.erase(0, pos + 1);
    return line;
}

void ClientHandler::flushWrites()
{
    if (_writeBuffer.empty())
        return;

    ssize_t n = send(_fd, _writeBuffer.c_str(), _writeBuffer.size(), MSG_DONTWAIT);
    if (n > 0)
        _writeBuffer.erase(0, n);
    else if (n < 0)
        markDead();
}
