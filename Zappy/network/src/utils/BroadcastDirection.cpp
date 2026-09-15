/*
** EPITECH PROJECT, 2026
** BroadcastDirection.cpp
** File description:
** BroadcastDirection class implementation
*/

#include "utils/BroadcastDirection.hpp"
#include <cmath>
#include <numbers>

int BroadcastDirection::shortestOffset(int offset, std::size_t size)
{
    int dimension = static_cast<int>(size);

    if (std::abs(offset) > dimension / 2)
        offset += offset > 0 ? -dimension : dimension;
    return offset;
}

int BroadcastDirection::orientationOffset(Directions direction)
{
    switch (direction) {
        case Directions::East:
            return 1;
        case Directions::South:
            return 2;
        case Directions::West:
            return 3;
        case Directions::North:
        default:
            return 0;
    }
}

int BroadcastDirection::compute(int senderX, int senderY, int receiverX,
    int receiverY, Directions receiverDirection, std::size_t width,
    std::size_t height)
{
    int dx = shortestOffset(senderX - receiverX, width);
    int dy = shortestOffset(senderY - receiverY, height);

    if (dx == 0 && dy == 0)
        return 0;
    double angle = std::atan2(static_cast<double>(-dx),
        static_cast<double>(-dy));
    int sector = static_cast<int>(std::lround(
        angle / (std::numbers::pi / 4.0)));
    int relativeSector = (sector
        + orientationOffset(receiverDirection) * 2) % 8;

    if (relativeSector < 0)
        relativeSector += 8;
    return relativeSector + 1;
}
