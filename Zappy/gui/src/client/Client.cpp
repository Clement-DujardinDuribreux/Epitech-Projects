#include "Client.hpp"
#include <iostream>
#include <memory>
#include <sstream>
#include <unistd.h>
#include <thread>
#include <chrono>

Client::Client() : _map(0, 0) {}

Client::~Client() { stopUpdateThread(); }

bool Client::connectToServer(std::string ip, int port) {
    try {
        _socket = std::make_unique<Socket>(ip, port);
        _poll.addFd(_socket->getFd());
        startUpdateThread();
        return true;
    } catch (const Socket::SocketException &e) {
        std::cerr << "Impossible de se connecter : " << e.what() << std::endl;
        return false;
    }
}

void Client::startUpdateThread() {
    _running = true;
    _thread = std::make_unique<Thread<void>>([this]() {
        while (_running) {
            update();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
}

void Client::stopUpdateThread() {
    _running = false;
    _thread.reset();
}

void Client::update() {
    ScopedLock lock(_mutex);
    if (!_socket)
        return;
    if (_poll.wait(0) > 0) {
        std::vector<pollfd> fds = _poll.getFds();
        if (!fds.empty() && (fds[0].revents & POLLIN)) {
            char tempBuffer[4096];
            ssize_t received = recv(_socket->getFd(), tempBuffer, sizeof(tempBuffer) - 1, 0);
            if (received > 0) {
                tempBuffer[received] = '\0';
                _buffer.append(tempBuffer);
                _extractCommands();
            } else if (received == 0) {
                _poll.removeFd(_socket->getFd());
                _socket.reset();
                return;
            }
        }
        if (!_socket)
            return;
        if (!fds.empty() && (fds[0].revents & POLLOUT)) {
            int sendedChars = send(_socket->getFd(), _writeBuffer.c_str(), _writeBuffer.size(), 0);
            if (sendedChars >= 0) {
                _writeBuffer.erase(0, sendedChars);
                _poll.getFds()[0].events = _writeBuffer.size() == 0 ? POLLIN : POLLIN | POLLOUT;
            }
        }
    }
}

void Client::_extractCommands() {
    size_t pos = 0;
    while ((pos = _buffer.find('\n')) != std::string::npos) {
        std::string command = _buffer.substr(0, pos);
        _buffer.erase(0, pos + 1);
        if (!command.empty())
            _parseCommand(command);
    }
}

void Client::sendMsg(std::string msg) {
    ScopedLock lock(_mutex);
    if (!_socket)
        return;
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastSendTime).count();
    if (elapsed < 200)
        return;
    _lastSendTime = now;
    _writeBuffer.append(msg);
    _poll.getFds()[0].events = POLLIN | POLLOUT;
}

bool Client::getEnd() const {
    ScopedLock lock(const_cast<MyMutex &>(_mutex));
    return _isEnding;
}

std::string Client::getWinnerTeam() const {
    ScopedLock lock(const_cast<MyMutex &>(_mutex));
    return _winnerTeam;
}

void Client::setEnd(bool end) {
    ScopedLock lock(_mutex);
    _isEnding = end;
}

int Client::getTimeUnit() const {
    ScopedLock lock(const_cast<MyMutex &>(_mutex));
    return _timeUnit;
}

void Client::setTimeUnit(int timeUnit) {
    ScopedLock lock(_mutex);
    _timeUnit = timeUnit;
}

Map Client::getMap() const {
    ScopedLock lock(const_cast<MyMutex &>(_mutex));
    return _map;
}



void Client::requestMapSize() { sendMsg("msz\n"); }
void Client::requestContentTile(int x, int y) { sendMsg("bct " + std::to_string(x) + " " + std::to_string(y) + "\n"); }
void Client::requestContentMap() { sendMsg("mct\n"); }
void Client::requestNameTeams() { sendMsg("tna\n"); }
void Client::requestPlayerPosition(int id) { sendMsg("ppo " + std::to_string(id) + "\n"); }
void Client::requestPlayerLevel(int id) { sendMsg("plv " + std::to_string(id) + "\n"); }
void Client::requestPlayerInventory(int id){ sendMsg("pin " + std::to_string(id) + "\n"); }
void Client::requestTimeUnit() { sendMsg("sgt\n"); }
void Client::requestEditTimeUnit(int t) { sendMsg("sst " + std::to_string(t) + "\n"); }

static int parseInt(const std::string& arg) {
    if (!arg.empty() && arg[0] == '#') {
        return std::stoi(arg.substr(1));
    }
    return std::stoi(arg);
}

void Client::_parseCommand(const std::string &command) {
    if (command.empty()) return;
    std::istringstream iss(command);
    std::vector<std::string> args;
    std::string word;

    while (iss >> word)
        args.push_back(word);
    if (args.empty())
        return;

    auto it = _commandMap.find(args[0]);
    if (it == _commandMap.end())
        return;

    try {
        it->second(args);
    } catch (const std::exception &) {
        return;
    }
}

void Client::_handleWelcome([[maybe_unused]] const std::vector<std::string> &args) {
    std::string response = "GRAPHIC\n";
    send(_socket->getFd(), response.c_str(), response.size(), 0);
}

void Client::_handleMapSize([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() != 3)
        return;
    int width = parseInt(args[1]);
    int height = parseInt(args[2]);

    if (width > 0 && height > 0)
        _map.resize(width, height);
}

void Client::_handleTileContent([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() == 10) {
        int x = parseInt(args[1]);
        int y = parseInt(args[2]);
        int food = parseInt(args[3]);
        int linemate = parseInt(args[4]);
        int deraumere = parseInt(args[5]);
        int sibur = parseInt(args[6]);
        int mendiane = parseInt(args[7]);
        int phiras = parseInt(args[8]);
        int thystame = parseInt(args[9]);
        _map.getCell(x, y).setResources({food, linemate, deraumere, sibur, mendiane, phiras, thystame});
    }
}

void Client::_handleTeamName([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() == 2)
        _teamName.push_back(args[1]);
}

void Client::_handlePlayerNew([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() == 7) {
        int id = parseInt(args[1]);
        int x = parseInt(args[2]);
        int y = parseInt(args[3]);
        int orientation = parseInt(args[4]);
        int level = parseInt(args[5]);
        std::string teamName = args[6];
        _map.getPlayers().emplace_back(id, x, y, orientation, level, teamName);
    }
}

void Client::_handlePlayerPos([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() == 5) {
        int id = parseInt(args[1]);
        int x = parseInt(args[2]);
        int y = parseInt(args[3]);
        int orientation = parseInt(args[4]);
        for (auto &player : _map.getPlayers())
            if (player.getId() == id) {
                player.setLocation(x, y);
                player.setOrientation(orientation);
                break;
        }
    }
}

void Client::_handlePlayerLevel([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() == 3) {
        int id = parseInt(args[1]);
        int level = parseInt(args[2]);
        for (auto &player : _map.getPlayers())
            if (player.getId() == id) {
                player.setLevel(level);
                break;
            }
    }
}

void Client::_handlePlayerInv([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() == 11) {
        int id = parseInt(args[1]);
        int food = parseInt(args[4]);
        int linemate = parseInt(args[5]);
        int deraumere = parseInt(args[6]);
        int sibur = parseInt(args[7]);
        int mendiane = parseInt(args[8]);
        int phiras = parseInt(args[9]);
        int thystame = parseInt(args[10]);
        for (auto &player : _map.getPlayers())
            if (player.getId() == id) {
                player.setInventory({food, linemate, deraumere, sibur, mendiane, phiras, thystame});
                break;
            }
    }
}

void Client::_handlePlayerExp([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() == 2) {
        int id = parseInt(args[1]);
        for (auto &player : _map.getPlayers())
            if (player.getId() == id) {
                player.setExpelling(true);
                break;
            }
    }
}

void Client::_handlePlayerBroadcast([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() >= 3) {
        int id = parseInt(args[1]);
        std::string message;
        for (size_t i = 2; i < args.size(); i++) {
            if (i > 2) message += " ";
            message += args[i];
        }
        for (auto &player : _map.getPlayers())
            if (player.getId() == id) {
                player.setBroadcast(message);
                break;
            }
    }
}

void Client::_handlePlayerDrop([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() == 3) {
        int id = parseInt(args[1]);
        for (auto &player : _map.getPlayers())
            if (player.getId() == id) {
                player.setDropping(true);
                break;
            }
    }
}

void Client::_handlePlayerTake([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() == 3) {
        int id = parseInt(args[1]);
        for (auto &player : _map.getPlayers())
            if (player.getId() == id) {
                player.setTaking(true);
                break;
            }
    }
}

void Client::_handlePlayerFork([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() == 2) {
        int id = parseInt(args[1]);
        for (auto &player : _map.getPlayers())
            if (player.getId() == id) {
                player.setForking(true);
                break;
            }
    }
}

void Client::_handlePlayerDead([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() == 2) {
        int id = parseInt(args[1]);
        auto &players = _map.getPlayers();
        for (auto it = players.begin(); it != players.end(); ++it)
            if (it->getId() == id) {
                players.erase(it);
                break;
            }
    }
}

void Client::_handleIncantStart([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() >= 5) {
        int x = parseInt(args[1]);
        int y = parseInt(args[2]);
        int level = parseInt(args[3]);
        _map.startIncantation(x, y, level);
        for (size_t i = 4; i < args.size(); i++) {
            int id = parseInt(args[i]);
            for (auto &player : _map.getPlayers())
                if (player.getId() == id) {
                    player.setIncantation(true);
                    break;
                }
        }
    }
}

void Client::_handleIncantEnd([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() == 4) {
        int x = parseInt(args[1]);
        int y = parseInt(args[2]);
        _map.stopIncantation(x, y);
        for (auto &player : _map.getPlayers())
            if (player.getX() == x && player.getY() == y && player.getIncanting())
                player.setIncantation(false);
    }
}

void Client::_handleEggNew([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() == 5)
        _map.getEggs().emplace_back(parseInt(args[1]), parseInt(args[2]), parseInt(args[3]), parseInt(args[4]));
}

void Client::_handleEggDead([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() == 2) {
        int id = parseInt(args[1]);
        _map.getEggs().erase(std::remove_if(_map.getEggs().begin(), _map.getEggs().end(), [id](const Egg &egg) {
            return egg.getId() == id;
        }), _map.getEggs().end());
    }
}

void Client::_handleEggBound([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() == 2) {
        int id = parseInt(args[1]);
        _map.getEggs().erase(std::remove_if(_map.getEggs().begin(), _map.getEggs().end(), [id](const Egg &egg) {
            return egg.getId() == id;
        }), _map.getEggs().end());
    }
}

void Client::_handleTimeGet([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() == 2)
        _timeUnit = parseInt(args[1]);
}

void Client::_handleMessageServer([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() >= 2) {
        std::string message;
        for (size_t i = 1; i < args.size(); i++) {
            if (i > 1)
                message += " ";
            message += args[i];
        }
        _serverMessage.push_back(message);
    }
}

void Client::_handleUnknownCommand([[maybe_unused]] const std::vector<std::string> &args) {
    _serverMessage.push_back("[ZAPPY ERROR] Server did not understand the command (suc).\n");
}

void Client::_handleBadParameter([[maybe_unused]] const std::vector<std::string> &args) {
    _serverMessage.push_back("[ZAPPY ERROR] Bad parameter sent to the server (sbp).\n");
}

void Client::_handleTimeSet([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() == 2)
        _timeUnit = parseInt(args[1]);
}

void Client::_handleGameEnd([[maybe_unused]] const std::vector<std::string> &args) {
    if (args.size() == 2) {
        _winnerTeam = args[1];
        _isEnding = true;
    }
}
