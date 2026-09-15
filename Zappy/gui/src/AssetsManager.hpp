#ifndef ASSETSMANAGER_HPP
    #define ASSETSMANAGER_HPP

    #include "SFML/Graphics/Font.hpp"
    #include <string>
    #include <map>
    #include <SFML/Graphics.hpp>

/**
 * @brief Loads and stores the textures and fonts used by the GUI.
 */
class AssetsManager {
    private:
        inline static std::map<std::string, sf::Texture> _textures;
        inline static std::map<std::string, sf::Font> _fonts;

    public:
        /**
         * @brief Error raised when a texture or font cannot be loaded or retrieved.
         */
        class AssetsManagerError : public std::exception {
            public:
                /**
                 * @brief Builds an asset manager error.
                 * @param message Human-readable error message.
                 */
                AssetsManagerError(const std::string &message) : _msg(message) {}

                /**
                 * @brief Returns the stored error message.
                 * @return Null-terminated error message.
                 */
                const char* what() const noexcept override { return _msg.c_str(); }
            private:
                std::string _msg;
        };

        /**
         * @brief Loads every texture and font required by the game GUI.
         * @throw AssetsManagerError If one asset cannot be opened.
         */
        static void loadGameAssets() {
            loadTexture("background", "gui/assets/img/background.png");
            loadTexture("sacrieur", "gui/assets/img/Sacrieur.png");
            loadTexture("iop", "gui/assets/img/Iop.png");
            loadTextureWithRect("egg1", "gui/assets/img/eggs.png", sf::IntRect({56, 36}, {104, 145}));
            loadTextureWithRect("egg2", "gui/assets/img/eggs.png", sf::IntRect({230, 36}, {104, 145}));
            loadTextureWithRect("egg3", "gui/assets/img/eggs.png", sf::IntRect({400, 36}, {104, 145}));
            loadTextureWithRect("egg4", "gui/assets/img/eggs.png", sf::IntRect({56, 225}, {104, 145}));
            loadTextureWithRect("egg5", "gui/assets/img/eggs.png", sf::IntRect({230, 225}, {104, 145}));
            loadTextureWithRect("egg6", "gui/assets/img/eggs.png", sf::IntRect({400, 225}, {104, 145}));
            loadTextureWithRect("bar", "gui/assets/img/ath1.png", sf::IntRect({15, 55}, {680, 160}));
            loadTextureWithRect("squares", "gui/assets/img/ath1.png", sf::IntRect({25, 215}, {670, 110}));
            loadTextureWithRect("PlayerSelect", "gui/assets/img/ath2.png", sf::IntRect({260, 20}, {230, 35}));
            loadTextureWithRect("circle", "gui/assets/img/ath2.png", sf::IntRect({835, 10}, {50, 50}));
            loadTextureWithRect("redRectangle", "gui/assets/img/ath2.png", sf::IntRect({650, 65}, {105, 45}));
            loadTextureWithRect("Food", "gui/assets/img/ressources.png", sf::IntRect({30, 60}, {130, 140}));
            loadTextureWithRect("R1", "gui/assets/img/ressources.png", sf::IntRect({170, 80}, {115, 115}));
            loadTextureWithRect("R2", "gui/assets/img/ressources.png", sf::IntRect({295, 80}, {115, 115}));
            loadTextureWithRect("R3", "gui/assets/img/ressources.png", sf::IntRect({420, 80}, {115, 115}));
            loadTextureWithRect("R4", "gui/assets/img/ressources.png", sf::IntRect({70, 225}, {115, 115}));
            loadTextureWithRect("R5", "gui/assets/img/ressources.png", sf::IntRect({220, 225}, {115, 115}));
            loadTextureWithRect("R6", "gui/assets/img/ressources.png", sf::IntRect({370, 225}, {115, 115}));
            loadTextureWithRect("stoneRectangle", "gui/assets/img/ath2.png", sf::IntRect({495, 125}, {50, 50}));
            loadTextureWithRect("woodTrapeze", "gui/assets/img/ath2.png", sf::IntRect({1085, 160}, {85, 30}));
            loadTexture("chest", "gui/assets/img/chest.png");
            loadTexture("levelUp", "gui/assets/img/level-up-transparent.png");

            loadFont("base", "gui/assets/fonts/base.otf");
            loadFont("oswald", "gui/assets/fonts/Oswald-VariableFont_wght.ttf");
        }

        /**
         * @brief Loads a full texture from disk.
         * @param name Key used to retrieve the texture later.
         * @param path Path to the image file.
         * @throw AssetsManagerError If the image cannot be opened.
         */
        static void loadTexture(const std::string &name, const std::string &path) {
            sf::Texture texture;
            if (!texture.loadFromFile(path))
                throw AssetsManagerError("Failed to load texture: " + path);
            _textures[name] = std::move(texture);
        }

        /**
         * @brief Loads a texture from a rectangular area of an image.
         * @param name Key used to retrieve the texture later.
         * @param path Path to the image file.
         * @param rect Rectangle copied from the source image.
         * @throw AssetsManagerError If the image cannot be opened.
         */
        static void loadTextureWithRect(const std::string &name, const std::string &path, const sf::IntRect &rect) {
            sf::Texture texture;
            if (!texture.loadFromFile(path, false, rect))
                throw AssetsManagerError("Failed to load texture: " + path);
            _textures[name] = std::move(texture);
        }

        /**
         * @brief Retrieves a loaded texture.
         * @param name Texture key.
         * @return Constant reference to the texture.
         * @throw AssetsManagerError If the key is unknown.
         */
        static const sf::Texture& getTexture(const std::string &name) {
            if (_textures.find(name) == _textures.end())
                throw AssetsManagerError("Texture not found: " + name);
            return _textures[name];
        }

        /**
         * @brief Loads a font from disk.
         * @param name Key used to retrieve the font later.
         * @param path Path to the font file.
         * @throw AssetsManagerError If the font cannot be opened.
         */
        static void loadFont(const std::string &name, const std::string &path) {
            sf::Font font;
            if (!font.openFromFile(path))
                throw AssetsManagerError("Failed to load font: " + path);
            _fonts[name] = std::move(font);
        }

        /**
         * @brief Retrieves a loaded font.
         * @param name Font key.
         * @return Constant reference to the font.
         * @throw AssetsManagerError If the key is unknown.
         */
        static const sf::Font& getFont(const std::string &name) {
            if (_fonts.find(name) == _fonts.end())
                throw AssetsManagerError("Font not found: " + name);
            return _fonts[name];
        }

        /**
         * @brief Releases every loaded texture and font.
         */
        static void clear() {
            _textures.clear();
            _fonts.clear();
        }
};

#endif
