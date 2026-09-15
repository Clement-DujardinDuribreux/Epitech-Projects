/*
** EPITECH PROJECT, 2026
** Player
** File description:
** Player def
*/

#ifndef PLAYER
    #define PLAYER

    #include <string>
    #include <vector>
    #include "AssetsManager.hpp"
    #include <SFML/System/Clock.hpp>
    #include <SFML/Graphics/Color.hpp>
    #include <chrono>

/**
 * @brief Graphical state of one player received from the server.
 */
class Player {
    private:
        int _id;

        int _x;
        int _y;
        std::string _teamName;
        int _level;
        std::vector<int> _inventory;
        int _orientation;

        std::string _texture;
        sf::Color _color;

        std::chrono::steady_clock::time_point _lastBroadcastTime;
        std::string _broadcastMessage = "";

        bool _isIncanting = false;
        bool _isTaking = false;
        bool _isDropping = false;
        bool _isExpelling = false;
        bool _isForking = false;
        bool _isLevelingUp = false;
        std::chrono::steady_clock::time_point _lastLevelUpTime;

        inline static std::map<std::string, std::pair<sf::Color, std::string>> _teamParams;

    public:
        /**
         * @brief Creates a player with protocol data and team display parameters.
         * @param id Player identifier from the graphical protocol.
         * @param x Tile x coordinate.
         * @param y Tile y coordinate.
         * @param orientation Protocol orientation value.
         * @param level Player level.
         * @param teamName Player team name.
         */
        Player(int id, int x, int y, int orientation, int level, const std::string& teamName) : _id(id), _x(x), _y(y), _teamName(teamName), _level(level), _inventory(7, 0), _orientation(orientation) {
            std::vector<std::string> textures = {"sacrieur", "iop"};
            if (_teamParams.find(teamName) == _teamParams.end())
                _teamParams[teamName] = std::make_pair(sf::Color(std::rand() % 256, std::rand() % 256, std::rand() % 256), textures[std::rand() % textures.size()]);
            _texture = _teamParams[teamName].second;
            _color = _teamParams[teamName].first;
        }
        /**
         * @brief Releases the player.
         */
        ~Player() = default;

        /**
         * @brief Returns the player identifier.
         * @return Protocol player id.
         */
        int getId() const { return _id; }

        /**
         * @brief Returns the player x coordinate.
         * @return Tile x coordinate.
         */
        int getX() const { return _x; }

        /**
         * @brief Returns the player y coordinate.
         * @return Tile y coordinate.
         */
        int getY() const { return _y; }

        /**
         * @brief Returns the current player level.
         * @return Level from 1 to 8.
         */
        int getLevel() const { return _level;}

        /**
         * @brief Returns the player inventory.
         * @return Resource vector ordered by the Zappy protocol.
         */
        std::vector<int> getInventory() const { return _inventory; }

        /**
         * @brief Returns the player team name.
         * @return Team name.
         */
        std::string getTeam() const { return _teamName; }

        /**
         * @brief Returns whether the player is in an expulsion animation state.
         * @return True when the last known state is expelling.
         */
        bool getExpulsion() const { return _isExpelling; }

        /**
         * @brief Returns the broadcast message while it is still visible.
         * @return Message text or an empty string after the display timeout.
         */
        std::string getBroadcast() const {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastBroadcastTime).count();
            return elapsed < 5000 ? _broadcastMessage : "";
        }

        /**
         * @brief Returns whether the player is in a drop animation state.
         * @return True when the player is dropping a resource.
         */
        bool getDropping() const { return _isDropping; }

        /**
         * @brief Returns whether the player is in a take animation state.
         * @return True when the player is taking a resource.
         */
        bool getTaking() const { return _isTaking; }

        /**
         * @brief Returns whether the player is incanting.
         * @return True when the player is part of an active incantation.
         */
        bool getIncanting() const { return _isIncanting; }

        /**
         * @brief Returns whether the player is forking.
         * @return True when the player started a fork action.
         */
        bool getForking() const { return _isForking; }

        /**
         * @brief Returns the progress of the level-up animation.
         * @return Progress from 0 to 1, or 1 when no animation is active.
         */
        float getLevelUpAnimationProgress() const {
            if (!_isLevelingUp)
                return 1.f;
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastLevelUpTime).count();

            if (elapsed >= 1600)
                return 1.f;
            return static_cast<float>(elapsed) / static_cast<float>(1600);
        }

        /**
         * @brief Returns whether the level-up animation should still be drawn.
         * @return True while the fade in/out animation is active.
         */
        bool isLevelUpAnimationVisible() const {
            return _isLevelingUp && getLevelUpAnimationProgress() < 1.f;
        }

        /**
         * @brief Builds the SFML sprite used to draw the player.
         * @param tileSize Current tile size in pixels.
         * @return Configured player sprite.
         */
        sf::Sprite getSprite(int tileSize) const {
            const sf::Texture &tex = AssetsManager::getTexture(_texture);
            sf::Sprite sprite(tex);
            sf::Vector2u texSize = tex.getSize();
            float scale = static_cast<float>(tileSize * 1.5f) / texSize.x;
            sprite.setScale({scale, scale});
            sprite.setOrigin({texSize.x / 2.f, static_cast<float>(texSize.y)});
            sprite.setColor(_color);
            return sprite;
        }

        /**
         * @brief Updates the player tile position.
         * @param x New tile x coordinate.
         * @param y New tile y coordinate.
         */
        void setLocation(int x, int y) { _x = x; _y = y; }

        /**
         * @brief Updates the player orientation.
         * @param orientation Protocol orientation value.
         */
        void setOrientation(int orientation) { _orientation = orientation; }

        /**
         * @brief Replaces the player inventory.
         * @param inventory Resource vector ordered by the Zappy protocol.
         */
        void setInventory(const std::vector<int> &inventory) { _inventory = inventory; }

        /**
         * @brief Updates the player level.
         * @param level New level value.
         */
        void setLevel(int level) {
            if (level > _level) {
                _isLevelingUp = true;
                _lastLevelUpTime = std::chrono::steady_clock::now();
            }
            _level = level;
        }

        /**
         * @brief Updates the player team name.
         * @param teamName New team name.
         */
        void setTeamName(std::string teamName) { _teamName = teamName; }

        /**
         * @brief Sets the incantation animation state.
         * @param incantation True when the player is incanting.
         */
        void setIncantation(bool incantation) { _isIncanting = incantation; }

        /**
         * @brief Sets the take animation state.
         * @param taking True when the player is taking a resource.
         */
        void setTaking(bool taking) { _isTaking = taking; }

        /**
         * @brief Sets the drop animation state.
         * @param dropping True when the player is dropping a resource.
         */
        void setDropping(bool dropping) { _isDropping = dropping; }

        /**
         * @brief Stores a broadcast message and starts its display timeout.
         * @param message Message received from the server.
         */
        void setBroadcast(const std::string &message) { _broadcastMessage = message; _lastBroadcastTime = std::chrono::steady_clock::now(); }

        /**
         * @brief Sets the expulsion animation state.
         * @param expulsion True when the player is expelling.
         */
        void setExpelling(bool expulsion) { _isExpelling = expulsion; }

        /**
         * @brief Sets the fork animation state.
         * @param forking True when the player is forking.
         */
        void setForking(bool forking) { _isForking = forking; }
};

#endif
