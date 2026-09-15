/*
** EPITECH PROJECT, 2026
** CommandFactory.hpp
** File description:
** CommandFactory class definition
*/

#ifndef COMMANDFACTORY
    #define COMMANDFACTORY

    #include <memory>
    #include "ICommand.hpp"

/**
 * @class CommandFactory
 * @brief Creates command objects from AI and GUI protocol names.
 */
class CommandFactory {
    public:
        /**
         * @brief Builds a command factory.
         */
        CommandFactory();
        ~CommandFactory() = default;

        /**
         * @brief Creates the command matching a protocol token.
         * @param command Command name without arguments.
         * @return Command instance, or nullptr when the name is unknown.
         */
        static std::unique_ptr<ICommand> createCommand(std::string command);
};

#endif
