/*
** EPITECH PROJECT, 2026
** ForwardCommand.hpp
** File description:
** ForwardCommand class definition
*/

#ifndef CONNECT_NBR
    #define CONNECT_NBR

    #include <iostream>
    #include "commands/ACommand.hpp"
    #include "Server.hpp"

/**
 * @class ConnectNbrCommand
 * @brief Sends the number of remaining connection slots for the AI team.
 */
class ConnectNbrCommand : public ACommand {
    public:
        /**
         * @brief Builds a Connect_nbr command with no execution delay.
         */
        ConnectNbrCommand() : ACommand(0) {}
        ~ConnectNbrCommand() = default;
        /**
         * @brief Sends the remaining slot count to the AI client.
         * @param args Unused command arguments.
         * @param server Server executing the command.
         * @param poll_index Poll index of the source AI.
         * @param game Unused game state.
         */
        void exec([[maybe_unused]]std::string args, Server &server, int poll_index, [[maybe_unused]]Game& game) override {
            int fd = server.getPoll().getFds()[poll_index].fd;
            std::string value = std::to_string(
                server.getTeams()[server.getClients().at(fd).getTeamName()]);
            server.sendToClient(poll_index, value + "\n");
        }
};

#endif
