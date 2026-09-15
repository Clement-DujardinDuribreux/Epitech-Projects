/*
** EPITECH PROJECT, 2026
** GameEventCommand.hpp
** File description:
** Deferred server-side game event
*/

#ifndef GAME_EVENT_COMMAND
    #define GAME_EVENT_COMMAND

    #include "commands/ACommand.hpp"
    #include "Server.hpp"

/**
 * @class GameEventCommand
 * @brief Executes a deferred server-side game event.
 */
class GameEventCommand : public ACommand {
    public:
        /**
         * @brief Builds a game event command with no additional cycle cost.
         */
        GameEventCommand() : ACommand(0) {}
        ~GameEventCommand() = default;

        /**
         * @brief Dispatches the event payload to the server.
         * @param args Event payload.
         * @param server Server executing the event.
         * @param pollIndex Must be -1 for server events.
         * @param game Unused game state.
         */
        void exec(std::string args, Server &server, int pollIndex,
            [[maybe_unused]] Game &game) override {
            if (pollIndex == -1)
                server.executeGameEvent(args);
        }
};

#endif
