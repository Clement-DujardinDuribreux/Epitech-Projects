/*
** EPITECH PROJECT, 2026
** Game.hpp
** File description:
** Game class definition
*/

#ifndef SQUARE
    #define SQUARE

    #include <utility>
    #include "Inventory.hpp"

/**
 * @class Square
 * @brief Represents one map tile and its resources.
 */
class Square {
    private:
        int _xPos;
        int _yPos;
        Inventory _ressources;
        
    public:
        /**
         * @brief Builds a tile at a fixed map position.
         * @param x Tile x coordinate.
         * @param y Tile y coordinate.
         */
        Square(std::size_t x, std::size_t y);
        ~Square() = default;

        /**
         * @brief Gets the tile position.
         * @return Pair containing x and y coordinates.
         */
        std::pair<std::size_t, std::size_t> getPosition();
        /**
         * @brief Gets the tile resource inventory.
         * @return Mutable resource inventory.
         */
        Inventory &getResources();
};

#endif
