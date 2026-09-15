/*
** EPITECH PROJECT, 2026
** ACommand.hpp
** File description:
** ACommand class definition
*/

#ifndef ACOMMAND
    #define ACOMMAND

    #include <memory>
    #include "ICommand.hpp"

/**
 * @class ACommand
 * @brief Base implementation storing the fixed cycle cost of a command.
 */
class ACommand : public ICommand {
    private:
        int _cycleNumber;

    public:
        /**
         * @brief Builds a command with a fixed duration.
         * @param cycleNumber Duration in game cycles.
         */
        ACommand(int cycleNumber);
        ~ACommand() = default;

        /**
         * @brief Gets the command duration.
         * @return Duration in game cycles.
         */
        int getCycleNumber() override;
};

#endif
