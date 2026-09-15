/*
** EPITECH PROJECT, 2026
** GraphicsEventCommand.hpp
** File description:
** Deferred graphical protocol event
*/

#ifndef GRAPHICS_EVENT_COMMAND
    #define GRAPHICS_EVENT_COMMAND

    #include "commands/ACommand.hpp"
    #include "Server.hpp"

/**
 * @class GraphicsEventCommand
 * @brief Sends a deferred payload to graphical clients.
 */
class GraphicsEventCommand : public ACommand {
    public:
        /**
         * @brief Builds a graphical event command with no cycle cost.
         */
        GraphicsEventCommand() : ACommand(0) {}
        ~GraphicsEventCommand() = default;

        /**
         * @brief Allows pending GUI events to flush after game over.
         * @return Always true.
         */
        bool canRunAfterGameOver() const override { return true; }

        /**
         * @brief Sends the payload to every graphical client.
         * @param payload Graphical protocol event.
         * @param server Server executing the event.
         * @param pollIndex Must be -1 for server events.
         * @param game Unused game state.
         */
        void exec(std::string payload, Server &server, int pollIndex, [[maybe_unused]] Game &game) override {
            if (pollIndex != -1)
                return;
            server.sendToGraphics(payload);
        }
};

#endif
