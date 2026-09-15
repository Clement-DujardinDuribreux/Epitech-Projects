#pragma once

#include <string>

class Client {
public:
    Client(const std::string &ip, int port);
    ~Client();

    void run();

private:
    void sendLine(const std::string &line);
    void handleServerMessage(const std::string &msg);

    int         _fd;
    std::string _readBuffer;
    std::string _writeBuffer;
};
