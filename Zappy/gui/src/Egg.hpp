#ifndef EGG_HPP
    #define EGG_HPP

    #include <SFML/Graphics.hpp>
    #include <vector>

    #include "AssetsManager.hpp"

/**
 * @brief Graphical representation of an egg announced by the server.
 */
class Egg {
    private:
        int _id;
        int _playerId;

        int _x;
        int _y;

        std::vector<std::string> _eggsTextures {"egg1", "egg2", "egg3", "egg4", "egg5", "egg6"};
        std::string _texture;

    public:
        /**
         * @brief Creates an egg and selects one egg texture variant.
         * @param id Egg identifier from the graphical protocol.
         * @param playerId Identifier of the player that created the egg.
         * @param x Tile x coordinate.
         * @param y Tile y coordinate.
         */
        Egg(int id, int playerId, int x, int y) : _id(id), _playerId(playerId), _x(x), _y(y) {
            _texture = _eggsTextures[rand() % _eggsTextures.size()];
        }

        /**
         * @brief Releases the egg.
         */
        ~Egg() = default;

        /**
         * @brief Returns the egg x coordinate.
         * @return Tile x coordinate.
         */
        int getX() const { return _x; }

        /**
         * @brief Returns the egg y coordinate.
         * @return Tile y coordinate.
         */
        int getY() const { return _y; }

        /**
         * @brief Returns the egg identifier.
         * @return Protocol egg id.
         */
        int getId() const { return _id; }

        /**
         * @brief Returns the player that created the egg.
         * @return Protocol player id.
         */
        int getPlayerId() const { return _playerId; }

        /**
         * @brief Builds the SFML sprite used to draw the egg.
         * @param tileSize Current tile size in pixels.
         * @return Configured egg sprite.
         */
        sf::Sprite getSprite(int tileSize) const {
            const sf::Texture &tex = AssetsManager::getTexture(_texture);
            sf::Sprite sprite(tex);
            sf::Vector2u texSize = tex.getSize();
            float scale = static_cast<float>(tileSize * 0.35f) / texSize.x;
            sprite.setScale({scale, scale});
            sprite.setOrigin({texSize.x / 2.f, static_cast<float>(texSize.y)});
            return sprite;
        }
};

#endif
