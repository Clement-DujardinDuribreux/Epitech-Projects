#ifndef INCANTATION_COMMAND
    #define INCANTATION_COMMAND

    #include "commands/ACommand.hpp"
    #include "Server.hpp"
    #include <array>

/**
 * @class IncantationCommand
 * @brief Starts an elevation attempt for players on the same tile.
 */
class IncantationCommand : public ACommand {
    public:
        /**
         * @brief Builds an Incantation command with immediate validation.
         */
        IncantationCommand() : ACommand(0) {}
        /**
         * @brief Validates prerequisites and schedules incantation completion.
         * @param args Unused command arguments.
         * @param server Server executing the command.
         * @param pollIndex Poll index of the initiating AI.
         * @param game Mutable game state.
         */
        void exec([[maybe_unused]] std::string args, Server &server,
            int pollIndex, [[maybe_unused]] Game &game) override {
            if (!startIncantation(server, game, pollIndex))
                server.sendToClient(pollIndex, "ko\n");
        }

    private:
        /**
         * @brief Checks resources and participants before starting elevation.
         * @param server Server executing the command.
         * @param game Mutable game state.
         * @param pollIndex Poll index of the initiating AI.
         * @return True when the incantation was scheduled.
         */
        bool startIncantation(Server &server, Game &game, int pollIndex) {
            const std::array<int, 7> requiredPlayers =
                {1, 2, 2, 4, 4, 6, 6};
            const std::array<std::array<int, 6>, 7> requiredResources = {{
                {{1, 0, 0, 0, 0, 0}},
                {{1, 1, 1, 0, 0, 0}},
                {{2, 0, 1, 0, 2, 0}},
                {{1, 1, 2, 0, 1, 0}},
                {{1, 2, 1, 3, 0, 0}},
                {{1, 2, 3, 0, 1, 0}},
                {{2, 2, 2, 2, 2, 1}}
            }};
            int fd = server.getPoll().getFds()[pollIndex].fd;
            Player &initiator = server.getClients().at(fd).getPlayer();
            int level = initiator.getLevel();
            if (level < 1 || level >= 8)
                return false;
            auto position = initiator.getPosition();
            Inventory &tile = game.square(position.first, position.second)
                .getResources();
            for (int resource = Ressources::Linemate;
                resource <= Ressources::Thystame; resource++) {
                if (tile.getRessourcesQuantity(static_cast<Ressources>(resource))
                    < requiredResources[level - 1][resource - 1])
                    return false;
            }
            std::vector<int> participants;
            for (std::size_t index = 1; index < server.getPoll().getFds().size(); index++) {
                int otherFd = server.getPoll().getFds()[index].fd;
                auto client = server.getClients().find(otherFd);
                if (client == server.getClients().end()
                    || client->second.getTeamName().empty()
                    || client->second.getTeamName() == "GRAPHIC"
                    || client->second.getPlayerId() < 0)
                    continue;
                Player &player = client->second.getPlayer();
                if (player.getPosition() == position && player.getLevel() == level)
                    participants.push_back(client->second.getPlayerId());
            }
            if (static_cast<int>(participants.size()) < requiredPlayers[level - 1])
                return false;
            server.notifyIncantationStart(position.first, position.second, level,
                participants);
            std::string event = "incantation " + std::to_string(position.first)
                + " " + std::to_string(position.second) + " "
                + std::to_string(level);
            for (int playerId : participants) {
                int participantIndex = server.findPollIndexByPlayerId(playerId);
                if (participantIndex >= 0) {
                    int participantFd = server.getPoll().getFds()[participantIndex].fd;
                    server.getClients().at(participantFd).setIncanting(true);
                    server.sendToClient(participantIndex, "Elevation underway\n");
                }
                event += " " + std::to_string(playerId);
            }
            server.scheduleGameEvent(300, event);
            return true;
        }
};

#endif
