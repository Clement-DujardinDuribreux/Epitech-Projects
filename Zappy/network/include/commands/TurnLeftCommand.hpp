/*
** EPITECH PROJECT, 2026
** ForwardCommand.hpp
** File description:
** ForwardCommand class definition
*/

#ifndef LEFT
    #define LEFT

    #include "commands/ACommand.hpp"
    #include "Server.hpp"

/**
 * @class LeftCommand
 * @brief Rotates an AI player counter-clockwise.
 */
class LeftCommand : public ACommand {
    public:
        /**
         * @brief Builds a Left command with a cost of 7 cycles.
         */
        LeftCommand() : ACommand(7) {}
        ~LeftCommand() = default;
        /**
         * @brief Executes the rotation and notifies graphical clients.
         * @param args Unused command arguments.
         * @param server Server executing the command.
         * @param poll_index Poll index of the source AI.
         * @param game Unused game state.
         */
        void exec([[maybe_unused]]std::string args, Server &server, int poll_index, [[maybe_unused]]Game& game) override {
            int fd = server.getPoll().getFds()[poll_index].fd;
            server.getClients().at(fd).getPlayer().turnLeft();
            server.sendToClient(poll_index, "ok\n");
            server.notifyPlayerPosition(poll_index);
        }
};

#endif
