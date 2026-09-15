#ifndef HELLO
    #define HELLO

    #include <iostream>
    #include "commands/ACommand.hpp"
    #include "Server.hpp"

/**
 * @class HelloCommand
 * @brief Debug command that sends a fixed greeting.
 */
class HelloCommand : public ACommand {
    public:
        /**
         * @brief Builds a HELLO command with a cost of 7 cycles.
         */
        HelloCommand() : ACommand(7) {}
        ~HelloCommand() = default;
        /**
         * @brief Sends the fixed greeting response to the client.
         * @param args Unused command arguments.
         * @param server Server executing the command.
         * @param client_index Poll index of the source client.
         * @param game Unused game state.
         */
        void exec([[maybe_unused]]std::string args, Server &server, int client_index, [[maybe_unused]]Game& game) override {
            server.sendToClient(client_index, "I have to say Hello");
        }
};

#endif
