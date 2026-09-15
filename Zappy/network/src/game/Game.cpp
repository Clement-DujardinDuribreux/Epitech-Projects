/*
** EPITECH PROJECT, 2026
** Game.cpp
** File description:
** Game class definition
*/

#include "game/Game.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <ctime>

Game::Game(std::size_t width, std::size_t height) : _widthMap(width),
    _heightMap(height), _nextEggId(0) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    _map.reserve(width * height);
    for (std::size_t i = 0; i < width * height; i++) {
        _map.emplace_back(i % width, i / width);
    }
    respawnResources();
}

Square& Game::square(std::size_t x, std::size_t y) {
    return _map[_widthMap * y + x];
}

std::size_t Game::getWidth() {
    return _widthMap;
}

std::size_t Game::getHeight() {
    return _heightMap;
}

std::vector<std::pair<std::size_t, std::size_t>> Game::respawnResources() {
    static const std::array<double, 7> densities =
        {0.5, 0.3, 0.15, 0.1, 0.1, 0.08, 0.05};
    std::vector<std::pair<std::size_t, std::size_t>> changed;
    std::size_t tileCount = _widthMap * _heightMap;

    for (int resource = Ressources::Food; resource <= Ressources::Thystame;
        resource++) {
        int current = 0;
        for (Square &tile : _map)
            current += tile.getResources().getRessourcesQuantity(
                static_cast<Ressources>(resource));
        int target = std::max(1, static_cast<int>(
            std::floor(tileCount * densities[resource])));
        for (int missing = target - current; missing > 0; missing--) {
            std::size_t index = static_cast<std::size_t>(std::rand()) % tileCount;
            _map[index].getResources().addRessources(
                static_cast<Ressources>(resource), 1);
            std::pair<std::size_t, std::size_t> position =
                _map[index].getPosition();
            if (std::find(changed.begin(), changed.end(), position)
                == changed.end())
                changed.push_back(position);
        }
    }
    return changed;
}

int Game::addEgg(int playerId, std::size_t x, std::size_t y,
    const std::string &teamName) {
    int eggId = _nextEggId++;
    _eggs.push_back({eggId, playerId, x, y, teamName});
    return eggId;
}

GameEgg *Game::findEggForTeam(const std::string &teamName) {
    std::vector<GameEgg *> matchingEggs;

    for (GameEgg &egg : _eggs) {
        if (egg.teamName == teamName)
            matchingEggs.push_back(&egg);
    }
    if (matchingEggs.empty())
        return nullptr;
    return matchingEggs[static_cast<std::size_t>(std::rand())
        % matchingEggs.size()];
}

bool Game::removeEgg(int eggId) {
    for (auto egg = _eggs.begin(); egg != _eggs.end(); egg++) {
        if (egg->id == eggId) {
            _eggs.erase(egg);
            return true;
        }
    }
    return false;
}

std::vector<GameEgg> Game::removeEggsAt(std::size_t x, std::size_t y) {
    std::vector<GameEgg> removed;

    for (auto egg = _eggs.begin(); egg != _eggs.end();) {
        if (egg->x == x && egg->y == y) {
            removed.push_back(*egg);
            egg = _eggs.erase(egg);
        } else {
            egg++;
        }
    }
    return removed;
}

const std::vector<GameEgg> &Game::getEggs() const {
    return _eggs;
}
