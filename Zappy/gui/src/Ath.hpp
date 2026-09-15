#ifndef ATH_HPP
    #define ATH_HPP

    #include "AssetsManager.hpp"
    #include "Map.hpp"
    #include "Player.hpp"
    #include "Cell.hpp"

    #include <SFML/Graphics/RenderWindow.hpp>
    #include <SFML/Graphics/Sprite.hpp>
    #include <SFML/Graphics/Texture.hpp>
    #include <SFML/System/Angle.hpp>

    #include "Utils.hpp"

    #include <optional>
    #include <utility>

/**
 * @brief Draws the heads-up display layered over the game map.
 */
class Ath {
    public:

        /**
         * @brief Draws the compact panel for one selected player.
         * @param player Player to display.
         * @param window SFML render window used as drawing target.
         * @param i Vertical index in the selected-player list.
         * @param tileSize Current map tile size in pixels.
         */
        static void drawPlayerSelect(Player *player, sf::RenderWindow &window, int i, int tileSize) {
            Utils::drawSprite(AssetsManager::getTexture("woodTrapeze"), window, sf::Vector2f(250, static_cast<float>(165 + i * 55)), sf::Vector2f(2.f, 0.9f), sf::Angle(sf::degrees(180)));
            Utils::drawSprite(AssetsManager::getTexture("PlayerSelect"), window, sf::Vector2f(40, static_cast<float>(110 + i * 55)), sf::Vector2f(1.f, 1.f), sf::Angle().Zero);
            Utils::drawSprite(AssetsManager::getTexture("circle"), window, sf::Vector2f(40, static_cast<float>(105 + i * 55)), sf::Vector2f(1.f, 1.f), sf::Angle().Zero);
            sf::Sprite playerSprite = player->getSprite(tileSize);
            playerSprite.setPosition({65, static_cast<float>(155 + i * 55)});
            playerSprite.setScale({0.09f, 0.09f});
            window.draw(playerSprite);
            Utils::drawSprite(AssetsManager::getTexture("circle"), window, sf::Vector2f(260, static_cast<float>(110 + i * 55)), sf::Vector2f(0.7f, 0.75f), sf::Angle().Zero);
            Utils::drawSprite(AssetsManager::getTexture("redRectangle"), window, sf::Vector2f({290, static_cast<float>(110 + i * 55)}), sf::Vector2f(0.5f, 0.8f), sf::Angle().Zero);
            Utils::drawText(AssetsManager::getFont("base"), window, std::to_string(player->getLevel()), sf::Vector2f(312, static_cast<float>(119 + i * 55)), 20, sf::Color::White);
            Utils::drawText(AssetsManager::getFont("base"), window, std::to_string(player->getInventory()[0]), sf::Vector2f(270, static_cast<float>(119 + i * 55)), 20, sf::Color::White);
            Utils::drawText(AssetsManager::getFont("base"), window, player->getTeam(), sf::Vector2f(120, static_cast<float>(140 + i * 55)), 18, sf::Color::White);
            for (int a = 1; a < 7; a++) {
                Utils::drawSprite(AssetsManager::getTexture("stoneRectangle"), window, sf::Vector2f(70 + a * 25, static_cast<float>(118 + i * 55)), sf::Vector2f(0.45f, 0.45f), sf::Angle().Zero);
                Utils::drawText(AssetsManager::getFont("base"), window, std::to_string(player->getInventory()[a]), sf::Vector2f(74 + a * 25, static_cast<float>(119 + i * 55)), 20, sf::Color::White);
            }
        }

        /**
         * @brief Draws resources and selected players for one cell.
         * @param pos Tile coordinates to inspect.
         * @param window SFML render window used as drawing target.
         * @param map Map containing cells and players.
         * @param actualize True when the selected player list must be refreshed.
         */
        static void drawCellInfos(std::pair<int, int> pos, sf::RenderWindow &window, Map &map, bool actualize) {
            static std::vector<int> playerIds;
            std::optional<Egg> egg = map.getEggOn(pos.first, pos.second);
            Cell& cell = map.getCell(pos.first, pos.second);

            if  (actualize) {
                playerIds.clear();
                for (Player* p : map.getPlayersOn(pos.first, pos.second))
                    playerIds.push_back(p->getId());
            }
            std::vector<Player*> players;
            for (int id : playerIds)
                for (Player& p : map.getPlayers())
                    if (p.getId() == id)
                        players.push_back(&p);
            cell.drawResources(window);
            if (!players.empty()) {
                for (int i = 0; i < static_cast<int>(players.size()); i++) {
                    drawPlayerSelect(players[i], window, i, map.getTileSize());
                }
            }
        }

        /**
         * @brief Draws the global resource counter.
         * @param window SFML render window used as drawing target.
         * @param resources Resource totals ordered by the Zappy protocol.
         */
        static void drawWholeResources(sf::RenderWindow& window, const std::vector<int>& resources) {
            constexpr float BASE_X = WINDOW_WIDTH - 360.f;
            Utils::drawSprite(AssetsManager::getTexture("bar"), window, sf::Vector2f(BASE_X, 20), sf::Vector2f(0.5f, 0.5f), sf::Angle().Zero);
            Utils::drawSprite(AssetsManager::getTexture("squares"), window, sf::Vector2f(BASE_X + 23, 32), sf::Vector2f(0.43f, 0.43f), sf::Angle().Zero);
            Utils::drawSprite(AssetsManager::getTexture("circle"), window, sf::Vector2f(BASE_X - 35, 30), sf::Vector2f(1.f, 1.f), sf::Angle().Zero);
            Utils::drawSprite(AssetsManager::getTexture("Food"), window, sf::Vector2f(BASE_X - 35, 30), sf::Vector2f(0.35f, 0.35f), sf::Angle().Zero);
            Utils::drawSprite(AssetsManager::getTexture("R6"), window, sf::Vector2f(BASE_X + 23, 35), sf::Vector2f(0.35f, 0.35f), sf::Angle().Zero);
            Utils::drawSprite(AssetsManager::getTexture("R5"), window, sf::Vector2f(BASE_X + 70, 35), sf::Vector2f(0.35f, 0.35f), sf::Angle().Zero);
            Utils::drawSprite(AssetsManager::getTexture("R4"), window, sf::Vector2f(BASE_X + 115, 35), sf::Vector2f(0.35f, 0.35f), sf::Angle().Zero);
            Utils::drawSprite(AssetsManager::getTexture("R3"), window, sf::Vector2f(BASE_X + 165, 35), sf::Vector2f(0.35f, 0.35f), sf::Angle().Zero);
            Utils::drawSprite(AssetsManager::getTexture("R2"), window, sf::Vector2f(BASE_X + 215, 35), sf::Vector2f(0.35f, 0.35f), sf::Angle().Zero);
            Utils::drawSprite(AssetsManager::getTexture("R1"), window, sf::Vector2f(BASE_X + 265, 35), sf::Vector2f(0.35f, 0.35f), sf::Angle().Zero);
            for (int i = 5; i >= 0; i--) {
                Utils::drawSprite(AssetsManager::getTexture("circle"), window, sf::Vector2f(BASE_X + 30 + i * 48, 70), sf::Vector2f(0.6f, 0.6f), sf::Angle().Zero);
                Utils::drawText(AssetsManager::getFont("base"), window, std::to_string(resources[6 - i]), sf::Vector2f(BASE_X + 38 + i * 48, 75), 20, sf::Color::White);
            }
            Utils::drawSprite(AssetsManager::getTexture("circle"), window, sf::Vector2f(BASE_X - 25, 70), sf::Vector2f(0.6f, 0.6f), sf::Angle().Zero);
            Utils::drawText(AssetsManager::getFont("base"), window, std::to_string(resources[0]), sf::Vector2f(BASE_X - 17, 75), 20, sf::Color::White);
        }

        /**
         * @brief Draws team resource totals and level distribution.
         * @param window SFML render window used as drawing target.
         * @param map Map used to aggregate team data.
         */
        static void drawTeamsInfos(sf::RenderWindow& window, Map &map) {
            float BASE_X = WINDOW_WIDTH - 260.f;
            std::vector<std::string> teamNames = map.getTeamNames();
            for (int i = 0; i < static_cast<int>(teamNames.size()); i++) {
                std::vector<int> teamsResources = map.getTeamsResources(teamNames[i]);
                std::vector<int> playersLevel = map.getPlayersLevelPerTeam(teamNames[i]);
                float y = static_cast<float>(110 + i * 110);
                Utils::drawSprite(AssetsManager::getTexture("PlayerSelect"), window,
                    sf::Vector2f(BASE_X, y), sf::Vector2f(1.f, 1.f), sf::Angle().Zero);
                Utils::drawText(AssetsManager::getFont("base"), window, teamNames[i],
                    sf::Vector2f(BASE_X + 40, y + 10), 20, sf::Color::White);
                for (int j = 0; j < 7; j++) {
                    Utils::drawSprite(AssetsManager::getTexture("circle"), window,
                        sf::Vector2f(BASE_X + 10 + j * 30, y + 30), sf::Vector2f(0.6f, 0.6f), sf::Angle().Zero);
                    Utils::drawText(AssetsManager::getFont("base"), window, std::to_string(teamsResources[j]),
                        sf::Vector2f(BASE_X + 18 + j * 30, y + 36), 15, sf::Color::White);
                }
                for (int k = 0; k < 8; k++) {
                    Utils::drawSprite(AssetsManager::getTexture("circle"), window,
                        sf::Vector2f(BASE_X + 10 + k * 30, y + 60), sf::Vector2f(0.6f, 0.6f), sf::Angle().Zero);
                    Utils::drawText(AssetsManager::getFont("base"), window, std::to_string(playersLevel[k]),
                        sf::Vector2f(BASE_X + 18 + k * 30, y + 66), 15, sf::Color::White);
                }
            }
        }
};

#endif
