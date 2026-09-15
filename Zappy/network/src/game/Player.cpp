/*
** EPITECH PROJECT, 2026
** Player.cpp
** File description:
** Player class definition
*/

#include "game/Player.hpp"

Player::Player() : _xPos(0), _yPos(0), _level(1), _direction(Directions::North) {
    _inventory.addRessources(Ressources::Food, 10);
}

std::pair<std::size_t, std::size_t> Player::getPosition() {
    return std::make_pair<std::size_t, std::size_t>(_xPos, _yPos);
}

void Player::goForward(std::size_t width, std::size_t height) {
    switch (_direction) {
        case Directions::North:
            _yPos = _yPos > 0 ? _yPos - 1 : static_cast<int>(height) - 1;
            break;
        case Directions::South: _yPos = (_yPos + 1) % height; break;
        case Directions::West:
            _xPos = _xPos > 0 ? _xPos - 1 : static_cast<int>(width) - 1;
            break;
        case Directions::East: _xPos = (_xPos + 1) % width; break;
    }
}

void Player::turnRight() {
    switch (_direction) {
        case Directions::North: _direction = Directions::East; break;
        case Directions::South: _direction = Directions::West; break;
        case Directions::West: _direction = Directions::North; break;
        case Directions::East: _direction = Directions::South; break;
    }
}

void Player::turnLeft() {
    switch (_direction) {
        case Directions::North: _direction = Directions::West; break;
        case Directions::South: _direction = Directions::East; break;
        case Directions::West: _direction = Directions::South; break;
        case Directions::East: _direction = Directions::North; break;
    }
}

Inventory Player::getInventory() const {
    return _inventory;
}

Inventory &Player::getInventoryRef() {
    return _inventory;
}

Directions Player::getDirection() {
    return _direction;
}

int Player::getLevel() {
    return _level;
}

void Player::setPosition(std::size_t x, std::size_t y) {
    _xPos = static_cast<int>(x);
    _yPos = static_cast<int>(y);
}

void Player::setLevel(int level) {
    _level = level;
}

std::vector<std::pair<int, int>> Player::look() {
    std::pair<int, int> directionVector(0, 0);
    std::pair<int, int> lateralVector(0, 0);
    std::vector<std::pair<int, int>> _cases;
    
    switch (_direction) {
        case Directions::North: directionVector.second -= 1; lateralVector.first += 1; break;
        case Directions::South: directionVector.second += 1; lateralVector.first -= 1; break;
        case Directions::West: directionVector.first -= 1; lateralVector.second += 1; break;
        case Directions::East: directionVector.first += 1; lateralVector.second -= 1; break;
    }

    for (int i = 0; i <= _level; i++) {
        std::pair<int, int> centerCase(_xPos + directionVector.first * i, _yPos + directionVector.second * i);
        
        for (int y = -i; y <= i; y++) {
            std::pair<int, int> newCase(centerCase.first + lateralVector.first * y, centerCase.second + lateralVector.second * y);

            _cases.push_back(newCase);
        }
    }
    return _cases;
}
