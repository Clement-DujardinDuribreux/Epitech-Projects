/*
** EPITECH PROJECT, 2026
** ServerArgs.hpp
** File description:
** ServerArgs class definition
*/

#ifndef SERVERARGS
    #define SERVERARGS

    #include <unistd.h>
    #include <iostream>
    #include <vector>

/**
 * @class ServerArgs
 * @brief Parses and stores command-line options for the Zappy server.
 */
class ServerArgs {
    private:
        int _argc;
        char **_argv;

        int _frequency = 0;
        int _port = 0;
        int _width = 0;
        int _height = 0;
        int _clientNumber = 0;
        std::vector<std::string> _teamsName;
        
    public:
        static const int MIN_FREQUENCY = 1;
        static const int MAX_FREQUENCY = 1000;
        static const int MIN_MAP_SIZE = 10;
        static const int MAX_MAP_SIZE = 42;
        static const int MIN_PORT = 1;
        static const int MAX_PORT = 65535;

        /**
         * @brief Parses the server command line.
         * @param argc Argument count received by main().
         * @param argv Argument vector received by main().
         * @throws ServerArgsException when required arguments are invalid.
         */
        ServerArgs(int argc, char **argv);
        ~ServerArgs() = default;

        /**
         * @brief Gets the game frequency.
         * @return Frequency in cycles per second.
         */
        int getFrequency() const;

        /**
         * @brief Gets the listening port.
         * @return TCP port number.
         */
        int getPort() const;

        /**
         * @brief Gets the map width.
         * @return Width in tiles.
         */
        int getWidth() const;

        /**
         * @brief Gets the map height.
         * @return Height in tiles.
         */
        int getHeight() const;

        /**
         * @brief Gets the initial slots per team.
         * @return Client count configured with -c.
         */
        int getClientNumber() const;

        /**
         * @brief Gets all configured team names.
         * @return Team names in command-line order.
         */
        std::vector<std::string> getTeamsName() const;

        /**
         * @class ServerArgsException
         * @brief Reports invalid or missing server command-line arguments.
         */
        class ServerArgsException : public std::exception {
            public:
                /**
                 * @brief Builds an exception with a diagnostic message.
                 * @param msg Message returned by what().
                 */
                ServerArgsException(const std::string &msg);
                ~ServerArgsException() = default;
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
