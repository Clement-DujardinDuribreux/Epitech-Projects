/*
** EPITECH PROJECT, 2026
** ForwardCommand.hpp
** File description:
** ForwardCommand class definition
*/

#ifndef LOOK
    #define LOOK

    #include <iostream>
    #include <array>
    #include "commands/ACommand.hpp"
    #include "Server.hpp"

/**
 * @class LookCommand
 * @brief Sends the visible tiles in front of the AI player.
 */
class LookCommand : public ACommand {
    public:
        /**
         * @brief Builds a Look command with a cost of 7 cycles.
         */
        LookCommand() : ACommand(7) {}
        ~LookCommand() = default;
        /**
         * @brief Formats visible players and resources according to AI protocol.
         * @param args Unused command arguments.
         * @param server Server executing the command.
         * @param poll_index Poll index of the source AI.
         * @param game Game state used to inspect map tiles.
         */
        void exec([[maybe_unused]]std::string args, Server &server,
            int poll_index, [[maybe_unused]]Game& game) override {
            const std::array<std::string, 7> resourceNames = {
                "food", "linemate", "deraumere", "sibur", "mendiane",
                "phiras", "thystame"
            };
            int fd = server.getPoll().getFds()[poll_index].fd;
            Player &viewer = server.getClients().at(fd).getPlayer();
            auto origin = viewer.getPosition();
            int forwardX = 0;
            int forwardY = 0;
            int sideX = 0;
            int sideY = 0;
            switch (viewer.getDirection()) {
                case Directions::North:
                    forwardY = -1; sideX = 1; break;
                case Directions::South:
                    forwardY = 1; sideX = -1; break;
                case Directions::East:
                    forwardX = 1; sideY = 1; break;
                case Directions::West:
                    forwardX = -1; sideY = -1; break;
            }
            std::string response = "[";
            bool firstTile = true;
            for (int distance = 0; distance <= viewer.getLevel(); distance++) {
                for (int offset = -distance; offset <= distance; offset++) {
                    if (!firstTile)
                        response += ", ";
                    firstTile = false;
                    int rawX = static_cast<int>(origin.first) + forwardX * distance + sideX * offset;
                    int rawY = static_cast<int>(origin.second) + forwardY * distance + sideY * offset;
                    std::size_t x = static_cast<std::size_t>((rawX + static_cast<int>(game.getWidth()) * 2) % static_cast<int>(game.getWidth()));
                    std::size_t y = static_cast<std::size_t>((rawY + static_cast<int>(game.getHeight()) * 2) % static_cast<int>(game.getHeight()));
                    bool firstObject = true;
                    for (auto &entry : server.getClients()) {
                        if (entry.second.getTeamName() == "GRAPHIC" || entry.second.getPlayerId() < 0 || entry.second.getPlayer().getPosition() != std::make_pair(x, y))
                            continue;
                        if (!firstObject)
                            response += " ";
                        response += "player";
                        firstObject = false;
                    }
                    Inventory &resources = game.square(x, y).getResources();
                    for (int resource = Ressources::Food;
                        resource <= Ressources::Thystame; resource++) {
                        int quantity = resources.getRessourcesQuantity(
                            static_cast<Ressources>(resource));
                        for (int count = 0; count < quantity; count++) {
                            response += firstObject ? resourceNames[resource] : " " + resourceNames[resource];
                            firstObject = false;
                        }
                    }
                }
            }
            response += "]";
            server.sendToClient(poll_index,
                response + "\n");
        }
};

#endif
