/*
** EPITECH PROJECT, 2026
** Server.cpp
** File description:
** Server class definition
*/

#include <Server.hpp>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include "commands/GameEventCommand.hpp"
#include "commands/GraphicsEventCommand.hpp"
#include "utils/TerminalStyle.hpp"

int Server::protocolOrientation(Directions direction)
{
    if (direction == Directions::North)
        return 1;
    if (direction == Directions::East)
        return 2;
    if (direction == Directions::South)
        return 3;
    return 4;
}

std::string Server::stripPlayerPrefix(std::string value)
{
    if (!value.empty() && value[0] == '#')
        value.erase(0, 1);
    return value;
}

std::string Server::cleanProtocalMessage(std::string message)
{
    for (char &character : message) {
        if (character == '\n' || character == '\r')
            character = ' ';
    }
    return message;
}

std::string Server::formatTile(std::size_t x, std::size_t y)
{
    Inventory &resources = _game.square(x, y).getResources();
    std::string response = "bct " + std::to_string(x) + " "
        + std::to_string(y);

    for (int resource = Ressources::Food; resource <= Ressources::Thystame;
        resource++)
        response += " " + std::to_string(resources.getRessourcesQuantity(
            static_cast<Ressources>(resource)));
    return response + "\n";
}

std::string Server::formatPlayerPosition(int playerId, Player &player)
{
    std::pair<std::size_t, std::size_t> position = player.getPosition();

    return "ppo #" + std::to_string(playerId) + " "
        + std::to_string(position.first) + " "
        + std::to_string(position.second) + " "
        + std::to_string(protocolOrientation(player.getDirection())) + "\n";
}

std::string Server::formatPlayerLevel(int playerId, Player &player)
{
    return "plv #" + std::to_string(playerId) + " "
        + std::to_string(player.getLevel()) + "\n";
}

std::string Server::formatPlayerInventory(int playerId, Player &player)
{
    std::pair<std::size_t, std::size_t> position = player.getPosition();
    Inventory inventory = player.getInventory();
    std::string response = "pin #" + std::to_string(playerId) + " "
        + std::to_string(position.first) + " "
        + std::to_string(position.second);

    for (int resource = Ressources::Food; resource <= Ressources::Thystame;
        resource++)
        response += " " + std::to_string(inventory.getRessourcesQuantity(
            static_cast<Ressources>(resource)));
    return response + "\n";
}

Server::Server(ServerArgs args) : _args(args), _isOn(true),
    _state(RUNNING), _socket(args.getPort()), _nextPlayerId(0),
    _timeUnit(args.getFrequency()), _winnerTeam(), _timeline(),
    _game(_args.getWidth(), _args.getHeight()) {
    _poll.addFd(_socket.getFd());
    
    for (std::string &team : _args.getTeamsName()) {
        _teamsRest.insert(std::make_pair(team, _args.getClientNumber()));
    }
    initializeEggs();
    scheduleGameEvent(20, "respawn");
}

void Server::initializeEggs()
{
    for (const std::string &team : _args.getTeamsName()) {
        for (int index = 0; index < _args.getClientNumber(); index++) {
            _game.addEgg(-1,
                static_cast<std::size_t>(std::rand()) % _game.getWidth(),
                static_cast<std::size_t>(std::rand()) % _game.getHeight(),
                team);
        }
    }
}

void Server::printStartupBanner() const
{
    std::cout << TerminalStyle::boldCyan()
        << "========================================\n"
        << "             ZAPPY SERVER\n"
        << "========================================"
        << TerminalStyle::reset() << "\n"
        << TerminalStyle::boldGreen() << "  Status       "
        << TerminalStyle::reset() << "RUNNING\n"
        << TerminalStyle::boldGreen() << "  Port         "
        << TerminalStyle::reset() << _args.getPort() << "\n"
        << TerminalStyle::boldGreen() << "  Map          "
        << TerminalStyle::reset() << _args.getWidth() << " x "
        << _args.getHeight() << "\n"
        << TerminalStyle::boldGreen() << "  Clients/team "
        << TerminalStyle::reset()
        << _args.getClientNumber() << "\n"
        << TerminalStyle::boldGreen() << "  Frequency    "
        << TerminalStyle::reset()
        << _args.getFrequency() << "\n"
        << TerminalStyle::boldGreen() << "  Teams        "
        << TerminalStyle::reset();
    const std::vector<std::string> teams = _args.getTeamsName();
    for (std::size_t index = 0; index < teams.size(); index++)
        std::cout << (index == 0 ? "" : ", ") << teams[index];
    std::cout << "\n" << TerminalStyle::dim()
        << "  Press Ctrl+C to stop the server."
        << TerminalStyle::reset() << "\n\n";
}

void Server::startUp() {
    printStartupBanner();
    while (_isOn) {
        int wait = _poll.wait(_timeline.getNextTimeout());
    
        if (wait < 0) {
            _isOn = false;
            break;
        } else if (wait > 0) {
            for (std::size_t i = 0; i < _poll.getFds().size(); i++) {
                if (!(_poll.getFds()[i].revents & POLLIN) && !(_poll.getFds()[i].revents & POLLOUT))
                    continue;
                
                if (i == 0) {
                    acceptClient();
                } else {
                    processClientSocket(i);
                }
            }
        }
        while (_timeline.getNextTimeout() == 0)
            executeTimeline();
    }
}

void Server::executeTimeline() {
    std::shared_ptr<Action> action = _timeline.pop();

    if (_state == GAME_OVER && !action->command->canRunAfterGameOver())
        return;
    if (action->poll_index == -1) {
        action->command->exec(action->args, *this, -1, _game);
        return;
    }
    if (action->poll_index < 0 || static_cast<std::size_t>(action->poll_index) >= _poll.getFds().size())
        return;
    int fd = _poll.getFds()[action->poll_index].fd;
    if (_clients.find(fd) != _clients.end()) {
        if (_clients.at(fd).isIncanting()) {
            _clients.at(fd).setActionTime(_clients.at(fd).getActionTime()
                - (action->command->getCycleNumber() * 1000) / _timeUnit);
            _clients.at(fd).setActionNumber(
                _clients.at(fd).getActionNumber() - 1);
            return;
        }
        action->command->exec(action->args, *this, action->poll_index, _game);
        _clients.at(fd).setActionTime(_clients.at(fd).getActionTime()
            - (action->command->getCycleNumber() * 1000) / _timeUnit);
        _clients.at(fd).setActionNumber(_clients.at(fd).getActionNumber() - 1);
    }
}

void Server::acceptClient() {
    try {
        int newFd = accept(_poll.getFds()[0].fd, nullptr, nullptr);
        if (newFd < 0)
            throw ServerException("accept error");
        Client newClient(newFd);
        _clients.insert({newFd, newClient});
        _poll.addFd(newFd);
        sendToClient(_poll.getFds().size() - 1, "WELCOME\n");
    } catch (std::exception &e) {
        std::cerr << "Accepting error: " << e.what() << std::endl;
    }
}

void Server::removeClient(int fd) {
    for (std::size_t index = 0; index < _poll.getFds().size(); index++) {
        if (_poll.getFds()[index].fd == fd) {
            _timeline.removePollIndex(static_cast<int>(index));
            break;
        }
    }
    _poll.removeFd(fd);
    _clients.erase(fd);
}

void Server::sendTeamNumber(int poll_index, std::string& teamName) {
        int fd = _poll.getFds()[poll_index].fd;
        if (teamName == "GRAPHIC") {
            _clients.at(fd).setTeamName(teamName);
            _graphicsFds.push_back(fd);
            sendToClient(poll_index, "msz " + std::to_string(_game.getWidth())
                + " " + std::to_string(_game.getHeight()) + "\n");
            sendToClient(poll_index, "sgt " + std::to_string(_timeUnit) + "\n");
            for (std::size_t y = 0; y < _game.getHeight(); y++) {
                for (std::size_t x = 0; x < _game.getWidth(); x++)
                    sendToClient(poll_index, formatTile(x, y));
            }
            for (const auto &team : _teamsRest)
                sendToClient(poll_index, "tna " + team.first + "\n");
            for (const auto &entry : _clients) {
                const Client &client = entry.second;
                if (client.getPlayerId() < 0)
                    continue;
                Player &player = _clients.at(entry.first).getPlayer();
                std::pair<std::size_t, std::size_t> position = player.getPosition();
                sendToClient(poll_index, "pnw #" + std::to_string(client.getPlayerId())
                    + " " + std::to_string(position.first) + " "
                    + std::to_string(position.second) + " "
                    + std::to_string(protocolOrientation(player.getDirection())) + " "
                    + std::to_string(player.getLevel()) + " "
                    + client.getTeamName() + "\n");
            }
            for (const GameEgg &egg : _game.getEggs())
                sendToClient(poll_index, "enw #" + std::to_string(egg.id)
                    + " #" + std::to_string(egg.playerId) + " "
                    + std::to_string(egg.x) + " "
                    + std::to_string(egg.y) + "\n");
            if (_state == GAME_OVER)
                sendToClient(poll_index, "seg " + _winnerTeam + "\n");
            return;
        }
        if (_state == GAME_OVER) {
            sendToClient(static_cast<std::size_t>(poll_index), "ko\n");
            disconnectClient(static_cast<std::size_t>(poll_index));
            return;
        }
        if (_teamsRest.find(teamName) == _teamsRest.end()) {
            _clients.at(fd).getReadBuffer().clear();
            sendToClient(static_cast<std::size_t>(poll_index), "Team not found");
            disconnectClient(static_cast<std::size_t>(poll_index));
            return;
        }
        if (_teamsRest[teamName] < 1) {
            _clients.at(fd).getReadBuffer().clear();
            sendToClient(static_cast<std::size_t>(poll_index), "ko\n");
            disconnectClient(static_cast<std::size_t>(poll_index));
            return;
        }
        _clients.at(fd).setTeamName(teamName);
        _clients.at(fd).setPlayerId(_nextPlayerId++);
        GameEgg *egg = _game.findEggForTeam(teamName);
        if (egg == nullptr) {
            _clients.at(fd).getReadBuffer().clear();
            sendToClient(static_cast<std::size_t>(poll_index), "ko\n");
            disconnectClient(static_cast<std::size_t>(poll_index));
            return;
        }
        int eggId = egg->id;
        _clients.at(fd).getPlayer().setPosition(egg->x, egg->y);
        _game.removeEgg(eggId);
        notifyEggConnection(eggId);
        _teamsRest[teamName]--;
        sendToClient(static_cast<std::size_t>(poll_index),
            std::to_string(_teamsRest[teamName]) + "\n");
        sendToClient(static_cast<std::size_t>(poll_index),
            std::to_string(_game.getWidth()) + " "
            + std::to_string(_game.getHeight()) + "\n");
        notifyPlayerConnection(poll_index);
        scheduleGameEvent(126, "hunger "
            + std::to_string(_clients.at(fd).getPlayerId()));
}

void Server::processClientSocket(int poll_index) {
    try {
        if (_poll.getFds()[poll_index].revents & POLLIN) {
            _clients.at(_poll.getFds()[poll_index].fd).readFromClient();
            processClientCommands(poll_index);
        }
        if (_poll.getFds()[poll_index].revents & POLLOUT) {
            _clients.at(_poll.getFds()[poll_index].fd).writeToClient();
            _poll.getFds()[poll_index].events = _clients.at(_poll.getFds()[poll_index].fd).getWriteBuffer().size() != 0 ? POLLIN | POLLOUT : POLLIN;
        }
        if (_clients.at(_poll.getFds()[poll_index].fd).getShouldDisconnect() && _clients.at(_poll.getFds()[poll_index].fd).getWriteBuffer().empty())
            clientDeconnection(_poll.getFds()[poll_index].fd);
    } catch(const std::exception& e) {
        std::cout << "client error: " << e.what() << std::endl;
        clientDeconnection(_poll.getFds()[poll_index].fd);
    }
}

void Server::processClientCommands(int poll_index) {
    int fd = _poll.getFds()[poll_index].fd;
    std::size_t pos = _clients.at(fd).getReadBuffer().find("\n");    
    while (pos != std::string::npos && !_clients.at(fd).getReadBuffer().empty()) {
        std::string commandStr = _clients.at(fd).getReadBuffer().substr(0, pos);
        _clients.at(fd).getReadBuffer().erase(0, pos + 1);
        pos = _clients.at(fd).getReadBuffer().find("\n");
        if (_clients.at(fd).getTeamName().empty()) {
            sendTeamNumber(poll_index, commandStr);
            continue;
        }
        if (_state == GAME_OVER
            && _clients.at(fd).getTeamName() != "GRAPHIC") {
            sendToClient(static_cast<std::size_t>(poll_index), "ko\n");
            continue;
        }
        if (_clients.at(fd).getTeamName() != "GRAPHIC" && _clients.at(fd).getActionNumber() >= 10) {
            sendToClient(static_cast<std::size_t>(poll_index), "ko\n");
            continue;
        }
        std::size_t argsPos = commandStr.find(" ");
        std::string commandName;
        if (argsPos != std::string::npos) {
            commandName = commandStr.substr(0, argsPos);
            commandStr.erase(0, argsPos + 1);
        } else {
            commandName = commandStr;
            commandStr = "";
        }
        std::unique_ptr<ICommand> command = CommandFactory::createCommand(commandName);
        if (command == nullptr) {
            sendToClient(poll_index,
                _clients.at(fd).getTeamName() == "GRAPHIC" ? "suc\n" : "ko\n");
            continue;
        }
        bool guiCommand = commandName == "msz" || commandName == "bct"
            || commandName == "mct" || commandName == "tna"
            || commandName == "ppo" || commandName == "plv"
            || commandName == "pin" || commandName == "sgt"
            || commandName == "sst";
        if ((_clients.at(fd).getTeamName() == "GRAPHIC") != guiCommand) {
            sendToClient(poll_index,
                _clients.at(fd).getTeamName() == "GRAPHIC" ? "suc\n" : "ko\n");
            continue;
        }
        if (_state == GAME_OVER) {
            if (commandName == "sst") {
                sendToClient(poll_index, "sbp\n");
                continue;
            }
            command->exec(commandStr, *this, poll_index, _game);
            continue;
        }
        if (_clients.at(fd).getTeamName() != "GRAPHIC"
            && _clients.at(fd).isIncanting()) {
            continue;
        }
        int cyclesTime = _clients.at(fd).getActionTime()
            + (command->getCycleNumber() * 1000) / _timeUnit;
        _timeline.addAction(std::chrono::system_clock::now() + std::chrono::milliseconds(cyclesTime), command, commandStr, poll_index);
        _clients.at(fd).setActionTime(cyclesTime);
        _clients.at(fd).setActionNumber(_clients.at(fd).getActionNumber() + 1);
    }
}

void Server::sendToClient(std::size_t poll_index, std::string str) {
    int fd = _poll.getFds()[poll_index].fd;
    if (_clients.find(fd) == _clients.end()) {
        throw ServerException("Invalid fd");
    } else {
        _clients.at(fd).addBuffer(str);
        _poll.getFds()[poll_index].events = POLLIN | POLLOUT;
    }
}

void Server::sendToGraphics(std::string str) {
    for (pollfd &pfd : _poll.getFds()) {
        int fd = pfd.fd;

        if (_clients.find(fd) != _clients.end() && _clients.at(fd).getTeamName() == "GRAPHIC") {
            _clients.at(fd).addBuffer(str);
            pfd.events = POLLIN | POLLOUT;
        }
    }
}

void Server::queueGraphicsEvent(const std::string &event)
{
    std::unique_ptr<ICommand> command = std::make_unique<GraphicsEventCommand>();

    _timeline.addAction(std::chrono::system_clock::now(), command,
        event, -1);
}

void Server::scheduleGameEvent(int cycles, const std::string &event)
{
    if (_state == GAME_OVER)
        return;
    std::unique_ptr<ICommand> command = std::make_unique<GameEventCommand>();
    int delay = (cycles * 1000) / _timeUnit;

    _timeline.addAction(std::chrono::system_clock::now() + std::chrono::milliseconds(delay), command, event, -1);
}

int Server::findPollIndexByPlayerId(int playerId)
{
    for (std::size_t index = 1; index < _poll.getFds().size(); index++) {
        int fd = _poll.getFds()[index].fd;
        auto client = _clients.find(fd);
        if (client != _clients.end()
            && client->second.getPlayerId() == playerId)
            return static_cast<int>(index);
    }
    return -1;
}

void Server::executeGameEvent(const std::string &args)
{
    std::istringstream input(args);
    std::string event;

    input >> event;
    if (event == "hunger") {
        int playerId = -1;
        if (input >> playerId)
            consumeFood(playerId);
    } else if (event == "respawn") {
        respawnResources();
    } else if (event == "incantation") {
        std::string rest;
        std::getline(input, rest);
        if (!rest.empty() && rest[0] == ' ')
            rest.erase(0, 1);
        finishIncantation(rest);
    } else if (event == "stop") {
        _isOn = false;
    }
}

void Server::consumeFood(int playerId)
{
    int pollIndex = findPollIndexByPlayerId(playerId);
    if (pollIndex < 0)
        return;
    int fd = _poll.getFds()[pollIndex].fd;
    Inventory &inventory = _clients.at(fd).getPlayer().getInventoryRef();

    if (!inventory.removeRessources(Ressources::Food, 1)) {
        sendToClient(pollIndex, "dead\n");
        _clients.at(fd).setShouldDisconnect(true);
        return;
    }
    notifyPlayerInventory(pollIndex);
    scheduleGameEvent(126, "hunger " + std::to_string(playerId));
}

void Server::respawnResources()
{
    for (const auto &position : _game.respawnResources())
        queueGraphicsEvent(formatTile(position.first, position.second));
    if (_state == RUNNING)
        scheduleGameEvent(20, "respawn");
}

bool Server::parseResource(const std::string &name, Ressources &resource)
{
    static const std::array<std::string, 7> names = {
        "food", "linemate", "deraumere", "sibur", "mendiane",
        "phiras", "thystame"
    };

    for (std::size_t index = 0; index < names.size(); index++) {
        if (name == names[index]) {
            resource = static_cast<Ressources>(index);
            return true;
        }
    }
    return false;
}


void Server::finishIncantation(const std::string &args)
{
    static const std::array<int, 7> requiredPlayers =
        {1, 2, 2, 4, 4, 6, 6};
    static const std::array<std::array<int, 6>, 7> requiredResources = {{
        {{1, 0, 0, 0, 0, 0}}, {{1, 1, 1, 0, 0, 0}},
        {{2, 0, 1, 0, 2, 0}}, {{1, 1, 2, 0, 1, 0}},
        {{1, 2, 1, 3, 0, 0}}, {{1, 2, 3, 0, 1, 0}},
        {{2, 2, 2, 2, 2, 1}}
    }};
    std::istringstream input(args);
    std::size_t x = 0;
    std::size_t y = 0;
    int level = 0;
    std::vector<int> participants;
    int playerId = -1;
    std::string teamName;
    input >> x >> y >> level;
    while (input >> playerId)
        participants.push_back(playerId);
    bool success = level >= 1 && level < 8
        && static_cast<int>(participants.size()) >= requiredPlayers[level - 1];
    Inventory &tile = _game.square(x, y).getResources();
    for (int resource = Ressources::Linemate;
        success && resource <= Ressources::Thystame; resource++) {
        if (tile.getRessourcesQuantity(static_cast<Ressources>(resource))
            < requiredResources[level - 1][resource - 1])
            success = false;
    }
    for (int id : participants) {
        int index = findPollIndexByPlayerId(id);
        if (index < 0) {
            success = false;
            continue;
        }
        Player &player = _clients.at(_poll.getFds()[index].fd).getPlayer();
        if (player.getPosition() != std::make_pair(x, y)
            || player.getLevel() != level)
            success = false;
    }
    if (success) {
        for (int resource = Ressources::Linemate;
            resource <= Ressources::Thystame; resource++)
            tile.removeRessources(static_cast<Ressources>(resource),
                requiredResources[level - 1][resource - 1]);
    }
    for (int id : participants) {
        int index = findPollIndexByPlayerId(id);
        if (index < 0)
            continue;
        int fd = _poll.getFds()[index].fd;
        Client &client = _clients.at(fd);
        client.setIncanting(false);
        if (success) {
            teamName = client.getTeamName();
            client.getPlayer().setLevel(level + 1);
            sendToClient(index, "Current level: "
                + std::to_string(level + 1) + "\n");
            notifyPlayerLevel(index);
        } else {
            sendToClient(index, "ko\n");
        }
    }
    notifyIncantationEnd(x, y, success);
    if (success)
        queueGraphicsEvent(formatTile(x, y));
    if (success)
        checkVictory(teamName);
}

void Server::checkVictory(const std::string &teamName)
{
    int winners = 0;
    for (auto &entry : _clients) {
        if (entry.second.getTeamName() == teamName
            && entry.second.getPlayer().getLevel() >= 8)
            winners++;
    }
    if (winners >= 6 && _state == RUNNING)
        enterGameOver(teamName);
}

void Server::enterGameOver(const std::string &teamName)
{
    _state = GAME_OVER;
    _winnerTeam = teamName;
    notifyGameEnd(teamName);
    std::cout << "\n" << TerminalStyle::boldYellow()
        << "========================================\n"
        << "               GAME OVER\n"
        << "========================================\n"
        << "  Winner: " << teamName << "\n"
        << "========================================"
        << TerminalStyle::reset() << "\n"
        << TerminalStyle::dim()
        << "  Server online. Press Ctrl+C to stop it."
        << TerminalStyle::reset() << "\n\n";
}

void Server::notifyPlayerConnection(int pollIndex)
{
    if (pollIndex < 0
        || static_cast<std::size_t>(pollIndex) >= _poll.getFds().size())
        return;
    int fd = _poll.getFds()[pollIndex].fd;
    if (_clients.find(fd) == _clients.end()
        || _clients.at(fd).getPlayerId() < 0)
        return;
    Client &client = _clients.at(fd);
    Player &player = client.getPlayer();
    std::pair<std::size_t, std::size_t> position = player.getPosition();

    queueGraphicsEvent("pnw #" + std::to_string(client.getPlayerId()) + " "
        + std::to_string(position.first) + " "
        + std::to_string(position.second) + " "
        + std::to_string(protocolOrientation(player.getDirection())) + " "
        + std::to_string(player.getLevel()) + " "
        + client.getTeamName() + "\n");
}

void Server::notifyPlayerPosition(int pollIndex)
{
    int playerId = getPlayerId(pollIndex);
    if (playerId < 0)
        return;
    int fd = _poll.getFds()[pollIndex].fd;
    queueGraphicsEvent(formatPlayerPosition(playerId,
        _clients.at(fd).getPlayer()));
}

void Server::notifyPlayerLevel(int pollIndex)
{
    int playerId = getPlayerId(pollIndex);
    if (playerId < 0)
        return;
    int fd = _poll.getFds()[pollIndex].fd;
    queueGraphicsEvent(formatPlayerLevel(playerId,
        _clients.at(fd).getPlayer()));
}

void Server::notifyPlayerInventory(int pollIndex)
{
    int playerId = getPlayerId(pollIndex);
    if (playerId < 0)
        return;
    int fd = _poll.getFds()[pollIndex].fd;
    queueGraphicsEvent(formatPlayerInventory(playerId,
        _clients.at(fd).getPlayer()));
}

void Server::notifyPlayerExpulsion(int playerId)
{
    queueGraphicsEvent("pex #" + std::to_string(playerId) + "\n");
}

void Server::notifyPlayerBroadcast(int pollIndex, const std::string &message)
{
    int playerId = getPlayerId(pollIndex);
    if (playerId >= 0)
        queueGraphicsEvent("pbc #" + std::to_string(playerId) + " "
            + cleanProtocalMessage(message) + "\n");
}

void Server::notifyIncantationStart(std::size_t x, std::size_t y, int level,
    const std::vector<int> &playerIds)
{
    std::string event = "pic " + std::to_string(x) + " "
        + std::to_string(y) + " " + std::to_string(level);

    for (int playerId : playerIds)
        event += " #" + std::to_string(playerId);
    queueGraphicsEvent(event + "\n");
}

void Server::notifyIncantationEnd(std::size_t x, std::size_t y, bool success)
{
    queueGraphicsEvent("pie " + std::to_string(x) + " "
        + std::to_string(y) + " " + (success ? "1\n" : "0\n"));
}

void Server::notifyPlayerFork(int pollIndex)
{
    int playerId = getPlayerId(pollIndex);
    if (playerId >= 0)
        queueGraphicsEvent("pfk #" + std::to_string(playerId) + "\n");
}

void Server::notifyResourceDrop(int pollIndex, Ressources resource)
{
    int playerId = getPlayerId(pollIndex);
    if (playerId < 0)
        return;
    queueGraphicsEvent("pdr #" + std::to_string(playerId) + " "
        + std::to_string(static_cast<int>(resource)) + "\n");
    notifyPlayerInventory(pollIndex);
    int fd = _poll.getFds()[pollIndex].fd;
    std::pair<std::size_t, std::size_t> position =
        _clients.at(fd).getPlayer().getPosition();
    queueGraphicsEvent(formatTile(position.first, position.second));
}

void Server::notifyResourceCollect(int pollIndex, Ressources resource)
{
    int playerId = getPlayerId(pollIndex);
    if (playerId < 0)
        return;
    queueGraphicsEvent("pgt #" + std::to_string(playerId) + " "
        + std::to_string(static_cast<int>(resource)) + "\n");
    notifyPlayerInventory(pollIndex);
    int fd = _poll.getFds()[pollIndex].fd;
    std::pair<std::size_t, std::size_t> position =
        _clients.at(fd).getPlayer().getPosition();
    queueGraphicsEvent(formatTile(position.first, position.second));
}

void Server::notifyPlayerDeath(int playerId)
{
    queueGraphicsEvent("pdi #" + std::to_string(playerId) + "\n");
}

void Server::notifyEggPutGround(int eggId, int playerId, std::size_t x,
    std::size_t y)
{
    queueGraphicsEvent("enw #" + std::to_string(eggId) + " #"
        + std::to_string(playerId) + " " + std::to_string(x) + " "
        + std::to_string(y) + "\n");
}

void Server::notifyEggConnection(int eggId)
{
    queueGraphicsEvent("ebo #" + std::to_string(eggId) + "\n");
}

void Server::notifyEggDeath(int eggId)
{
    queueGraphicsEvent("edi #" + std::to_string(eggId) + "\n");
}

void Server::notifyGameEnd(const std::string &teamName)
{
    queueGraphicsEvent("seg " + teamName + "\n");
}

void Server::notifyServerMessage(const std::string &message)
{
    queueGraphicsEvent("smg " + cleanProtocalMessage(message) + "\n");
}

void Server::executeGuiCommand(const std::string &command,
    const std::string &args, int pollIndex)
{
    if (pollIndex < 0 || static_cast<std::size_t>(pollIndex) >= _poll.getFds().size())
        return;
    int fd = _poll.getFds()[pollIndex].fd;
    if (_clients.find(fd) == _clients.end()
        || _clients.at(fd).getTeamName() != "GRAPHIC")
        return;
    std::istringstream input(args);
    std::string extra;

    if (command == "msz") {
        if (input >> extra) {
            sendToClient(pollIndex, "sbp\n");
            return;
        }
        sendToClient(pollIndex, "msz " + std::to_string(_game.getWidth())
            + " " + std::to_string(_game.getHeight()) + "\n");
        return;
    }
    if (command == "bct") {
        int x = 0;
        int y = 0;
        if (!(input >> x >> y) || input >> extra || x < 0 || y < 0
            || static_cast<std::size_t>(x) >= _game.getWidth()
            || static_cast<std::size_t>(y) >= _game.getHeight()) {
            sendToClient(pollIndex, "sbp\n");
            return;
        }
        sendToClient(pollIndex, formatTile(x, y));
        return;
    }
    if (command == "mct") {
        if (input >> extra) {
            sendToClient(pollIndex, "sbp\n");
            return;
        }
        for (std::size_t y = 0; y < _game.getHeight(); y++) {
            for (std::size_t x = 0; x < _game.getWidth(); x++) {
                sendToClient(pollIndex, formatTile(x, y));
            }
        }
        return;
    }
    if (command == "tna") {
        if (input >> extra) {
            sendToClient(pollIndex, "sbp\n");
            return;
        }
        for (const auto &team : _teamsRest)
            sendToClient(pollIndex, "tna " + team.first + "\n");
        return;
    }
    if (command == "sgt") {
        if (input >> extra) {
            sendToClient(pollIndex, "sbp\n");
            return;
        }
        sendToClient(pollIndex, "sgt " + std::to_string(_timeUnit) + "\n");
        return;
    }
    if (command == "sst") {
        int timeUnit = 0;
        if (!(input >> timeUnit) || input >> extra
            || timeUnit < ServerArgs::MIN_FREQUENCY
            || timeUnit > ServerArgs::MAX_FREQUENCY) {
            sendToClient(pollIndex, "sbp\n");
            return;
        }
        _timeUnit = timeUnit;
        queueGraphicsEvent("sst " + std::to_string(_timeUnit) + "\n");
        return;
    }

    std::string playerArgument;
    if (!(input >> playerArgument) || input >> extra) {
        sendToClient(pollIndex, "sbp\n");
        return;
    }
    int playerId = -1;
    try {
        std::size_t parsed = 0;
        std::string id = stripPlayerPrefix(playerArgument);
        playerId = std::stoi(id, &parsed);
        if (parsed != id.size())
            throw std::invalid_argument("invalid player id");
    } catch (const std::exception &) {
        sendToClient(pollIndex, "sbp\n");
        return;
    }
    Client *client = nullptr;
    for (auto &entry : _clients) {
        if (entry.second.getPlayerId() == playerId) {
            client = &entry.second;
            break;
        }
    }
    if (client == nullptr) {
        sendToClient(pollIndex, "sbp\n");
        return;
    }
    Player &player = client->getPlayer();
    if (command == "ppo") {
        sendToClient(pollIndex, formatPlayerPosition(playerId, player));
    } else if (command == "plv") {
        sendToClient(pollIndex, formatPlayerLevel(playerId, player));
    } else if (command == "pin") {
        sendToClient(pollIndex, formatPlayerInventory(playerId, player));
    }
}

void Server::disconnectClient(std::size_t poll_index) {
    int fd = _poll.getFds()[poll_index].fd;
    if (_clients.find(fd) == _clients.end()) {
        throw ServerException("Invalid index");
    } else {
        _clients.at(fd).setShouldDisconnect(true);
        _poll.getFds()[poll_index].events = POLLOUT;
    }
}

void Server::clientDeconnection(int fd) {
    if (_clients.find(fd) == _clients.end())
        return;
    std::cout << "Client quit: fd=" << fd;
    if (_clients.at(fd).getPlayerId() >= 0)
        std::cout << " player=#" << _clients.at(fd).getPlayerId();
    if (!_clients.at(fd).getTeamName().empty())
        std::cout << " team=" << _clients.at(fd).getTeamName();
    std::cout << std::endl;
    if (_state == RUNNING
        && !_clients.at(fd).getTeamName().empty()
        && _clients.at(fd).getTeamName() != "GRAPHIC") {
        if (_clients.at(fd).getPlayerId() >= 0) {
            notifyPlayerDeath(_clients.at(fd).getPlayerId());
            Player &player = _clients.at(fd).getPlayer();
            const std::pair<std::size_t, std::size_t> position =
                player.getPosition();
            int eggId = _game.addEgg(_clients.at(fd).getPlayerId(),
                position.first, position.second,
                _clients.at(fd).getTeamName());
            notifyEggPutGround(eggId, _clients.at(fd).getPlayerId(),
                position.first, position.second);
        }
        _teamsRest[_clients.at(fd).getTeamName()]++;
    } else if (_clients.at(fd).getTeamName() == "GRAPHIC") {
        for (std::vector<int>::iterator iterator = _graphicsFds.begin(); iterator != _graphicsFds.end(); ++iterator) {
            if (*iterator == fd) {
                _graphicsFds.erase(iterator);
                break;
            }
        }
    }
    removeClient(_clients.at(fd).getFd());
}

std::map<int, Client>& Server::getClients() {
    return _clients;
}

std::map<std::string, int>& Server::getTeams() {
    return _teamsRest;
}

int Server::getPlayerId(int pollIndex)
{
    if (pollIndex < 0
        || static_cast<std::size_t>(pollIndex) >= _poll.getFds().size())
        return -1;
    int fd = _poll.getFds()[pollIndex].fd;
    auto client = _clients.find(fd);

    if (client == _clients.end())
        return -1;
    return client->second.getPlayerId();
}

Poll& Server::getPoll() {
    return _poll;
}

Server::ServerException::ServerException(const std::string &msg) : _msg(msg) {}

const char *Server::ServerException::what() const noexcept {
    return _msg.c_str();
}
