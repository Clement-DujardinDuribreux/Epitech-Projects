#ifndef TAKE_COMMAND
    #define TAKE_COMMAND

    #include "commands/ACommand.hpp"
    #include "Server.hpp"

/**
 * @class TakeCommand
 * @brief Moves one resource from the current tile to the player inventory.
 */
class TakeCommand : public ACommand {
    public:
        /**
         * @brief Builds a Take command with a cost of 7 cycles.
         */
        TakeCommand() : ACommand(7) {}
        /**
         * @brief Executes the resource pickup.
         * @param args Resource protocol name.
         * @param server Server executing the command.
         * @param poll_index Poll index of the source AI.
         * @param game Mutable game state.
         */
        void exec(std::string args, Server &server, int poll_index, Game &game) override {
            server.sendToClient(poll_index,
                takeResource(server, game, poll_index, args) ? "ok\n" : "ko\n");
        }

    private:
        /**
         * @brief Applies the pickup to inventories when the resource exists.
         * @param server Server executing the command.
         * @param game Mutable game state.
         * @param pollIndex Poll index of the source AI.
         * @param args Resource protocol name.
         * @return True when the pickup succeeded.
         */
        bool takeResource(Server &server, Game &game, int pollIndex, std::string args) {
            Ressources resource;
            if (!server.parseResource(args, resource))
                return false;
            int fd = server.getPoll().getFds()[pollIndex].fd;
            Player &player = server.getClients().at(fd).getPlayer();
            auto position = player.getPosition();
            Inventory &tile = game.square(position.first, position.second).getResources();

            if (!tile.removeRessources(resource, 1))
                return false;
            player.getInventoryRef().addRessources(resource, 1);
            server.notifyResourceCollect(pollIndex, resource);
            return true;
        }
};

#endif
