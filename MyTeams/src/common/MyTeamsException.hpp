#pragma once

#include <exception>
#include <string>

class MyTeamsException : public std::exception {
public:
    explicit MyTeamsException(const std::string &msg) : _msg(msg) {}
    const char *what() const noexcept override { return _msg.c_str(); }
private:
    std::string _msg;
};

class NotFoundException      : public MyTeamsException { using MyTeamsException::MyTeamsException; };
class AlreadyExistsException : public MyTeamsException { using MyTeamsException::MyTeamsException; };
class UnauthorizedException  : public MyTeamsException { using MyTeamsException::MyTeamsException; };
class SerializationException : public MyTeamsException { using MyTeamsException::MyTeamsException; };
