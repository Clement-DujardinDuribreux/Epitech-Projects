/*
** EPITECH PROJECT, 2026
** GuiCommand.hpp
** File description:
** GUI protocol request command
*/

#ifndef GUI_COMMAND
    #define GUI_COMMAND

    #include "commands/ACommand.hpp"
    #include "Server.hpp"

/**
 * @class GuiCommand
 * @brief Wraps one graphical protocol request.
 */
class GuiCommand : public ACommand {
    private:
        std::string _name;

    public:
        /**
         * @brief Builds a GUI command for a protocol request name.
         * @param name GUI command name.
         */
        GuiCommand(const std::string &name) : ACommand(0), _name(name) {}
        ~GuiCommand() = default;

        /**
         * @brief Dispatches the GUI request to the server.
         * @param args Raw request arguments.
         * @param server Server executing the command.
         * @param pollIndex Poll index of the graphical client.
         * @param game Unused game state.
         */
        void exec(std::string args, Server &server, int pollIndex, [[maybe_unused]] Game &game) override {
            server.executeGuiCommand(_name, args, pollIndex);
        }
};

#endif
