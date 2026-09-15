#ifndef EJECT_COMMAND
    #define EJECT_COMMAND

    #include "commands/ACommand.hpp"
    #include "Server.hpp"

/**
 * @class EjectCommand
 * @brief Pushes players and destroys eggs from the ejector tile.
 */
class EjectCommand : public ACommand {
    public:
        /**
         * @brief Builds an Eject command with a cost of 7 cycles.
         */
        EjectCommand() : ACommand(7) {}
        /**
         * @brief Moves affected players, removes eggs, and sends protocol answers.
         * @param args Unused command arguments.
         * @param server Server executing the command.
         * @param pollIndex Poll index of the ejecting AI.
         * @param game Mutable game state.
         */
        void exec([[maybe_unused]] std::string args, Server &server,
            int pollIndex, [[maybe_unused]] Game &game) override {
            
            int ejectorFd = server.getPoll().getFds()[pollIndex].fd;
            Client &ejector = server.getClients().at(ejectorFd);
            Player &ejectorPlayer = ejector.getPlayer();
            auto origin = ejectorPlayer.getPosition();
            int dx = 0;
            int dy = 0;

            switch (ejectorPlayer.getDirection()) {
                case Directions::North: dy = -1; break;
                case Directions::South: dy = 1; break;
                case Directions::East: dx = 1; break;
                case Directions::West: dx = -1; break;
            }
            bool ejected = false;
            for (std::size_t index = 1; index < server.getPoll().getFds().size(); index++) {
                if (static_cast<int>(index) == pollIndex)
                    continue;
                int fd = server.getPoll().getFds()[index].fd;
                auto client = server.getClients().find(fd);
                if (client == server.getClients().end()
                    || client->second.getTeamName() == "GRAPHIC")
                    continue;
                Player &player = client->second.getPlayer();
                if (player.getPosition() != origin)
                    continue;
                std::size_t x = static_cast<std::size_t>(
                    (static_cast<int>(origin.first) + dx + static_cast<int>(game.getWidth())) % static_cast<int>(game.getWidth()));
                std::size_t y = static_cast<std::size_t>((static_cast<int>(origin.second) + dy + static_cast<int>(game.getHeight())) % static_cast<int>(game.getHeight()));
                player.setPosition(x, y);
                client->second.setIncanting(false);
                server.sendToClient(index, "eject: " + std::to_string(server.protocolOrientation(ejectorPlayer.getDirection())) + "\n");
                server.notifyPlayerPosition(index);
                ejected = true;
            }
            std::vector<GameEgg> eggs = game.removeEggsAt(origin.first, origin.second);
            for (const GameEgg &egg : eggs) {
                if (server.getTeams()[egg.teamName] > 0)
                    server.getTeams()[egg.teamName]--;
                server.notifyEggDeath(egg.id);
            }
            server.notifyPlayerExpulsion(ejector.getPlayerId());
            server.sendToClient(pollIndex,
                ejected || !eggs.empty() ? "ok\n" : "ko\n");
        }
};

#endif
