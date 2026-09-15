#ifndef CAMERA_HPP
    #define CAMERA_HPP

    #include <SFML/Graphics/Rect.hpp>
    #include <SFML/Graphics/RenderWindow.hpp>
    #include <SFML/Graphics.hpp>
    #include <algorithm>

    #define MIN_ZOOM_WIDTH 200.f

/**
 * @brief Controls the SFML view used to inspect the game map.
 */
class Camera {
    private:
        sf::View _view;
        sf::FloatRect _worldBounds;

    public:
        /**
         * @brief Creates a camera matching the current window size.
         * @param window Render window used to initialize the view and world bounds.
         */
        Camera(sf::RenderWindow& window) {
            _view.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
            _view.setCenter(sf::Vector2f(static_cast<float>(window.getSize().x) / 2, static_cast<float>(window.getSize().y) / 2));
            _worldBounds = sf::FloatRect({0.f, 0.f}, {static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});
        }

        /**
         * @brief Moves the camera view.
         * @param direction Offset applied to the view center.
         */
        void move(const sf::Vector2f& direction) { _view.move(direction); }

        /**
         * @brief Applies a zoom factor to the camera view.
         * @param factor SFML zoom factor.
         */
        void zoom(float factor) { _view.zoom(factor); }

        /**
         * @brief Resets the camera to the window default view.
         * @param window Render window that owns the default view.
         */
        void reset(sf::RenderWindow& window) { _view = window.getDefaultView(); }

        /**
         * @brief Keeps the view inside the configured world bounds and zoom limits.
         */
        void clamp() {
            if (_worldBounds.size.x <= 0.f || _worldBounds.size.y <= 0.f)
                return;

            sf::Vector2f center = _view.getCenter();
            sf::Vector2f size   = _view.getSize();

            if (size.x > _worldBounds.size.x || size.y > _worldBounds.size.y) {
                float ratio = std::max(size.x / _worldBounds.size.x, size.y / _worldBounds.size.y);
                _view.zoom(1.0f / ratio);
                size = _view.getSize();
            }
            if (size.x < MIN_ZOOM_WIDTH) {
                _view.zoom(MIN_ZOOM_WIDTH / size.x);
                size = _view.getSize();
            }

            const float left = _worldBounds.position.x;
            const float top = _worldBounds.position.y;
            const float right = left + _worldBounds.size.x;
            const float bottom = top  + _worldBounds.size.y;

            center.x = std::clamp(center.x, left + size.x / 2.f, right  - size.x / 2.f);
            center.y = std::clamp(center.y, top  + size.y / 2.f, bottom - size.y / 2.f);
            _view.setCenter(center);
        }

        /**
         * @brief Replaces the current view.
         * @param view New SFML view.
         */
        void setView(sf::View& view) { _view = view; }

        /**
         * @brief Returns the current view.
         * @return Mutable reference to the SFML view.
         */
        sf::View& getView() { return _view; }

};

#endif
