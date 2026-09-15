#ifndef GUI_HPP
    #define GUI_HPP

    #include <algorithm>
    #include <cmath>
    #include <iostream>
    #include <string>
    #include <set>
    #include <utility>
    #include <SFML/Graphics.hpp>
    #include <SFML/Window.hpp>
    #include <SFML/System.hpp>
    #include <SFML/Network.hpp>

    #include "Camera.hpp"
    #include "client/Client.hpp"
    #include "AssetsManager.hpp"
    #include "Map.hpp"

    #include "Ath.hpp"

    #define WINDOW_WIDTH 1080
    #define WINDOW_HEIGHT 720

/**
 * @brief Main SFML application that renders the Zappy graphical client.
 */
class GUI {
    private:
        // SFML
        sf::RenderWindow _window;
        Camera _camera;
        sf::Clock _clock;
        float _deltaTime;
        std::set<sf::Keyboard::Key> _keysPressed;
        bool _mousePressed = false;
        sf::Vector2f _mousePos;

        // GAME
        Map _map;
        bool _running = true;
        bool _showCellInfos = false;
        sf::Vector2f _cellPos;
        Client _client;
        sf::Clock _victoryClock;
        bool _victoryAnimationStarted = false;
        std::string _winnerTeam;

        /**
         * @brief Error raised when the GUI cannot start.
         */
        class GuiException : public std::exception {
            private:
                std::string _message;
            public:
                /**
                 * @brief Builds a GUI error.
                 * @param message Human-readable error message.
                 */
                GuiException(const std::string &message) : _message(message) {}

                /**
                 * @brief Returns the stored error message.
                 * @return Null-terminated error message.
                 */
                const char *what() const noexcept override { return _message.c_str(); }
        };

        /**
         * @brief Draws the scaled background texture.
         */
        void _printBackground() {
            const sf::Texture texture = AssetsManager::getTexture("background");
            sf::Sprite sprite(texture);
            sf::Vector2u textureSize = texture.getSize();
            sprite.setScale({
                static_cast<float>(WINDOW_WIDTH) / textureSize.x,
                static_cast<float>(WINDOW_HEIGHT) / textureSize.y
            });
            _window.draw(sprite);
        }

        /**
         * @brief Checks whether a keyboard key is currently held.
         * @param key SFML key code.
         * @return True when the key is in the pressed-key set.
         */
        bool _isKeyPressed(sf::Keyboard::Key key) const {
            return _keysPressed.count(key) > 0;
        }

        /**
         * @brief Refreshes dynamic player information from the server.
         */
        void _updatePlayers() {
            auto players = _map.getPlayers();
            for (auto& player : players) {
                _client.requestPlayerInventory(player.getId());
                _client.requestPlayerLevel(player.getId());
            }
        }

        /**
         * @brief Applies keyboard camera movement, zoom and reset controls.
         */
        void _handleCamera() {
            const float moveSpeed = 250.f * _deltaTime;
            const float zoomSpeed = 1.5f * _deltaTime;

            if (_isKeyPressed(sf::Keyboard::Key::Up) || _isKeyPressed(sf::Keyboard::Key::Z))
                _camera.move({0.f, -moveSpeed});
            if (_isKeyPressed(sf::Keyboard::Key::Down) || _isKeyPressed(sf::Keyboard::Key::S))
                _camera.move({0.f,  moveSpeed});
            if (_isKeyPressed(sf::Keyboard::Key::Left) || _isKeyPressed(sf::Keyboard::Key::Q))
                _camera.move({-moveSpeed, 0.f});
            if (_isKeyPressed(sf::Keyboard::Key::Right) || _isKeyPressed(sf::Keyboard::Key::D))
                _camera.move({ moveSpeed, 0.f});
            if (_isKeyPressed(sf::Keyboard::Key::Equal) || _isKeyPressed(sf::Keyboard::Key::Add))
                _camera.zoom(1.f - zoomSpeed);
            if (_isKeyPressed(sf::Keyboard::Key::Hyphen) || _isKeyPressed(sf::Keyboard::Key::Subtract))
                _camera.zoom(1.f + zoomSpeed);
            if (_isKeyPressed(sf::Keyboard::Key::Tab))
                _camera.reset(_window);
            _camera.clamp();
        }

        /**
         * @brief Applies one SFML event to GUI state.
         * @param event Event polled from the render window.
         */
        void _handleEvent(const sf::Event &event) {
            if (event.is<sf::Event::Closed>())
                _running = false;
            if (const auto *kp = event.getIf<sf::Event::KeyPressed>()) {
                if (kp->code == sf::Keyboard::Key::Escape)
                    _running = false;
                _keysPressed.insert(kp->code);
            }
            if (const auto *kr = event.getIf<sf::Event::KeyReleased>())
                _keysPressed.erase(kr->code);
            if (event.is<sf::Event::FocusLost>())
                _keysPressed.clear();
            auto mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
            if (mouseEvent) {
                _mousePressed = true;
                _mousePos = sf::Vector2f(mouseEvent->position.x, mouseEvent->position.y);
                sf::Vector2f worldPos = _window.mapPixelToCoords(mouseEvent->position, _camera.getView());
                auto cellPos = _map.getCellPos(static_cast<int>(worldPos.x), static_cast<int>(worldPos.y));
                if (cellPos.first != -1 && cellPos.second != -1) {
                    _cellPos = {static_cast<float>(cellPos.first), static_cast<float>(cellPos.second)};
                    _showCellInfos = true;
                } else
                    _showCellInfos = false;
            }
            if (event.is<sf::Event::MouseButtonReleased>())
                _mousePressed = false;
        }

        /**
         * @brief Draws the game-over winner overlay with a fade-in.
         */
        void _drawVictoryOverlay() {
            float elapsed = _victoryClock.getElapsedTime().asSeconds();
            float fade = std::min(1.f, elapsed / 1.2f);
            float pulse = 1.f + std::sin(elapsed * 3.2f) * 0.025f;
            sf::RectangleShape overlay({static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)});
            sf::Text title(AssetsManager::getFont("oswald"), "Game Won", 58);
            sf::Text subtitle(AssetsManager::getFont("base"), "by " + _winnerTeam, 34);

            overlay.setFillColor(sf::Color(0, 0, 0, static_cast<std::uint8_t>(185.f * fade)));
            _window.draw(overlay);
            title.setFillColor(sf::Color(255, 238, 160, static_cast<std::uint8_t>(255.f * fade)));
            title.setStyle(sf::Text::Bold);
            title.setOrigin({title.getLocalBounds().size.x / 2.f, title.getLocalBounds().size.y / 2.f});
            title.setScale({pulse, pulse});
            title.setPosition({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - 55.f});
            _window.draw(title);
            subtitle.setFillColor(sf::Color(255, 255, 255, static_cast<std::uint8_t>(240.f * fade)));
            subtitle.setOrigin({subtitle.getLocalBounds().size.x / 2.f, subtitle.getLocalBounds().size.y / 2.f});
            subtitle.setPosition({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f + 18.f});
            _window.draw(subtitle);
        }

    public:
        /**
         * @brief Creates the GUI and connects it to the Zappy server.
         * @param port TCP port used by the server.
         * @param hostname Hostname or IP address of the server.
         */
        GUI(int &port, std::string &hostname) : _window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Zappy"), _camera(_window), _map(13, 13) {
            if (!_client.connectToServer(hostname, port)) {
                std::cerr << "Impossible de se connecter au serveur !" << std::endl;
                _running = false;
                return;
            }
            try {
                _client.requestMapSize();
            } catch (const std::exception &e) {
                _running = false;
            }
        }

        /**
         * @brief Releases loaded assets when the GUI is destroyed.
         */
        ~GUI() { AssetsManager::clear(); }

        /**
         * @brief Runs the render and network refresh loop until the GUI closes.
         */
        void run() {
            while (_running) {
                try {
                    _client.requestContentMap();
                    _updatePlayers();
                    Map updatedMap = _client.getMap();
                    if (updatedMap.isValid())
                        _map = std::move(updatedMap);
                    if (_client.getEnd() && !_victoryAnimationStarted) {
                        _victoryAnimationStarted = true;
                        _winnerTeam = _client.getWinnerTeam();
                        _victoryClock.restart();
                    }
                    _deltaTime = _clock.restart().asSeconds();
                    while (const auto event = _window.pollEvent())
                        _handleEvent(*event);
                    _handleCamera();

                    _window.clear();
                    _window.setView(_camera.getView());
                    _printBackground();
                    _map.draw(_window);
                    _window.setView(_window.getDefaultView());
                    Ath::drawWholeResources(_window, _map.getResources());
                    Ath::drawTeamsInfos(_window, _map);
                    if (_showCellInfos)
                        Ath::drawCellInfos({_cellPos.x, _cellPos.y}, _window, _map, _mousePressed);
                    if (_victoryAnimationStarted)
                        _drawVictoryOverlay();
                    _window.display();
                } catch (AssetsManager::AssetsManagerError &e) {
                    std::cerr << e.what() << std::endl;
                }
            }
            _window.close();
        }

        /**
         * @brief Ensures an X11 display is available before opening the window.
         * @throw GuiException If the DISPLAY environment variable is missing or invalid.
         */
        static void checkDisplay() {
            char *display = getenv("DISPLAY");
            if (!display || display[0] != ':' || display[1] < '0' || display[1] > '9')
                throw GuiException("DISPLAY environment variable not set correctly\n\nSkill issue detected: Sir, this is a GUI. It needs a display. Crazy concept, I know.\n67 67 67 67 67 67 67 67 67 67 67 67 67");
        }
};

#endif
