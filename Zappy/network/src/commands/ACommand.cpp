/*
** EPITECH PROJECT, 2026
** ACommand.cpp
** File description:
** ACommand class definition
*/

#include "commands/ACommand.hpp"

ACommand::ACommand(int cycleNumber) : _cycleNumber(cycleNumber) {}

int ACommand::getCycleNumber() {
    return _cycleNumber;
}
