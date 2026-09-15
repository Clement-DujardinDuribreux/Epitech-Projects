/*
** EPITECH PROJECT, 2026
** ForwardCommand.hpp
** File description:
** ForwardCommand class definition
*/

#ifndef FORWARD
    #define FORWARD

    #include <iostream>
    #include "commands/ACommand.hpp"
    #include "Server.hpp"

/**
 * @class ForwardCommand
 * @brief Moves an AI player one tile forward.
 */
class ForwardCommand : public ACommand {
    public:
        /**
         * @brief Builds a Forward command with a cost of 7 cycles.
         */
        ForwardCommand() : ACommand(7) {}
        ~ForwardCommand() = default;
        /**
         * @brief Executes the movement and notifies graphical clients.
         * @param args Unused command arguments.
         * @param server Server executing the command.
         * @param poll_index Poll index of the source AI.
         * @param game Mutable game state used for map dimensions.
         */
        void exec([[maybe_unused]]std::string args, Server &server, int poll_index, [[maybe_unused]]Game& game) override {
            int fd = server.getPoll().getFds()[poll_index].fd;
            server.getClients().at(fd).getPlayer().goForward(game.getWidth(), game.getHeight());
            server.sendToClient(poll_index, "ok\n");
            server.notifyPlayerPosition(poll_index);
        }
};

#endif
