/*
** EPITECH PROJECT, 2026
** ForwardCommand.hpp
** File description:
** ForwardCommand class definition
*/

#ifndef INVENTORY_COMMAND
    #define INVENTORY_COMMAND

    #include <iostream>
    #include "commands/ACommand.hpp"
    #include "Server.hpp"

/**
 * @class InventoryCommand
 * @brief Sends the AI player's inventory.
 */
class InventoryCommand : public ACommand {
    public:
        /**
         * @brief Builds an Inventory command with a cost of 1 cycle.
         */
        InventoryCommand() : ACommand(1) {}
        ~InventoryCommand() = default;
        /**
         * @brief Formats and sends the current inventory to the AI client.
         * @param args Unused command arguments.
         * @param server Server executing the command.
         * @param poll_index Poll index of the source AI.
         * @param game Unused game state.
         */
        void exec([[maybe_unused]]std::string args, Server &server, int poll_index, [[maybe_unused]]Game& game) override {
            int fd = server.getPoll().getFds()[poll_index].fd;
            std::string inventory =
                server.getClients().at(fd).getPlayer().getInventory().toString();
            inventory += "\n";
            server.sendToClient(poll_index, inventory);
        }
};

#endif
