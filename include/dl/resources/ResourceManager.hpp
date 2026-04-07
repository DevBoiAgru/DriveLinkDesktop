#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

namespace dl {

class ResourceManager {
public:
    bool loadTexture(const std::string& texId, const std::string& path, bool isSmooth = false);
    sf::Texture& getTexture(const std::string& texId);

    bool loadFont(const std::string& fontId, const std::string& path);
    sf::Font& getFont(const std::string& fontId);

    bool loadShader(const std::string& shaderId, sf::Shader::Type shaderType, const std::string& path);
    sf::Shader& getShader(const std::string& shaderId);

private:
    std::unordered_map<std::string, sf::Texture> m_textureMap;
    std::unordered_map<std::string, sf::Font> m_fontMap;
    std::unordered_map<std::string, std::unique_ptr<sf::Shader>> m_shaderMap;
};

} // namespace dl