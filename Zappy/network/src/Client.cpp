/*
** EPITECH PROJECT, 2026
** Server.cpp
** File description:
** Server class definition
*/

#include <Client.hpp>

Client::Client(int fd) : _fd(fd), _playerId(-1), _actionTime(0),
    _actionNumber(0), _shouldDisconnect(false), _incanting(false) {}

int Client::getFd() {
    return _fd;
}

int Client::getPlayerId() const {
    return _playerId;
}

void Client::setPlayerId(int playerId) {
    _playerId = playerId;
}

std::string& Client::getWriteBuffer() {
    return _writeBuffer;
}

std::string& Client::getReadBuffer() {
    return _readBuffer;
}

int Client::getActionTime() const {
    return _actionTime;
}

void Client::setActionTime(int newActionTime) {
    _actionTime = newActionTime;
}

int Client::getActionNumber() const {
    return _actionNumber;
}

void Client::setActionNumber(int actionNumber) {
    _actionNumber = actionNumber;
}

void Client::addBuffer(std::string str) {
    _writeBuffer.append(str);
}

void Client::writeToClient() {
    int bytesSend = write(_fd, _writeBuffer.c_str(), _writeBuffer.size());
    if (bytesSend < 0)
        throw ClientException("Can't write");

    _writeBuffer.erase(0, bytesSend);
}

void Client::readFromClient() {
    char buffer[1025] = {0};
    int bytes = read(_fd, buffer, 1024);
    if (bytes == 0)
        throw ClientException("Connection finished");
    if (bytes < 0)
        throw ClientException("Read error");
    
    _readBuffer.append(buffer);
}

std::string Client::getTeamName() const {
    return _teamName;
}

void Client::setTeamName(std::string& teamName) {
    _teamName = teamName;
}

bool Client::getShouldDisconnect() const {
    return _shouldDisconnect;
}

void Client::setShouldDisconnect(bool shouldDisconnect) {
    _shouldDisconnect = shouldDisconnect;
}

bool Client::isIncanting() const {
    return _incanting;
}

void Client::setIncanting(bool incanting) {
    _incanting = incanting;
}

Player& Client::getPlayer() {
    return _player;
}

Client::ClientException::ClientException(const std::string &msg) : _msg(msg) {}

const char *Client::ClientException::what() const noexcept {
    return _msg.c_str();
}
