#ifndef SET_COMMAND
    #define SET_COMMAND

    #include "commands/ACommand.hpp"
    #include "Server.hpp"

/**
 * @class SetCommand
 * @brief Moves one resource from the player inventory to the current tile.
 */
class SetCommand : public ACommand {
    public:
        /**
         * @brief Builds a Set command with a cost of 7 cycles.
         */
        SetCommand() : ACommand(7) {}
        /**
         * @brief Executes the resource drop.
         * @param args Resource protocol name.
         * @param server Server executing the command.
         * @param poll_index Poll index of the source AI.
         * @param game Mutable game state.
         */
        void exec(std::string args, Server &server, int poll_index,
            [[maybe_unused]] Game &game) override {
            server.sendToClient(poll_index,
                dropResource(server, game, poll_index, args) ? "ok\n" : "ko\n");
        }

    private:
        /**
         * @brief Applies the drop to inventories when the player owns the resource.
         * @param server Server executing the command.
         * @param game Mutable game state.
         * @param pollIndex Poll index of the source AI.
         * @param args Resource protocol name.
         * @return True when the drop succeeded.
         */
        bool dropResource(Server &server, Game &game, int pollIndex, std::string args) {
            Ressources resource;
            if (!server.parseResource(args, resource))
                return false;
            int fd = server.getPoll().getFds()[pollIndex].fd;
            Player &player = server.getClients().at(fd).getPlayer();
            if (!player.getInventoryRef().removeRessources(resource, 1))
                return false;
            auto position = player.getPosition();
            game.square(position.first, position.second).getResources()
                .addRessources(resource, 1);
            server.notifyResourceDrop(pollIndex, resource);
            return true;
        }
};

#endif
