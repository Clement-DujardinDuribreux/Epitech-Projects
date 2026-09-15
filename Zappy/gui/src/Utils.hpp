#ifndef UTILS_HPP
    #define UTILS_HPP

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/Graphics.hpp>

/**
 * @brief Small rendering helpers shared by the GUI widgets.
 */
class Utils {
    public:
        /**
         * @brief Draws a texture as a sprite.
         * @param texture Texture to draw.
         * @param window SFML render window used as drawing target.
         * @param position Sprite position in pixels.
         * @param scale Sprite scale on x and y axes.
         * @param rotation Sprite rotation.
         */
        static void drawSprite(const sf::Texture &texture, sf::RenderWindow &window, sf::Vector2f position, sf::Vector2f scale, sf::Angle rotation) {
            sf::Sprite sprite(texture);
            sprite.setPosition(position);
            sprite.setScale(scale);
            sprite.setRotation(rotation);
            window.draw(sprite);
        }

        /**
         * @brief Draws a text string.
         * @param font Font used to render the string.
         * @param window SFML render window used as drawing target.
         * @param text Text content.
         * @param position Text position in pixels.
         * @param characterSize Character size in pixels.
         * @param color Text fill color.
         */
        static void drawText(const sf::Font &font, sf::RenderWindow &window, const std::string &text, sf::Vector2f position, int characterSize, sf::Color color) {
            sf::Text textSprite(font);
            textSprite.setString(text);
            textSprite.setCharacterSize(characterSize);
            textSprite.setPosition(position);
            textSprite.setFillColor(color);
            window.draw(textSprite);
        }
};

#endif
