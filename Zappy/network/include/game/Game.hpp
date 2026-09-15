/*
** EPITECH PROJECT, 2026
** Game.hpp
** File description:
** Game class definition
*/

#ifndef GAME
    #define GAME

    #include <vector>
    #include <string>
    #include "Square.hpp"
    #include "Player.hpp"
    #include "Egg.hpp"

/**
 * @class Game
 * @brief Owns the map, resources, and eggs for one server instance.
 */
class Game {
    private:
        std::size_t _widthMap;
        std::size_t _heightMap;
        std::vector<Square> _map;
        std::vector<GameEgg> _eggs;
        int _nextEggId;
    public:
        /**
         * @brief Builds a toroidal map and performs the initial resource spawn.
         * @param width Map width.
         * @param height Map height.
         */
        Game(std::size_t width, std::size_t height);
        ~Game() = default;

        /**
         * @brief Gets the map width.
         * @return Width in tiles.
         */
        std::size_t getWidth();

        /**
         * @brief Gets the map height.
         * @return Height in tiles.
         */
        std::size_t getHeight();

        /**
         * @brief Gets a map tile by coordinates.
         * @param x Tile x coordinate.
         * @param y Tile y coordinate.
         * @return Mutable tile reference.
         */
        Square& square(std::size_t x, std::size_t y);
        /**
         * @brief Spawns missing resources according to Zappy densities.
         * @return Positions whose resource content changed.
         */
        std::vector<std::pair<std::size_t, std::size_t>> respawnResources();
        /**
         * @brief Adds an egg to the map.
         * @param playerId Creator player id, or -1 for an initial egg.
         * @param x Egg x coordinate.
         * @param y Egg y coordinate.
         * @param teamName Team allowed to use the egg.
         * @return Created egg identifier.
         */
        int addEgg(int playerId, std::size_t x, std::size_t y, const std::string &teamName);
        /**
         * @brief Finds a random egg available for a team.
         * @param teamName Team name to search.
         * @return Pointer to an egg, or nullptr when none exists.
         */
        GameEgg *findEggForTeam(const std::string &teamName);
        /**
         * @brief Removes one egg by identifier.
         * @param eggId Egg identifier.
         * @return True when an egg was removed.
         */
        bool removeEgg(int eggId);
        /**
         * @brief Removes all eggs placed on a tile.
         * @param x Tile x coordinate.
         * @param y Tile y coordinate.
         * @return Removed egg snapshots.
         */
        std::vector<GameEgg> removeEggsAt(std::size_t x, std::size_t y);
        /**
         * @brief Gets all eggs currently present on the map.
         * @return Read-only egg list.
         */
        const std::vector<GameEgg> &getEggs() const;
};

#endif
