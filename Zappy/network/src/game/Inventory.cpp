/*
** EPITECH PROJECT, 2026
** Inventory.cpp
** File description:
** Inventory class definition
*/

#include "game/Inventory.hpp"

Inventory::Inventory() {
    _inventory[Ressources::Food] = 0;
    _inventory[Ressources::Linemate] = 0;
    _inventory[Ressources::Deraumere] = 0;
    _inventory[Ressources::Sibur] = 0;
    _inventory[Ressources::Mendiane] = 0;
    _inventory[Ressources::Phiras] = 0;
    _inventory[Ressources::Thystame] = 0;
}

int Inventory::getRessourcesQuantity(Ressources ressource) {
    return _inventory[ressource];
}

void Inventory::addRessources(Ressources ressource, int quantity) {
    _inventory[ressource] += quantity;
}

bool Inventory::removeRessources(Ressources ressource, int quantity) {
    if (quantity < 0 || _inventory[ressource] < quantity)
        return false;
    _inventory[ressource] -= quantity;
    return true;
}

std::string Inventory::toString() {
    std::string stringInventory = "[";

    stringInventory += "food " + std::to_string(_inventory[Ressources::Food]);
    stringInventory += ", linemate " + std::to_string(_inventory[Ressources::Linemate]);
    stringInventory += ", deraumere " + std::to_string(_inventory[Ressources::Deraumere]);
    stringInventory += ", sibur " + std::to_string(_inventory[Ressources::Sibur]);
    stringInventory += ", mendiane " + std::to_string(_inventory[Ressources::Mendiane]);
    stringInventory += ", phiras " + std::to_string(_inventory[Ressources::Phiras]);
    stringInventory += ", thystame " + std::to_string(_inventory[Ressources::Thystame]);
    stringInventory += "]";
    return stringInventory;
}
