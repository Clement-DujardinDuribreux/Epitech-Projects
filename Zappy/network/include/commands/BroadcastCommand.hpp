/*
** EPITECH PROJECT, 2026
** ForwardCommand.hpp
** File description:
** ForwardCommand class definition
*/

#ifndef BROADCAST
    #define BROADCAST

    #include <iostream>
    #include "commands/ACommand.hpp"
    #include "Server.hpp"
    #include "utils/BroadcastDirection.hpp"

/**
 * @class BroadcastCommand
 * @brief Sends a relative-direction message to every connected AI player.
 */
class BroadcastCommand : public ACommand {
    public:
        /**
         * @brief Builds a Broadcast command with a cost of 7 cycles.
         */
        BroadcastCommand() : ACommand(7) {}
        ~BroadcastCommand() = default;
        /**
         * @brief Dispatches the sanitized message and notifies graphical clients.
         * @param args Message payload.
         * @param server Server executing the command.
         * @param poll_index Poll index of the sender AI.
         * @param game Game state used to compute toroidal directions.
         */
        void exec([[maybe_unused]]std::string args, Server &server, int poll_index, [[maybe_unused]]Game& game) override {
            int senderFd = server.getPoll().getFds()[poll_index].fd;
            Player &sender = server.getClients().at(senderFd).getPlayer();
            auto senderPosition = sender.getPosition();

            for (std::size_t index = 1; index < server.getPoll().getFds().size(); index++) {
                int fd = server.getPoll().getFds()[index].fd;
                if (static_cast<int>(index) == poll_index
                    || server.getClients().find(fd) == server.getClients().end()
                    || server.getClients().at(fd).getTeamName().empty()
                    || server.getClients().at(fd).getTeamName() == "GRAPHIC"
                    || server.getClients().at(fd).getPlayerId() < 0)
                    continue;
                Player &receiver = server.getClients().at(fd).getPlayer();
                auto receiverPosition = receiver.getPosition();
                int direction = BroadcastDirection::compute(
                    static_cast<int>(senderPosition.first),
                    static_cast<int>(senderPosition.second),
                    static_cast<int>(receiverPosition.first),
                    static_cast<int>(receiverPosition.second),
                    receiver.getDirection(), game.getWidth(), game.getHeight());
                server.sendToClient(index, "message " + std::to_string(direction) + ", "
                    + server.cleanProtocalMessage(args) + "\n");
            }
            server.notifyPlayerBroadcast(poll_index, args);
            server.sendToClient(poll_index, "ok\n");
        }
};

#endif
