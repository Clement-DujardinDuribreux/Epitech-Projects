#include "Client.hpp"
#include "../common/MyTeamsException.hpp"
extern "C" {
#include "../../libs/myteams/logging_client.h"
}

#include <sstream>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

Client::Client(const std::string &ip, int port)
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0)
        throw MyTeamsException(std::string("socket: ") + strerror(errno));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0)
        throw MyTeamsException("invalid ip address");

    if (connect(_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
        throw MyTeamsException(std::string("connect: ") + strerror(errno));
}

Client::~Client()
{
    close(_fd);
}

void Client::run()
{
    while (true) {
        pollfd fds[2];
        fds[0] = {STDIN_FILENO, POLLIN, 0};
        fds[1] = {_fd, static_cast<short>(POLLIN | (_writeBuffer.empty() ? 0 : POLLOUT)), 0};

        if (poll(fds, 2, -1) < 0)
            break;

        if (fds[0].revents & POLLIN) {
            std::string line;
            if (!std::getline(std::cin, line))
                break;
            sendLine(line);
        }

        if (fds[1].revents & POLLOUT && !_writeBuffer.empty()) {
            ssize_t n = send(_fd, _writeBuffer.c_str(), _writeBuffer.size(), MSG_DONTWAIT);
            if (n > 0)
                _writeBuffer.erase(0, n);
        }

        if (fds[1].revents & POLLIN) {
            char buf[4096];
            ssize_t n = recv(_fd, buf, sizeof(buf), 0);
            if (n <= 0)
                break;
            _readBuffer.append(buf, n);
            auto pos = _readBuffer.find('\n');
            while (pos != std::string::npos) {
                handleServerMessage(_readBuffer.substr(0, pos));
                _readBuffer.erase(0, pos + 1);
                pos = _readBuffer.find('\n');
            }
        }
    }
}

void Client::sendLine(const std::string &line)
{
    _writeBuffer += line + "\n";
}

void Client::handleServerMessage(const std::string &msg)
{
    std::vector<std::string> args;
    std::string token;
    std::istringstream ss(msg);
    int code;
    ss >> code;        

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

    if (code >= 200 && code < 300) {
        if (code == 201) { std::cout << "HELP" << std::endl; }
        else if (code == 220 && args.size() >= 2) { client_event_logged_in(args[0].c_str(), args[1].c_str()); }
        else if (code == 221 && args.size() >= 2) { client_event_logged_out(args[0].c_str(), args[1].c_str()); }
        else if (code == 250 && args.size() >= 3) { client_print_user(args[0].c_str(), args[1].c_str(), std::atoi(args[2].c_str())); }
        else if (code == 251 && args.size() >= 3) { client_print_users(args[0].c_str(), args[1].c_str(), std::atoi(args[2].c_str())); }
        else if (code == 260 && args.size() >= 2) { client_event_private_message_received(args[0].c_str(), args[1].c_str()); }
        else if (code == 261 && args.size() >= 3) { client_private_message_print_messages(args[0].c_str(), (time_t)std::stoll(args[1]), args[2].c_str()); }
        else if (code == 270 && args.size() >= 2) { client_print_subscribed(args[0].c_str(), args[1].c_str()); }
        else if (code == 271 && args.size() >= 2) { client_print_unsubscribed(args[0].c_str(), args[1].c_str()); }
        else if (code == 211 && args.size() >= 5) { client_print_thread(args[0].c_str(), args[1].c_str(), (time_t)std::stoll(args[2]), args[3].c_str(), args[4].c_str()); }
        else if (code == 212 && args.size() >= 3) { client_print_channel(args[0].c_str(), args[1].c_str(), args[2].c_str()); }
        else if (code == 213 && args.size() >= 3) { client_print_team(args[0].c_str(), args[1].c_str(), args[2].c_str()); }
        else if (code == 281 && args.size() >= 4) { client_thread_print_replies(args[0].c_str(), args[1].c_str(), (time_t)std::stoll(args[2]), args[3].c_str()); }
        else if (code == 282 && args.size() >= 5) { client_channel_print_threads(args[0].c_str(), args[1].c_str(), (time_t)std::stoll(args[2]), args[3].c_str(), args[4].c_str()); }
        else if (code == 283 && args.size() >= 3) { client_team_print_channels(args[0].c_str(), args[1].c_str(), args[2].c_str()); }
        else if (code == 284 && args.size() >= 3) { client_print_teams(args[0].c_str(), args[1].c_str(), args[2].c_str()); }
        else if (code == 291 && args.size() >= 4) { client_print_reply_created(args[0].c_str(), args[1].c_str(), (time_t)std::stoll(args[2].c_str()), args[3].c_str()); }
        else if (code == 292 && args.size() >= 5) { client_print_thread_created(args[0].c_str(), args[1].c_str(), (time_t)std::stoll(args[2]), args[3].c_str(), args[4].c_str()); }
        else if (code == 293 && args.size() >= 3) { client_print_channel_created(args[0].c_str(), args[1].c_str(), args[2].c_str()); }
        else if (code == 294 && args.size() >= 3) { client_print_team_created(args[0].c_str(), args[1].c_str(), args[2].c_str()); }

        else if (code == 295 && args.size() >= 4) { client_event_thread_reply_received(args[0].c_str(), args[1].c_str(), args[2].c_str(), args[3].c_str()); }
        else if (code == 296 && args.size() >= 5) { client_event_thread_created(args[0].c_str(), args[1].c_str(), (time_t)std::stoll(args[2].c_str()), args[3].c_str(), args[4].c_str()); }
        else if (code == 297 && args.size() >= 3) { client_event_channel_created(args[0].c_str(), args[1].c_str(), args[2].c_str()); }
        else if (code == 298 && args.size() >= 3) { client_event_team_created(args[0].c_str(), args[1].c_str(), args[2].c_str()); }

        else { std::cout << msg << std::endl; }
    } else if (code >= 500 && code < 600) {
        if (code == 501) { client_error_unauthorized(); }
        else if (code == 502) { std::cout << "Bad Arguments" << std::endl; }
        else if (code == 503 && args.size() >= 1) { client_error_unknown_user(args[0].c_str()); }
        else if (code == 504 && args.size() >= 1) { client_error_unknown_team(args[0].c_str()); }
        else if (code == 505 && args.size() >= 1) { client_error_unknown_channel(args[0].c_str()); }
        else if (code == 506 && args.size() >= 1) { client_error_unknown_thread(args[0].c_str()); }
        else if (code == 507) { client_error_already_exist(); }
        else { std::cout << msg << std::endl; }
    } else
        std::cout << msg << std::endl;
}
