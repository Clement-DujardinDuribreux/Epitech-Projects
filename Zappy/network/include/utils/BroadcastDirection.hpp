/*
** EPITECH PROJECT, 2026
** BroadcastDirection.hpp
** File description:
** BroadcastDirection class definition
*/

#ifndef BROADCAST_DIRECTION_HPP
    #define BROADCAST_DIRECTION_HPP

    #include <cstddef>
    #include "game/Direction.hpp"

/**
 * @class BroadcastDirection
 * @brief Computes the relative sound direction for AI Broadcast messages.
 */
class BroadcastDirection {
    private:
        /**
         * @brief Selects the shortest offset on a toroidal axis.
         * @param offset Raw coordinate delta.
         * @param size Axis size.
         * @return Wrapped delta with the shortest distance.
         */
        static int shortestOffset(int offset, std::size_t size);
        /**
         * @brief Converts an orientation to a sector rotation.
         * @param direction Receiver orientation.
         * @return Quarter-turn offset used by the protocol direction map.
         */
        static int orientationOffset(Directions direction);

    public:
        /**
         * @brief Computes the Zappy protocol broadcast direction.
         * @param senderX Sender x coordinate.
         * @param senderY Sender y coordinate.
         * @param receiverX Receiver x coordinate.
         * @param receiverY Receiver y coordinate.
         * @param receiverDirection Orientation of the receiver.
         * @param width Map width.
         * @param height Map height.
         * @return Direction from 0 to 8, where 0 means same tile.
         */
        static int compute(int senderX, int senderY, int receiverX,
            int receiverY, Directions receiverDirection, std::size_t width,
            std::size_t height);
};

#endif
