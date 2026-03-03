#include "dl/resources/ResourceManager.hpp"

namespace dl {
bool ResourceManager::loadTexture(const std::string& texId, const std::string& path, bool isSmooth) {

    sf::Texture texture;
    if (!texture.loadFromFile(path)) {
        return false;
    }
    texture.setSmooth(isSmooth);

    m_textureMap.emplace(texId, std::move(texture));

    return true;
}

sf::Texture& ResourceManager::getTexture(const std::string& texId) {
    return m_textureMap.at(texId);
}

bool ResourceManager::loadFont(const std::string& fontId, const std::string& path) {

    sf::Font font;
    if (!font.openFromFile(path))
        return false;

    m_fontMap.emplace(fontId, std::move(font));

    return true;
}

sf::Font& ResourceManager::getFont(const std::string& fontId) {
    return m_fontMap.at(fontId);
}

bool ResourceManager::loadShader(const std::string& shaderId, sf::Shader::Type shaderType, const std::string& path) {

    auto shader = std::make_unique<sf::Shader>();
    if (!shader->loadFromFile(path, shaderType))
        return false;

    m_shaderMap.emplace(shaderId, std::move(shader));

    return true;
}

sf::Shader& ResourceManager::getShader(const std::string& shaderId) {
    return *m_shaderMap.at(shaderId);
}

} // namespace dl