/*
** EPITECH PROJECT, 2026
** Egg.hpp
** File description:
** Server-side egg state
*/

#ifndef GAME_EGG
    #define GAME_EGG

    #include <string>

/**
 * @struct GameEgg
 * @brief Server-side egg state used to reserve future team slots.
 */
struct GameEgg {
    /** Unique egg identifier. */
    int id;
    /** Identifier of the player that created the egg, or -1 for initial eggs. */
    int playerId;
    /** Egg x coordinate. */
    std::size_t x;
    /** Egg y coordinate. */
    std::size_t y;
    /** Team allowed to consume this egg. */
    std::string teamName;
};

#endif
