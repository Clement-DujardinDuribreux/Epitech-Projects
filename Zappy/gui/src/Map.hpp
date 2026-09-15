#ifndef MAP
    #define MAP

    #include <algorithm>
    #include <chrono>
    #include <cmath>
    #include <cstdint>
    #include <SFML/Graphics.hpp>
    #include <vector>

    #include "Player.hpp"
    #include "Egg.hpp"

    #define TILE_SIZE 48
    #define OFFSET_X 560
    #define OFFSET_Y 80
    #define WINDOW_WIDTH 1080
    #define WINDOW_HEIGHT 720
    #define INCANTATION_FADE_IN_MS 450
    #define INCANTATION_FADE_OUT_MS 650

    #include "Cell.hpp"

/**
 * @brief Stores the graphical state of the Zappy world and draws it.
 */
class Map {
    private:
        int _width;
        int _height;
        std::vector<Player> _players;
        std::vector<Egg> _eggs;
        std::vector<std::vector<Cell>> _cells;

        /**
         * @brief Runtime state for one tile incantation visual effect.
         */
        struct IncantationAnimation {
            int x;
            int y;
            int level;
            bool ending;
            std::chrono::steady_clock::time_point startTime;
            std::chrono::steady_clock::time_point endTime;
        };

        std::vector<IncantationAnimation> _incantations;

        /**
         * @brief Error raised when a map operation receives invalid coordinates or dimensions.
         */
        class MapException : public std::exception {
            public:
                /**
                 * @brief Builds a map error.
                 * @param msg Human-readable error message.
                 */
                MapException(const std::string &msg) : _msg(msg) {}

                /**
                 * @brief Returns the stored error message.
                 * @return Null-terminated error message.
                 */
                const char *what() const noexcept override { return _msg.c_str(); }
            private:
                std::string _msg;
        };

    public:

        /**
         * @brief Creates a map with the requested dimensions.
         * @param width Number of tiles on the x axis.
         * @param height Number of tiles on the y axis.
         */
        Map(int width, int height) : _width(width), _height(height), _cells(std::vector<std::vector<Cell>>(width, std::vector<Cell>(height))) {}

        /**
         * @brief Converts a tile position into an isometric screen position.
         * @param tx Tile x coordinate.
         * @param ty Tile y coordinate.
         * @param map Map used to resolve the current tile size.
         * @return Screen position in pixels.
         */
        static sf::Vector2f getTileScreenPosition(float tx, float ty, const Map& map) {
            const float az = 46.f * static_cast<float>(M_PI) / 180.f;
            const float cosA = std::cos(az);
            const float sinA = std::sin(az);
            const float originX = OFFSET_X;
            const float originY = OFFSET_Y;
            const float ftx = static_cast<float>(tx);
            const float fty = static_cast<float>(ty);
            return {
                originX + (ftx * cosA - fty * sinA) * map.getTileSize(),
                originY + (ftx * sinA + fty * cosA) * map.getTileSize() * 0.57f
            };
        }

        /**
         * @brief Returns the map width.
         * @return Number of tiles on the x axis.
         */
        int getWidth() const { return _width; }

        /**
         * @brief Returns the map height.
         * @return Number of tiles on the y axis.
         */
        int getHeight() const { return _height; }

        /**
         * @brief Checks whether the map dimensions can be rendered.
         * @return True when width and height are positive.
         */
        bool isValid() const { return _width > 0 && _height > 0; }

        /**
         * @brief Computes the tile size from the current map dimensions.
         * @return Tile size in pixels.
         */
        int getTileSize() const {
            if (!isValid())
                return TILE_SIZE;
            return std::max(1, TILE_SIZE * 26 / (_width + _height));
        }
        /**
         * @brief Returns the mutable player list.
         * @return Reference to stored players.
         */
        std::vector<Player> &getPlayers() { return _players; }

        /**
         * @brief Returns the mutable egg list.
         * @return Reference to stored eggs.
         */
        std::vector<Egg> &getEggs() { return _eggs; }

        /**
         * @brief Returns a mutable cell by coordinate.
         * @param x Tile x coordinate.
         * @param y Tile y coordinate.
         * @return Reference to the requested cell.
         * @throw MapException If the coordinates are outside the map.
         */
        Cell& getCell(int x, int y) {
            if (x < 0 || x >= _width || y < 0 || y >= _height)
                throw MapException("Cell out of range");
            return _cells[x][y];
        }

        /**
         * @brief Builds the unique list of teams currently visible.
         * @return Team names found in the player list.
         */
        std::vector<std::string> getTeamNames() const {
            std::vector<std::string> names;
            for (const auto& player : _players) {
                if (std::find(names.begin(), names.end(), player.getTeam()) == names.end())
                    names.push_back(player.getTeam());
            }
            return names;
        }

        /**
         * @brief Sums all inventories owned by one team.
         * @param teamName Team name to aggregate.
         * @return Resource totals ordered by the Zappy protocol.
         */
        std::vector<int> getTeamsResources(const std::string& teamName) const {
            std::vector<int> resources(7, 0);
            for (const Player& player : _players) {
                if (player.getTeam() == teamName) {
                    std::vector<int> inv = player.getInventory();
                    for (int i = 0; i < 7; i++)
                        resources[i] += inv[i];
                }
            }
            return resources;
        }

        /**
         * @brief Counts players per level for one team.
         * @param teamName Team name to inspect.
         * @return Eight counters, one for each level from 1 to 8.
         */
        std::vector<int> getPlayersLevelPerTeam(const std::string& teamName) const {
            std::vector<int> levels(8, 0);
            for (const Player& player : _players) {
                if (player.getTeam() == teamName) {
                    int lvl = player.getLevel();
                    if (lvl >= 1 && lvl <= 8)
                        levels[lvl - 1]++;
                }
            }
            return levels;
        }

        /**
         * @brief Sums resources present on every cell of the map.
         * @return Resource totals ordered by the Zappy protocol.
         */
        std::vector<int> getResources() const {
            std::vector<int> total(7, 0);
            for (const auto& col : _cells)
                for (const auto &cell : col)
                    for (int i = 0; i < 7; i++)
                        total[i] += cell.getResources()[i];
            return total;
        }

        /**
         * @brief Sets the map width without reallocating cells.
         * @param width New width in tiles.
         */
        void setWidth(int width) { _width = width; }

        /**
         * @brief Sets the map height without reallocating cells.
         * @param height New height in tiles.
         */
        void setHeight(int height) { _height = height; }

        /**
         * @brief Resizes the map and resets all cells.
         * @param width New width in tiles.
         * @param height New height in tiles.
         * @throw MapException If one dimension is not positive.
         */
        void resize(int width, int height) {
            if (width <= 0 || height <= 0)
                throw MapException("Invalid map size");
            _width = width;
            _height = height;
            _cells.assign(width, std::vector<Cell>(height));
        }

        /**
         * @brief Converts a screen position into a tile position.
         * @param x Screen x coordinate.
         * @param y Screen y coordinate.
         * @return Tile coordinates or {-1, -1} when outside the map.
         */
        std::pair<int, int> getCellPos(int x, int y) const {
            const float az = 46.f * static_cast<float>(M_PI) / 180.f;
            const float cosA = std::cos(az);
            const float sinA = std::sin(az);

            float dx = static_cast<float>(x) - OFFSET_X;
            float dy = (static_cast<float>(y) - OFFSET_Y) / 0.57f;
            int tx = static_cast<int>((dx * cosA + dy * sinA) / getTileSize());
            int ty = static_cast<int>((-dx * sinA + dy * cosA) / getTileSize());

            if (tx < 0 || tx >= _width || ty < 0 || ty >= _height)
                return {-1, -1};
            return {tx, ty};
        }

        /**
         * @brief Finds players standing on a tile.
         * @param x Tile x coordinate.
         * @param y Tile y coordinate.
         * @return Pointers to players currently on the tile.
         */
        std::vector<Player *> getPlayersOn(int x, int y) {
            std::vector<Player *> players;
            for (Player& player : _players) {
                if (player.getX() == x && player.getY() == y)
                    players.push_back(&player);
            }
            return players;
        }

        /**
         * @brief Finds an egg on a tile.
         * @param x Tile x coordinate.
         * @param y Tile y coordinate.
         * @return Egg copy when one exists on the tile.
         */
        std::optional<Egg> getEggOn(int x, int y) {
            for (Egg& egg : _eggs) {
                if (egg.getX() == x && egg.getY() == y)
                    return egg;
            }
            return std::nullopt;
        }

        /**
         * @brief Starts or refreshes the visual effect for an incantation tile.
         * @param x Tile x coordinate.
         * @param y Tile y coordinate.
         * @param level Incantation level announced by the protocol.
         */
        void startIncantation(int x, int y, int level) {
            if (x < 0 || y < 0 || x >= _width || y >= _height)
                return;
            for (IncantationAnimation &animation : _incantations) {
                if (animation.x == x && animation.y == y) {
                    animation.level = level;
                    animation.ending = false;
                    animation.startTime = std::chrono::steady_clock::now();
                    return;
                }
            }
            _incantations.push_back({
                x, y, level, false,
                std::chrono::steady_clock::now(),
                std::chrono::steady_clock::now()
            });
        }

        /**
         * @brief Starts the fade-out for a finished incantation tile.
         * @param x Tile x coordinate.
         * @param y Tile y coordinate.
         */
        void stopIncantation(int x, int y) {
            auto now = std::chrono::steady_clock::now();

            for (IncantationAnimation &animation : _incantations) {
                if (animation.x == x && animation.y == y) {
                    animation.ending = true;
                    animation.endTime = now;
                }
            }
        }

        /**
         * @brief Checks whether a tile has a visible incantation effect.
         * @param x Tile x coordinate.
         * @param y Tile y coordinate.
         * @return True when an effect is active or fading out.
         */
        bool hasIncantationAt(int x, int y) const {
            for (const IncantationAnimation &animation : _incantations) {
                if (animation.x == x && animation.y == y)
                    return true;
            }
            return false;
        }

        /**
         * @brief Draws the full map layer.
         * @param window SFML render window used as drawing target.
         */
        void draw(sf::RenderWindow& window) {
            if (!isValid())
                return;
            sf::VertexArray grid(sf::PrimitiveType::Lines);
            for (int x = 0; x <= _width; x++) {
                grid.append(sf::Vertex{getTileScreenPosition(x, 0, *this), sf::Color::White});
                grid.append(sf::Vertex{getTileScreenPosition(x, _height, *this), sf::Color::White});
            }
            for (int y = 0; y <= _height; y++) {
                grid.append(sf::Vertex{getTileScreenPosition(0, y, *this), sf::Color::White});
                grid.append(sf::Vertex{getTileScreenPosition(_width, y, *this), sf::Color::White});
            }
            window.draw(grid);
            drawChests(window);
            drawIncantations(window);
            drawEggs(window);
            drawPlayers(window);
            drawBroadcast(window);
        }

        /**
         * @brief Checks whether a finished incantation fade-out can be removed.
         * @param animation Incantation animation state.
         * @param now Shared timestamp for the current frame.
         * @return True when the animation should be removed.
         */
        bool isIncantationFinished(const IncantationAnimation &animation,
            std::chrono::steady_clock::time_point now) const {
            if (!animation.ending)
                return false;
            long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                now - animation.endTime).count();
            return elapsed >= INCANTATION_FADE_OUT_MS;
        }

        /**
         * @brief Draws every active incantation visual effect.
         * @param window SFML render window used as drawing target.
         */
        void drawIncantations(sf::RenderWindow& window) {
            auto now = std::chrono::steady_clock::now();

            for (auto it = _incantations.begin(); it != _incantations.end();) {
                if (isIncantationFinished(*it, now))
                    it = _incantations.erase(it);
                else
                    it++;
            }
            for (const IncantationAnimation &animation : _incantations)
                drawIncantation(window, animation, now);
        }

        /**
         * @brief Draws one rune and particle effect on an incantation tile.
         * @param window SFML render window used as drawing target.
         * @param animation Incantation animation state.
         * @param now Shared timestamp for the current frame.
         */
        void drawIncantation(sf::RenderWindow& window, const IncantationAnimation &animation,
            std::chrono::steady_clock::time_point now) const {
            long aliveMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                now - animation.startTime).count();
            float seconds = static_cast<float>(aliveMs) / 1000.f;
            float fade = 1.f;

            if (aliveMs < INCANTATION_FADE_IN_MS)
                fade = static_cast<float>(aliveMs) / static_cast<float>(INCANTATION_FADE_IN_MS);
            if (animation.ending) {
                long endingMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                    now - animation.endTime).count();
                float endFade = 1.f - static_cast<float>(endingMs) / static_cast<float>(INCANTATION_FADE_OUT_MS);
                fade = std::min(fade, std::max(0.f, endFade));
            }
            if (fade <= 0.f)
                return;

            sf::Vector2f center = Map::getTileScreenPosition(
                static_cast<float>(animation.x) + 0.5f,
                static_cast<float>(animation.y) + 0.5f, *this);
            float tileSize = static_cast<float>(getTileSize());
            float pulse = 1.f + std::sin(seconds * 4.4f) * 0.08f;
            float radius = tileSize * 0.92f * pulse;

            drawIncantationDisk(window, center, radius, fade);
            drawIncantationRing(window, center, radius * 0.78f, seconds * 32.f, fade, false);
            drawIncantationRing(window, center, radius * 0.48f, -seconds * 48.f, fade, true);
            drawIncantationParticles(window, center, radius, seconds, fade, animation.level);
        }

        /**
         * @brief Draws the soft glowing base ellipse of an incantation.
         */
        void drawIncantationDisk(sf::RenderWindow& window, sf::Vector2f center,
            float radius, float fade) const {
            sf::CircleShape disk(radius, 96);

            disk.setOrigin({radius, radius});
            disk.setScale({1.f, 0.54f});
            disk.setPosition(center);
            disk.setFillColor(sf::Color(32, 210, 255, static_cast<std::uint8_t>(52.f * fade)));
            disk.setOutlineColor(sf::Color(255, 238, 150, static_cast<std::uint8_t>(145.f * fade)));
            disk.setOutlineThickness(std::max(1.f, radius * 0.055f));
            window.draw(disk);
        }

        /**
         * @brief Draws a segmented rotating incantation ring.
         */
        void drawIncantationRing(sf::RenderWindow& window, sf::Vector2f center,
            float radius, float rotation, float fade, bool alternateColor) const {
            sf::Color color = alternateColor
                ? sf::Color(255, 250, 180, static_cast<std::uint8_t>(180.f * fade))
                : sf::Color(60, 230, 255, static_cast<std::uint8_t>(190.f * fade));

            for (int index = 0; index < 8; index++) {
                float angle = (rotation + static_cast<float>(index) * 45.f) * static_cast<float>(M_PI) / 180.f;
                float nextAngle = angle + 20.f * static_cast<float>(M_PI) / 180.f;
                sf::ConvexShape segment(4);
                float inner = radius * 0.88f;
                float outer = radius;

                segment.setPoint(0, {center.x + std::cos(angle) * outer,
                    center.y + std::sin(angle) * outer * 0.54f});
                segment.setPoint(1, {center.x + std::cos(nextAngle) * outer,
                    center.y + std::sin(nextAngle) * outer * 0.54f});
                segment.setPoint(2, {center.x + std::cos(nextAngle) * inner,
                    center.y + std::sin(nextAngle) * inner * 0.54f});
                segment.setPoint(3, {center.x + std::cos(angle) * inner,
                    center.y + std::sin(angle) * inner * 0.54f});
                segment.setFillColor(color);
                window.draw(segment);
            }
        }

        /**
         * @brief Draws orbiting light particles around an incantation tile.
         */
        void drawIncantationParticles(sf::RenderWindow& window, sf::Vector2f center,
            float radius, float seconds, float fade, int level) const {
            int particleCount = std::max(6, std::min(12, level + 5));

            for (int index = 0; index < particleCount; index++) {
                float offset = static_cast<float>(index) * 360.f / static_cast<float>(particleCount);
                float angle = (seconds * 85.f + offset) * static_cast<float>(M_PI) / 180.f;
                float wave = (std::sin(seconds * 5.f + static_cast<float>(index)) + 1.f) * 0.5f;
                float particleRadius = radius * (0.55f + wave * 0.36f);
                float dotSize = std::max(2.f, radius * (0.045f + wave * 0.02f));
                sf::CircleShape dot(dotSize, 24);

                dot.setOrigin({dotSize, dotSize});
                dot.setPosition({
                    center.x + std::cos(angle) * particleRadius,
                    center.y + std::sin(angle) * particleRadius * 0.54f - wave * radius * 0.25f
                });
                dot.setFillColor(sf::Color(255, 245, 170, static_cast<std::uint8_t>((110.f + wave * 120.f) * fade)));
                window.draw(dot);
            }
        }

        /**
         * @brief Draws resource chests on non-empty cells.
         * @param window SFML render window used as drawing target.
         */
        void drawChests(sf::RenderWindow& window) const {
            for (size_t i = 0; i < _cells.size(); i++) {
                for (size_t j = 0; j < _cells[i].size(); j++) {
                    if (std::count(_cells[i][j].getResources().begin(), _cells[i][j].getResources().end(), 0) != static_cast<long>(_cells[i][j].getResources().size())) {
                        sf::Vector2f pos = Map::getTileScreenPosition(i, j, *this);
                        Utils::drawSprite(AssetsManager::getTexture("chest"), window, {pos.x - 20, pos.y - 5}, sf::Vector2f(0.2f, 0.2f), sf::Angle().Zero);
                    }
                }
            }
        }

        /**
         * @brief Draws every player sprite at its tile position.
         * @param window SFML render window used as drawing target.
         */
        void drawPlayers(sf::RenderWindow& window) const {
            for (const Player& player : _players) {
                int px = player.getX();
                int py = player.getY();
                sf::Vector2f playerPosition = Map::getTileScreenPosition(px + 0.9f, py + 0.9f, *this);
                sf::Sprite sprite = player.getSprite(getTileSize());
                sprite.setPosition(playerPosition);
                window.draw(sprite);
                drawLevelUpAnimation(window, player, playerPosition);
            }
        }

        /**
         * @brief Draws the transient level-up banner above one player.
         * @param window SFML render window used as drawing target.
         * @param player Player owning the animation.
         * @param playerPosition Current player screen position.
         */
        void drawLevelUpAnimation(sf::RenderWindow& window, const Player& player, sf::Vector2f playerPosition) const {
            if (!player.isLevelUpAnimationVisible())
                return;

            static const std::string levelUpTextureName = "levelUp";
            float progress = player.getLevelUpAnimationProgress();
            float ratio = 1.f;

            if (progress < 0.25f)
                ratio = progress / 0.25f;
            else if (progress > 0.70f)
                ratio = (1.f - progress) / 0.30f;
            if (ratio < 0.f)
                ratio = 0.f;
            if (ratio > 1.f)
                ratio = 1.f;

            const sf::Texture &texture = AssetsManager::getTexture(levelUpTextureName);
            sf::Sprite sprite(texture);
            sf::Vector2u textureSize = texture.getSize();
            float scale = (static_cast<float>(getTileSize()) * (3.6f + progress * 0.35f)) / static_cast<float>(textureSize.x);

            sprite.setOrigin({textureSize.x / 2.f, textureSize.y / 2.f});
            sprite.setScale({scale, scale});
            sprite.setPosition({
                playerPosition.x,
                playerPosition.y - static_cast<float>(getTileSize()) * (1.85f + progress * 0.35f)
            });
            sprite.setColor(sf::Color(255, 255, 255, static_cast<std::uint8_t>(255.f * ratio)));
            window.draw(sprite);
        }

        /**
         * @brief Draws every egg sprite at its tile position.
         * @param window SFML render window used as drawing target.
         */
        void drawEggs(sf::RenderWindow& window) const {
            for (const Egg& egg : _eggs) {
                int px = egg.getX();
                int py = egg.getY();
                sf::Sprite sprite = egg.getSprite(getTileSize());
                sprite.setPosition(Map::getTileScreenPosition(px + 0.6f, py + 0.6f, *this));
                window.draw(sprite);
            }
        }

        /**
         * @brief Draws the latest visible broadcast messages.
         * @param window SFML render window used as drawing target.
         */
        void drawBroadcast(sf::RenderWindow& window) const {
            std::vector<std::string> messages;
            for (const Player& player : _players) {
                std::string msg = player.getBroadcast();
                if (!msg.empty()) {
                    std::string shortMsg = msg;
                    if (shortMsg.size() > 15) {
                        shortMsg = shortMsg.substr(0, 15) + "...";
                    }
                    std::string fullMsg = "[" + player.getTeam() + "] " + shortMsg;
                    messages.push_back(fullMsg);
                    if (messages.size() >= 5)
                        break;
                }
            }
            if (messages.empty())
                return;
            sf::Text title(AssetsManager::getFont("oswald"), "Player Chat", 16);
            title.setFillColor(sf::Color(255, 255, 255));
            title.setStyle(sf::Text::Bold);
            float maxWidth = title.getLocalBounds().size.x;
            std::vector<sf::Text> textObjects;
            for (const std::string& msg : messages) {
                sf::Text text(AssetsManager::getFont("oswald"), msg, 13);
                text.setFillColor(sf::Color::White);
                if (text.getLocalBounds().size.x > maxWidth)
                    maxWidth = text.getLocalBounds().size.x;
                textObjects.push_back(text);
            }
            float paddingX = 20.0f;
            float paddingTop = 10.0f;
            float paddingBottom = 10.0f;
            float lineHeight = 20.0f;
            float titleSpace = 25.0f;
            float boxWidth = std::max(200.0f, maxWidth + paddingX * 2.0f);
            float boxHeight = paddingTop + titleSpace + (messages.size() * lineHeight) + paddingBottom;
            sf::RectangleShape bg({boxWidth, boxHeight});
            bg.setFillColor(sf::Color(0, 0, 0, 180));
            bg.setOutlineColor(sf::Color(255, 255, 255, 100));
            bg.setOutlineThickness(1.0f);
            float margin = 20.0f;
            float startX = WINDOW_WIDTH - boxWidth - margin;
            float startY = WINDOW_HEIGHT - boxHeight - margin;
            bg.setPosition({startX, startY});
            window.draw(bg);
            sf::RectangleShape separator({boxWidth - 20.0f, 1.0f});
            separator.setFillColor(sf::Color(255, 255, 255, 50));
            separator.setPosition({startX + 10.0f, startY + paddingTop + 18.0f});
            window.draw(separator);
            title.setPosition({startX + (boxWidth / 2.0f) - (title.getLocalBounds().size.x / 2.0f), startY + paddingTop - title.getLocalBounds().position.y});
            window.draw(title);
            float currentY = startY + paddingTop + titleSpace;
            for (auto& text : textObjects) {
                text.setPosition({startX + paddingX, currentY - text.getLocalBounds().position.y});
                window.draw(text);
                currentY += lineHeight;
            }
        }

};

#endif
