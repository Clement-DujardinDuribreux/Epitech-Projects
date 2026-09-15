/*
** EPITECH PROJECT, 2026
** Socket.hpp
** File description:
** Socket class definition
*/

#ifndef SOCKET
    #define SOCKET

    #include <iostream>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>

/**
 * @class Socket
 * @brief RAII wrapper around a TCP socket file descriptor.
 */
class Socket {
    private:
        int _fd;

    public:
        /**
         * @brief Creates, binds, and listens on a server socket.
         * @param port Local TCP port to bind.
         * @throws SocketException when socket creation, bind, or listen fails.
         */
        Socket(int port);
        /**
         * @brief Creates and connects a client socket.
         * @param ip IPv4 address to connect to.
         * @param port Remote TCP port.
         * @throws SocketException when creation, address conversion, or connect fails.
         */
        Socket(std::string ip, int port);
        /**
         * @brief Closes the socket file descriptor when valid.
         */
        ~Socket();

        /**
         * @brief Gets the wrapped file descriptor.
         * @return Socket file descriptor.
         */
        int getFd() const;

        /**
         * @class SocketException
         * @brief Reports socket setup and connection failures.
         */
        class SocketException : public std::exception {
            public:
                /**
                 * @brief Builds an exception with a diagnostic message.
                 * @param msg Message returned by what().
                 */
                SocketException(const std::string &msg);
                ~SocketException() = default;
                /**
                 * @brief Gets the diagnostic message.
                 * @return Null-terminated message string.
                 */
                const char *what() const noexcept override;

            private:
                std::string _msg;
        };

};

#endif
