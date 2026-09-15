/*
** EPITECH PROJECT, 2026
** ForwardCommand.hpp
** File description:
** ForwardCommand class definition
*/

#ifndef RIGHT
    #define RIGHT

    #include "commands/ACommand.hpp"
    #include "Server.hpp"

/**
 * @class RightCommand
 * @brief Rotates an AI player clockwise.
 */
class RightCommand : public ACommand {
    public:
        /**
         * @brief Builds a Right command with a cost of 7 cycles.
         */
        RightCommand() : ACommand(7) {}
        ~RightCommand() = default;
        /**
         * @brief Executes the rotation and notifies graphical clients.
         * @param args Unused command arguments.
         * @param server Server executing the command.
         * @param poll_index Poll index of the source AI.
         * @param game Unused game state.
         */
        void exec([[maybe_unused]]std::string args, Server &server, int poll_index, [[maybe_unused]]Game& game) override {
            int fd = server.getPoll().getFds()[poll_index].fd;
            server.getClients().at(fd).getPlayer().turnRight();
            server.sendToClient(poll_index, "ok\n");
            server.notifyPlayerPosition(poll_index);
        }
};

#endif
