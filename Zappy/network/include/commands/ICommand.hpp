/*
** EPITECH PROJECT, 2026
** ICommand.hpp
** File description:
** ICommand class definition
*/

#ifndef ICOMMAND
    #define ICOMMAND

    #include <iostream>
    #include "game/Game.hpp"

class Server;

/**
 * @class ICommand
 * @brief Common interface for every scheduled server command.
 */
class ICommand {
    public:
        virtual ~ICommand() = default;
        /**
         * @brief Executes the command against the server and game state.
         * @param args Raw command arguments without the command name.
         * @param server Server executing the command.
         * @param poll_index Poll index of the source client.
         * @param game Mutable game state.
         */
        virtual void exec(std::string args, Server& server, int poll_index, Game& game) = 0;

        /**
         * @brief Gets the command duration.
         * @return Duration in game cycles.
         */
        virtual int getCycleNumber() = 0;
        /**
         * @brief Tells whether the command may run after victory.
         * @return True when execution remains allowed after game over.
         */
        virtual bool canRunAfterGameOver() const { return false; }
};

#endif
