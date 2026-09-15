#ifndef CELL_HPP
    #define CELL_HPP

    #include <SFML/Graphics.hpp>
    #include <vector>

    #include "AssetsManager.hpp"
    #include "Utils.hpp"


/**
 * @brief Stores and renders the seven resources present on one map tile.
 */
class Cell {
    private:
        std::vector<int> _resources;

    public:
        /**
         * @brief Creates an empty cell with all resources set to zero.
         */
        Cell() : _resources(std::vector<int>(7, 0)) {}

        /**
         * @brief Releases the cell.
         */
        ~Cell() = default;

        /**
         * @brief Returns the resources stored on the tile.
         * @return Resource vector ordered by the Zappy protocol.
         */
        const std::vector<int>& getResources() const { return _resources; }

        /**
         * @brief Replaces all resources stored on the tile.
         * @param resources Resource vector ordered by the Zappy protocol.
         */
        void setResources(const std::vector<int>& resources) { _resources = resources; }

        /**
         * @brief Adds an amount to one resource type.
         * @param type Resource index in the protocol order.
         * @param amount Quantity to add.
         */
        void addResource(int type, int amount) { _resources[type] += amount; }

        /**
         * @brief Removes an amount from one resource type.
         * @param type Resource index in the protocol order.
         * @param amount Quantity to remove.
         */
        void removeResource(int type, int amount) { _resources[type] -= amount; }

        /**
         * @brief Draws the cell resource panel in the HUD.
         * @param window SFML render window used as drawing target.
         */
        void drawResources(sf::RenderWindow& window) const {
            Utils::drawSprite(AssetsManager::getTexture("bar"), window, sf::Vector2f(20, 20), sf::Vector2f(0.5f, 0.5f), sf::Angle().Zero);
            Utils::drawSprite(AssetsManager::getTexture("squares"), window, sf::Vector2f(43, 32), sf::Vector2f(0.43f, 0.43f), sf::Angle().Zero);
            Utils::drawSprite(AssetsManager::getTexture("circle"), window, sf::Vector2f(350, 30), sf::Vector2f(1.f, 1.f), sf::Angle().Zero);
            Utils::drawSprite(AssetsManager::getTexture("Food"), window, sf::Vector2f(350, 30), sf::Vector2f(0.35f, 0.35f), sf::Angle().Zero);
            Utils::drawSprite(AssetsManager::getTexture("R1"), window, sf::Vector2f(43, 35), sf::Vector2f(0.35f, 0.35f), sf::Angle().Zero);
            Utils::drawSprite(AssetsManager::getTexture("R2"), window, sf::Vector2f(90, 35), sf::Vector2f(0.35f, 0.35f), sf::Angle().Zero);
            Utils::drawSprite(AssetsManager::getTexture("R3"), window, sf::Vector2f(135, 35), sf::Vector2f(0.35f, 0.35f), sf::Angle().Zero);
            Utils::drawSprite(AssetsManager::getTexture("R4"), window, sf::Vector2f(185, 35), sf::Vector2f(0.35f, 0.35f), sf::Angle().Zero);
            Utils::drawSprite(AssetsManager::getTexture("R5"), window, sf::Vector2f(235, 35), sf::Vector2f(0.35f, 0.35f), sf::Angle().Zero);
            Utils::drawSprite(AssetsManager::getTexture("R6"), window, sf::Vector2f(285, 35), sf::Vector2f(0.35f, 0.35f), sf::Angle().Zero);
            for (int i = 0; i <= 5; i++) {
                Utils::drawSprite(AssetsManager::getTexture("circle"), window, sf::Vector2f(50 + i * 48, 70), sf::Vector2f(0.6f, 0.6f), sf::Angle().Zero);
                Utils::drawText(AssetsManager::getFont("base"), window, std::to_string(_resources[i + 1]), sf::Vector2f(58 + i * 48, 75), 20, sf::Color::White);
            }
            Utils::drawSprite(AssetsManager::getTexture("circle"), window, sf::Vector2f(360, 70), sf::Vector2f(0.6f, 0.6f), sf::Angle().Zero);
            Utils::drawText(AssetsManager::getFont("base"), window, std::to_string(_resources[0]), sf::Vector2f(368, 75), 20, sf::Color::White);
        }
};

#endif
