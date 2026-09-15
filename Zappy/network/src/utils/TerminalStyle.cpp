/*
** EPITECH PROJECT, 2026
** TerminalStyle.cpp
** File description:
** TerminalStyle class implementation
*/

#include "utils/TerminalStyle.hpp"

TerminalStyle::TerminalStyle(Code code) : _code(code)
{
}

TerminalStyle TerminalStyle::reset()
{
    return TerminalStyle(RESET);
}

TerminalStyle TerminalStyle::boldCyan()
{
    return TerminalStyle(BOLD_CYAN);
}

TerminalStyle TerminalStyle::boldGreen()
{
    return TerminalStyle(BOLD_GREEN);
}

TerminalStyle TerminalStyle::boldYellow()
{
    return TerminalStyle(BOLD_YELLOW);
}

TerminalStyle TerminalStyle::dim()
{
    return TerminalStyle(DIM);
}

const char *TerminalStyle::sequence() const
{
    switch (_code) {
        case BOLD_CYAN:
            return "\033[1;36m";
        case BOLD_GREEN:
            return "\033[1;32m";
        case BOLD_YELLOW:
            return "\033[1;33m";
        case DIM:
            return "\033[2m";
        case RESET:
        default:
            return "\033[0m";
    }
}

std::ostream &operator<<(std::ostream &stream, const TerminalStyle &style)
{
    return stream << style.sequence();
}
