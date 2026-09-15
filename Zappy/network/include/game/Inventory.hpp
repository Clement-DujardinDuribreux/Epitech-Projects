/*
** EPITECH PROJECT, 2026
** Inventory.hpp
** File description:
** Inventory class definition
*/

#ifndef INVENTORY
    #define INVENTORY

    #include <map>
    #include <iostream>
    #include "Ressources.hpp"

/**
 * @class Inventory
 * @brief Stores resource quantities for a player or a map tile.
 */
class Inventory {
    private:
        std::map<Ressources, int> _inventory;

    public:
        /**
         * @brief Builds an inventory with every resource set to zero.
         */
        Inventory();
        ~Inventory() = default;

        /**
         * @brief Gets the quantity of one resource.
         * @param ressource Resource to inspect.
         * @return Stored quantity.
         */
        int getRessourcesQuantity(Ressources ressource);

        /**
         * @brief Adds resources to the inventory.
         * @param ressource Resource to add.
         * @param quantity Quantity to add.
         */
        void addRessources(Ressources ressource, int quantity);
        /**
         * @brief Removes resources when enough quantity is available.
         * @param ressource Resource to remove.
         * @param quantity Quantity to remove.
         * @return True when the removal succeeded.
         */
        bool removeRessources(Ressources ressource, int quantity);

        /**
         * @brief Formats the inventory for the AI protocol.
         * @return Protocol inventory string.
         */
        std::string toString();
};

#endif
