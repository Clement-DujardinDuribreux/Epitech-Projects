/*
** EPITECH PROJECT, 2026
** CommandFactory.cpp
** File description:
** CommandFactory class definition
*/

#include "commands/CommandFactory.hpp"
#include "commands/BroadcastCommand.hpp"
#include "commands/ConnectNbrCommand.hpp"
#include "commands/EjectCommand.hpp"
#include "commands/ForkCommand.hpp"
#include "commands/ForwardCommand.hpp"
#include "commands/GuiCommand.hpp"
#include "commands/HelloCommand.hpp"
#include "commands/IncantationCommand.hpp"
#include "commands/InventoryCommand.hpp"
#include "commands/LookCommand.hpp"
#include "commands/SetCommand.hpp"
#include "commands/TakeCommand.hpp"
#include "commands/TurnLeftCommand.hpp"
#include "commands/TurnRightCommand.hpp"

std::unique_ptr<ICommand> CommandFactory::createCommand(std::string command) {
    if (command == "HELLO")
        return std::make_unique<HelloCommand>();
    if (command == "Forward")
        return std::make_unique<ForwardCommand>();
    if (command == "Right")
        return std::make_unique<RightCommand>();
    if (command == "Left")
        return std::make_unique<LeftCommand>();
    if (command == "Inventory")
        return std::make_unique<InventoryCommand>();
    if (command == "Look")
        return std::make_unique<LookCommand>();
    if (command == "Broadcast")
        return std::make_unique<BroadcastCommand>();
    if (command == "Connect_nbr")
        return std::make_unique<ConnectNbrCommand>();
    if (command == "Fork")
        return std::make_unique<ForkCommand>();
    if (command == "Take")
        return std::make_unique<TakeCommand>();
    if (command == "Set")
        return std::make_unique<SetCommand>();
    if (command == "Eject")
        return std::make_unique<EjectCommand>();
    if (command == "Incantation")
        return std::make_unique<IncantationCommand>();
    if (command == "msz" || command == "bct" || command == "mct"
        || command == "tna" || command == "ppo" || command == "plv"
        || command == "pin" || command == "sgt" || command == "sst")
        return std::make_unique<GuiCommand>(command);
    else
        return nullptr;
}
