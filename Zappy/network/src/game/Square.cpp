/*
** EPITECH PROJECT, 2026
** Square.cpp
** File description:
** Square class definition
*/

#include "game/Square.hpp"

Square::Square(std::size_t x, std::size_t y) : _xPos(x), _yPos(y) {}

std::pair<std::size_t, std::size_t> Square::getPosition() {
    return std::make_pair<std::size_t, std::size_t>(_xPos, _yPos);
}

Inventory &Square::getResources() {
    return _ressources;
}
