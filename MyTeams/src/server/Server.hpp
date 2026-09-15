#pragma once

#include "ClientHandler.hpp"
#include "../common/DataStore.hpp"

#include <memory>
#include <vector>

class Server {
public:
    explicit Server(int port);
    ~Server();

    void run();
    void saveAndExit();

    static Server *instance;
    static void    onSigint(int);

private:
    void acceptNewClient();
    void handleClient(ClientHandler &client);
    void handleCommands(ClientHandler &client, std::string line);
    void disconnectDead();
    void broadcastToSubscribers(const std::string &teamUuid, const std::string &message);
    void broadcastToLoggedIn(const std::string &message);

    int                                    _listenFd;
    std::vector<std::unique_ptr<ClientHandler>> _clients;
    DataStore                              _store;

    static constexpr const char *SAVE_DIR = ".";


    /* COMMANDS */
    void help(ClientHandler &client, std::vector<std::string> &args);
    void login(ClientHandler &client, std::vector<std::string> &args);
    void logout(ClientHandler &client, std::vector<std::string> &args);
    void users(ClientHandler &client, std::vector<std::string> &args);
    void user(ClientHandler &client, std::vector<std::string> &args);
    void send(ClientHandler &client, std::vector<std::string> &args);
    void messages(ClientHandler &client, std::vector<std::string> &args);
    void subscribe(ClientHandler &client, std::vector<std::string> &args);
    void subscribed(ClientHandler &client, std::vector<std::string> &args);
    void unsubscribe(ClientHandler &client, std::vector<std::string> &args);
    void use(ClientHandler &client, std::vector<std::string> &args);
    void create(ClientHandler &client, std::vector<std::string> &args);
    void list(ClientHandler &client, std::vector<std::string> &args);
    void info(ClientHandler &client, std::vector<std::string> &args);
};
