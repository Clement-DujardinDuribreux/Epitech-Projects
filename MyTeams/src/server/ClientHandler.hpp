#pragma once

#include <string>

class ClientHandler {
public:
    explicit ClientHandler(int fd);
    ~ClientHandler();

    int         fd()         const { return _fd; }
    bool        loggedIn()   const { return !_userUuid.empty(); }
    const std::string &userUuid() const { return _userUuid; }
    void        setUserUuid(const std::string &uuid) { _userUuid = uuid; }

    std::string getUserName() { return _userName; }
    void        setUsername(const std::string &name) { _userName = name; }

    void        appendToWriteBuffer(const std::string &data);
    bool        hasDataToWrite()  const { return !_writeBuffer.empty(); }

    std::string readLine();
    void        flushWrites();

    bool        dead() const { return _dead; }
    void        markDead() { _dead = true; }

    std::string getCurrentTeam() const { return _currentTeam; }
    std::string getCurrentChannel() const { return _currentChannel; }
    std::string getCurrentThread() const { return _currentThread; }

    void setCurrentTeam(const std::string &team) { _currentTeam = team; }
    void setCurrentChannel(const std::string &channel) { _currentChannel = channel; }
    void setCurrentThread(const std::string &thread) { _currentThread = thread; }

private:
    int         _fd;
    std::string _userUuid;
    std::string _userName;
    std::string _readBuffer;
    std::string _writeBuffer;
    bool        _dead;

    std::string _currentTeam;
    std::string _currentChannel;
    std::string _currentThread;
};
