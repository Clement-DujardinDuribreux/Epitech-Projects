/*
** EPITECH PROJECT, 2026
** ForwardCommand.hpp
** File description:
** ForwardCommand class definition
*/

#ifndef FORK
    #define FORK

    #include <iostream>
    #include "commands/ACommand.hpp"
    #include "Server.hpp"

/**
 * @class ForkCommand
 * @brief Creates an egg for the player's team.
 */
class ForkCommand : public ACommand {
    public:
        /**
         * @brief Builds a Fork command with a cost of 42 cycles.
         */
        ForkCommand() : ACommand(42) {}
        ~ForkCommand() = default;
        /**
         * @brief Places an egg on the player's tile and increases team slots.
         * @param args Unused command arguments.
         * @param server Server executing the command.
         * @param poll_index Poll index of the source AI.
         * @param game Mutable game state receiving the egg.
         */
        void exec([[maybe_unused]]std::string args, Server &server, int poll_index, [[maybe_unused]]Game& game) override {
            int fd = server.getPoll().getFds()[poll_index].fd;
            Client &client = server.getClients().at(fd);
            auto position = client.getPlayer().getPosition();
            int eggId = game.addEgg(client.getPlayerId(), position.first,
                position.second, client.getTeamName());

            server.getTeams()[client.getTeamName()]++;
            server.notifyPlayerFork(poll_index);
            server.notifyEggPutGround(eggId, client.getPlayerId(), position.first,
                position.second);
            server.sendToClient(poll_index, "ok\n");
        }
};

#endif
