#include "Server.hpp"
#include "../common/MyTeamsException.hpp"
#include "../common/Uuid.hpp"
extern "C" {
#include "logging_server.h"
}

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>

Server *Server::instance = nullptr;

void Server::onSigint(int)
{
    if (instance)
        instance->saveAndExit();
    exit(0);
}

Server::Server(int port)
{
    _listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_listenFd < 0)
        throw MyTeamsException(std::string("socket: ") + strerror(errno));

    int opt = 1;
    setsockopt(_listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(port);

    if (bind(_listenFd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
        throw MyTeamsException(std::string("bind: ") + strerror(errno));

    if (listen(_listenFd, SOMAXCONN) < 0)
        throw MyTeamsException(std::string("listen: ") + strerror(errno));

    _store.load(SAVE_DIR, [](const User &u) {
        server_event_user_loaded(u.uuid().c_str(), u.name().c_str());
    });

    instance = this;
}

Server::~Server()
{
    close(_listenFd);
}

void Server::run()
{
    while (true) {
        std::vector<pollfd> fds;
        fds.push_back({_listenFd, POLLIN, 0});

        for (const auto &client : _clients) {
            short events = POLLIN;
            if (client->hasDataToWrite())
                events |= POLLOUT;
            fds.push_back({client->fd(), events, 0});
        }

        int ready = poll(fds.data(), fds.size(), -1);
        if (ready < 0) {
            if (errno == EINTR)
                continue;
            throw MyTeamsException(std::string("poll: ") + strerror(errno));
        }

        if (fds[0].revents & POLLIN)
            acceptNewClient();

        for (size_t i = 1; i < fds.size(); ++i) {
            auto &client = *_clients[i - 1];
            if (fds[i].revents & POLLIN)
                handleClient(client);
            if (fds[i].revents & POLLOUT)
                client.flushWrites();
        }

        disconnectDead();
    }
}

void Server::saveAndExit()
{
    _store.save(SAVE_DIR);
    exit(0);
}

void Server::acceptNewClient()
{
    int fd = accept(_listenFd, nullptr, nullptr);
    if (fd < 0)
        return;
    _clients.push_back(std::make_unique<ClientHandler>(fd));
}

void Server::handleClient(ClientHandler &client)
{
    std::string line = client.readLine();

    if (line.empty())
        return;
    if (line[0] == '/')
        handleCommands(client, line.substr(1));
    else
        client.appendToWriteBuffer("Not a command");
}

void Server::handleCommands(ClientHandler &client, std::string line) {
    std::vector<std::string> args;
    std::istringstream ss(line);
    std::string command;
    std::string token;

    ss >> command;

    while (ss >> token) {
        if (token.front() == '"' && token.back() == '"' && token.size() > 1) {
            args.push_back(token.substr(1, token.size() - 2));
        } else if (token.front() == '"') {
            std::string quoted = token.substr(1);
            std::string next;
            while (ss >> next) {
                if (next.back() == '"') {
                    quoted += " " + next.substr(0, next.size() - 1);
                    break;
                }
                quoted += " " + next;
            }
            args.push_back(quoted);
        } else {
            args.push_back(token);
        }
    }

    std::map<std::string, std::function<void(ClientHandler&, std::vector<std::string>)>> commands = {
        {"help", [this](ClientHandler &c, std::vector<std::string> a) { this->help(c, a); }},
        {"login", [this](ClientHandler &c, std::vector<std::string> a) { this->login(c, a); }},
        {"logout", [this](ClientHandler &c, std::vector<std::string> a) { this->logout(c, a); }},
        {"users", [this](ClientHandler &c, std::vector<std::string> a) { this->users(c, a); }},
        {"user", [this](ClientHandler &c, std::vector<std::string> a) { this->user(c, a); }},
        {"send", [this](ClientHandler &c, std::vector<std::string> a) { this->send(c, a); }},
        {"messages", [this](ClientHandler &c, std::vector<std::string> a) { this->messages(c, a); }},
        {"subscribe", [this](ClientHandler &c, std::vector<std::string> a) { this->subscribe(c, a); }},
        {"subscribed", [this](ClientHandler &c, std::vector<std::string> a) { this->subscribed(c, a); }},
        {"unsubscribe", [this](ClientHandler &c, std::vector<std::string> a) { this->unsubscribe(c, a); }},
        {"use", [this](ClientHandler &c, std::vector<std::string> a) { this->use(c, a); }},
        {"create", [this](ClientHandler &c, std::vector<std::string> a) { this->create(c, a); }},
        {"list", [this](ClientHandler &c, std::vector<std::string> a) { this->list(c, a); }},
        {"info", [this](ClientHandler &c, std::vector<std::string> a) { this->info(c, a); }}
    };

    if (commands.find(command) != commands.end())
        commands[command](client, args);
    else
        client.appendToWriteBuffer("Invalid Command");
}

void Server::disconnectDead()
{
    for (auto it = _clients.begin(); it != _clients.end(); ) {
        if ((*it)->dead()) {
            if ((*it)->loggedIn()) {
                try {
                    auto &user = _store.findUser((*it)->userUuid());
                    user.setConnected(false);
                    server_event_user_logged_out(user.uuid().c_str());
                } catch (const MyTeamsException &) {}
            }
            it = _clients.erase(it);
        } else {
            ++it;
        }
    }
}

void Server::broadcastToLoggedIn(const std::string &message)
{
    for (auto &client : _clients)
        if (client->loggedIn())
            client->appendToWriteBuffer(message);
}

void Server::broadcastToSubscribers(const std::string &teamUuid, const std::string &message)
{
    for (auto &client : _clients) {
        if (client->loggedIn() && _store.isSubscribed(client->userUuid(), teamUuid))
            client->appendToWriteBuffer(message);
    }
}

/* COMMANDS */

void Server::help([[maybe_unused]] ClientHandler &client, [[maybe_unused]] std::vector<std::string> &args) {
    client.appendToWriteBuffer("201");
}

void Server::login([[maybe_unused]] ClientHandler &client, [[maybe_unused]] std::vector<std::string> &args) {
    if (args.empty()) {
        client.appendToWriteBuffer("502");
        return;
    }
    User *usrPtr = nullptr;
    try {
        usrPtr = _store.findUserByName(args[0]);
        if (usrPtr == nullptr)
            throw std::exception();
    }
    catch (std::exception &e) {
        usrPtr = &_store.addUser(args[0]);
        server_event_user_created(usrPtr->uuid().c_str(), usrPtr->name().c_str());
    }
    usrPtr->setConnected(true);
    server_event_user_logged_in(usrPtr->uuid().c_str());
    client.setUsername(usrPtr->name());
    client.setUserUuid(usrPtr->uuid());
    for (auto &Sclient : _clients) {
        if (Sclient.get() == &client || Sclient->loggedIn()) {
            Sclient->appendToWriteBuffer("220 " + usrPtr->uuid() + " " + usrPtr->name());
        }
    }
}

void Server::logout([[maybe_unused]] ClientHandler &client, [[maybe_unused]] std::vector<std::string> &args) {
    if (!client.loggedIn()) {
        client.appendToWriteBuffer("501");
        return;
    }
    auto user = &_store.findUser(client.userUuid());
    user->setConnected(false);
    server_event_user_logged_out(client.userUuid().c_str());
    for (auto &Sclient : _clients) {
        if (Sclient.get() == &client || Sclient->loggedIn()) {
            Sclient->appendToWriteBuffer("221 " + user->uuid() + " " + user->name());
        }
    }
    client.markDead();
}

void Server::users([[maybe_unused]] ClientHandler &client, [[maybe_unused]] std::vector<std::string> &args) {
    if (client.loggedIn()) {
        for (auto [_, u] : _store.users()) {
            client.appendToWriteBuffer("251 " + u.uuid() + " " + u.name() + " " + (u.connected() ? "1" : "0"));
        }
    } else
        client.appendToWriteBuffer("501");
}

void Server::user([[maybe_unused]] ClientHandler &client, [[maybe_unused]] std::vector<std::string> &args) {
    if (args.empty()) {
        client.appendToWriteBuffer("502");
        return;
    }
    if (client.loggedIn()) {
        try {
            User &u = _store.findUser(args[0]);
            client.appendToWriteBuffer("250 " + u.uuid() + " " + u.name() + " " + (u.connected() ? "1" : "0"));
        } catch (NotFoundException &e) {
            client.appendToWriteBuffer("503 " + args[0]);
            return;
        }
    }
    else
        client.appendToWriteBuffer("501");
}

void Server::send([[maybe_unused]] ClientHandler &client, [[maybe_unused]] std::vector<std::string> &args) {
    User usr;
    if (args.empty()) {
        client.appendToWriteBuffer("502");
        return;
    }
    if (client.loggedIn()) {
        try { usr = _store.findUser(args[0]); }
        catch (NotFoundException &e) {
            client.appendToWriteBuffer("503 " + args[0]);
        }
        _store.addPrivateMessage(client.userUuid(), usr.uuid(), args[1]);
        server_event_private_message_sended(client.userUuid().c_str(), args[0].c_str(), args[1].c_str());
        for (auto &Sclient : _clients)
            if (Sclient->userUuid() == args[0])
                Sclient->appendToWriteBuffer("260 " + client.userUuid() + " \"" + args[1] + "\"");
    } else
        client.appendToWriteBuffer("501");
}

void Server::messages([[maybe_unused]] ClientHandler &client, [[maybe_unused]] std::vector<std::string> &args) {
    if (args.empty()) {
        client.appendToWriteBuffer("502");
        return;
    }
    if (client.loggedIn()) {
        try { _store.findUser(args[0]); }
        catch (NotFoundException &e) {
            client.appendToWriteBuffer("503");
            return;
        }
        auto messages = _store.messagesBetween(client.userUuid(), args[0]);
        for (auto &message : messages) {
            client.appendToWriteBuffer("261 " + message.senderUuid() + " " + std::to_string(message.timestamp()) + " \"" + message.body() + "\"");
        }
    } else
        client.appendToWriteBuffer("501");
}

void Server::subscribe([[maybe_unused]] ClientHandler &client, [[maybe_unused]] std::vector<std::string> &args) {
    if (args.empty()) {
        client.appendToWriteBuffer("502");
        return;
    }
    if (client.loggedIn()) {
        try {
            _store.subscribe(client.userUuid(), args[0]);
            server_event_user_subscribed(args[0].c_str(), client.userUuid().c_str());
        } catch (NotFoundException &e) {
            client.appendToWriteBuffer("504 " + args[0]);
            return;
        }
        client.appendToWriteBuffer("270");
    } else
        client.appendToWriteBuffer("501");
}

void Server::subscribed([[maybe_unused]] ClientHandler &client, [[maybe_unused]] std::vector<std::string> &args) {
    if (client.loggedIn()) {
        if (args.empty()) {
            auto teams = _store.subscribedTeamsOf(client.userUuid());
            for (auto &team : teams) {
                client.appendToWriteBuffer("284 " + client.userUuid() + " " + team);
            }
        } else {
            try {
                _store.findTeam(args[0]);
            } catch (NotFoundException &e) {
                client.appendToWriteBuffer("504 " + args[0]);
                return;
            }
            auto subscribers = _store.subscribersOf(args[0]);
            for (auto &subscriber : subscribers) {
                User usr = _store.findUser(subscriber);
                client.appendToWriteBuffer("251 " + subscriber + " \"" + usr.name() + "\" " + (usr.connected() ? "1" : "0"));
            }
        }
    } else
        client.appendToWriteBuffer("501");
}

void Server::unsubscribe([[maybe_unused]] ClientHandler &client, [[maybe_unused]] std::vector<std::string> &args) {
    if (args.empty()) {
        client.appendToWriteBuffer("502");
        return;
    }
    if (client.loggedIn()) {
        try {
            _store.findTeam(args[0]);
        } catch(NotFoundException &e) {
            client.appendToWriteBuffer("504 " + args[0]);
            return;
        }
        _store.unsubscribe(client.userUuid(), args[0]);
        server_event_user_unsubscribed(args[0].c_str(), client.userUuid().c_str());
        client.appendToWriteBuffer("271 " + client.userUuid() + " " + args[0]);
    }
}

void Server::use([[maybe_unused]] ClientHandler &client, [[maybe_unused]] std::vector<std::string> &args) {
    if (!client.loggedIn()) {
        client.appendToWriteBuffer("501");
        return;
    }
    client.setCurrentTeam("");
    client.setCurrentChannel("");
    client.setCurrentThread("");
    if (args.size() > 0) {
        try {
            _store.findTeam(args[0]);
        } catch (NotFoundException &e) {
            client.appendToWriteBuffer("504 " + args[0]);
            return;
        }
        client.setCurrentTeam(args[0]);
    }
    if (args.size() > 1) {
        try {
            _store.findChannel(args[1]);
        } catch (NotFoundException &e) {
            client.appendToWriteBuffer("505 " + args[1]);
            return;
        }
        client.setCurrentChannel(args[1]);
    }
    if (args.size() > 2) {
        try {
            _store.findThread(args[2]);
        } catch (NotFoundException &e) {
            client.appendToWriteBuffer("506 " + args[2]);
            return;
        }
        client.setCurrentThread(args[2]);
    }
    if (!_store.isGoodPath(client.getCurrentTeam(), client.getCurrentChannel(), client.getCurrentThread())) {
        client.setCurrentTeam("");
        client.setCurrentChannel("");
        client.setCurrentThread("");
    }
    client.appendToWriteBuffer("280");
}

void Server::create([[maybe_unused]] ClientHandler &client, [[maybe_unused]] std::vector<std::string> &args) {
    if (args.empty()) {
        client.appendToWriteBuffer("502");
        return;
    }
    if (!client.getCurrentThread().empty()) {
        auto reply = _store.addReply(args[0], client.userUuid(), client.getCurrentThread());
        server_event_reply_created(client.getCurrentThread().c_str(), reply.creatorUuid().c_str(), reply.body().c_str());
        client.appendToWriteBuffer("291 " + client.getCurrentTeam() + " " + client.getCurrentThread() + " " + reply.creatorUuid() + " \"" + args[0] + "\"");
        for (auto &Sclient : _clients)
            Sclient->appendToWriteBuffer("295 " + client.getCurrentTeam() + " " + client.getCurrentThread() + " " + client.userUuid() + " \"" + args[0] + "\"");
    }
    else if (!client.getCurrentChannel().empty()) {
        try { _store.findThreadByName(args[0]); }
        catch (NotFoundException &e) {
            auto thread = _store.addThread(args[0], args[1], client.userUuid(), client.getCurrentChannel());
            server_event_thread_created(client.getCurrentChannel().c_str(), thread.uuid().c_str(), thread.creatorUuid().c_str(), thread.name().c_str(), thread.body().c_str());
            client.appendToWriteBuffer("292 " + thread.uuid() + " " + client.userUuid() + " " + std::to_string(thread.timestamp()) + " \"" + thread.name() + "\" \"" + thread.body() + "\"");
            client.appendToWriteBuffer("296 " + thread.uuid() + " " + client.userUuid() + " " + std::to_string(thread.timestamp()) + " \"" + thread.name() + "\" \"" + thread.body() + "\"");
            return;
        }
        client.appendToWriteBuffer("507");
    }
    else if (!client.getCurrentTeam().empty()) {
        try { _store.findChannelByName(args[0]); }
        catch (NotFoundException &e) {
            auto channel = _store.addChannel(args[0], args[1], client.getCurrentTeam());
            server_event_channel_created(client.getCurrentTeam().c_str(), channel.uuid().c_str(), channel.name().c_str());
            client.appendToWriteBuffer("293 " + channel.uuid() + " \"" + channel.name() + "\" \"" + channel.description() + "\"");
            client.appendToWriteBuffer("297 " + channel.uuid() + " \"" + channel.name() + "\" \"" + channel.description() + "\"");
            return;
        }
        client.appendToWriteBuffer("507");
    }
    else {
        try { _store.findTeamByName(args[0]); }
        catch (NotFoundException &e) {
            auto team = _store.addTeam(args[0], args[1], client.userUuid());
            server_event_team_created(client.getCurrentTeam().c_str(), team.name().c_str(), client.userUuid().c_str());
            client.appendToWriteBuffer("294 " + team.uuid() + " \"" + team.name() + "\" \"" + team.description() + "\"");
            for (auto &Sclient : _clients)
                Sclient->appendToWriteBuffer("298 " + team.uuid() + " \"" + team.name() + "\" \"" + team.description() + "\"");
            return;
        }
        client.appendToWriteBuffer("507");
    }
}

void Server::list([[maybe_unused]] ClientHandler &client, [[maybe_unused]] std::vector<std::string> &args) {
    if (!args.empty()) {
        client.appendToWriteBuffer("502");
        return;
    }
    if (!client.getCurrentThread().empty()) {
        for (auto &reply : _store.repliesForThread(client.getCurrentThread()))
            client.appendToWriteBuffer("281 " + client.getCurrentThread() + " " + reply.creatorUuid() + " " + std::to_string(reply.timestamp()) + " \"" + reply.body() + "\"");
    }
    else if (!client.getCurrentChannel().empty()) {
        for (auto &thread : _store.threadsOfChannel(client.getCurrentChannel()))
            client.appendToWriteBuffer("282 " + thread.uuid() + " " + thread.creatorUuid() + " " + std::to_string(thread.timestamp()) + " \"" + thread.name() + "\" \"" + thread.body() + "\"");
    }
    else if (!client.getCurrentTeam().empty()) {
        for (auto &channel : _store.channelsOfTeam(client.getCurrentTeam()))
            client.appendToWriteBuffer("283 " + channel.uuid() + " \"" + channel.name() + "\" \"" + channel.description() + "\"");

    }
    else {
        for (auto &[_, team] : _store.teams())
        client.appendToWriteBuffer("284 " + team.uuid() + " \"" + team.name() + "\" \"" + team.description() + "\"");
    }
}

void Server::info([[maybe_unused]] ClientHandler &client, [[maybe_unused]] std::vector<std::string> &args) {
    if (!args.empty()) {
        client.appendToWriteBuffer("502");
        return;
    }
    if (!client.getCurrentThread().empty()) {
        auto thread = _store.findThread(client.getCurrentThread());
        client.appendToWriteBuffer("211 " + client.getCurrentThread() + " " + thread.creatorUuid() + " " + std::to_string(thread.timestamp()) + " \"" + thread.name() + "\" \"" + thread.body() + "\"");
    }
    else if (!client.getCurrentChannel().empty()) {
        auto channel = _store.findChannel(client.getCurrentChannel());
        client.appendToWriteBuffer("212 " + client.getCurrentChannel() + " \"" + channel.name() + "\" \"" + channel.description() + "\"");
    }
    else if (!client.getCurrentTeam().empty()) {
        auto team = _store.findTeam(client.getCurrentTeam());
        client.appendToWriteBuffer("213 " + client.getCurrentTeam() + " \"" + team.name() + "\" \"" + team.description() + "\"");
    }
    else {
        client.appendToWriteBuffer("250 " + client.userUuid() + " \"" + client.getUserName() + "\" 1");
    }
}