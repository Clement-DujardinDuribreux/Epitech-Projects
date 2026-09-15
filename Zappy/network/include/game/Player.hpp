/*
** EPITECH PROJECT, 2026
** Player.hpp
** File description:
** Player class definition
*/

#ifndef PLAYER
    #define PLAYER

    #include <vector>
    #include "Inventory.hpp"
    #include "Direction.hpp"

/**
 * @class Player
 * @brief Stores the in-game state of one AI-controlled player.
 */
class Player {
    private:
        int _xPos;
        int _yPos;
        int _level;
        Inventory _inventory;
        Directions _direction;

    public:
        /**
         * @brief Builds a level-one player at (0, 0), facing north.
         */
        Player();
        ~Player() = default;

        /**
         * @brief Gets the current map position.
         * @return Pair containing x and y coordinates.
         */
        std::pair<std::size_t, std::size_t> getPosition();

        /**
         * @brief Gets the elevation level.
         * @return Player level.
         */
        int getLevel();

        /**
         * @brief Gets a copy of the player inventory.
         * @return Inventory copy.
         */
        Inventory getInventory() const;
        /**
         * @brief Gets the mutable player inventory.
         * @return Inventory reference.
         */
        Inventory &getInventoryRef();

        /**
         * @brief Gets the current orientation.
         * @return Player direction.
         */
        Directions getDirection();

        /**
         * @brief Moves the player forward on a toroidal map.
         * @param width Map width.
         * @param height Map height.
         */
        void goForward(std::size_t width, std::size_t height);

        /**
         * @brief Rotates the player clockwise.
         */
        void turnRight();

        /**
         * @brief Rotates the player counter-clockwise.
         */
        void turnLeft();
        /**
         * @brief Sets the map position.
         * @param x New x coordinate.
         * @param y New y coordinate.
         */
        void setPosition(std::size_t x, std::size_t y);
        /**
         * @brief Sets the elevation level.
         * @param level New player level.
         */
        void setLevel(int level);

        /**
         * @brief Computes visible relative tile coordinates.
         * @return Coordinates covered by the Look cone.
         */
        std::vector<std::pair<int, int>> look();
};

#endif
